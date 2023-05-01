#include "Android/JavaClassObjectWrapper.h"
#include "Android/AndroidJavaEnv.h"

#if USE_ANDROID_JNI

jclass UriClass = nullptr;
jmethodID UriParseMethod = nullptr;

FJavaClassObjectWrapper::FJavaClassObjectWrapper(jobject LocalObject)
	: Object(LocalObject)
	, Class(nullptr)
{
}

FJavaClassObjectWrapper::~FJavaClassObjectWrapper()
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	if (Object)
		JEnv->DeleteGlobalRef(Object);
	if (Class)
		JEnv->DeleteGlobalRef(Class);
}

void FJavaClassObjectWrapper::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	
	jobject LocalObject = Object;
	
	if (LocalObject == nullptr)
	{
		Class = AndroidJavaEnv::FindJavaClassGlobalRef(ClassName);
		check(Class);
		
		jmethodID Constructor = JEnv->GetMethodID(Class, "<init>", CtorSig);
		check(Constructor);
	
		LocalObject = JEnv->NewObjectV(Class, Constructor, Args);
		VerifyException();
		check(LocalObject);
	}
	else
	{
		jclass LocalClass = JEnv->GetObjectClass(LocalObject);
		check(LocalClass);
		Class = (jclass)JEnv->NewGlobalRef(LocalClass);
		JEnv->DeleteLocalRef(LocalClass);
	}
	
	// Promote local references to global
	Object = JEnv->NewGlobalRef(LocalObject);
	JEnv->DeleteLocalRef(LocalObject);
}

FJavaClassMethod FJavaClassObjectWrapper::GetClassMethod(const char* MethodName, const char* FuncSig)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	FJavaClassMethod Method;
	Method.Method = JEnv->GetMethodID(Class, MethodName, FuncSig);
	Method.Name = MethodName;
	Method.Signature = FuncSig;
	// Is method valid?
	checkf(Method.Method, TEXT("Unable to find Java Method %s with Signature %s"), UTF8_TO_TCHAR(MethodName), UTF8_TO_TCHAR(FuncSig));
	return Method;
}

template<>
void FJavaClassObjectWrapper::CallMethod<void>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	JEnv->CallVoidMethodV(Object, Method.Method, Params);
	va_end(Params);
	LogException();
}

template<>
bool FJavaClassObjectWrapper::CallMethod<bool>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	bool RetVal = JEnv->CallBooleanMethodV(Object, Method.Method, Params);
	va_end(Params);
	LogException();
	return RetVal;
}

template<>
int FJavaClassObjectWrapper::CallMethod<int>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int RetVal = JEnv->CallIntMethodV(Object, Method.Method, Params);
	va_end(Params);
	LogException();
	return RetVal;
}

template<>
jobject FJavaClassObjectWrapper::CallMethod<jobject>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = JEnv->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	LogException();
	jobject RetVal = JEnv->NewGlobalRef(val);
	JEnv->DeleteLocalRef(val);
	return RetVal;
}

template<>
jobjectArray FJavaClassObjectWrapper::CallMethod<jobjectArray>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = JEnv->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	LogException();
	jobjectArray RetVal = (jobjectArray)JEnv->NewGlobalRef(val);
	JEnv->DeleteLocalRef(val);
	return RetVal;
}

template<>
int64 FJavaClassObjectWrapper::CallMethod<int64>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int64 RetVal = JEnv->CallLongMethodV(Object, Method.Method, Params);
	va_end(Params);
	LogException();
	return RetVal;
}

template<>
FString FJavaClassObjectWrapper::CallMethod<FString>(FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jstring RetVal = static_cast<jstring>(JEnv->CallObjectMethodV(Object, Method.Method, Params));
	va_end(Params);
	LogException();
	auto Result = FJavaHelper::FStringFromLocalRef(JEnv, RetVal);
	return Result;
}

template<>
jobject FJavaClassObjectWrapper::CallThrowableMethod<jobject>(bool& bExceptionThrown, FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = JEnv->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	bExceptionThrown = JEnv->ExceptionCheck();
	if (bExceptionThrown)
	{
		JEnv->ExceptionDescribe();
		JEnv->ExceptionClear();
	}
	jobject RetVal = JEnv->NewGlobalRef(val);
	JEnv->DeleteLocalRef(val);
	return RetVal;
}

FScopedJavaObject<jstring> FJavaClassObjectWrapper::GetJString(const FString& String)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	return FJavaHelper::ToJavaString(JEnv, String);
}

FScopedJavaObject<jobject> FJavaClassObjectWrapper::GetJUri(const FString& Uri)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	if (!UriClass)
		UriClass = JEnv->FindClass("android/net/Uri");
	if (!UriParseMethod)
		UriParseMethod = JEnv->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	jobject JUri = JEnv->CallStaticObjectMethod(UriClass, UriParseMethod, *FJavaHelper::ToJavaString(JEnv, Uri));
	return NewScopedJavaObject(JEnv, JUri);	
}

void FJavaClassObjectWrapper::VerifyException()
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	if (JEnv->ExceptionCheck())
	{
		JEnv->ExceptionDescribe();
		JEnv->ExceptionClear();
		verify(false && "Java JNI call failed with an exception.");
	}
}

void FJavaClassObjectWrapper::LogException()
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	if (JEnv->ExceptionCheck())
	{
		JEnv->ExceptionDescribe();
		JEnv->ExceptionClear();
		UE_LOG(LogAndroid, Warning, TEXT("Java JNI call failed with an exception."));
	}	
}

#endif

