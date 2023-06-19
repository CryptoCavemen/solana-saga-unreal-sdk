//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "ArrayList.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

using namespace SeedVault;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FArrayList, FJavaClassObjectWrapper, "java/util/ArrayList", "()V")
	GetMethod = GetClassMethod("get", "(I)Ljava/lang/Object;");
	SizeMethod = GetClassMethod("size", "()I");
END_IMPLEMENT_JAVA_CLASS_OBJECT

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
