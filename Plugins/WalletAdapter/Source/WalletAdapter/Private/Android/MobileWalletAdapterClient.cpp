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

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient", "(I)V", int ClientTimeoutMs)
	AuthorizeMethod = GetClassMethod("authorize",
		"(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;");
	ReauthorizeMethod = GetClassMethod("reauthorize",
		"(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;");
	DeauthorizeMethod = GetClassMethod("deauthorize",
		"(Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$DeauthorizeFuture;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

TSharedPtr<FFuture> FMobileWalletAdapterClient::Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Authorizing client: IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', Cluster = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *Cluster);

	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, AuthorizeMethod,
		!IdentityUri.IsEmpty() ? *GetJUri(IdentityUri) : nullptr,
		!IconUri.IsEmpty() ? *GetJUri(IconUri) : nullptr,
		!IdentityName.IsEmpty() ? *GetJString(IdentityName) : nullptr,
		!Cluster.IsEmpty() ? *GetJString(Cluster) : nullptr);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;		
			
	return RetVal ? FFuture::MakeFromExistingObject(RetVal) : TSharedPtr<FFuture>();
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::Reauthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& AuthToken, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Reauthorizing client: IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', AuthToken = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *AuthToken);

	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, ReauthorizeMethod,
		!IdentityUri.IsEmpty() ? *GetJUri(IdentityUri) : nullptr,
		!IconUri.IsEmpty() ? *GetJUri(IconUri) : nullptr,
		!IdentityName.IsEmpty() ? *GetJString(IdentityName) : nullptr,
		!AuthToken.IsEmpty() ? *GetJString(AuthToken) : nullptr);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;		
	
	return RetVal ? FFuture::MakeFromExistingObject(RetVal) : TSharedPtr<FFuture>();
}

TSharedPtr<FFuture> FMobileWalletAdapterClient::Deauthorize(const FString& AuthToken, TSharedPtr<FThrowable>& OutException)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Deauthorizing client: AuthToken = '%s'"), *AuthToken);
	
	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, DeauthorizeMethod, !AuthToken.IsEmpty() ? *GetJString(AuthToken) : nullptr);
	OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	
	return RetVal ? FFuture::MakeFromExistingObject(RetVal) : TSharedPtr<FFuture>();
}


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FAuthorizationResult, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationResult",
		"(Ljava/lang/String;[BLjava/lang/String;Landroid/net/Uri;)V",
		const FString& AuthToken, const TArray<uint8>& PublicKey, const FString& AccountLabel, const FString& WalletUriBase)
	AuthTokenField = GetClassField("authToken", "Ljava/lang/String;");
	PublicKeyField = GetClassField("publicKey", "[B");
	AccountLabelField = GetClassField("accountLabel", "Ljava/lang/String;");
	WalletUriBaseField = GetClassField("walletUriBase", "Landroid/net/Uri;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

FString FAuthorizationResult::GetAuthToken()
{
	return GetStringField(AuthTokenField);
}

TArray<uint8> FAuthorizationResult::GetPublicKey()
{
	return GetByteArrayField(PublicKeyField);
}

FString FAuthorizationResult::GetAccountLabel()
{
	return GetStringField(AccountLabelField);
}

FString FAuthorizationResult::GetWalletUriBase()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	auto JUri = FScopedJavaObject(Env, GetObjectField(WalletUriBaseField));
	FString Uri = JUriToString(*JUri);
	return Uri;
}

#endif
