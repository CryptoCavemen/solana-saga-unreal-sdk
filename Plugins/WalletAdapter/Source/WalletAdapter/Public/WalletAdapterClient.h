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
	
	FSolanaTransaction() {}
	FSolanaTransaction(const TArray<uint8>& InData) : Data(InData) {}
};

/**
 * 
 */
UCLASS(BlueprintType)
class WALLETADAPTER_API UWalletAdapterClient : public UObject
{
	GENERATED_BODY()
	
public:
#if PLATFORM_ANDROID
	void SetClientImpl(const TSharedPtr<FMobileWalletAdapterClient>& InClient);
#endif

public:
	DECLARE_DELEGATE(FSuccessDelegate);
	DECLARE_DELEGATE_OneParam(FAuthSuccessDelegate, const FString& AuthToken);
	DECLARE_DELEGATE_OneParam(FSignSuccessDelegate, const TArray<FSolanaTransaction>& Transactions);
	DECLARE_DELEGATE(FSignAndSendSuccessDelegate);
	DECLARE_DELEGATE_OneParam(FFailureDelegate, const FString& ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE(FSuccessDynDelegate);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FAuthSuccessDynDelegate, const FString&, AuthToken);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FSignSuccessDynDelegate, const TArray<FSolanaTransaction>&, Transactions);
	DECLARE_DYNAMIC_DELEGATE(FSignAndSendSuccessDynDelegate);	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureDynDelegate, const FString&, ErrorMessage);
	
public:
	/** Authorizes a client. */
	void Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster, const FAuthSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Reauthorizes a client. */
	void Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, FString AuthorizationToken, const FAuthSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Deauthorizes a client. */
	void Deauthorize(const FString& AuthorizationToken, const FSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Signs transactions. */
	void SignTransactions(const TArray<FSolanaTransaction>& Transactions, const FSignSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Signs and sends transactions. */
	void SignAndSendTransactions(const TArray<FSolanaTransaction>& Transactions, int32 MinContextSlot, const FSignAndSendSuccessDelegate& Success, const FFailureDelegate& Failure);

public:
	/** Authorizes a client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Authorize", ScriptName="Authorize"), Category="Solana")
	void K2_Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster, const FAuthSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	/** Reauthorizes a client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Reauthorize", ScriptName="Reauthorize"), Category="Solana")
	void K2_Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, FString AuthorizationToken, const FAuthSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	/** Deauthorizes a client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Deauthorize", ScriptName="Deauthorize"), Category="Solana")
	void K2_Deauthorize(FString AuthorizationToken, const FSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	/** Signs transactions. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="SignTransactions", ScriptName="SignTransactions"), Category="Solana")
	void K2_SignTransactions(const TArray<FSolanaTransaction>& Transactions, const FSignSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);
	/** Signs and sends transactions. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="SignAndSendTransactions", ScriptName="SignAndSendTransactions"), Category="Solana")
	void K2_SignAndSendTransactions(const TArray<FSolanaTransaction>& Transactions, int32 MinContextSlot, const FSignAndSendSuccessDynDelegate& Success, const FFailureDynDelegate& Failure);	

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
