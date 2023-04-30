// Copyright (c) Sergey A. Makovkin (makovkin.s@gmail.com). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

#define DEFAULT_CLIENT_TIMEOUT_MS 90000

/**
 * Wrapper for com.solana.mobilewalletadapter.common.protocol.MobileWalletAdapterSessionCommon 
 */
class FMobileWalletAdapterSessionCommon : public FJavaClassObjectEx
{
protected:
	FMobileWalletAdapterSessionCommon();
public:
	virtual ~FMobileWalletAdapterSessionCommon() override;
	static FMobileWalletAdapterSessionCommon* Construct();
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;
};

#endif
