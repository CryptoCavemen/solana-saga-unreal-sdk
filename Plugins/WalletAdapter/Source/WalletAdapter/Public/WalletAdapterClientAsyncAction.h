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
	
	UFUNCTION(BlueprintCallable, Category = "Solana", meta = (BlueprintInternalUseOnly = "true"))
	static UWalletAdapterClientAsyncAction* Authorize(UWalletAdapterClient* Client, FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);
	UFUNCTION(BlueprintCallable, Category = "Solana", meta = (BlueprintInternalUseOnly = "true"))
	static UWalletAdapterClientAsyncAction* Reauthorize(UWalletAdapterClient* Client, FString IdentityUri, FString IconUri, FString IdentityName, FString AuthToken);
	UFUNCTION(BlueprintCallable, Category = "Solana", meta = (BlueprintInternalUseOnly = "true"))
	static UWalletAdapterClientAsyncAction* Deauthorize(UWalletAdapterClient* Client, FString AuthToken);	
	
	UFUNCTION()
	void OnSuccess();
	UFUNCTION()
	void OnError();

protected:
	void StartAuthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster);
	void StartReauthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& AuthToken);
	void StartDeauthorize(const FString& AuthToken);
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthorizeResult, UWalletAdapterClient*, Client);
	
	UPROPERTY(BlueprintAssignable)
	FAuthorizeResult Success;
	UPROPERTY(BlueprintAssignable)
	FAuthorizeResult Fail;

private:
	UPROPERTY()
	UWalletAdapterClient* Client;	 
};
