//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#pragma once

#include "CoreMinimal.h"
#include "Defines.h"

#if PLATFORM_ANDROID
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
	static TSharedRef<ImplClass> CreateInstance(Args... args)\
	{\
		return MakeShareable(Construct(nullptr, args...));\
	}\
	static TSharedRef<ImplClass> CreateFromExisting(jobject JObject)\
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

/**
 * Must be defined in a header file for every class derived from FJavaClassObjectWrapper if it has any static methods or fields.
 */
#define DECLARE_JAVA_CLASS_OBJECT_STATIC()\
public:\
	static void StaticConstruct();

/**
 * Must be defined in a cpp file for every class derived from FJavaClassObjectWrapper if it has any static methods or fields.
 */
#define BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(ImplClass, JavaClassName)\
void ImplClass::StaticConstruct()\
{\
	jclass StaticClass = FAndroidApplication::FindJavaClassGlobalRef(JavaClassName);\
	check(StaticClass);

/**
 *  Must be defined after BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC
 */
#define END_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC }

struct FJavaClassField
{
	FName		Name;
	FName		Signature;
	jfieldID	Field;
};

struct FJavaClassStaticField
{
	jclass		Class;
	FName		Name;
	FName		Signature;
	jfieldID	Field;
};

struct FJavaClassMethod
{
	FName		Name;
	FName		Signature;
	jmethodID	Method;
};

struct FJavaClassStaticMethod
{
	jclass		Class;
	FName		Name;
	FName		Signature;
	jmethodID	Method;
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
	static FJavaClassObjectWrapper* Construct(jobject JObject);
	static TSharedRef<FJavaClassObjectWrapper> CreateFromExisting(jobject JObject);
protected:
	virtual void PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args);
public:

	FJavaClassMethod GetClassMethod(const char* MethodName, const char* FuncSig) const;
	FJavaClassField GetClassField(const char* FieldName, const char* FuncSig) const;
	static FJavaClassStaticMethod GetClassStaticMethod(jclass StaticClass, const char* MethodName, const char* FuncSig);
	static FJavaClassStaticField GetClassStaticField(jclass StaticClass, const char* FieldName, const char* FuncSig);

	template<typename ReturnType>
	ReturnType CallMethod(FJavaClassMethod Method, ...);
	template<typename ReturnType>
	static ReturnType CallStaticMethod(FJavaClassStaticMethod Method, ...);	

	/** Calls the method and stores a local reference to an exception if any occured. */
	template<typename ReturnType>
	ReturnType CallThrowableMethod(jthrowable& Exception, FJavaClassMethod Method, ...);
	/** Calls the static method and stores a local reference to an exception if any occured. */
	template<typename ReturnType>
	static ReturnType CallThrowableStaticMethod(jthrowable& Exception, FJavaClassStaticMethod Method, ...);	

	jobject GetObjectField(FJavaClassField Field) const;
	bool GetBooleanField(FJavaClassField Field) const;
	FString GetStringField(FJavaClassField Field) const;
	uint8 GetByteField(FJavaClassField Field) const;
	uint32 GetIntField(FJavaClassField Field) const;
	TArray<jobject> GetObjectArrayField(FJavaClassField Field) const;
	TArray<uint8> GetByteArrayField(FJavaClassField Field) const;
	TArray<TArray<uint8>> GetArrayOfByteArrayField(FJavaClassField Field) const;

	FString ToString();

	/** Returns the underlying JNI pointer */
	FORCEINLINE jobject GetJObject() const { return Object; }
	/** Returns the underlying JNI pointer */
	FORCEINLINE jobject operator*() const { return Object; }
	operator bool() const;	

protected:
	/** Native java object */
	jobject Object;
	/** The java class of a stored object */
	jclass Class;
	
	FJavaClassMethod ToStringMethod;	

private:
	FJavaClassObjectWrapper(const FJavaClassObjectWrapper& rhs);
	FJavaClassObjectWrapper& operator = (const FJavaClassObjectWrapper& rhs);

	/** Stores the class of the passed java object. */
	void StoreObjectClass(jobject InObject);
	/** Stores a reference to the passed java object creating a global reference from a local if required. */
	void StoreObjectReference(jobject InObject);
};

//
// METHODS
//

template<>
void FJavaClassObjectWrapper::CallMethod<void>(FJavaClassMethod Method, ...);

template<>
bool FJavaClassObjectWrapper::CallMethod<bool>(FJavaClassMethod Method, ...);

template<>
int16 FJavaClassObjectWrapper::CallMethod<int16>(FJavaClassMethod Method, ...);
	
template<>
int32 FJavaClassObjectWrapper::CallMethod<int32>(FJavaClassMethod Method, ...);

template<>
TArray<uint8> FJavaClassObjectWrapper::CallMethod<TArray<uint8>>(FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallMethod<jobject>(FJavaClassMethod Method, ...);

template<>
jobjectArray FJavaClassObjectWrapper::CallMethod<jobjectArray>(FJavaClassMethod Method, ...);

template<>
int64 FJavaClassObjectWrapper::CallMethod<int64>(FJavaClassMethod Method, ...);

template<>
FString FJavaClassObjectWrapper::CallMethod<FString>(FJavaClassMethod Method, ...);

//
// STATIC METHODS
//
	
template<>
void FJavaClassObjectWrapper::CallStaticMethod<void>(FJavaClassStaticMethod Method, ...);

template<>
bool FJavaClassObjectWrapper::CallStaticMethod<bool>(FJavaClassStaticMethod Method, ...);

template<>
int FJavaClassObjectWrapper::CallStaticMethod<int>(FJavaClassStaticMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallStaticMethod<jobject>(FJavaClassStaticMethod Method, ...);

template<>
jobjectArray FJavaClassObjectWrapper::CallStaticMethod<jobjectArray>(FJavaClassStaticMethod Method, ...);

template<>
int64 FJavaClassObjectWrapper::CallStaticMethod<int64>(FJavaClassStaticMethod Method, ...);

template<>
FString FJavaClassObjectWrapper::CallStaticMethod<FString>(FJavaClassStaticMethod Method, ...);	

//
// THROWABLE METHODS
//
	
template<>
void FJavaClassObjectWrapper::CallThrowableMethod<void>(jthrowable& Exception, FJavaClassMethod Method, ...);

template<>
bool FJavaClassObjectWrapper::CallThrowableMethod<bool>(jthrowable& Exception, FJavaClassMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallThrowableMethod<jobject>(jthrowable& Exception, FJavaClassMethod Method, ...);

//
// THROWABLE STATIC METHODS
//
	
template<>
void FJavaClassObjectWrapper::CallThrowableStaticMethod<void>(jthrowable& Exception, FJavaClassStaticMethod Method, ...);

template<>
bool FJavaClassObjectWrapper::CallThrowableStaticMethod<bool>(jthrowable& Exception, FJavaClassStaticMethod Method, ...);

template<>
jobject FJavaClassObjectWrapper::CallThrowableStaticMethod<jobject>(jthrowable& Exception, FJavaClassStaticMethod Method, ...);


// Helper types
typedef TSharedRef<FJavaClassObjectWrapper> FJavaClassObjectWrapperRef;
typedef TSharedPtr<FJavaClassObjectWrapper> FJavaClassObjectWrapperPtr;

#endif
