//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif
#include "SeedVaultWallet.generated.h"

UENUM(BlueprintType)
enum class EWalletContractV1 : uint8
{
	PURPOSE_SIGN_SOLANA_TRANSACTION = 0,
};

/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
UCLASS()
class SEEDVAULT_API USeedVaultWallet : public UBlueprintFunctionLibrary
{
	friend class FSeedVaultModule;
	GENERATED_BODY()
protected:
	/** Initialize java objects and cache them for further usage. Called when the module is loaded. */
	static void StaticConstruct();

public:

	DECLARE_DYNAMIC_DELEGATE(FSuccessDynDelegate);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateSeedDynDelegate, int64, AuthToken);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureDynDelegate, const FString&, ErrorMessage);
	
	UFUNCTION(BlueprintCallable)
	static void CreateSeed(EWalletContractV1 Purpose, const FCreateSeedDynDelegate& Success, const FFailureDynDelegate& Failure);
	static void OnCreateSeedSuccess(int64 AuthToken);
	static void OnCreateSeedFailure(const FString& ErrorMessage);

protected:
	
	static FCreateSeedDynDelegate CreateSeedSuccess;
	static FFailureDynDelegate CreateSeedFailure;
};
