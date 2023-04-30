// Copyright (c) Sergey A. Makovkin (makovkin.s@gmail.com). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MobileWalletAdapterSessionCommon.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

#define DEFAULT_CLIENT_TIMEOUT_MS 90000

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterSession 
 */
class FMobileWalletAdapterSession : public FMobileWalletAdapterSessionCommon
{
protected:
	FMobileWalletAdapterSession();
public:
	virtual ~FMobileWalletAdapterSession() override;
	static FMobileWalletAdapterSession* Construct();
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;
};

#endif
