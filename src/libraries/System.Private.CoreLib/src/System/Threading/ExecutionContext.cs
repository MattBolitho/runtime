// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.ComponentModel;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.ExceptionServices;
using System.Runtime.Serialization;

namespace System.Threading
{
    public delegate void ContextCallback(object? state);

    internal delegate void ContextCallback<TState>(ref TState state);

    /// <summary>
    /// Manages the execution context for the current thread.
    /// </summary>
    public sealed class ExecutionContext : IDisposable, ISerializable
    {
        internal static readonly ExecutionContext Default = new ExecutionContext();
        private static ExecutionContext? s_defaultFlowSuppressed;

        private readonly IAsyncLocalValueMap? m_localValues;
        private readonly IAsyncLocal[]? m_localChangeNotifications;
        private readonly bool m_isFlowSuppressed;
        private readonly bool m_isDefault;

        private ExecutionContext()
        {
            m_isDefault = true;
        }

        private ExecutionContext(
            IAsyncLocalValueMap localValues,
            IAsyncLocal[]? localChangeNotifications,
            bool isFlowSuppressed)
        {
            m_localValues = localValues;
            m_localChangeNotifications = localChangeNotifications;
            m_isFlowSuppressed = isFlowSuppressed;
        }

        [Obsolete(Obsoletions.LegacyFormatterImplMessage, DiagnosticId = Obsoletions.LegacyFormatterImplDiagId, UrlFormat = Obsoletions.SharedUrlFormat)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            throw new PlatformNotSupportedException();
        }

        public static ExecutionContext? Capture()
        {
            ExecutionContext? executionContext = Thread.CurrentThread._executionContext;
            if (executionContext == null)
            {
                executionContext = Default;
            }
            else if (executionContext.m_isFlowSuppressed)
            {
                executionContext = null;
            }

            return executionContext;
        }

        // Allows capturing asynclocals for a FlowSuppressed ExecutionContext rather than returning null.
        internal static ExecutionContext? CaptureForRestore()
        {
            // This is a short cut for:
            //
            // ExecutionContext.RestoreFlow()
            // var ec = ExecutionContext.Capture()
            // ExecutionContext.SuppressFlow();
            // ...
            // ExecutionContext.Restore(ec)
            // ExecutionContext.SuppressFlow();

            return Thread.CurrentThread._executionContext;
        }

        private ExecutionContext? ShallowClone(bool isFlowSuppressed)
        {
            Debug.Assert(isFlowSuppressed != m_isFlowSuppressed);

            if (m_localValues == null || AsyncLocalValueMap.IsEmpty(m_localValues))
            {
#pragma warning disable CA1825, IDE0300 // Avoid unnecessary zero-length array allocations
                return isFlowSuppressed ?
                    (s_defaultFlowSuppressed ??= new ExecutionContext(AsyncLocalValueMap.Empty, new IAsyncLocal[0], isFlowSuppressed: true)) :
                    null; // implies the default context
#pragma warning restore CA1825, IDE0300
            }

            return new ExecutionContext(m_localValues, m_localChangeNotifications, isFlowSuppressed);
        }

        public static AsyncFlowControl SuppressFlow()
        {
            Thread currentThread = Thread.CurrentThread;
            ExecutionContext? executionContext = currentThread._executionContext ?? Default;

            AsyncFlowControl asyncFlowControl = default;
            if (!executionContext.m_isFlowSuppressed)
            {
                currentThread._executionContext = executionContext.ShallowClone(isFlowSuppressed: true);
                asyncFlowControl.Initialize(currentThread);
            }

            return asyncFlowControl;
        }

        public static void RestoreFlow()
        {
            Thread currentThread = Thread.CurrentThread;
            ExecutionContext? executionContext = currentThread._executionContext;
            if (executionContext == null || !executionContext.m_isFlowSuppressed)
            {
                throw new InvalidOperationException(SR.InvalidOperation_CannotRestoreUnsuppressedFlow);
            }

            currentThread._executionContext = executionContext.ShallowClone(isFlowSuppressed: false);
        }

        public static bool IsFlowSuppressed()
        {
            ExecutionContext? executionContext = Thread.CurrentThread._executionContext;
            return executionContext != null && executionContext.m_isFlowSuppressed;
        }

        internal bool HasChangeNotifications => m_localChangeNotifications != null;

        internal bool IsDefault => m_isDefault;

        public static void Run(ExecutionContext executionContext, ContextCallback callback, object? state)
        {
            // Note: ExecutionContext.Run is an extremely hot function and used by every await, ThreadPool execution, etc.
            if (executionContext == null)
            {
                ThrowNullContext();
            }

            RunInternal(executionContext, callback, state);
        }

        internal static void RunInternal(ExecutionContext? executionContext, ContextCallback callback, object? state)
        {
            // Note: ExecutionContext.RunInternal is an extremely hot function and used by every await, ThreadPool execution, etc.
            // Note: Manual enregistering may be addressed by "Exception Handling Write Through Optimization"
            //       https://github.com/dotnet/runtime/blob/main/docs/design/coreclr/jit/eh-writethru.md

            // Enregister previousExecutionCtx0 so they can be used in registers without EH forcing them to stack

            Thread currentThread = Thread.CurrentThread;
            ExecutionContext? previousExecutionCtx0 = currentThread._executionContext;
            if (previousExecutionCtx0 != null && previousExecutionCtx0.m_isDefault)
            {
                // Default is a null ExecutionContext internally
                previousExecutionCtx0 = null;
            }

            ExecutionContext? previousExecutionCtx = previousExecutionCtx0;
            SynchronizationContext? previousSyncCtx = currentThread._synchronizationContext;

            if (executionContext != null && executionContext.m_isDefault)
            {
                // Default is a null ExecutionContext internally
                executionContext = null;
            }

            if (previousExecutionCtx != executionContext)
            {
                RestoreChangedContextToThread(currentThread, executionContext, previousExecutionCtx);
            }

            ExceptionDispatchInfo? edi = null;
            try
            {
                callback.Invoke(state);
            }
            catch (Exception ex)
            {
                // Note: we have a "catch" rather than a "finally" because we want
                // to stop the first pass of EH here.  That way we can restore the previous
                // context before any of our callers' EH filters run.
                edi = ExceptionDispatchInfo.Capture(ex);
            }

            // The common case is that these have not changed, so avoid the cost of a write barrier if not needed.
            if (currentThread._synchronizationContext != previousSyncCtx)
            {
                // Restore changed SynchronizationContext back to previous
                currentThread._synchronizationContext = previousSyncCtx;
            }

            ExecutionContext? currentExecutionCtx = currentThread._executionContext;
            if (currentExecutionCtx != previousExecutionCtx)
            {
                RestoreChangedContextToThread(currentThread, previousExecutionCtx, currentExecutionCtx);
            }

            // If exception was thrown by callback, rethrow it now original contexts are restored
            edi?.Throw();
        }

        /// <summary>
        /// Restores a captured execution context to on the current thread.
        /// </summary>
        /// <remarks>
        /// To revert to the current execution context; capture it before Restore, and Restore it again.
        /// It will not automatically be reverted unlike <see cref="Run"/>.
        /// </remarks>
        /// <param name="executionContext">The ExecutionContext to set.</param>
        /// <exception cref="InvalidOperationException"><paramref name="executionContext"/> is null.</exception>
        public static void Restore(ExecutionContext executionContext)
        {
            if (executionContext == null)
            {
                ThrowNullContext();
            }

            RestoreInternal(executionContext);
        }

        internal static void RestoreInternal(ExecutionContext? executionContext)
        {
            Thread currentThread = Thread.CurrentThread;

            ExecutionContext? currentExecutionCtx = currentThread._executionContext;
            if (currentExecutionCtx != null && currentExecutionCtx.m_isDefault)
            {
                // Default is a null ExecutionContext internally
                currentExecutionCtx = null;
            }

            if (executionContext != null && executionContext.m_isDefault)
            {
                // Default is a null ExecutionContext internally
                executionContext = null;
            }

            if (currentExecutionCtx != executionContext)
            {
                RestoreChangedContextToThread(currentThread, executionContext, currentExecutionCtx);
            }
        }

        internal static void RunFromThreadPoolDispatchLoop(Thread threadPoolThread, ExecutionContext executionContext, ContextCallback callback, object state)
        {
            Debug.Assert(threadPoolThread == Thread.CurrentThread);
            CheckThreadPoolAndContextsAreDefault();
            // ThreadPool starts on Default Context so we don't need to save the "previous" state as we know it is Default (null)

            // Default is a null ExecutionContext internally
            if (executionContext != null && !executionContext.m_isDefault)
            {
                // Non-Default context to restore
                RestoreChangedContextToThread(threadPoolThread, contextToRestore: executionContext, currentContext: null);
            }

            ExceptionDispatchInfo? edi = null;
            try
            {
                callback.Invoke(state);
            }
            catch (Exception ex)
            {
                // Note: we have a "catch" rather than a "finally" because we want
                // to stop the first pass of EH here.  That way we can restore the previous
                // context before any of our callers' EH filters run.
                edi = ExceptionDispatchInfo.Capture(ex);
            }

            // Enregister threadPoolThread as it crossed EH, and use enregistered variable
            Thread currentThread = threadPoolThread;

            ExecutionContext? currentExecutionCtx = currentThread._executionContext;

            // Restore changed SynchronizationContext back to Default
            currentThread._synchronizationContext = null;
            if (currentExecutionCtx != null)
            {
                // The EC always needs to be reset for this overload, as it will flow back to the caller if it performs
                // extra work prior to returning to the Dispatch loop. For example for Task-likes it will flow out of await points
                RestoreChangedContextToThread(currentThread, contextToRestore: null, currentExecutionCtx);
            }

            // If exception was thrown by callback, rethrow it now original contexts are restored
            edi?.Throw();
        }

        internal static void RunForThreadPoolUnsafe<TState>(ExecutionContext executionContext, Action<TState> callback, in TState state)
        {
            // We aren't running in try/catch as if an exception is directly thrown on the ThreadPool either process
            // will crash or its a ThreadAbortException.

            CheckThreadPoolAndContextsAreDefault();
            Debug.Assert(executionContext != null && !executionContext.m_isDefault, "ExecutionContext argument is Default.");

            // Restore Non-Default context
            Thread.CurrentThread._executionContext = executionContext;
            if (executionContext.HasChangeNotifications)
            {
                OnValuesChanged(previousExecutionCtx: null, executionContext);
            }

            callback.Invoke(state);

            // ThreadPoolWorkQueue.Dispatch will handle notifications and reset EC and SyncCtx back to default
        }

        internal static void RestoreChangedContextToThread(Thread currentThread, ExecutionContext? contextToRestore, ExecutionContext? currentContext)
        {
            Debug.Assert(currentThread == Thread.CurrentThread);
            Debug.Assert(contextToRestore != currentContext);

            // Restore changed ExecutionContext back to previous
            currentThread._executionContext = contextToRestore;
            if ((currentContext != null && currentContext.HasChangeNotifications) ||
                (contextToRestore != null && contextToRestore.HasChangeNotifications))
            {
                // There are change notifications; trigger any affected
                OnValuesChanged(currentContext, contextToRestore);
            }
        }

        // Inline as only called in one place and always called
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void ResetThreadPoolThread(Thread currentThread)
        {
            ExecutionContext? currentExecutionCtx = currentThread._executionContext;

            // Reset to defaults
            currentThread._synchronizationContext = null;
            currentThread._executionContext = null;

            if (currentExecutionCtx != null && currentExecutionCtx.HasChangeNotifications)
            {
                OnValuesChanged(currentExecutionCtx, nextExecutionCtx: null);

                // Reset to defaults again without change notifications in case the Change handler changed the contexts
                currentThread._synchronizationContext = null;
                currentThread._executionContext = null;
            }
        }

        [Conditional("DEBUG")]
        internal static void CheckThreadPoolAndContextsAreDefault()
        {
            Debug.Assert(!Thread.IsThreadStartSupported || Thread.CurrentThread.IsThreadPoolThread); // there are no dedicated threadpool threads on runtimes where we can't start threads
            Debug.Assert(Thread.CurrentThread._executionContext == null, "ThreadPool thread not on Default ExecutionContext.");
            Debug.Assert(Thread.CurrentThread._synchronizationContext == null, "ThreadPool thread not on Default SynchronizationContext.");
        }

        internal static void OnValuesChanged(ExecutionContext? previousExecutionCtx, ExecutionContext? nextExecutionCtx)
        {
            Debug.Assert(previousExecutionCtx != nextExecutionCtx);

            // Collect Change Notifications
            IAsyncLocal[]? previousChangeNotifications = previousExecutionCtx?.m_localChangeNotifications;
            IAsyncLocal[]? nextChangeNotifications = nextExecutionCtx?.m_localChangeNotifications;

            // At least one side must have notifications
            Debug.Assert(previousChangeNotifications != null || nextChangeNotifications != null);

            // Fire Change Notifications
            try
            {
                if (previousChangeNotifications != null && nextChangeNotifications != null)
                {
                    // Notifications can't exist without values
                    Debug.Assert(previousExecutionCtx!.m_localValues != null);
                    Debug.Assert(nextExecutionCtx!.m_localValues != null);
                    // Both contexts have change notifications, check previousExecutionCtx first
                    foreach (IAsyncLocal local in previousChangeNotifications)
                    {
                        previousExecutionCtx.m_localValues.TryGetValue(local, out object? previousValue);
                        nextExecutionCtx.m_localValues.TryGetValue(local, out object? currentValue);

                        if (previousValue != currentValue)
                        {
                            local.OnValueChanged(previousValue, currentValue, contextChanged: true);
                        }
                    }

                    if (nextChangeNotifications != previousChangeNotifications)
                    {
                        // Check for additional notifications in nextExecutionCtx
                        foreach (IAsyncLocal local in nextChangeNotifications)
                        {
                            // If the local has a value in the previous context, we already fired the event
                            // for that local in the code above.
                            if (!previousExecutionCtx.m_localValues.TryGetValue(local, out object? previousValue))
                            {
                                nextExecutionCtx.m_localValues.TryGetValue(local, out object? currentValue);
                                if (previousValue != currentValue)
                                {
                                    local.OnValueChanged(previousValue, currentValue, contextChanged: true);
                                }
                            }
                        }
                    }
                }
                else if (previousChangeNotifications != null)
                {
                    // Notifications can't exist without values
                    Debug.Assert(previousExecutionCtx!.m_localValues != null);
                    // No current values, so just check previous against null
                    foreach (IAsyncLocal local in previousChangeNotifications)
                    {
                        previousExecutionCtx.m_localValues.TryGetValue(local, out object? previousValue);
                        if (previousValue != null)
                        {
                            local.OnValueChanged(previousValue, null, contextChanged: true);
                        }
                    }
                }
                else // Implied: nextChangeNotifications != null
                {
                    // Notifications can't exist without values
                    Debug.Assert(nextExecutionCtx!.m_localValues != null);
                    // No previous values, so just check current against null
                    foreach (IAsyncLocal local in nextChangeNotifications!)
                    {
                        nextExecutionCtx.m_localValues.TryGetValue(local, out object? currentValue);
                        if (currentValue != null)
                        {
                            local.OnValueChanged(null, currentValue, contextChanged: true);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Environment.FailFast(
                    SR.ExecutionContext_ExceptionInAsyncLocalNotification,
                    ex);
            }
        }

        [DoesNotReturn]
        [StackTraceHidden]
        private static void ThrowNullContext()
        {
            throw new InvalidOperationException(SR.InvalidOperation_NullContext);
        }

        internal static object? GetLocalValue(IAsyncLocal local)
        {
            ExecutionContext? current = Thread.CurrentThread._executionContext;
            if (current == null)
            {
                return null;
            }

            Debug.Assert(!current.IsDefault);
            Debug.Assert(current.m_localValues != null, "Only the default context should have null, and we shouldn't be here on the default context");
            current.m_localValues.TryGetValue(local, out object? value);
            return value;
        }

        internal static void SetLocalValue(IAsyncLocal local, object? newValue, bool needChangeNotifications)
        {
            ExecutionContext? current = Thread.CurrentThread._executionContext;

            object? previousValue = null;
            bool hadPreviousValue = false;
            if (current != null)
            {
                Debug.Assert(!current.IsDefault);
                Debug.Assert(current.m_localValues != null, "Only the default context should have null, and we shouldn't be here on the default context");

                hadPreviousValue = current.m_localValues.TryGetValue(local, out previousValue);
            }

            if (previousValue == newValue)
            {
                return;
            }

            // Regarding 'treatNullValueAsNonexistent: !needChangeNotifications' below:
            // - When change notifications are not necessary for this IAsyncLocal, there is no observable difference between
            //   storing a null value and removing the IAsyncLocal from 'm_localValues'
            // - When change notifications are necessary for this IAsyncLocal, the IAsyncLocal's absence in 'm_localValues'
            //   indicates that this is the first value change for the IAsyncLocal and it needs to be registered for change
            //   notifications. So in this case, a null value must be stored in 'm_localValues' to indicate that the IAsyncLocal
            //   is already registered for change notifications.
            IAsyncLocal[]? newChangeNotifications = null;
            IAsyncLocalValueMap newValues;
            bool isFlowSuppressed = false;
            if (current != null)
            {
                Debug.Assert(!current.IsDefault);
                Debug.Assert(current.m_localValues != null, "Only the default context should have null, and we shouldn't be here on the default context");

                isFlowSuppressed = current.m_isFlowSuppressed;
                newValues = current.m_localValues.Set(local, newValue, treatNullValueAsNonexistent: !needChangeNotifications);
                newChangeNotifications = current.m_localChangeNotifications;
            }
            else
            {
                // First AsyncLocal
                newValues = AsyncLocalValueMap.Create(local, newValue, treatNullValueAsNonexistent: !needChangeNotifications);
            }

            //
            // Either copy the change notification array, or create a new one, depending on whether we need to add a new item.
            //
            if (needChangeNotifications)
            {
                if (hadPreviousValue)
                {
                    Debug.Assert(newChangeNotifications != null);
                    Debug.Assert(Array.IndexOf(newChangeNotifications, local) >= 0);
                }
                else if (newChangeNotifications == null)
                {
                    newChangeNotifications = [local];
                }
                else
                {
                    int newNotificationIndex = newChangeNotifications.Length;
                    Array.Resize(ref newChangeNotifications, newNotificationIndex + 1);
                    newChangeNotifications[newNotificationIndex] = local;
                }
            }

            Thread.CurrentThread._executionContext =
                (!isFlowSuppressed && AsyncLocalValueMap.IsEmpty(newValues)) ?
                null : // No values, return to Default context
                new ExecutionContext(newValues, newChangeNotifications, isFlowSuppressed);

            if (needChangeNotifications)
            {
                local.OnValueChanged(previousValue, newValue, contextChanged: false);
            }
        }

        public ExecutionContext CreateCopy()
        {
            return this; // since CoreCLR's ExecutionContext is immutable, we don't need to create copies.
        }

        public void Dispose()
        {
            // For CLR compat only
        }
    }

    public struct AsyncFlowControl : IEquatable<AsyncFlowControl>, IDisposable
    {
        private Thread? _thread;

        internal void Initialize(Thread currentThread)
        {
            Debug.Assert(currentThread == Thread.CurrentThread);
            _thread = currentThread;
        }

        public void Undo()
        {
            if (_thread is null)
            {
                return;
            }

            if (Thread.CurrentThread != _thread)
            {
                throw new InvalidOperationException(SR.InvalidOperation_CannotUseAFCOtherThread);
            }

            // An async flow control cannot be undone when a different execution context is applied. The .NET Framework
            // mutates the execution context when its state changes, and only changes the instance when an execution context
            // is applied (for instance, through ExecutionContext.Run). The framework prevents a suppressed-flow execution
            // context from being applied by returning null from ExecutionContext.Capture, so the only type of execution
            // context that can be applied is one whose flow is not suppressed. After suppressing flow and changing an async
            // local's value, the .NET Framework verifies that a different execution context has not been applied by
            // checking the execution context instance against the one saved from when flow was suppressed. In .NET Core,
            // since the execution context instance will change after changing the async local's value, it verifies that a
            // different execution context has not been applied, by instead ensuring that the current execution context's
            // flow is suppressed.
            if (!ExecutionContext.IsFlowSuppressed())
            {
                throw new InvalidOperationException(SR.InvalidOperation_AsyncFlowCtrlCtxMismatch);
            }

            _thread = null;
            ExecutionContext.RestoreFlow();
        }

        public void Dispose()
        {
            Undo();
        }

        public override bool Equals([NotNullWhen(true)] object? obj)
        {
            return obj is AsyncFlowControl asyncControl && Equals(asyncControl);
        }

        public bool Equals(AsyncFlowControl obj)
        {
            return _thread == obj._thread;
        }

        public override int GetHashCode()
        {
            return _thread?.GetHashCode() ?? 0;
        }

        public static bool operator ==(AsyncFlowControl a, AsyncFlowControl b) => a.Equals(b);

        public static bool operator !=(AsyncFlowControl a, AsyncFlowControl b) => !(a == b);
    }
}
