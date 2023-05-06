//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "LocalAssociationIntentCreator.h"
#include "MobileWalletAdapterSession.h"
#include "WalletAdapter.h"

#if PLATFORM_ANDROID
#include "Android/JavaClassObjectWrapper.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

static jclass Class;
static jmethodID CreateAssociationIntentMethod;


void FLocalAssociationIntentCreator::StaticConstruct()
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	
	Class = FAndroidApplication::FindJavaClassGlobalRef("com/solana/mobilewalletadapter/clientlib/scenario/LocalAssociationIntentCreator");
	check(Class);
	CreateAssociationIntentMethod = Env->GetStaticMethodID(Class, "createAssociationIntent",
		"(Landroid/net/Uri;ILcom/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterSession;)Landroid/content/Intent;");
	check(CreateAssociationIntentMethod);	
}

FJavaClassObjectWrapperRef FLocalAssociationIntentCreator::CreateAssociationIntent(const FString& EndpointPrefix, int32 Port, const FMobileWalletAdapterSession& Session)
{
	UE_LOG(LogWalletAdapter, Verbose, TEXT("Creating Association Intent: EnpointPrefix = '%s'"), *EndpointPrefix);
	JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
	jobject RetVal = JEnv->CallStaticObjectMethod(
		Class,
		CreateAssociationIntentMethod,
		!EndpointPrefix.IsEmpty() ? *FJavaClassObjectWrapper::GetJUri(EndpointPrefix) : nullptr, 
		Port,
		Session.GetJObject());
	AndroidJavaEnv::CheckJavaException();
	return MakeShared<FJavaClassObjectWrapper>(RetVal);
}

#endif
