// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

namespace System.Security.Cryptography.X509Certificates
{
    internal sealed partial class AppleCertificatePal : ICertificatePal
    {
        public DSA? GetDSAPrivateKey()
        {
            if (_identityHandle == null)
                return null;

            throw new PlatformNotSupportedException();
        }

        public ICertificatePal CopyWithPrivateKey(DSA privateKey)
        {
            throw new PlatformNotSupportedException();
        }

        public ICertificatePal CopyWithPrivateKey(ECDsa privateKey)
        {
            return ImportPkcs12(this, privateKey);
        }

        public ICertificatePal CopyWithPrivateKey(ECDiffieHellman privateKey)
        {
            return ImportPkcs12(this, privateKey);
        }

        public ICertificatePal CopyWithPrivateKey(MLDsa privateKey)
        {
            throw new PlatformNotSupportedException(
                SR.Format(SR.Cryptography_AlgorithmNotSupported, nameof(MLDsa)));
        }

        public ICertificatePal CopyWithPrivateKey(RSA privateKey)
        {
            return ImportPkcs12(this, privateKey);
        }
    }
}
