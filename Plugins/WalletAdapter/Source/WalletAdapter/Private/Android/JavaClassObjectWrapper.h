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
	ImplClass();\
public:\
	ImplClass(jobject LocalObject);\
	static ImplClass* Construct(jobject LocalObject, ...);\
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;\
public:\
	template<typename... Args>\
	static TSharedRef<ImplClass> MakeInstance(Args... args)\
	{\
		return MakeShareable(Construct(nullptr, args...));\
	}\
	template<typename... Args>\
	static TSharedRef<ImplClass> MakeFromExistingObject(jobject LocalObject, Args... args)\
	{\
		return MakeShareable(Construct(LocalObject, args...));\
	}

/**
 * Must be defined in a cpp file for every class derived from FJavaClassObjectEx  
 */
#define BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(ImplClass, ParentClass, JavaClassName, JavaCtorSig, ...)\
ImplClass::ImplClass()\
	: ParentClass()\
{\
}\
\
ImplClass::ImplClass(jobject LocalObject)\
	: ParentClass(LocalObject)\
{\
}\
\
ImplClass* ImplClass::Construct(jobject LocalObject, ...)\
{\
	ImplClass* Object = LocalObject ? new ImplClass(LocalObject) : new ImplClass();\
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
	FJavaClassObjectWrapper();
public:	
	FJavaClassObjectWrapper(jobject InObject);
	virtual ~FJavaClassObjectWrapper();
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args);
public:

	FJavaClassMethod GetClassMethod(const char* MethodName, const char* FuncSig);
	
	template<typename ReturnType>
	ReturnType CallMethod(FJavaClassMethod Method, ...);

	/** Calls the method and stores a local reference to an exception if any occured. */
	template<typename ReturnType>
	ReturnType CallThrowableMethod(jthrowable& Exception, FJavaClassMethod Method, ...);	

	/** Returns the underlying JNI pointer */
	FORCEINLINE jobject GetJObject() const { return Object; }
	/** Returns the underlying JNI pointer */
	FORCEINLINE jobject operator*() const { return Object; }
	FORCEINLINE operator bool() const { return !Env || !Object || Env->IsSameObject(Object, nullptr); }
	
	static FScopedJavaObject<jstring> GetJString(const FString& String);
	static FScopedJavaObject<jobject> GetJUri(const FString& Uri);

	void VerifyException();
	void LogException();

protected:

	JNIEnv*	Env;
	jobject Object;
	jclass Class;

private:
	FJavaClassObjectWrapper(const FJavaClassObjectWrapper& rhs);
	FJavaClassObjectWrapper& operator = (const FJavaClassObjectWrapper& rhs);

	void StoreObjectClass(jobject InObject);
	void StoreObjectReference(jobject InObject);
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
jobject FJavaClassObjectWrapper::CallThrowableMethod<jobject>(jthrowable& Exception, FJavaClassMethod Method, ...);


typedef TSharedRef<FJavaClassObjectWrapper> FJavaClassObjectWrapperRef;
typedef TSharedPtr<FJavaClassObjectWrapper> FJavaClassObjectWrapperPtr;

#endif
