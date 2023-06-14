//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Defines.h"

#if PLATFORM_ANDROID
#include "Wallet.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidPlatform.h"

extern "C"
{
	// Functions that are called on Android lifecycle events.
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeed(JNIEnv* LocalJNIEnv, jobject LocalThis, jboolean bSuccess, jlong AuthToken)
	{
		UWallet::OnCreateSeed(bSuccess, AuthToken);
	}
}

#endif