//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/ArrayList.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FArrayList, FJavaClassObjectWrapper, "java/util/ArrayList", "()V")
	AddMethod = GetClassMethod("add", "(Ljava/lang/Object;)Z");
	GetMethod = GetClassMethod("get", "(I)Ljava/lang/Object;");
	SizeMethod = GetClassMethod("size", "()I");
END_IMPLEMENT_JAVA_CLASS_OBJECT

bool FArrayList::Add(FJavaClassObjectWrapperRef Element)
{
	return CallMethod<bool>(AddMethod, **Element);
}

bool FArrayList::Add(jobject Element)
{
	return CallMethod<bool>(AddMethod, Element);
}

FJavaClassObjectWrapperRef FArrayList::Get(int32 Index)
{
	jobject JElement = CallMethod<jobject>(GetMethod);
	FJavaClassObjectWrapperRef Element(new FJavaClassObjectWrapper(JElement));
	return Element;
}

int32 FArrayList::Size()
{
	return CallMethod<int32>(SizeMethod);
}

#endif
