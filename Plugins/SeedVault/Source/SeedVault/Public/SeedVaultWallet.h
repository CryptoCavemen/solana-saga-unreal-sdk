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
 * FSignature
 */
USTRUCT(BlueprintType)
struct FSignatureByteArray
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> Data;
	
	FSignatureByteArray() {}
	FSignatureByteArray(const TArray<uint8>& InData) : Data(InData) {}
};

/**
 * FSigningResponse
 */
USTRUCT(BlueprintType)
struct FSigningResponse
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FSignatureByteArray> Signatures;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> ResolvedDerivationPaths;
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
	DECLARE_DYNAMIC_DELEGATE_OneParam(FSignSuccessDynDelegate, TArray<FSigningResponse>, SigningResponses);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureDynDelegate, const FString&, ErrorMessage);

	/** Request authorization of a new seed for the specified purpose. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void AuthorizeSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure);

	/** Request creation of a new seed for the specified purpose. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void CreateSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure);

	/** Request import of an existing seed for the specified purpose. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void ImportSeed(EWalletContractV1 Purpose, const FSuccessWithTokenDynDelegate& Success, const FFailureDynDelegate& Failure);

	/** Request that the provided transaction be signed. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void SignTransaction(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Transaction,
		const FSignSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	
	/** Request that the provided transaction be signed. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void SignMessage(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Message,
		const FSignSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);	
		
	/** Deauthorize the specified seed for the current app. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static bool DeauthorizeSeed(int64 AuthToken);

	/** Test whether there are any unauthorized seeds with the specified purpose available for the current app */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static bool HasUnauthorizedSeedsForPurpose(EWalletContractV1 Purpose);	

public:
	static FSuccessWithTokenDynDelegate AuthorizeSeedSuccess;
	static FFailureDynDelegate AuthorizeSeedFailure;
	static FSuccessWithTokenDynDelegate CreateSeedSuccess;
	static FFailureDynDelegate CreateSeedFailure;
	static FSuccessWithTokenDynDelegate ImportSeedSuccess;
	static FFailureDynDelegate ImportSeedFailure;
	static FSignSuccessDynDelegate SignTransactionsSuccess;
	static FFailureDynDelegate SignTransactionsFailure;
	static FSignSuccessDynDelegate SignMessagesSuccess;
	static FFailureDynDelegate SignMessagesFailure;
};
