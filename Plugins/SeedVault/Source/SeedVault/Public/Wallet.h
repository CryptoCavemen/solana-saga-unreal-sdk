// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif
#include "Defines.h"
#include "Wallet.generated.h"

UENUM(BlueprintType)
enum class EWalletContractV1 : uint8
{
	PURPOSE_SIGN_SOLANA_TRANSACTION = 0,
};

/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
UCLASS()
class SEEDVAULT_API UWallet : public UBlueprintFunctionLibrary
{
	friend class FSeedVaultModule;
	GENERATED_BODY()
protected:
	/** Initialize java objects and cache them for further usage. Called when the module is loaded. */
	static void StaticConstruct();

#if PLATFORM_ANDROID
	static void StartActivityForResult(jobject Intent, int32 RequestCode);
#endif
	
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FCreateSeedCallback, bool, bSuccess, int64, AuthToken);
	UFUNCTION(BlueprintCallable)
	static void CreateSeed(FCreateSeedCallback FinishCallback, EWalletContractV1 Purpose = EWalletContractV1::PURPOSE_SIGN_SOLANA_TRANSACTION);
	static void OnCreateSeed(bool bSuccess, int64 AuthToken);

protected:
	
	static FCreateSeedCallback OnCreateSeedCallback;
};