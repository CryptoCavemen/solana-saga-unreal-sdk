#pragma once

#include "CoreMinimal.h"
#include "Scenario.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterClient 
 */
class FMobileWalletAdapterClient : public FJavaClassObjectEx
{
protected:
	FMobileWalletAdapterClient();
public:
	virtual ~FMobileWalletAdapterClient() override;
	static FMobileWalletAdapterClient* Construct(int32 ClientTimeoutMs = DEFAULT_CLIENT_TIMEOUT_MS, ...);
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;

public:
	jobject Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster);

protected:
	void OnAuthorizeInternal(bool bSuccess);

private:
	FJavaClassMethod AuthorizeMethod;
};

#endif