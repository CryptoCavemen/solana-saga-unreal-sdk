//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/List.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FList, FJavaClassObjectWrapper, "java/util/List", "()V")
	AddMethod = GetClassMethod("add", "(Ljava/lang/Object;)V");
	GetMethod = GetClassMethod("get", "(I)Ljava/lang/Object;");
	SizeMethod = GetClassMethod("size", "()I");
END_IMPLEMENT_JAVA_CLASS_OBJECT

void FList::Add(jobject Element)
{
	CallMethod<bool>(AddMethod, Element);
}

FJavaClassObjectWrapperRef FList::Get(int32 Index)
{
	jobject JRetVal = CallMethod<jobject>(GetMethod, Index);
	return FJavaClassObjectWrapperRef(new FJavaClassObjectWrapper(JRetVal));
}

int32 FList::Size()
{
	return CallMethod<int32>(SizeMethod);
}

#endif
