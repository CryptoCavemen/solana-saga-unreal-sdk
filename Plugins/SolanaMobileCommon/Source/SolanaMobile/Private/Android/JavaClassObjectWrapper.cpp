//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/JavaClassObjectWrapper.h"

#if PLATFORM_ANDROID
#include "Android/JavaUtils.h"
#include "Android/AndroidJavaEnv.h"

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

FJavaClassObjectWrapper* FJavaClassObjectWrapper::Construct(jobject JObject)
{
	check(JObject);
	FJavaClassObjectWrapper* Object = new FJavaClassObjectWrapper(JObject);	
	va_list Args;
	Object->PostConstruct(nullptr, nullptr, Args);
	return Object;	
}

TSharedRef<FJavaClassObjectWrapper> FJavaClassObjectWrapper::CreateFromExisting(jobject JObject)
{
	return MakeShareable(Construct(JObject));
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
		FJavaUtils::VerifyException(Env);
		check(Object);
	}
	
	// Convert the passed reference to a global one if required and store the pointer.
	StoreObjectReference(Object);

	ToStringMethod = GetClassMethod("toString", "()Ljava/lang/String;");
}

FJavaClassMethod FJavaClassObjectWrapper::GetClassMethod(const char* MethodName, const char* FuncSig) const
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

FJavaClassField FJavaClassObjectWrapper::GetClassField(const char* FieldName, const char* FuncSig) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	FJavaClassField Field;
	Field.Field = Env->GetFieldID(Class, FieldName, FuncSig);
	Field.Name = FieldName;
	Field.Signature = FuncSig;
	// Is field valid?
	checkf(Field.Field, TEXT("Unable to find Java Field %s with Signature %s"), UTF8_TO_TCHAR(FieldName), UTF8_TO_TCHAR(FuncSig));
	return Field;
}

FJavaClassStaticMethod FJavaClassObjectWrapper::GetClassStaticMethod(jclass StaticClass, const char* MethodName, const char* FuncSig)
{
	check(StaticClass);
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	FJavaClassStaticMethod Method;
	Method.Class = StaticClass;
	Method.Method = Env->GetStaticMethodID(StaticClass, MethodName, FuncSig);
	Method.Name = MethodName;
	Method.Signature = FuncSig;
	// Is method valid?
	checkf(Method.Method, TEXT("Unable to find Java Method %s with Signature %s"), UTF8_TO_TCHAR(MethodName), UTF8_TO_TCHAR(FuncSig));
	return Method;
}

FJavaClassStaticField FJavaClassObjectWrapper::GetClassStaticField(jclass StaticClass, const char* FieldName, const char* FuncSig)
{
	check(StaticClass);
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	
	FJavaClassStaticField Field;
	Field.Class = StaticClass;
	Field.Field = Env->GetStaticFieldID(StaticClass, FieldName, FuncSig);
	Field.Name = FieldName;
	Field.Signature = FuncSig;
	// Is field valid?
	checkf(Field.Field, TEXT("Unable to find Java Field %s with Signature %s"), UTF8_TO_TCHAR(FieldName), UTF8_TO_TCHAR(FuncSig));
	return Field;	
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
	FJavaUtils::VerifyException(Env);
}

template<>
bool FJavaClassObjectWrapper::CallMethod<bool>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	bool RetVal = Env->CallBooleanMethodV(Object, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

template<>
int16 FJavaClassObjectWrapper::CallMethod<int16>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int16 RetVal = Env->CallShortMethodV(Object, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

template<>
int32 FJavaClassObjectWrapper::CallMethod<int32>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int32 RetVal = Env->CallIntMethodV(Object, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

template<>
TArray<uint8> FJavaClassObjectWrapper::CallMethod<TArray<uint8>>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jbyteArray val = (jbyteArray)Env->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return FJavaUtils::JByteArrayToTArray(val);
}

template<>
jobject FJavaClassObjectWrapper::CallMethod<jobject>(FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
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
	FJavaUtils::VerifyException(Env);
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
	FJavaUtils::VerifyException(Env);
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
	FJavaUtils::VerifyException(Env);
	auto Result = FJavaHelper::FStringFromLocalRef(Env, RetVal);
	return Result;
}

template<>
void FJavaClassObjectWrapper::CallStaticMethod<void>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	Env->CallStaticObjectMethodV(Method.Class, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
}

template<>
bool FJavaClassObjectWrapper::CallStaticMethod<bool>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	bool RetVal = Env->CallStaticBooleanMethodV(Method.Class, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

template<>
int FJavaClassObjectWrapper::CallStaticMethod<int>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int RetVal = Env->CallStaticIntMethodV(Method.Class, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

template<>
jobject FJavaClassObjectWrapper::CallStaticMethod<jobject>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallStaticObjectMethodV(Method.Class, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	jobject RetVal = Env->NewGlobalRef(val);
	Env->DeleteLocalRef(val);
	return RetVal;
}

template<>
jobjectArray FJavaClassObjectWrapper::CallStaticMethod<jobjectArray>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallStaticObjectMethodV(Method.Class, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	jobjectArray RetVal = (jobjectArray)Env->NewGlobalRef(val);
	Env->DeleteLocalRef(val);
	return RetVal;
}

template<>
int64 FJavaClassObjectWrapper::CallStaticMethod<int64>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	int64 RetVal = Env->CallStaticLongMethodV(Method.Class, Method.Method, Params);
	va_end(Params);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

template<>
FString FJavaClassObjectWrapper::CallStaticMethod<FString>(FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jstring RetVal = static_cast<jstring>(Env->CallStaticObjectMethodV(Method.Class, Method.Method, Params));
	va_end(Params);
	FJavaUtils::VerifyException(Env);
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
bool FJavaClassObjectWrapper::CallThrowableMethod<bool>(jthrowable& Exception, FJavaClassMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	bool RetVal = Env->CallBooleanMethodV(Object, Method.Method, Params);
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
	
	return RetVal;
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

template<>
void FJavaClassObjectWrapper::CallThrowableStaticMethod<void>(jthrowable& Exception, FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	Env->CallStaticVoidMethodV(Method.Class, Method.Method, Params);
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
bool FJavaClassObjectWrapper::CallThrowableStaticMethod<bool>(jthrowable& Exception, FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	bool RetVal = Env->CallStaticBooleanMethodV(Method.Class, Method.Method, Params);
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

	return RetVal;
}

template<>
jobject FJavaClassObjectWrapper::CallThrowableStaticMethod<jobject>(jthrowable& Exception, FJavaClassStaticMethod Method, ...)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	va_list Params;
	va_start(Params, Method);
	jobject val = Env->CallStaticObjectMethodV(Method.Class, Method.Method, Params);
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

jobject FJavaClassObjectWrapper::GetObjectField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jobject RetVal = Env->GetObjectField(Object, Field.Field);
	FJavaUtils::VerifyException(Env);
	return RetVal;		
}

bool FJavaClassObjectWrapper::GetBooleanField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jboolean RetVal = Env->GetBooleanField(Object, Field.Field);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

FString FJavaClassObjectWrapper::GetStringField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jstring RetVal = static_cast<jstring>(Env->GetObjectField(Object, Field.Field));
	FJavaUtils::VerifyException(Env);
	auto Result = FJavaHelper::FStringFromLocalRef(Env, RetVal);
	return Result;	
}

uint8 FJavaClassObjectWrapper::GetByteField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jbyte RetVal = Env->GetByteField(Object, Field.Field);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

uint32 FJavaClassObjectWrapper::GetIntField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jint RetVal = Env->GetIntField(Object, Field.Field);
	FJavaUtils::VerifyException(Env);
	return RetVal;
}

TArray<jobject> FJavaClassObjectWrapper::GetObjectArrayField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	TArray<jobject> ObjectArray;

	jobjectArray JObjectArray = static_cast<jobjectArray>(Env->GetObjectField(Object, Field.Field));
	FJavaUtils::VerifyException(Env);
	int32 ObjectArraySize = Env->GetArrayLength(JObjectArray);
	
	for (int32 ObjectIndex = 0; ObjectIndex < ObjectArraySize; ObjectIndex++)
	{
		jobject JObject = Env->GetObjectArrayElement(JObjectArray, ObjectIndex);
		FJavaUtils::VerifyException(Env);
		jobject JObjectGlobalRef = Env->NewGlobalRef(JObject);
		Env->DeleteLocalRef(JObject);
		ObjectArray.Add(JObjectGlobalRef);
	}

	return ObjectArray;
}

TArray<uint8> FJavaClassObjectWrapper::GetByteArrayField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	jbyteArray JByteArray = static_cast<jbyteArray>(Env->GetObjectField(Object, Field.Field));
	FJavaUtils::VerifyException(Env);
	
	TArray<uint8> ByteArray = FJavaUtils::JByteArrayToTArray(JByteArray);	
		
	Env->DeleteLocalRef(JByteArray);
	
	return ByteArray;
}

TArray<TArray<uint8>> FJavaClassObjectWrapper::GetArrayOfByteArrayField(FJavaClassField Field) const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();

	TArray<TArray<uint8>> ArrayOfByteArray;

	jobjectArray JObjectArray = static_cast<jobjectArray>(Env->GetObjectField(Object, Field.Field));
	FJavaUtils::VerifyException(Env);
	int32 ObjectArraySize = Env->GetArrayLength(JObjectArray);
	
	for (int32 ObjectIndex = 0; ObjectIndex < ObjectArraySize; ObjectIndex++)
	{
		jbyteArray JByteArray = static_cast<jbyteArray>(Env->GetObjectArrayElement(JObjectArray, ObjectIndex));
		FJavaUtils::VerifyException(Env);
		
		jbyte* ArrayDataPtr = Env->GetByteArrayElements(JByteArray, 0);
		int32 ArraySize = Env->GetArrayLength(JByteArray);

		TArray<uint8>& ByteArray = ArrayOfByteArray.AddDefaulted_GetRef();
		
		if (JByteArray != nullptr)
		{
			ByteArray.SetNumUninitialized(ArraySize);
			memcpy(ByteArray.GetData(), ArrayDataPtr, ArraySize);
			Env->ReleaseByteArrayElements(JByteArray, ArrayDataPtr, JNI_ABORT);
		}
		
		Env->DeleteLocalRef(JByteArray);
	}
	
	Env->DeleteLocalRef(JObjectArray);
	
	return ArrayOfByteArray;
}

FString FJavaClassObjectWrapper::ToString()
{
	return CallMethod<FString>(ToStringMethod);
}

FJavaClassObjectWrapper::operator bool() const
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
	return !Env || !Object || Env->IsSameObject(Object, nullptr);
}

#endif