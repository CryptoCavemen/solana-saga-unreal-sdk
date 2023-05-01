#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Defines.h"

/**
 * Wrapper for com.epicgames.ue4.GameActivity 
 */
class FGameActivity : FJavaClassObjectWrapper	
{
	DECLARE_JAVA_CLASS_OBJECT(FGameActivity);

public:
	void StartActivity(const FGlobalJavaClassObjectRef& Intent);
	void StartActivityForResult(const FGlobalJavaClassObjectRef& Intent, int32 RequestCode);
	
protected:
	FJavaClassMethod StartActivityMethod;
	FJavaClassMethod StartActivityForResultMethod;
};


#endif