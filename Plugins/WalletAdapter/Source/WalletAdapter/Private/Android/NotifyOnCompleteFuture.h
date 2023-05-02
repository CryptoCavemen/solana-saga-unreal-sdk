// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Future.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Defines.h"

/**
 * Wrapper for com.solana.mobilewalletadapter.common.util.NotifyOnCompleteFuture 
 */
class FNotifyOnCompleteFuture : public FFuture
{
	DECLARE_JAVA_CLASS_OBJECT(FNotifyOnCompleteFuture);
public:
protected:
};


#endif