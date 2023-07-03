//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Android/Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Android/Future.h"
	
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
	/** Returns capabilities of the device. */
	TSharedPtr<FFuture> GetCapabilities(TSharedPtr<FThrowable>& OutException);
	/** Sign transactions. */
	TSharedPtr<FFuture> SignTransactions(const TArray<TArray<uint8>>& Transactions, TSharedPtr<FThrowable>& OutException);
	/** Sign and send transactions. MinContextSlot might be null. */
	TSharedPtr<FFuture> SignAndSendTransactions(const TArray<TArray<uint8>>& Transactions, const int32* MinContextSlot, TSharedPtr<FThrowable>& OutException);
	/** Sign messages. */
	TSharedPtr<FFuture> SignMessagesDetached(const TArray<TArray<uint8>>& Messages, const TArray<TArray<uint8>>& Addresses, TSharedPtr<FThrowable>& OutException);	

protected:
	FJavaClassMethod AuthorizeMethod;
	FJavaClassMethod ReauthorizeMethod;
	FJavaClassMethod DeauthorizeMethod;
	FJavaClassMethod GetCapabilitiesMethod;
	FJavaClassMethod SignTransactionsMethod;
	FJavaClassMethod SignMessagesDetachedMethod;
	FJavaClassMethod SignAndSendTransactionsMethod;
};

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$GetCapabilitiesResult
 */
class FGetCapabilitiesResultWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FGetCapabilitiesResultWrapper);
public:
	bool GetSupportsCloneAuthorization() const;
	bool GetSupportsSignAndSendTransactions() const;
	int32 GetMaxTransactionsPerSigningRequest() const;
	int32 GetMaxMessagesPerSigningRequestFieldRequest() const;
	TArray<FJavaClassObjectWrapperRef> GetSupportedTransactionVersions() const;
protected:
	FJavaClassField SupportsCloneAuthorizationField;
	FJavaClassField SupportsSignAndSendTransactionsField;
	FJavaClassField MaxTransactionsPerSigningRequestField;
	FJavaClassField MaxMessagesPerSigningRequestField;
	FJavaClassField SupportedTransactionVersionsField;
};


/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$AuthorizationResult
 */
class FAuthorizationResultWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FAuthorizationResultWrapper, const FString& AuthToken, const TArray<uint8>& PublicKey, const FString& AccountLabel, const FString& WalletUriBase);
public:
	FString GetAuthToken() const;
	TArray<uint8> GetPublicKey() const;
	FString GetAccountLabel() const;
	FString GetWalletUriBase() const;
protected:
	FJavaClassField AuthTokenField;
	FJavaClassField PublicKeyField;
	FJavaClassField AccountLabelField;
	FJavaClassField WalletUriBaseField;
};

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$SignPayloadsResult
 */
class FSignPayloadsResultWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FSignPayloadsResultWrapper, const TArray<TArray<uint8>>& SignedPayloads);
public:
	TArray<TArray<uint8>> GetSignedPayloads() const;
protected:
	FJavaClassField SignedPayloadsField;
};

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$SignAndSendTransactionsResult
 */
class FSignAndSendTransactionsResultWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FSignAndSendTransactionsResultWrapper, const TArray<TArray<uint8>>& SignedPayloads);
public:
	TArray<TArray<uint8>> GetSignatures() const;
protected:
	FJavaClassField SignaturesField;
};

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$SignMessagesResult$SignedMessage
 */
class FSignedMessageWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FSignedMessageWrapper, const TArray<uint8>& Message, const TArray<TArray<uint8>>& Signatures, const TArray<TArray<uint8>>& Addresses);
public:
	TArray<uint8> GetMessage() const;
	TArray<TArray<uint8>> GetSignatures() const;
	TArray<TArray<uint8>> GetAddresses() const;
protected:
	FJavaClassField MessageField;
	FJavaClassField SignaturesField;
	FJavaClassField AddressesField;
};

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient$SignMessagesResult
 */
class FSignMessagesResultWrapper : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FSignMessagesResultWrapper, const TArray<FSignedMessage>& Messages);
public:
	TArray<TSharedRef<FSignedMessageWrapper>> GetMessages() const;
protected:
	FJavaClassField MessagesField;
};


#endif