// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
#include "Defines.h"

#define DEFAULT_CLIENT_TIMEOUT_MS 90000

/**
 * Wrapper for com.solana.mobilewalletadapter.clientlib.scenario.Scenario 
 */
class FScenario : public FJavaClassObjectEx
{
	DECLARE_JAVA_CLASS_OBJECT(FScenario, int32 ClientTimeoutMs);
public:
};

#endif