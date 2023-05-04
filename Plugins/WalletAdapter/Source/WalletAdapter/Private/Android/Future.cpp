// Fill out your copyright notice in the Description page of Project Settings.

#include "Future.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FFuture, FJavaClassObjectWrapper, "jvav/util/concurrent/Future", nullptr)
	CancelMethod = GetClassMethod("cancel", "(Z)Z");
	IsCancelledMethod = GetClassMethod("isCancelled", "()Z");
	IsDoneMethod = GetClassMethod("isDone", "()Z");
	GetMethod = GetClassMethod("get", "()Ljava/lang/Object;");
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

FJavaClassObjectWrapperRef FFuture::Get()
{
	jobject RetVal = CallMethod<jobject>(GetMethod);
	return MakeShared<FJavaClassObjectWrapper>(RetVal);
}

FJavaClassObjectWrapperPtr FFuture::Get(TSharedPtr<FThrowable>& Throwable)
{
	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, GetMethod);
	Throwable = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	return RetVal ? MakeShared<FJavaClassObjectWrapper>(RetVal) : TSharedPtr<FJavaClassObjectWrapper>();	
}

FJavaClassObjectWrapperRef FFuture::Get(int64 TimeoutMilliseconds)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	jclass TimeUnitClass = JEnv->FindClass("java/util/concurrent/TimeUnit");
	check(TimeUnitClass);

	jfieldID MillisecondsFieldId = JEnv->GetStaticFieldID(TimeUnitClass, "MILLISECONDS", "Ljava/util/concurrent/TimeUnit;");
	check(MillisecondsFieldId);

	jobject MillisecondsObject = JEnv->GetStaticObjectField(TimeUnitClass, MillisecondsFieldId);
	check(MillisecondsObject);
	
	jobject RetVal = CallMethod<jobject>(GetWithTimeoutMethod, TimeoutMilliseconds, MillisecondsObject);
	return MakeShared<FJavaClassObjectWrapper>(RetVal);
}

FJavaClassObjectWrapperPtr FFuture::Get(int64 TimeoutMilliseconds, TSharedPtr<FThrowable>& Throwable)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	jclass TimeUnitClass = JEnv->FindClass("java/util/concurrent/TimeUnit");
	check(TimeUnitClass);

	jfieldID MillisecondsFieldId = JEnv->GetStaticFieldID(TimeUnitClass, "MILLISECONDS", "Ljava/util/concurrent/TimeUnit;");
	check(MillisecondsFieldId);

	jobject MillisecondsObject = JEnv->GetStaticObjectField(TimeUnitClass, MillisecondsFieldId);
	check(MillisecondsObject);

	jthrowable JThrowable;
	jobject RetVal = CallThrowableMethod<jobject>(JThrowable, GetWithTimeoutMethod, TimeoutMilliseconds, MillisecondsObject);
	Throwable = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	
	return RetVal ? MakeShared<FJavaClassObjectWrapper>(RetVal) : TSharedPtr<FJavaClassObjectWrapper>();	
}

#endif
