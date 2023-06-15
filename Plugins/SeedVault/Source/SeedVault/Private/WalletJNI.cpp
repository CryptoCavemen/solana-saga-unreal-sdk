//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Defines.h"
#include "Android/JavaUtils.h"

#if PLATFORM_ANDROID
#include "SeedVaultWallet.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidPlatform.h"

using namespace SeedVault;

extern "C"
{
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeedSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jlong AuthToken)
	{
		USeedVaultWallet::OnCreateSeedSuccess(AuthToken);
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeedFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		USeedVaultWallet::OnCreateSeedFailure(FJavaUtils::JUriToString(errorMessage));
	}	
}

#endif