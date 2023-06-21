//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Future.h"

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

FJavaClassObjectWrapperPtr FFuture::Get(TSharedPtr<FThrowable>* OutException)
{	
	jobject RetVal;
	if (OutException)
	{
		jthrowable JThrowable;
		RetVal = CallThrowableMethod<jobject>(JThrowable, GetMethod);		
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
		RetVal = CallMethod<jobject>(GetMethod);
	return RetVal ? MakeShared<FJavaClassObjectWrapper>(RetVal) : TSharedPtr<FJavaClassObjectWrapper>();	
}

FJavaClassObjectWrapperPtr FFuture::Get(int64 TimeoutMilliseconds, TSharedPtr<FThrowable>* OutException)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	jclass TimeUnitClass = Env->FindClass("java/util/concurrent/TimeUnit");
	check(TimeUnitClass);

	jfieldID MillisecondsFieldId = Env->GetStaticFieldID(TimeUnitClass, "MILLISECONDS", "Ljava/util/concurrent/TimeUnit;");
	check(MillisecondsFieldId);

	jobject MillisecondsObject = Env->GetStaticObjectField(TimeUnitClass, MillisecondsFieldId);
	check(MillisecondsObject);

	jobject RetVal;
	if (OutException)
	{
		jthrowable JThrowable;
		RetVal = CallThrowableMethod<jobject>(JThrowable, GetWithTimeoutMethod, TimeoutMilliseconds, MillisecondsObject);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
		RetVal = CallMethod<jobject>(GetWithTimeoutMethod, TimeoutMilliseconds, MillisecondsObject);
	
	return RetVal ? MakeShared<FJavaClassObjectWrapper>(RetVal) : TSharedPtr<FJavaClassObjectWrapper>();	
}

#endif
