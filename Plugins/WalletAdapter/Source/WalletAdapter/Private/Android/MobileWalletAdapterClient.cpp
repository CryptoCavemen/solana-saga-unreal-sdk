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
END_IMPLEMENT_JAVA_CLASS_OBJECT

TSharedRef<FFuture> FMobileWalletAdapterClient::Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster)
{
	UE_LOG(LogWalletAdapter, Log, TEXT("Authorizing client: IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', Cluster = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *Cluster);
	
	jobject RetVal = CallMethod<jobject>(AuthorizeMethod,
		!IdentityUri.IsEmpty() ? *GetJUri(IdentityUri) : nullptr,
		!IconUri.IsEmpty() ? *GetJUri(IconUri) : nullptr,
		!IdentityName.IsEmpty() ? *GetJString(IdentityName) : nullptr,
		!Cluster.IsEmpty() ? *GetJString(Cluster) : nullptr);
	
	return FFuture::MakeFromExistingObject(RetVal);
}

#endif
