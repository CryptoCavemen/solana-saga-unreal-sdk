#pragma once

#include "CoreMinimal.h"

#if USE_ANDROID_JNI
#include "Android/AndroidJava.h"
#include "Android/AndroidJavaEnv.h"
#include <jni.h>

class UObject;

class FJavaClassObjectEx
{
protected:
	// !!  All Java objects returned by JNI functions are local references.
	FJavaClassObjectEx();
public:
	virtual ~FJavaClassObjectEx();
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args);
public:

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



/*** Helper class that automatically creates a global ref from a local ref, calls DeleteLocalRef
 *   and calls DeleteGlobalRef on the passed-in Java object when goes out of scope */
template <typename T>
class FGlobalScopedJavaObject
{
public:
	FGlobalScopedJavaObject(JNIEnv* InEnv, const T& InObjRef)
		: Env(InEnv)
		, ObjRef(InObjRef)
	{
		T ObjGlobalRef = (T)Env->NewGlobalRef(ObjRef);
		Env->DeleteLocalRef(ObjRef);
		ObjRef = ObjGlobalRef;
	}
	
	FGlobalScopedJavaObject(FGlobalScopedJavaObject&& Other)
		: Env(Other.Env)
		, ObjRef(Other.ObjRef)
	{
		Other.Env = nullptr;
		Other.ObjRef = nullptr;
	}
	
	FGlobalScopedJavaObject(const FGlobalScopedJavaObject& Other) = delete;
	FGlobalScopedJavaObject& operator=(const FGlobalScopedJavaObject& Other) = delete;
	
	~FGlobalScopedJavaObject()
	{
		if (*this)
		{
			Env->DeleteGlobalRef(ObjRef);
		}
	}
	
	// Returns the underlying JNI pointer
	T operator*() const { return ObjRef; }
	
	operator bool() const
	{
		if (!Env || !ObjRef || Env->IsSameObject(ObjRef, NULL))
		{
			return false;
		}
		
		return true;
	}
	
private:
	JNIEnv* Env = nullptr;
	T ObjRef = nullptr;
};

/**
 Helper function that allows template deduction on the java object type, for example:
 auto ScopeObject = NewGlobalScopedJavaObject(Env, JavaString);
 instead of FGlobalScopedJavaObject<jstring> ScopeObject(Env, JavaString);
 */
template <typename T>
CORE_API FGlobalScopedJavaObject<T> NewGlobalScopedJavaObject(JNIEnv* InEnv, const T& InObjRef)
{
	return FGlobalScopedJavaObject<T>(InEnv, InObjRef);
}

typedef TSharedRef<FGlobalScopedJavaObject<jobject>> FJavaObjectRef;

#endif
