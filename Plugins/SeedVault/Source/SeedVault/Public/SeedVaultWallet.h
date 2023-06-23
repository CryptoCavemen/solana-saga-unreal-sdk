//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "Android/Defines.h"
#include "Data.h"
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
 * FSigningResponse
 */
USTRUCT(BlueprintType)
struct FSigningResponse
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FByteArray> Signatures;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> ResolvedDerivationPaths;
};

/**
 * FPublicKeyResponse
 */
USTRUCT(BlueprintType)
struct FPublicKeyResponse
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> PublicKey;
	UPROPERTY(BlueprintReadWrite)
	FString PublicKeyEncoded;
	UPROPERTY(BlueprintReadWrite)
	FString ResolvedDerivationPath;
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
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetPublicKeysSuccessDynDelegate, TArray<FPublicKeyResponse>, PublicKeyResponses);
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

	/** Request the public key for a given BipDerivationPath of a seed. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void RequestPublicKey(int64 AuthToken, const FString& DerivationPaths,
		const FGetPublicKeysSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	
	/** Request the public keys for a set of BipDerivationPaths of a seed. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static void RequestPublicKeys(int64 AuthToken, const TArray<FString>& DerivationPaths,
		const FGetPublicKeysSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);	
		
	/** Deauthorize the specified seed for the current app. */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static bool DeauthorizeSeed(int64 AuthToken);

	/** Test whether there are any unauthorized seeds with the specified purpose available for the current app */
	UFUNCTION(BlueprintCallable, Category="Solana")
	static bool HasUnauthorizedSeedsForPurpose(EWalletContractV1 Purpose);
	
	/** Resolve the provided BIP derivation path Uri with scheme
	  * WalletContractV1#BIP32_URI_SCHEME or WalletContractV1#BIP44_URI_SCHEME and
	  * the provided @code WalletContractV1.PURPOSE_* purpose to a BIP32 derivation path.*/
	UFUNCTION(BlueprintCallable, Category="Solana")
	static FString ResolveDerivationPath(const FString& DerivationPath, EWalletContractV1 Purpose);

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
	static FGetPublicKeysSuccessDynDelegate GetPublicKeysSuccess;
	static FFailureDynDelegate GetPublicKeysFailure;	
};
