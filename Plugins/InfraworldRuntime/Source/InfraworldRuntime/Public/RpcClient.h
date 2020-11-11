// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


#include "Queue.h"
#include "SubclassOf.h"

#include "RpcClientWorker.h"
#include "ChannelCredentials.h"

#include "RpcClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRpcErrorSignature, URpcClient*, Dispatcher, const FRpcError&, Error);

/**
 * An RPC client used to interact with GRPC services from Blueprints and UE-compatible C++ code.
 * Being used as a base class for generated RPC clients, an RPC Client contains a set of methods to manage an RPC Channel.
 *
 * @note You should never instantiate it directly.
 *
 * You should use a Proto2Cpp converter to create GRPC wrappers for an every single service.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class INFRAWORLDRUNTIME_API URpcClient : public UObject
{
	GENERATED_BODY()
	
    bool Init(const FRpcClientInstantiationParameters& StartupParameters);
    
public:
	URpcClient();
	virtual ~URpcClient();
    
    // Being called in implementations
    virtual void HierarchicalInit() PURE_VIRTUAL(URpcClient::HierarchicalInit,);
    
    // Being called in implementations
    virtual void HierarchicalUpdate() PURE_VIRTUAL(URpcClient::HierarchicalUpdate,);
    
    /**
     * Stops and disables this instance of RPC Client.
     * This operation is irreversible, you can't either send or receive requests and responses after doing so.
     *
     * @param bSynchronous
     *        Stop and wait a sec. Oh when you look at me like that my darling - What did you expect?
     *        If seriously - you should think thousand times before unchecking this option, because
     *        you can experience heavy and unpredictable racing hazard.
     */
    UFUNCTION(BlueprintCallable, Category="Vizor|RPC Client")
    void Stop(bool bSynchronous = true);
    
    /**
     * An update function, used to tell a Client, when to check queries and dispatch messages.
     * In widget (for example) you should call it every update (or redraw/invalidate).
     *
     * @note that frequency, you're calling this method won't ever affect the speed of message processing.
     */
    UFUNCTION(BlueprintCallable, Category="Vizor|RPC Client")
    void Update();
    
    /**
     * Checks whether the RPC Client could send requests.
     *
     * @return True if the RPC Client is properly initialized and can send requests. If not - all requests will be ignored.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Vizor|RPC Client", meta=(DisplayName="Can Send Requests?"))
    bool CanSendRequests() const;
    
    /**
     * Instantiates a new RPC Dispatcher. You should use this function, not 'Construct Object from Class', to properly initialize the instance.
     *
     * @param Class
     *        A subclass of RPC Client, that will be instantiated.
     *        Please don't select an abstract 'Rpc Client'
     * @param InstantiationParameters
     *        Parameters, will be used for instantiation,
     * @param Outer
     *        An outer object, None (or empty) is valid as well, if so,
     *        GetTransientPackage() will be used to retrieve a static outer.
     * @return A newly created instance of RPC Client.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="Vizor|RPC Client", meta=(DisplayName="Create RPC Client", DeterminesOutputType="Class"))
    static URpcClient* CreateRpcClient(TSubclassOf<URpcClient> Class, FRpcClientInstantiationParameters InstantiationParameters, UObject* Outer = nullptr);
    
    /**
     * Called when did received any kind of error.
     */
    UPROPERTY(BlueprintAssignable, Category="Vizor|RPC Client", meta=(DisplayName="Event RPC Error"))
    FRpcErrorSignature EventRpcError;

protected:
    /** A pointer to an inner RpcClientWorker sending and receiving messages */
    TUniquePtr<RpcClientWorker> InnerWorker;
    
private:
    /** Whether the RPC Client could send requests or not */
    bool bCanSendRequests = false;
    
    /** A thread, where RPC client worker will reside */
    FRunnableThread* Thread = nullptr;
    
    /** An accumulator for error messages */
    TQueue<FRpcError> ErrorMessageQueue;
};

template <class T>
FORCEINLINE T* NewRpcClient(const FRpcClientInstantiationParameters& InstantiationParameters, UObject* Outer = nullptr)
{
    static_assert(TIsDerivedFrom<T, URpcClient>::IsDerived, "T must derive URpcClient");
    static_assert(!TIsSame<T, URpcClient>::Value, "T must derive URpcClient, but mustn't be a bare URpcClient");
    
    if (T* const Result = Cast<T>(URpcClient::CreateRpcClient(T::StaticClass(), InstantiationParameters, Outer)))
    {
        return Result;
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Can not create RPC Client. Instantiation error."));
        return nullptr;
    }
}
