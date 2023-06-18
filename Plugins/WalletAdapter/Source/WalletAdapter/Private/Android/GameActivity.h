//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Throwable.h"

namespace WalletAdapter
{
	
/**
 * Wrapper for com.epicgames.ue4.GameActivity 
 */
class FGameActivity : FJavaClassObjectWrapper	
{
	DECLARE_JAVA_CLASS_OBJECT(FGameActivity);

public:
	FJavaClassObjectWrapperRef GetApplicationContext();
	FJavaClassObjectWrapperRef GetApplication();
	void StartActivity(const FJavaClassObjectWrapperRef& Intent, TSharedPtr<FThrowable>* OutException = nullptr);
	void StartActivityForResult(const FJavaClassObjectWrapperRef& Intent, int32 RequestCode, TSharedPtr<FThrowable>* OutException = nullptr);

protected:
	FJavaClassMethod GetApplicationContextMethod;
	FJavaClassMethod GetApplicationMethod;
	FJavaClassMethod StartActivityMethod;
	FJavaClassMethod StartActivityForResultMethod;
};
	
}

#endif