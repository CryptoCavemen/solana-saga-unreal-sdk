//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data.h"
#include "WalletAdapterClient.generated.h"

#if PLATFORM_ANDROID
class FMobileWalletAdapterClient;
class FSignedMessageWrapper;
#endif


/**
 * FSignedMessage
 */
USTRUCT(BlueprintType)
struct FSignedMessage
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> Message;	
	UPROPERTY(BlueprintReadOnly)
	TArray<FByteArray> Signatures;
	UPROPERTY(BlueprintReadOnly)
	TArray<FByteArray> Addresses;
	
	FSignedMessage() {}
	
#if PLATFORM_ANDROID
	FSignedMessage(const FSignedMessageWrapper& SignedMessage);
#endif	
};


/**
 * UWalletAdapterClient
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
	DECLARE_DELEGATE_OneParam(FSignSuccessDelegate, const TArray<FByteArray>& Transactions);
	DECLARE_DELEGATE_OneParam(FSignMessagesSuccessDelegate, const TArray<FSignedMessage>& SignedMessages);
	DECLARE_DELEGATE_OneParam(FFailureDelegate, const FString& ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE(FSuccessDynDelegate);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FAuthSuccessDynDelegate, const FString&, AuthToken);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FSignSuccessDynDelegate, const TArray<FByteArray>&, Transactions);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FSignMessagesSuccessDynDelegate, const TArray<FSignedMessage>&, SignedMessages);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFailureDynDelegate, const FString&, ErrorMessage);
	
public:
	/** Authorizes a client. */
	void Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster, const FAuthSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Reauthorizes a client. */
	void Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, FString AuthorizationToken, const FAuthSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Deauthorizes a client. */
	void Deauthorize(const FString& AuthorizationToken, const FSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Signs transactions. */
	void SignTransactions(const TArray<FByteArray>& Transactions, const FSignSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Signs and sends transactions. */
	void SignAndSendTransactions(const TArray<FByteArray>& Transactions, int32 MinContextSlot, const FSignSuccessDelegate& Success, const FFailureDelegate& Failure);
	/** Signs messages. */
	void SignMessagesDetached(const TArray<FByteArray>& Messages, const TArray<FByteArray>& Addresses, const FSignMessagesSuccessDelegate& Success, const FFailureDelegate& Failure);	

public:
	/** Authorizes a client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Authorize", ScriptName="Authorize"), Category="Solana")
	void K2_Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster, FAuthSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Reauthorizes a client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Reauthorize", ScriptName="Reauthorize"), Category="Solana")
	void K2_Reauthorize(FString IdentityUri, FString IconUri, FString IdentityName, FString AuthorizationToken, FAuthSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Deauthorizes a client. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Deauthorize", ScriptName="Deauthorize"), Category="Solana")
	void K2_Deauthorize(FString AuthorizationToken, FSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Signs transactions. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="SignTransactions", ScriptName="SignTransactions"), Category="Solana")
	void K2_SignTransactions(const TArray<FByteArray>& Transactions, FSignSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Signs and sends transactions. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="SignAndSendTransactions", ScriptName="SignAndSendTransactions"), Category="Solana")
	void K2_SignAndSendTransactions(const TArray<FByteArray>& Transactions, int32 MinContextSlot, FSignSuccessDynDelegate Success, FFailureDynDelegate Failure);
	/** Signs messages. */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="SignMessagesDetached", ScriptName="SignMessagesDetached"), Category="Solana")
	void K2_SignMessagesDetached(const TArray<FByteArray>& Messages, const TArray<FByteArray>& Addresses, FSignMessagesSuccessDynDelegate Success, FFailureDynDelegate Failure);	

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
