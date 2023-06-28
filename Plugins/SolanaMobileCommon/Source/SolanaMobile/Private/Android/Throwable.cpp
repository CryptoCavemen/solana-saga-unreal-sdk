//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Throwable.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FThrowable, FJavaClassObjectWrapper, "java/lang/Throwable", "()V")
	GetMessageMethod = GetClassMethod("getMessage", "()Ljava/lang/String;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

FString FThrowable::GetMessage()
{
	return CallMethod<FString>(GetMessageMethod);
}

#endif
