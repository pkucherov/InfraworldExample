// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Queue.h"
#include "ChannelCredentials.h"
#include "Runnable.h"

#include <memory>
#include <chrono>

//namespace grpc
//{
//    class Channel;
//}

/**
 * Base RPC Client Worker, it 'lives' in a separate thread and updates all conduits with responses.
 */
class INFRAWORLDRUNTIME_API RpcClientWorker : public FRunnable
{
public:
    RpcClientWorker();
	virtual ~RpcClientWorker();

    virtual uint32 Run() override;

    FORCEINLINE bool IsPendingStopped() const
    {
        return bPendingStopped;
    }
    
    FORCEINLINE void MarkPendingStopped()
    {
        bPendingStopped = true;
    }

    virtual bool HierarchicalInit() = 0;
	virtual void HierarchicalUpdate() = 0;
    
    void DispatchError(const FString& ErrorMessage);
    
//public:
    FRpcClientInstantiationParameters DispatcherParameters;
    TQueue<FRpcError>* ErrorMessageQueue;

private:
    // False by default. Being set to true when the thread is need to be shut down.
	volatile bool bPendingStopped;
};
