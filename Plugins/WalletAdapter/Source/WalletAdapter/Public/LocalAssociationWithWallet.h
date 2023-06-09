﻿//
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
#endif

/**
 * ULocalAssociationScenarioBP
 */
UCLASS(BlueprintType)
class WALLETADAPTER_API ULocalAssociationWithWallet : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FStartSuccessDelegate, UWalletAdapterClient* Client);
	DECLARE_DELEGATE(FCloseSuccessDelegate);
	DECLARE_DELEGATE_OneParam(FFailureDelegate, const FString& ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FStartSuccessDynDelegate, UWalletAdapterClient*, Client);
	DECLARE_DYNAMIC_DELEGATE(FCloseSuccessDynDelegate);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureDynDelegate, const FString&, ErrorMessage);	

public:
	/** Starts a local association with a wallet. */
	void Start(const FStartSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Closes the local association. */	
	void Close(const FCloseSuccessDelegate& Success, const FFailureDelegate& Failure);	
	
public:
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Open Wallet", ScriptName="OpenWallet"), Category = "Solana")
	bool OpenWallet(const FString& UriPrefix);
	/** Starts a local association with a wallet. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Start", ScriptName="Start"), Category = "Solana")
	void K2_Start(FStartSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Closes the local association. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Close", ScriptName="Close"), Category = "Solana")
	void K2_Close(FCloseSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Returns a mobile wallet adapter client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Get WalletAdapterClient", ScriptName="GetWalletAdapterClient"), Category = "Solana")
	UWalletAdapterClient* GetWalletAdapterClient();
	
protected:
#if PLATFORM_ANDROID
	TSharedPtr<FLocalAssociationScenario> LocalAssociation;
#endif
	UPROPERTY()
	UWalletAdapterClient* Client;
};
