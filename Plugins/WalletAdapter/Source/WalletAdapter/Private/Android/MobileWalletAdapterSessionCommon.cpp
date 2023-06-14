//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "MobileWalletAdapterSessionCommon.h"

using namespace WalletAdapter;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FMobileWalletAdapterSessionCommon, FJavaClassObjectWrapper,
	"com/solana/mobilewalletadapter/common/protocol/FMobileWalletAdapterSessionCommon",
	"(Lcom/solana/mobilewalletadapter/common/protocol/MessageReceiver;Lcom/solana/mobilewalletadapter/common/protocol/MobileWalletAdapterSessionCommon$StateCallbacks;)V")
END_IMPLEMENT_JAVA_CLASS_OBJECT