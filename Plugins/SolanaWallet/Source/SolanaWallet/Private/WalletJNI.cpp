// Copyright (c) Sergey Makovkin (makovkin.s@gmail.com). All Rights Reserved.

#include "Defines.h"

#if PLATFORM_ANDROID

#include "Wallet.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidPlatform.h"

extern "C"
{
	// Functions that are called on Android lifecycle events.
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeed(JNIEnv* LocalJNIEnv, jobject LocalThis, jlong AuthToken)
	{
		UWallet::OnCreateSeed(AuthToken);
	}
}

#endif