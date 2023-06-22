﻿//
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

bool FArrayList::Add(jobject Element)
{
	return CallMethod<bool>(AddMethod, Element);
}

FScopedJavaObject<jobject> FArrayList::Get(int32 Index)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jobject JRetVal = CallMethod<jobject>(GetMethod, Index);
	return NewScopedJavaObject(Env, JRetVal);
}

int32 FArrayList::Size()
{
	return CallMethod<int32>(SizeMethod);
}

#endif
