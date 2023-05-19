//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LocalAssociationWithWallet.generated.h"

class UWalletAdapterClient;

#if PLATFORM_ANDROID
class FLocalAssociationScenario;
class FFuture;
#endif

/**
 * ULocalAssociationScenarioBP
 */
UCLASS(BlueprintType)
class WALLETADAPTER_API ULocalAssociationWithWallet : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_DELEGATE_OneParam(FSuccessStartCallback, UWalletAdapterClient*, Client);
	DECLARE_DYNAMIC_DELEGATE(FSuccessCloseCallback);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailCallback, const FString&, ErrorMessage);	

public:
	UFUNCTION(BlueprintCallable, Category = "Solana")
	bool OpenWallet(const FString& UriPrefix);
	/** Starts a local association with a wallet. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void Start(const FSuccessStartCallback& Success, const FFailCallback& Fail);
	/** Closes the local association. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void Close(const FSuccessCloseCallback& Success, const FFailCallback& Fail);
	/** Returns a mobile wallet adapter client. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	UWalletAdapterClient* GetMobileWalletAdapterClient();	

protected:
#if PLATFORM_ANDROID
	TSharedPtr<FLocalAssociationScenario> LocalAssociation;
#endif
	UPROPERTY()
	UWalletAdapterClient* Client;
};
