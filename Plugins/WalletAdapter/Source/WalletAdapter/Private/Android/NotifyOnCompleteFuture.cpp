// Fill out your copyright notice in the Description page of Project Settings.

#include "NotifyOnCompleteFuture.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FNotifyOnCompleteFuture, FFuture, "com/solana/mobilewalletadapter/common/util/NotifyOnCompleteFuture", "()V")
END_IMPLEMENT_JAVA_CLASS_OBJECT

#endif
