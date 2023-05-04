// Fill out your copyright notice in the Description page of Project Settings.

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