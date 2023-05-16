//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Future.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient
 */
class FMobileWalletAdapterClient : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, int32 ClientTimeoutMs);
public:
	/** Authorizes a client. Returns FAuthorizationResult future. */
	TSharedPtr<FFuture> Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster, TSharedPtr<FThrowable>& OutException);
	/** Reauthorizes a client. Returns FAuthorizationResult future. */
	TSharedPtr<FFuture> Reauthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& AuthToken, TSharedPtr<FThrowable>& OutException);
	/** Deauthorizes a client. */
	TSharedPtr<FFuture> Deauthorize(const FString& AuthToken, TSharedPtr<FThrowable>& OutException);
	/** Sign transactions. */
	TSharedPtr<FFuture> SignTransactions(const TArray<TArray<uint8>>& Transactions, TSharedPtr<FThrowable>& OutException);
	/** Sign and send transactions. MinContextSlot might be null. */
	TSharedPtr<FFuture> SignAndSendTransactions(const TArray<TArray<uint8>>& Transactions, const int32* MinContextSlot, TSharedPtr<FThrowable>& OutException);	
protected:
	FJavaClassMethod AuthorizeMethod;
	FJavaClassMethod ReauthorizeMethod;
	FJavaClassMethod DeauthorizeMethod;
	FJavaClassMethod GetCapabilitiesMethod;
	FJavaClassMethod SignTransactionsMethod;
	FJavaClassMethod SignMessagesMethod;
	FJavaClassMethod SignMessagesDetachedMethod;
	FJavaClassMethod SignAndSendTransactionsMethod;
};

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$AuthorizationResult
 */
class FAuthorizationResult : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FAuthorizationResult, const FString& AuthToken, const TArray<uint8>& PublicKey, const FString& AccountLabel, const FString& WalletUriBase);
public:
	FString GetAuthToken();
	TArray<uint8> GetPublicKey();
	FString GetAccountLabel();
	FString GetWalletUriBase();
protected:
	FJavaClassField AuthTokenField;
	FJavaClassField PublicKeyField;
	FJavaClassField AccountLabelField;
	FJavaClassField WalletUriBaseField;
};

#endif