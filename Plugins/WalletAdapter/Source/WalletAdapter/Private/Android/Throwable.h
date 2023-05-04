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
 * Wrapper for java.lang.Throwable 
 */
class FThrowable : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FThrowable);
public:
	FString GetMessage();
protected:
	FJavaClassMethod GetMessageMethod;
};

#endif