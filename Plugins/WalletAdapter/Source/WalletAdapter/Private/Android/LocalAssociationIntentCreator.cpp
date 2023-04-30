// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalAssociationIntentCreator.h"

#include "MobileWalletAdapterSession.h"

#if PLATFORM_ANDROID
#include "Android/JavaClassObjectEx.h"
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

FJavaObjectRef FLocalAssociationIntentCreator::CreateAssociationIntent(const FString& EndpointPrefix, int32 Port, const FMobileWalletAdapterSession& Session)
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jobject Intent = Env->CallStaticObjectMethod(Class, CreateAssociationIntentMethod, *FJavaClassObjectEx::GetJUri(EndpointPrefix), Port, Session.GetJObject());
	AndroidJavaEnv::CheckJavaException();
	return FJavaObjectRef(new FGlobalScopedJavaObject<jobject>(Env, Intent));
}

#endif
