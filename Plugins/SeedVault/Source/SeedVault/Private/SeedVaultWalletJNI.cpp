﻿//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Defines.h"

#if PLATFORM_ANDROID
#include "SeedVaultWallet.h"
#include "SeedVault.h"
#include "Android/JavaUtils.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidPlatform.h"

using namespace SeedVault;

extern "C"
{
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeedSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jlong AuthToken)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Seed created: AuthToken = %lld"), AuthToken);

		AsyncTask(ENamedThreads::GameThread, [AuthToken] {
			USeedVaultWallet::CreateSeedSuccess.Execute(AuthToken);
			USeedVaultWallet::CreateSeedSuccess.Unbind();
			USeedVaultWallet::CreateSeedFailure.Unbind();
		});
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeedFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Seed creation failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::CreateSeedFailure.Execute(ErrorMessage);
			USeedVaultWallet::CreateSeedSuccess.Unbind();
			USeedVaultWallet::CreateSeedFailure.Unbind();
		});
	}	
}

#endif