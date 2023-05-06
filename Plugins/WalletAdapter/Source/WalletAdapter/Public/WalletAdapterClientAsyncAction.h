//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "LocalAssociationWithWallet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WalletAdapterClientAsyncAction.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UWalletAdapterClientAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UWalletAdapterClientAsyncAction();
	
	UFUNCTION(BlueprintCallable, Category = "Solana", meta = (BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"))
	static UWalletAdapterClientAsyncAction* Authorize(UObject* WorldContextObject, UWalletAdapterClient* Client, FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);
	
	UFUNCTION()
	void OnSuccess();
	UFUNCTION()
	void OnError();

protected:
	void Start(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster);	
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthorizeResult, UWalletAdapterClient*, Client);
	
	UPROPERTY(BlueprintAssignable)
	FAuthorizeResult Success;
	UPROPERTY(BlueprintAssignable)
	FAuthorizeResult Fail;

private:
	UPROPERTY()
	UObject* WorldContextObject;
	UPROPERTY()
	UWalletAdapterClient* Client;	 
};
