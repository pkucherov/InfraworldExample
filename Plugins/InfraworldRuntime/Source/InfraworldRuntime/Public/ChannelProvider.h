
#pragma once

#include <grpc++/channel.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "RpcClientWorker.h"
#include "ChannelCredentials.h"


namespace channel
{
    bool WaitUntilChannelIsReady(const std::shared_ptr<grpc::Channel>& Channel, std::chrono::system_clock::time_point Deadline)
    {
        grpc_connectivity_state State = Channel->GetState(true);
        
        while (State != GRPC_CHANNEL_READY)
        {
            if (!Channel->WaitForStateChange(State, Deadline))
                return false;
            
            State = Channel->GetState(true);
        }
        
        return true;
    }
    
    bool WaitForConnection(float Seconds, const std::shared_ptr<grpc::Channel>& Channel)
    {
        bool IsConnected = false;
        
        const int64 Milliseconds = (int64)((double)Seconds * 1000.0);
        
        std::chrono::system_clock::time_point start_tp = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point end_tp = std::chrono::system_clock::now() + std::chrono::milliseconds(Milliseconds);
        
        std::chrono::system_clock::time_point current_tp = start_tp;
        
        while (!IsConnected)
        {
            std::chrono::system_clock::time_point delta_tp = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
            
            if (current_tp < end_tp)
                IsConnected = WaitUntilChannelIsReady(Channel, delta_tp);
            else
                break;
            
            current_tp = std::chrono::system_clock::now();
        }
        
        return IsConnected;
    }
    
    std::shared_ptr<grpc::ChannelCredentials> GetGrpcCredentials(UChannelCredentials* const Credentials)
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
            
            Options.pem_cert_chain = TCHAR_TO_ANSI(*(SslCredentials->PemRootCerts));
            Options.pem_private_key = TCHAR_TO_ANSI(*(SslCredentials->PemPrivateKey));
            Options.pem_root_certs = TCHAR_TO_ANSI(*(SslCredentials->PemCertChain));
            
            return grpc::SslCredentials(Options);
        }
        
        // Unknown credentials
        UE_LOG(LogTemp, Error, TEXT("Don't know how to process credentials:'%s'. Replacement is grpc::InsecureChannelCredentials()."), *(Credentials->GetClass()->GetName()));
        return grpc::InsecureChannelCredentials();
    }
    
    std::shared_ptr<grpc::Channel> CreateChannel(RpcClientWorker* Worker)
    {
        UChannelCredentials* const ChannelCredentials = Worker->DispatcherParameters.ChannelCredentials;
        UE_CLOG(!ChannelCredentials, LogTemp, Fatal, TEXT("Channel Credentials mustn't be null"));

        const FString& ChannelAddess = Worker->DispatcherParameters.GetURI();
        UE_LOG(LogTemp, Display, TEXT("The following Channel Credentials is used: %s. Connecting to: %s"), *(ChannelCredentials->GetName()), *ChannelAddess);

        //std::shared_ptr<grpc::Channel> Channel = grpc::CreateChannel(TCHAR_TO_UTF8(*ChannelAddess), UChannelCredentials::GetGrpcCredentials(ChannelCredentials));
        
        std::shared_ptr<grpc::ChannelCredentials> GrpcCredentials = GetGrpcCredentials(ChannelCredentials);
        std::shared_ptr<grpc::Channel> Channel = grpc::CreateChannel(TCHAR_TO_ANSI(*ChannelAddess), GrpcCredentials);

        bool b = WaitForConnection(3, Channel);
        UE_LOG(LogTemp, Display, TEXT(""));
        
        if (!b)
        {
            Worker->DispatchError(TEXT("Connection failure"));
            return std::shared_ptr<grpc::Channel>(nullptr);
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Connection established!"));
        }
        
        return Channel;
    }
}
