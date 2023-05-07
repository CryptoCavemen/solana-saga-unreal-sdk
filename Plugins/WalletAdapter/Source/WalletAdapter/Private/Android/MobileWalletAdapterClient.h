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
	TSharedPtr<FFuture> Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster, TSharedPtr<FThrowable>* OutException = nullptr);
	/** Reauthorizes a client. Returns FAuthorizationResult future. */
	TSharedPtr<FFuture> Reauthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& AuthToken, TSharedPtr<FThrowable>* OutException = nullptr);
	/** Deauthorizes a client. */
	TSharedPtr<FFuture> Deauthorize(const FString& AuthToken, TSharedPtr<FThrowable>* OutException = nullptr);
protected:
	FJavaClassMethod AuthorizeMethod;
	FJavaClassMethod ReauthorizeMethod;
	FJavaClassMethod DeauthorizeMethod;
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