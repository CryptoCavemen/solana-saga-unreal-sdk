#include "MobileWalletAdapterClientWrapper.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

FMobileWalletAdapterClientWrapper::FMobileWalletAdapterClientWrapper(int32 ClientTimeoutMs)
	: FJavaClassObjectEx(GetClassName(), "(I)V", ClientTimeoutMs)
	, AuthorizeMethod(GetClassMethod("authorize", "(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;"))
{
}

FMobileWalletAdapterClientWrapper::~FMobileWalletAdapterClientWrapper()
{
}

jobject FMobileWalletAdapterClientWrapper::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
	UE_LOG(LogAndroid, Verbose, TEXT("Authorize(): IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', Cluster = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *Cluster);

	bool bExceptionThrown;
	jobject AuthorizationFuture = CallThrowableMethod<jobject>(bExceptionThrown, AuthorizeMethod, *GetJUri(IdentityUri), *GetJUri(IconUri), *GetJString(IdentityName), *GetJString(Cluster));
	return AuthorizationFuture;
}

FName FMobileWalletAdapterClientWrapper::GetClassName()
{
	return FName("com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient");
}


void FMobileWalletAdapterClientWrapper::OnAuthorizeInternal(bool bSuccess)
{
}

#endif