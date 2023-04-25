#include "MobileWalletAdapterClient.h"

UMobileWalletAdapterClient::UMobileWalletAdapterClient()
{
#if PLATFORM_ANDROID
	if (!HasAnyFlags(RF_ClassDefaultObject))
		Wrapper = MakeShared<FMobileWalletAdapterClientWrapper>(10000);
#endif
}

void UMobileWalletAdapterClient::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
#if PLATFORM_ANDROID
	Wrapper->Authorize(IdentityUri, IconUri, IdentityName, Cluster);
#endif
}
