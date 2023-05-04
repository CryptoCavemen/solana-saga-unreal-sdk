//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Throwable.h"
#include "Defines.h"

/**
 * Wrapper for com.epicgames.ue4.GameActivity 
 */
class FGameActivity : FJavaClassObjectWrapper	
{
	DECLARE_JAVA_CLASS_OBJECT(FGameActivity);

public:
	void StartActivity(const FJavaClassObjectWrapperRef& Intent, TSharedPtr<FThrowable>* OutException = nullptr);
	void StartActivityForResult(const FJavaClassObjectWrapperRef& Intent, int32 RequestCode, TSharedPtr<FThrowable>* OutException = nullptr);
	
protected:
	FJavaClassMethod StartActivityMethod;
	FJavaClassMethod StartActivityForResultMethod;
};


#endif