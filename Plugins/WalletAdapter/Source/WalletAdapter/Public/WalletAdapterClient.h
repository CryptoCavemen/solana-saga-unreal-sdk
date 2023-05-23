//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WalletAdapterClient.generated.h"

class FMobileWalletAdapterClient;

USTRUCT(BlueprintType)
struct FSolanaTransaction
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> Data;
};

/**
 * 
 */
UCLASS(BlueprintType)
class WALLETADAPTER_API UWalletAdapterClient : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_DELEGATE(FSuccessCallback);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FAuthSuccessCallback, const FString&, AuthToken);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailCallback, const FString&, ErrorMessage);
	
public:
#if PLATFORM_ANDROID
	void SetClientImpl(const TSharedPtr<FMobileWalletAdapterClient>& InClient);
#endif

public:
	/** Authorizes a client. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster, const FAuthSuccessCallback& Success, const FFailCallback& Fail);
	/** Reauthorizes a client. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, const FAuthSuccessCallback& Success, const FFailCallback& Fail);
	/** Deauthorizes a client. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void Deauthorize(const FSuccessCallback& Success, const FFailCallback& Fail);
	/** Signs transactions. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void SignTransactions(const TArray<FSolanaTransaction>& Transactions, const FSuccessCallback& Success, const FFailCallback& Fail);
	/** Signs a transaction. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void SignTransaction(const FSolanaTransaction& Transaction, const FSuccessCallback& Success, const FFailCallback& Fail);
	/** Signs and sends transactions. */
	UFUNCTION(BlueprintCallable, Category = "Solana")
	void SignAndSendTransactions(const TArray<FSolanaTransaction>& Transactions, int32 MinContextSlot, const FSuccessCallback& Success, const FFailCallback& Fail);	

public:
	UPROPERTY(BlueprintReadOnly)
	FString AuthToken;
	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> PublicKey;
	UPROPERTY(BlueprintReadOnly)
	FString AccountLabel;
	UPROPERTY(BlueprintReadOnly)
	FString WalletUriBase;
	
protected:
#if PLATFORM_ANDROID
	TSharedPtr<FMobileWalletAdapterClient> Client;
#endif
};
