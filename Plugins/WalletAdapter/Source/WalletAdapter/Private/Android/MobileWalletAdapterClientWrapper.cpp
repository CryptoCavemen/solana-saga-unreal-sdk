#include "MobileWalletAdapterClientWrapper.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"


jobject MyCallMethod(jobject Object, FJavaClassMethod Method, ...)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();

	va_list Params;
	va_start(Params, Method);
	jobject val = JEnv->CallObjectMethodV(Object, Method.Method, Params);
	va_end(Params);

	if (JEnv->ExceptionCheck())
	{
		JEnv->ExceptionDescribe();
		JEnv->ExceptionClear();
		UE_LOG(LogTemp, Error, TEXT("Java JNI call failed with an exception."));
	}
	
	jobject RetVal = JEnv->NewGlobalRef(val);
	JEnv->DeleteLocalRef(val);
	return RetVal;
}

FScopedJavaObject<jobject> GetJUriFromString(const FString& Uri)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	
	jclass UriClass = JEnv->FindClass("android/net/Uri");
	jmethodID ParseMethod = JEnv->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	return NewScopedJavaObject(JEnv, JEnv->CallStaticObjectMethod(UriClass, ParseMethod, *FJavaHelper::ToJavaString(JEnv, Uri)));
}

FMobileWalletAdapterClientWrapper::FMobileWalletAdapterClientWrapper(int32 ClientTimeoutMs)
	: FJavaClassObject(GetClassName(), "(I)V", ClientTimeoutMs)
	, AuthorizeMethod(GetClassMethod("authorize", "(Landroid/net/Uri;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Lcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient$AuthorizationFuture;"))
{
}

FMobileWalletAdapterClientWrapper::~FMobileWalletAdapterClientWrapper()
{
}

jobject FMobileWalletAdapterClientWrapper::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
	UE_LOG(LogTemp, Log, TEXT("Authorize(): IdentityUri = '%s', IconUri = '%s', IdentityName = '%s', Cluster = '%s'"),
		*IdentityUri, *IconUri, *IdentityName, *Cluster);
	
	jobject AuthorizationFuture = MyCallMethod(Object, AuthorizeMethod, *GetJUriFromString(IdentityUri), *GetJUriFromString(IconUri), *GetJString(IdentityName), *GetJString(Cluster));
	return AuthorizationFuture;
}

FName FMobileWalletAdapterClientWrapper::GetClassName()
{
	return FName("com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterClient");
}


void FMobileWalletAdapterClientWrapper::OnAuthorizeInternal()
{
}

#endif