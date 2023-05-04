// Fill out your copyright notice in the Description page of Project Settings.

#include "GameActivity.h"

#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FGameActivity, FJavaClassObjectWrapper, "com/epicgames/unreal/GameActivity", "()V")
	StartActivityMethod = GetClassMethod("startActivity", "(Landroid/content/Intent;)V");
	StartActivityForResultMethod = GetClassMethod("startActivityForResult", "(Landroid/content/Intent;I)V");
END_IMPLEMENT_JAVA_CLASS_OBJECT

void FGameActivity::StartActivity(const FJavaClassObjectWrapperRef& Intent)
{
	CallMethod<void>(StartActivityMethod, **Intent);
}

void FGameActivity::StartActivityForResult(const FJavaClassObjectWrapperRef& Intent, int32 RequestCode)
{
	CallMethod<void>(StartActivityForResultMethod, **Intent, RequestCode);
}

#endif
