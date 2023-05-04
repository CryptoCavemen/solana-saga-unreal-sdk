//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "MobileWalletAdapterSessionCommon.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Defines.h"

#define DEFAULT_CLIENT_TIMEOUT_MS 90000

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.protocol.MobileWalletAdapterSession 
 */
class FMobileWalletAdapterSession : public FMobileWalletAdapterSessionCommon
{
	DECLARE_JAVA_CLASS_OBJECT(FMobileWalletAdapterSession);
};

#endif
