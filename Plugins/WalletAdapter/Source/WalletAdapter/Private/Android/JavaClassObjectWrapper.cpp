//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/JavaClassObjectWrapper.h"
#include "Android/AndroidJavaEnv.h"

#if USE_ANDROID_JNI

FJavaClassObjectWrapper::FJavaClassObjectWrapper()
	: Object(nullptr)
	, Class(nullptr)
{
}

FJavaClassObjectWrapper::FJavaClassObjectWrapper(jobject InObject)
	: Object(nullptr)
	, Class(nullptr)
{
	check(InObject);
	StoreObjectClass(InObject);
	StoreObjectReference(InObject);
}

FJavaClassObjectWrapper::~FJavaClassObjectWrapper()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	if (Object)
		Env->DeleteGlobalRef(Object);
	if (Class)
		Env->DeleteGlobalRef(Class);
}

void FJavaClassObjectWrapper::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	if (!CtorSig)
		verifyf(Object, TEXT("If the ctor signature is null, a reference to an object must be passed because the object can't be constructed"));
	if (!ClassName)
		verifyf(Object, TEXT("If the class name is null, a reference to an object must be passed because the object can't be constructed"));

	if (Object)
		check(Class);
	
	if (Object == nullptr)
	{
		JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
		
		Class = AndroidJavaEnv::FindJavaClassGlobalRef(ClassName);
		check(Class);
		
		jmethodID Constructor = Env->GetMethodID(Class, "<init>", CtorSig);
		check(Constructor);

		// Construct a new object
		Object = Env->NewObjectV(Class, Constructor, Args);
		VerifyException(Env);
		check(Object);
	}
	
	// Convert the passed reference to a global one if required and store the pointer.
	StoreObjectReference(Object);
}

FJavaClassMethod FJavaClassObjectWrapper::GetClassMethod(const char* MethodName, const char* FuncSig)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	FJavaClassMethod Method;
	Method.Method = Env->GetMethodID(Class, MethodName, FuncSig);
	Method.Name = MethodName;
	Method.Signature = FuncSig;
	// Is method valid?
	checkf(Method.Method, TEXT("Unable to find Java Method %s with Signature %s"), UTF8_TO_TCHAR(MethodName), UTF8_TO_TCHAR(FuncSig));
	return Method;
}

void FJavaClassObjectWrapper::StoreObjectClass(jobject InObject)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	jclass LocalClass = Env->GetObjectClass(InObject);
	check(LocalClass);
	Class = (jclass)Env->NewGlobalRef(LocalClass);
	Env->DeleteLocalRef(LocalClass);	
}

void FJavaClassObjectWrapper::StoreObjectReference(jobject InObject)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	jobjectRefType RefType = Env->GetObjectRefType(InObject);
	switch (RefType)
	{
	case JNILocalRefType:
		// Promote local ref to global
		Object = Env->NewGlobalRef(InObject);
		Env->DeleteLocalRef(InObject);		
		break;
	case JNIGlobalRefType:
		Object = Env->NewGlobalRef(InObject);
		break;
	case JNIWeakGlobalRefType:
		verifyf(false, TEXT("Weak global reference is not supported"));
		break;
	case JNIInvalidRefType:
		verifyf(false, TEXT("Reference is invalid"));
		break;		
	}	
}

template<>
void FJavaClassObjectWrapper::CallMethod<void>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	Env->CallVoidMethodV(Object, Method.Method, Params);
	va_end(Params);
	VerifyException(Env);
}

template<>
bool FJavaClassObjectWrapper::CallMethod<bool>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	bool RetVal = Env->CallBooleanMethodV(Object, Method.Method, Params);
	va_end(Params);
	VerifyException(Env);
	return RetVal;
}

template<>
int FJavaClassObjectWrapper::CallMethod<int>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int RetVal = Env->CallIntMethodV(Object, Method.Method, Params);
	va_end(Params);
	VerifyException(Env);
	return RetVal;
}

template<>
jobject FJavaClassObjectWrapper::CallMethod<jobject>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	VerifyException(Env);
	jobject RetVal = Env->NewGlobalRef(val);
	Env->DeleteLocalRef(val);
	return RetVal;
}

template<>
jobjectArray FJavaClassObjectWrapper::CallMethod<jobjectArray>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	VerifyException(Env);
	jobjectArray RetVal = (jobjectArray)Env->NewGlobalRef(val);
	Env->DeleteLocalRef(val);
	return RetVal;
}

template<>
int64 FJavaClassObjectWrapper::CallMethod<int64>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int64 RetVal = Env->CallLongMethodV(Object, Method.Method, Params);
	va_end(Params);
	VerifyException(Env);
	return RetVal;
}

template<>
FString FJavaClassObjectWrapper::CallMethod<FString>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jstring RetVal = static_cast<jstring>(Env->CallObjectMethodV(Object, Method.Method, Params));
	va_end(Params);
	VerifyException(Env);
	auto Result = FJavaHelper::FStringFromLocalRef(Env, RetVal);
	return Result;
}

template<>
void FJavaClassObjectWrapper::CallThrowableMethod<void>(jthrowable& Exception, FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	Env->CallVoidMethodV(Object, Method.Method, Params);
	va_end(Params);
	
	if (Env->ExceptionCheck())
	{
		Exception = Env->ExceptionOccurred();		
		Env->ExceptionDescribe();
		Env->ExceptionClear();
	}
	else
	{
		Exception = nullptr;
	}	
}

template<>
jobject FJavaClassObjectWrapper::CallThrowableMethod<jobject>(jthrowable& Exception, FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);

	if (Env->ExceptionCheck())
	{
		Exception = Env->ExceptionOccurred();		
		Env->ExceptionDescribe();
		Env->ExceptionClear();
	}
	else
	{
		Exception = nullptr;
	}
	
	jobject RetVal = Env->NewGlobalRef(val);
	Env->DeleteLocalRef(val);
	return RetVal;
}

FJavaClassObjectWrapper::operator bool() const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	return !Env || !Object || Env->IsSameObject(Object, nullptr);
}

FScopedJavaObject<jstring> FJavaClassObjectWrapper::GetJString(const FString& String)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	return FJavaHelper::ToJavaString(Env, String);
}

FScopedJavaObject<jobject> FJavaClassObjectWrapper::GetJUri(const FString& Uri)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	if (Uri.IsEmpty())
		return FScopedJavaObject<jobject>(Env, nullptr);
	
	jclass UriClass = Env->FindClass("android/net/Uri");
	jmethodID UriParseMethod = Env->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	auto JStringScoped = FJavaHelper::ToJavaString(Env, Uri);
	jobject JUri = Env->CallStaticObjectMethod(UriClass, UriParseMethod, *JStringScoped);
	LogException(Env);
	return NewScopedJavaObject(Env, JUri);		
}

void FJavaClassObjectWrapper::VerifyException(JNIEnv* Env)
{
	if (Env->ExceptionCheck())
	{
		Env->ExceptionDescribe();
		Env->ExceptionClear();
		verify(false && "Java JNI call failed with an exception.");
	}
}

void FJavaClassObjectWrapper::LogException(JNIEnv* Env)
{
	if (Env->ExceptionCheck())
	{
		Env->ExceptionDescribe();
		Env->ExceptionClear();
		UE_LOG(LogAndroid, Warning, TEXT("Java JNI call failed with an exception."));
	}	
}

#endif

