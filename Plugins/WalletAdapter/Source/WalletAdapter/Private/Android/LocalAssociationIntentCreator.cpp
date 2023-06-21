//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "LocalAssociationIntentCreator.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

#include "Android/JavaUtils.h"
#include "Android/Throwable.h"

#include "MobileWalletAdapterSession.h"
#include "WalletAdapter.h"


FJavaClassMethod FLocalAssociationIntentCreator::CreateAssociationIntentMethod;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(FLocalAssociationIntentCreator, "com/solana/mobilewalletadapter/clientlib/scenario/LocalAssociationIntentCreator")
	CreateAssociationIntentMethod = GetClassStaticMethod("createAssociationIntent", "(Landroid/net/Uri;ILcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterSession;)Landroid/content/Intent;");
END_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC

FJavaClassObjectWrapperPtr FLocalAssociationIntentCreator::CreateAssociationIntent(
	const FString& EndpointPrefix, int32 Port, const FMobileWalletAdapterSession& Session, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	UE_LOG(LogWalletAdapter, Verbose, TEXT("Creating Association Intent: EnpointPrefix = '%s'"), *EndpointPrefix);
	
	if (OutException)
	{
		jthrowable JThrowable;
		
		jobject JIntent = CallThrowableStaticMethod<jobject>(
			JThrowable,
			CreateAssociationIntentMethod,
			!EndpointPrefix.IsEmpty() ? *FJavaUtils::GetJUri(EndpointPrefix) : nullptr, 
			Port,
			Session.GetJObject());
		
		if (JThrowable)
		{
			*OutException = MakeShareable(FThrowable::Construct(JThrowable));
		}
		else
		{
			*OutException = nullptr;
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
		}
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(
			CreateAssociationIntentMethod,
			!EndpointPrefix.IsEmpty() ? *FJavaUtils::GetJUri(EndpointPrefix) : nullptr, 
			Port,
			Session.GetJObject());
		
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}

#endif
