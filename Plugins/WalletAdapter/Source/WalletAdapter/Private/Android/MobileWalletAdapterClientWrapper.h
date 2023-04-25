#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient 
 */
class FMobileWalletAdapterClientWrapper : public FJavaClassObjectEx
{
public:
	FMobileWalletAdapterClientWrapper(int32 ClientTimeoutMs);
	~FMobileWalletAdapterClientWrapper();
	
	jobject Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

protected:
	static FName GetClassName();
	void OnAuthorizeInternal(bool bSuccess);

private:
	FJavaClassMethod AuthorizeMethod;
};

#endif