#include "MobileWalletAdapterClient.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"


BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, FJavaClassObjectEx, "com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient", "(I)V", int ClientTimeoutMs)
	AuthorizeMethod = GetClassMethod("authorize",
		"(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

jobject FMobileWalletAdapterClient::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
	UE_LOG(LogAndroid, Verbose, TEXT("Authorize(): IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', Cluster = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *Cluster);

	bool bExceptionThrown;
	jobject AuthorizationFuture = CallThrowableMethod<jobject>(bExceptionThrown, AuthorizeMethod, *GetJUri(IdentityUri), *GetJUri(IconUri), *GetJString(IdentityName), *GetJString(Cluster));
	return AuthorizationFuture;
}

void FMobileWalletAdapterClient::OnAuthorizeInternal(bool bSuccess)
{
}

#endif