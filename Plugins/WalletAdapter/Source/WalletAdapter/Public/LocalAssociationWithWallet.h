//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LocalAssociationWithWallet.generated.h"

// __RESHARPER__ is only defined while in IDE. Used to help editing this file with proper highlighting.
#ifdef __RESHARPER__
	#define PLATFORM_ANDROID 1
	#define USE_ANDROID_JNI 1
#endif

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
	void K2_Start(const FStartSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	/** Closes the local association. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Close", ScriptName="Close"), Category = "Solana")
	void K2_Close(const FCloseSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
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
