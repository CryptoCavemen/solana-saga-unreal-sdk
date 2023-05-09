//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.Scenario 
 */
class FScenario : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FScenario, int32 ClientTimeoutMs);
public:
};

#endif