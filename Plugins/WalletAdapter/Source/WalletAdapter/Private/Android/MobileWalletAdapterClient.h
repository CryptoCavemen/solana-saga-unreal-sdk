#pragma once

#include "CoreMinimal.h"
#include "Scenario.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient 
 */
class FMobileWalletAdapterClient : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, int32 ClientTimeoutMs);
public:
	jobject Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

protected:
	void OnAuthorizeInternal(bool bSuccess);

private:
	FJavaClassMethod AuthorizeMethod;
};

#endif