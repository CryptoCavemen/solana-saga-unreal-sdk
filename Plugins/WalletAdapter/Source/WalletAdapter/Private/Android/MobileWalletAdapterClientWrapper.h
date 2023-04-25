#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient 
 */
class FMobileWalletAdapterClientWrapper : public FJavaClassObject
{
public:
	FMobileWalletAdapterClientWrapper(int32 ClientTimeoutMs);
	~FMobileWalletAdapterClientWrapper();
	
	jobject Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

protected:
	static FName GetClassName();
	void OnAuthorizeInternal();

private:
	FJavaClassMethod AuthorizeMethod;
};

#endif