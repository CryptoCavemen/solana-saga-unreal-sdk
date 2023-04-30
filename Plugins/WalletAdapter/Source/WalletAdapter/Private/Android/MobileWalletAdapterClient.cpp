#include "MobileWalletAdapterClient.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

FMobileWalletAdapterClient::FMobileWalletAdapterClient()
{
}

FMobileWalletAdapterClient::~FMobileWalletAdapterClient()
{
}

FMobileWalletAdapterClient* FMobileWalletAdapterClient::Construct(int32 ClientTimeoutMs, ...)
{
	FMobileWalletAdapterClient* Object = new FMobileWalletAdapterClient();
	
	va_list Args;
	va_start(Args, ClientTimeoutMs);
	Object->PostConstruct("com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient", "(I)V", Args);
	va_end(Args);
	
	return Object;	
}

void FMobileWalletAdapterClient::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	FJavaClassObjectEx::PostConstruct(ClassName, CtorSig, Args);
	AuthorizeMethod = GetClassMethod("authorize", "(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;");	
}

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