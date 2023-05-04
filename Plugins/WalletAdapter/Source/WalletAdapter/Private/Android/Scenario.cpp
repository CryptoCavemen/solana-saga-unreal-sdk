//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Scenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FScenario, FJavaClassObjectWrapper, "com/solana/mobilewalletadapter/clientlib/scenario/Scenario", "(I)V", int ClientTimeoutMs)
END_IMPLEMENT_JAVA_CLASS_OBJECT

#endif
