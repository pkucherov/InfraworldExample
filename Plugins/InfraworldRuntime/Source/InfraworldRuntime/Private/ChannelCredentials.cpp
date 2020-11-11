// Fill out your copyright notice in the Description page of Project Settings.

#include "InfraworldRuntime.h"
#include "ChannelCredentials.h"

#include <grpc++/security/credentials.h>

// Since GRPC actively uses winapi, we need to forbid windows macros
// (such as GetMessage, MemoryBarrier, etc.) in our remaining code.
// To do it, we 'wrap' all the C++ file's including ANY GRPC header files
// content into Allow/Hide WindowsPlatformTypes.
// We're unable to 'isolate' the WinAPI usage within a single C++ file thanks to Unity Build.
#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#endif

std::shared_ptr<grpc::ChannelCredentials> UChannelCredentials::GetGrpcCredentials(UChannelCredentials* const Credentials)
{
    // Check whether provided credentails are null.
    if (!Credentials)
    {
        UE_LOG(LogTemp, Error, TEXT("Provided credentials are NULL. (Did you forget to pass ChannelCredentials to instantiation parameters?). Replacement is grpc::InsecureChannelCredentials()."));
        return grpc::InsecureChannelCredentials();
    }

    // Classify the credentials
    if (Credentials->IsA<UGoogleDefaultCredentials>())
    {
        return grpc::GoogleDefaultCredentials();
    }
    else if (Credentials->IsA<UInsecureChannelCredentials>())
    {
        return grpc::InsecureChannelCredentials();
    }
    else if (const USslCredentials* const SslCredentials = Cast<USslCredentials>(Credentials))
    {
        grpc::SslCredentialsOptions Options;

        Options.pem_cert_chain = TCHAR_TO_UTF8(*(SslCredentials->PemRootCerts));
        Options.pem_private_key = TCHAR_TO_UTF8(*(SslCredentials->PemPrivateKey));
        Options.pem_root_certs = TCHAR_TO_UTF8(*(SslCredentials->PemCertChain));

        return grpc::SslCredentials(Options);
    }
    
    // Unknown credentials
    UE_LOG(LogTemp, Error, TEXT("Don't know how to process credentials:'%s'. Replacement is grpc::InsecureChannelCredentials()."), *(Credentials->GetClass()->GetName()));
    return grpc::InsecureChannelCredentials();
}

UChannelCredentials* UChannelCredentials::MakeGoogleDefaultCredentials()
{
    return NewObject<UGoogleDefaultCredentials>();
}

UChannelCredentials* UChannelCredentials::MakeSslCredentials(FString PemRootCerts, FString PemPrivateKey, FString PemCertChain)
{
    USslCredentials* const SslCredentials = NewObject<USslCredentials>();

    SslCredentials->PemRootCerts = PemRootCerts;
    SslCredentials->PemPrivateKey = PemPrivateKey;
    SslCredentials->PemCertChain = PemCertChain;

    return SslCredentials;
}

UChannelCredentials* UChannelCredentials::MakeInsecureChannelCredentials()
{
    return NewObject<UInsecureChannelCredentials>();
}

#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif
