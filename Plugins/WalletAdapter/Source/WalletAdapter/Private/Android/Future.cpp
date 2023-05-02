// Fill out your copyright notice in the Description page of Project Settings.

#include "Future.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FFuture, FJavaClassObjectWrapper, "jvav/util/concurrent/Future", "()V")
	CancelMethod = GetClassMethod("cancel", "(Z)Z");
	IsCancelledMethod = GetClassMethod("isCancelled", "()Z");
	IsDoneMethod = GetClassMethod("isDone", "()Ljava/lang/Object;");
	GetMethod = GetClassMethod("get", "()Z");
	GetWithTimeoutMethod = GetClassMethod("get", "(JLjava/util/concurrent/TimeUnit;)Ljava/lang/Object;");
END_IMPLEMENT_JAVA_CLASS_OBJECT

bool FFuture::Cancel(bool MayInterruptIfRunning)
{
	return CallMethod<bool>(CancelMethod, MayInterruptIfRunning);
}

bool FFuture::IsCancelled()
{
	return CallMethod<bool>(IsCancelledMethod);
}

bool FFuture::IsDone()
{
	return CallMethod<bool>(IsDoneMethod);
}

FGlobalJavaClassObjectRef FFuture::Get()
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	jobject JObject = CallMethod<jobject>(GetMethod);
	return FGlobalJavaClassObjectRef(new FGlobalJavaClassObject(JEnv, JObject));
}

FGlobalJavaClassObjectRef FFuture::Get(int64 TimeoutMilliseconds)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	jclass TimeUnitClass = JEnv->FindClass("java/util/concurrent/TimeUnit");
	check(TimeUnitClass);

	jfieldID MillisecondsField = JEnv->GetStaticFieldID(TimeUnitClass, "MILLISECONDS", "LMILLISECONDS;");
	check(MillisecondsField);
	
	jobject JObject = CallMethod<jobject>(GetWithTimeoutMethod, TimeoutMilliseconds, MillisecondsField);
	return FGlobalJavaClassObjectRef(new FGlobalJavaClassObject(JEnv, JObject));
}

#endif
