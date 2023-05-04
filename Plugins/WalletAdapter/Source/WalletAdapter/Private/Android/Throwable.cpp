//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Throwable.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FThrowable, FJavaClassObjectWrapper, "java/lang/Throwable", "()V")
	GetMessageMethod = GetClassMethod("getMessage", "()Ljava/lang/String;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

FString FThrowable::GetMessage()
{
	return CallMethod<FString>(GetMessageMethod);
}

#endif
