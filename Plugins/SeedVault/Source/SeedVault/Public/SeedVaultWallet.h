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
 * The main class that is used to access Seed Vault.
 */
UCLASS()
class SEEDVAULT_API USeedVaultWallet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE(FSuccessDynDelegate);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FSuccessWithTokenDynDelegate, int64, AuthToken);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureDynDelegate, const FString&, ErrorMessage);

	UFUNCTION(BlueprintCallable, Category="Solana")
	static void AuthorizeSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure);	
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void CreateSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure);
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void ImportSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure);	

public:
	static FSuccessWithTokenDynDelegate AuthorizeSeedSuccess;
	static FFailureDynDelegate AuthorizeSeedFailure;
	static FSuccessWithTokenDynDelegate CreateSeedSuccess;
	static FFailureDynDelegate CreateSeedFailure;
	static FSuccessWithTokenDynDelegate ImportSeedSuccess;
	static FFailureDynDelegate ImportSeedFailure;	
};
