//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "Defines.h"
#if USE_ANDROID_JNI
#include "Android/AndroidJava.h"
#include "Android/AndroidJavaEnv.h"
#include <jni.h>

/**
 * Must be defined in a header file for every class derived from FJavaClassObjectWrapper
 */
#define DECLARE_JAVA_CLASS_OBJECT(ImplClass, ...)\
protected:\
	ImplClass();\
public:\
	ImplClass(jobject JObject);\
	static ImplClass* Construct(jobject JObject, ...);\
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args) override;\
public:\
	template<typename... Args>\
	static TSharedRef<ImplClass> MakeInstance(Args... args)\
	{\
		return MakeShareable(Construct(nullptr, args...));\
	}\
	static TSharedRef<ImplClass> MakeFromExistingObject(jobject JObject)\
	{\
		return MakeShareable(Construct(JObject));\
	}

/**
 * Must be defined in a cpp file for every class derived from FJavaClassObjectWrapper  
 */
#define BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(ImplClass, ParentClass, JavaClassName, JavaCtorSig, ...)\
ImplClass::ImplClass()\
	: ParentClass()\
{\
}\
\
ImplClass::ImplClass(jobject JObject)\
	: ParentClass(JObject)\
{\
}\
\
ImplClass* ImplClass::Construct(jobject JObject, ...)\
{\
	ImplClass* Object = JObject ? new ImplClass(JObject) : new ImplClass();\
\
	va_list Args;\
	va_start(Args, JObject);\
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


struct FJavaClassField
{
	FName		Name;
	FName		Signature;
	jfieldID	Field;
};

/**
 * Base class for java object wrappers. 
 */
class FJavaClassObjectWrapper
{
protected:
	FJavaClassObjectWrapper();
public:
	/** Constructs the wrapper for a java object. Both local and global references are accepted.*/
	FJavaClassObjectWrapper(jobject InObject);
	virtual ~FJavaClassObjectWrapper();
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args);
public:

	FJavaClassMethod GetClassMethod(const char* MethodName, const char* FuncSig) const;
	FJavaClassField GetClassField(const char* FieldName, const char* FuncSig) const;
	
	template<typename ReturnType>
	ReturnType CallMethod(FJavaClassMethod Method, ...);

	/** Calls the method and stores a local reference to an exception if any occured. */
	template<typename ReturnType>
	ReturnType CallThrowableMethod(jthrowable& Exception, FJavaClassMethod Method, ...);

	jobject GetObjectField(FJavaClassField Field) const;
	FString GetStringField(FJavaClassField Field) const;
	uint8 GetByteField(FJavaClassField Field) const;
	TArray<jobject> GetObjectArrayField(FJavaClassField Field) const;
	TArray<uint8> GetByteArrayField(FJavaClassField Field) const;
	TArray<TArray<uint8>> GetArrayOfByteArrayField(FJavaClassField Field) const;

	/** Returns the underlying JNI pointer */
	FORCEINLINE jobject GetJObject() const { return Object; }
	/** Returns the underlying JNI pointer */
	FORCEINLINE jobject operator*() const { return Object; }
	operator bool() const;

protected:
	jobject Object;
	jclass Class;

private:
	FJavaClassObjectWrapper(const FJavaClassObjectWrapper& rhs);
	FJavaClassObjectWrapper& operator = (const FJavaClassObjectWrapper& rhs);

	/** Stores the class of the passed java object. */
	void StoreObjectClass(jobject InObject);
	/** Stores a reference to the passed java object creating a global reference from a local if required. */
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
void FJavaClassObjectWrapper::CallThrowableMethod<void>(jthrowable& Exception, FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallThrowableMethod<jobject>(jthrowable& Exception, FJavaClassMethod Method, ...);

typedef TSharedRef<FJavaClassObjectWrapper> FJavaClassObjectWrapperRef;
typedef TSharedPtr<FJavaClassObjectWrapper> FJavaClassObjectWrapperPtr;

#endif
