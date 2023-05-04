#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Future.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.Scenario 
 */
class FMobileWalletAdapterClient : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FMobileWalletAdapterClient, int32 ClientTimeoutMs);
public:
	TSharedRef<FFuture> Authorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster);
	void OnAuthorizeInternal(bool bSuccess);
protected:
	FJavaClassMethod AuthorizeMethod;	
};

#endif