#pragma once

#include "CoreMinimal.h"

#if USE_ANDROID_JNI
#include "Android/AndroidJava.h"
#include "Android/AndroidJavaEnv.h"
#include "Defines.h"
#include <jni.h>

/**
 * Must be defined in a header file for every class derived from FJavaClassObjectEx
 */
#define DECLARE_JAVA_CLASS_OBJECT(ImplClass, ...)\
protected:\
	ImplClass(jobject LocalObject);\
public:\
	static ImplClass* Construct(jobject LocalObject, ...);\
protected:\
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;

/**
 * Must be defined in a cpp file for every class derived from FJavaClassObjectEx  
 */
#define BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(ImplClass, ParentClass, JavaClassName, JavaCtorSig, ...)\
ImplClass::ImplClass(jobject LocalObject)\
	: ParentClass(LocalObject)\
{\
}\
\
ImplClass* ImplClass::Construct(jobject LocalObject, ...)\
{\
	ImplClass* Object = new ImplClass(LocalObject);\
\
	va_list Args;\
	va_start(Args, LocalObject);\
	Object->PostConstruct(JavaClassName, JavaCtorSig, Args);\
	va_end(Args);\
\
	return Object;\
}\
\
void ImplClass::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)\
{\
	ParentClass::PostConstruct(ClassName, CtorSig, Args);

/**
 *  Must be defined after BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT
 */
#define END_IMPLEMENT_JAVA_CLASS_OBJECT }
	
/**
 * Base class for java object wrappers. 
 */
class FJavaClassObjectWrapper
{
protected:
	/**
	 * Constructs a new java object or stores a pointer to it if passed LocalObject is not null.
	 * Local reference to the passed java object is deleted automatically!
	 * 
	 * !! Please note, that all Java objects returned by JNI functions are local references.
	 */	
	FJavaClassObjectWrapper(jobject LocalObject);
public:
	virtual ~FJavaClassObjectWrapper();
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
	FJavaClassObjectWrapper(const FJavaClassObjectWrapper& rhs);
	FJavaClassObjectWrapper& operator = (const FJavaClassObjectWrapper& rhs);
};

template<>
void FJavaClassObjectWrapper::CallMethod<void>(FJavaClassMethod Method, ...);

template<>
bool FJavaClassObjectWrapper::CallMethod<bool>(FJavaClassMethod Method, ...);

template<>
int FJavaClassObjectWrapper::CallMethod<int>(FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallMethod<jobject>(FJavaClassMethod Method, ...);

template<>
jobjectArray FJavaClassObjectWrapper::CallMethod<jobjectArray>(FJavaClassMethod Method, ...);

template<>
int64 FJavaClassObjectWrapper::CallMethod<int64>(FJavaClassMethod Method, ...);

template<>
FString FJavaClassObjectWrapper::CallMethod<FString>(FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallThrowableMethod<jobject>(bool& bExceptionThrown, FJavaClassMethod Method, ...);



/** Helper class that automatically creates a global ref from a local ref, calls DeleteLocalRef for it
 *  and then calls DeleteGlobalRef on the passed-in Java object when goes out of scope */
template <typename T>
class FGlobalJavaObject
{
public:
	FGlobalJavaObject(JNIEnv* InEnv, const T& InObjRef)
		: Env(InEnv)
		, ObjRef(InObjRef)
	{
		T ObjGlobalRef = (T)Env->NewGlobalRef(ObjRef);
		Env->DeleteLocalRef(ObjRef);
		ObjRef = ObjGlobalRef;
	}
	
	FGlobalJavaObject(FGlobalJavaObject&& Other)
		: Env(Other.Env)
		, ObjRef(Other.ObjRef)
	{
		Other.Env = nullptr;
		Other.ObjRef = nullptr;
	}
	
	FGlobalJavaObject(const FGlobalJavaObject& Other) = delete;
	FGlobalJavaObject& operator=(const FGlobalJavaObject& Other) = delete;
	
	~FGlobalJavaObject()
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
CORE_API FGlobalJavaObject<T> NewGlobalJavaObject(JNIEnv* InEnv, const T& InObjRef)
{
	return FGlobalJavaObject<T>(InEnv, InObjRef);
}

typedef TSharedRef<FGlobalJavaObject<jobject>> FGlobalJavaObjectRef;

#endif
