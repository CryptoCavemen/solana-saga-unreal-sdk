//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "LocalAssociationWithWallet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LocalAssociateWithWalletAsyncAction.generated.h"

UENUM(BlueprintType)
enum class ELocalAssociateWithWalletError : uint8
{
	None,
	NoCompatibleWallet,
	AssociationFailed,
	CloseFailed
};

/**
 * 
 */
UCLASS(MinimalAPI)
class ULocalAssociateWithWalletAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	ULocalAssociateWithWalletAsyncAction();
	
	UFUNCTION(BlueprintCallable, Category = "Solana", meta = (BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"))
	static ULocalAssociateWithWalletAsyncAction* LocalAssociateWithWallet(UObject* WorldContextObject, FString UriPrefix);
	UFUNCTION(BlueprintCallable, Category = "Solana", meta = (BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"))
	static ULocalAssociateWithWalletAsyncAction* CloseLocalAssociationWithWallet(UObject* WorldContextObject, ULocalAssociationWithWallet* LocalAssociation);	
	
	UFUNCTION()
	void OnSuccess();
	UFUNCTION()
	void OnError(ELocalAssociateWithWalletError InError);

protected:
	void Start(FString UriPrefix);
	void Close();
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLocalAssociateWithWalletResult, ULocalAssociationWithWallet*, LocalAssociation, ELocalAssociateWithWalletError, ErrorCode);
	
	UPROPERTY(BlueprintAssignable)
	FLocalAssociateWithWalletResult Success;
	UPROPERTY(BlueprintAssignable)
	FLocalAssociateWithWalletResult Fail;

private:
	UPROPERTY()
	ULocalAssociationWithWallet* LocalAssociation;	 
};
