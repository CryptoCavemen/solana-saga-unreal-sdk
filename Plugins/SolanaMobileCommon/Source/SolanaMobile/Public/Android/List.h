//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "Defines.h"
#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"

/**
 * Wrapper for java.util.List 
 */
class FList : public FJavaClassObjectWrapper
{
	DECLARE_JAVA_CLASS_OBJECT(FList);
public:
	void Add(jobject Element);
	FScopedJavaObject<jobject> Get(int32 Index);
	int32 Size();
protected:
	FJavaClassMethod AddMethod;
	FJavaClassMethod GetMethod;
	FJavaClassMethod SizeMethod;
};

#endif