//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "MobileWalletAdapterClient.h"
#include "WalletAdapter.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

#include "Android/JavaUtils.h"


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient", "(I)V", int ClientTimeoutMs)
	AuthorizeMethod = GetClassMethod("authorize",
		"(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;");
	ReauthorizeMethod = GetClassMethod("reauthorize",
		"(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;");
	DeauthorizeMethod = GetClassMethod("deauthorize",
		"(Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$DeauthorizeFuture;");
	GetCapabilitiesMethod = GetClassMethod("getCapabilities",
		"()Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$GetCapabilitiesFuture;");
	SignTransactionsMethod = GetClassMethod("signTransactions",
		"([[B)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignPayloadsFuture;");
	SignMessagesDetachedMethod = GetClassMethod("signMessagesDetached",
		"([[B[[B)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignMessagesFuture;");
	SignAndSendTransactionsMethod = GetClassMethod("signAndSendTransactions",
		"([[BLjava/lang/Integer;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignAndSendTransactionsFuture;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

TSharedPtr<FFuture> FMobileWalletAdapterClient::Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Authorizing client: IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', Cluster = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *Cluster);

	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, AuthorizeMethod,
		!IdentityUri.IsEmpty() ? *FJavaUtils::GetJUri(IdentityUri) : nullptr,
		!IconUri.IsEmpty() ? *FJavaUtils::GetJUri(IconUri) : nullptr,
		!IdentityName.IsEmpty() ? *FJavaUtils::GetJString(IdentityName) : nullptr,
		!Cluster.IsEmpty() ? *FJavaUtils::GetJString(Cluster) : nullptr);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;		
			
	return RetVal ? FFuture::CreateFromExisting(RetVal) : TSharedPtr<FFuture>();
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::Reauthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& AuthToken, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Reauthorizing client: IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', AuthToken = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *AuthToken);

	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, ReauthorizeMethod,
		!IdentityUri.IsEmpty() ? *FJavaUtils::GetJUri(IdentityUri) : nullptr,
		!IconUri.IsEmpty() ? *FJavaUtils::GetJUri(IconUri) : nullptr,
		!IdentityName.IsEmpty() ? *FJavaUtils::GetJString(IdentityName) : nullptr,
		!AuthToken.IsEmpty() ? *FJavaUtils::GetJString(AuthToken) : nullptr);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;		
	
	return RetVal ? FFuture::CreateFromExisting(RetVal) : TSharedPtr<FFuture>();
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::Deauthorize(const FString& AuthToken, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Deauthorizing client: AuthToken = '%s'"), *AuthToken);
	
	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, DeauthorizeMethod, !AuthToken.IsEmpty() ? *FJavaUtils::GetJString(AuthToken) : nullptr);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	
	return RetVal ? FFuture::CreateFromExisting(RetVal) : TSharedPtr<FFuture>();
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::SignTransactions(const TArray<TArray<uint8>>& Transactions, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Signing %d transactions"), Transactions.Num());
	
	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, SignTransactionsMethod, *FJavaUtils::GetArrayOfByteArray(Transactions));
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	
	return RetVal ? FFuture::CreateFromExisting(RetVal) : TSharedPtr<FFuture>();	
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::SignAndSendTransactions(const TArray<TArray<uint8>>& Transactions, const int32* MinContextSlot, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Signing and sending %d transactions"), Transactions.Num());
	
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	jobject JMinContextSlot = nullptr;
	if (MinContextSlot)
	{
		jclass IntegerClass = Env->FindClass("java/lang/Integer");
		jmethodID IntegerCtor = Env->GetMethodID(IntegerClass, "<init>", "(I)V");
		JMinContextSlot = Env->NewObject(IntegerClass, IntegerCtor, *MinContextSlot);
	}

	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, SignAndSendTransactionsMethod, *FJavaUtils::GetArrayOfByteArray(Transactions), JMinContextSlot);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	
	return RetVal ? FFuture::CreateFromExisting(RetVal) : TSharedPtr<FFuture>();		
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::SignMessagesDetached(const TArray<TArray<uint8>>& Messages, const TArray<TArray<uint8>>& Addresses, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Signing %d messages"), Messages.Num());
	
	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, SignMessagesDetachedMethod, *FJavaUtils::GetArrayOfByteArray(Messages), *FJavaUtils::GetArrayOfByteArray(Addresses));
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	
	return RetVal ? FFuture::CreateFromExisting(RetVal) : TSharedPtr<FFuture>();		
}


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FAuthorizationResultWrapper, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationResult",
                                  "(Ljava/lang/String;[BLjava/lang/String;Landroid/net/Uri;)V",
                                  const FString& AuthToken, const TArray<uint8>& PublicKey, const FString& AccountLabel, const FString& WalletUriBase)
	AuthTokenField = GetClassField("authToken", "Ljava/lang/String;");
	PublicKeyField = GetClassField("publicKey", "[B");
	AccountLabelField = GetClassField("accountLabel", "Ljava/lang/String;");
	WalletUriBaseField = GetClassField("walletUriBase", "Landroid/net/Uri;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

FString FAuthorizationResultWrapper::GetAuthToken() const
{
	return GetStringField(AuthTokenField);
}

TArray<uint8> FAuthorizationResultWrapper::GetPublicKey() const
{
	return GetByteArrayField(PublicKeyField);
}

FString FAuthorizationResultWrapper::GetAccountLabel() const
{
	return GetStringField(AccountLabelField);
}

FString FAuthorizationResultWrapper::GetWalletUriBase() const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	auto JUri = FScopedJavaObject(Env, GetObjectField(WalletUriBaseField));
	FString Uri = FJavaUtils::JUriToString(*JUri);
	return Uri;
}


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FSignPayloadsResultWrapper, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignPayloadsResult",
                                  "([[B)V",
                                  const TArray<TArray<uint8>>& SignedPayloads)
	SignedPayloadsField = GetClassField("signedPayloads", "[[B");
END_IMPLEMENT_JAVA_CLASS_OBJECT

TArray<TArray<uint8>> FSignPayloadsResultWrapper::GetSignedPayloads() const
{
	return GetArrayOfByteArrayField(SignedPayloadsField);
}


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FSignAndSendTransactionsResultWrapper, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignAndSendTransactionsResult",
								  "([[B)V",
								  const TArray<TArray<uint8>>& SignedPayloads)
	SignaturesField = GetClassField("signatures", "[[B");
END_IMPLEMENT_JAVA_CLASS_OBJECT

TArray<TArray<uint8>> FSignAndSendTransactionsResultWrapper::GetSignatures() const
{
	return GetArrayOfByteArrayField(SignaturesField);
}


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FSignedMessageWrapper, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignMessagesResult$SignedMessage",
								  "([B[[B[[B)V",
								  const TArray<uint8>& Message, const TArray<TArray<uint8>>& Signatures, const TArray<TArray<uint8>>& Addresses)
	MessageField = GetClassField("message", "[B");
	SignaturesField = GetClassField("signatures", "[[B");
	AddressesField = GetClassField("addresses", "[[B");
END_IMPLEMENT_JAVA_CLASS_OBJECT

TArray<uint8> FSignedMessageWrapper::GetMessage() const
{
	return GetByteArrayField(MessageField);
}

TArray<TArray<uint8>> FSignedMessageWrapper::GetSignatures() const
{
	return GetArrayOfByteArrayField(SignaturesField);
}

TArray<TArray<uint8>> FSignedMessageWrapper::GetAddresses() const
{
	return GetArrayOfByteArrayField(AddressesField);
}


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FSignMessagesResultWrapper, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignMessagesResult",
								  "([Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignMessagesResult$SignedMessage;)V",
								  const TArray<FSignedMessage>& Messages)
	MessagesField = GetClassField("messages", "[Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$SignMessagesResult$SignedMessage;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

TArray<TSharedRef<FSignedMessageWrapper>> FSignMessagesResultWrapper::GetMessages() const
{
	TArray<TSharedRef<FSignedMessageWrapper>> Messages;
	
	TArray<jobject> JObjectArray = GetObjectArrayField(MessagesField);
	for (jobject JObject : JObjectArray)
		 Messages.Add(FSignedMessageWrapper::CreateFromExisting(JObject));
	
	return Messages;
}

#endif