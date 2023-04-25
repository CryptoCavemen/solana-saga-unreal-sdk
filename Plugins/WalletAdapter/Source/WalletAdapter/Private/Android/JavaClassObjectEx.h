#pragma once

#include "CoreMinimal.h"

#if USE_ANDROID_JNI
#include "Android/AndroidJava.h"
#include "Android/AndroidJavaEnv.h"
#include <jni.h>

class FJavaClassObjectEx
{
public:
	// !!  All Java objects returned by JNI functions are local references.
	FJavaClassObjectEx(FName ClassName, const char* CtorSig, ...);

	~FJavaClassObjectEx();

	FJavaClassMethod GetClassMethod(const char* MethodName, const char* FuncSig);
	
	template<typename ReturnType>
	ReturnType CallMethod(FJavaClassMethod Method, ...);

	template<typename ReturnType>
	ReturnType CallThrowableMethod(bool& bExceptionThrown, FJavaClassMethod Method, ...);	

	FORCEINLINE jobject GetJObject() const
	{
		return Object;
	}

	static FScopedJavaObject<jstring> GetJString(const FString& String);
	static FScopedJavaObject<jobject> GetJUri(const FString& Uri);

	void VerifyException();
	void LogException();

protected:

	jobject			Object;
	jclass			Class;

private:
	FJavaClassObjectEx(const FJavaClassObjectEx& rhs);
	FJavaClassObjectEx& operator = (const FJavaClassObjectEx& rhs);
};

template<>
void FJavaClassObjectEx::CallMethod<void>(FJavaClassMethod Method, ...);

template<>
bool FJavaClassObjectEx::CallMethod<bool>(FJavaClassMethod Method, ...);

template<>
int FJavaClassObjectEx::CallMethod<int>(FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectEx::CallMethod<jobject>(FJavaClassMethod Method, ...);

template<>
jobjectArray FJavaClassObjectEx::CallMethod<jobjectArray>(FJavaClassMethod Method, ...);

template<>
int64 FJavaClassObjectEx::CallMethod<int64>(FJavaClassMethod Method, ...);

template<>
FString FJavaClassObjectEx::CallMethod<FString>(FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectEx::CallThrowableMethod<jobject>(bool& bExceptionThrown, FJavaClassMethod Method, ...);

#endif
