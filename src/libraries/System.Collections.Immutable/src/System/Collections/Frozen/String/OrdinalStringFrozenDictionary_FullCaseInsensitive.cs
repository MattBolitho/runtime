﻿// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Collections.Generic;

namespace System.Collections.Frozen
{
    internal sealed partial class OrdinalStringFrozenDictionary_FullCaseInsensitive<TValue> : OrdinalStringFrozenDictionary<TValue>
    {
        private readonly ulong _lengthFilter;

        internal OrdinalStringFrozenDictionary_FullCaseInsensitive(
            string[] keys,
            TValue[] values,
            IEqualityComparer<string> comparer,
            int minimumLength,
            int maximumLengthDiff,
            ulong lengthFilter)
            : base(keys, values, comparer, minimumLength, maximumLengthDiff)
        {
            _lengthFilter = lengthFilter;
        }

        // See comment in OrdinalStringFrozenDictionary for why these overrides exist. Do not remove.
        private protected override ref readonly TValue GetValueRefOrNullRefCore(string key) => ref base.GetValueRefOrNullRefCore(key);

        private protected override bool Equals(string? x, string? y) => StringComparer.OrdinalIgnoreCase.Equals(x, y);
        private protected override bool Equals(ReadOnlySpan<char> x, string? y) => x.Equals(y.AsSpan(), StringComparison.OrdinalIgnoreCase);
        private protected override int GetHashCode(string s) => Hashing.GetHashCodeOrdinalIgnoreCase(s.AsSpan());
        private protected override int GetHashCode(ReadOnlySpan<char> s) => Hashing.GetHashCodeOrdinalIgnoreCase(s);
        private protected override bool CheckLengthQuick(uint length) => (_lengthFilter & (1UL << (int)(length % 64))) > 0;
    }
}
