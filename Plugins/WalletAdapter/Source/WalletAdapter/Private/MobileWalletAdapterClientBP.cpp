#include "MobileWalletAdapterClientBP.h"
#include "Android/Scenario.h"

UMobileWalletAdapterClientBP::UMobileWalletAdapterClientBP()
{
#if PLATFORM_ANDROID
	if (!HasAnyFlags(RF_ClassDefaultObject))
		Wrapper = FMobileWalletAdapterClient::Construct(nullptr, DEFAULT_CLIENT_TIMEOUT_MS);
	else
		Wrapper = nullptr;
#endif
}

UMobileWalletAdapterClientBP::~UMobileWalletAdapterClientBP()
{
#if PLATFORM_ANDROID	
	if (Wrapper)
		delete Wrapper;
#endif
}

void UMobileWalletAdapterClientBP::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
#if PLATFORM_ANDROID
	Wrapper->Authorize(IdentityUri, IconUri, IdentityName, Cluster);
#endif
}
