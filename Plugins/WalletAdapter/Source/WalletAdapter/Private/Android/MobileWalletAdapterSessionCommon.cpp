#include "MobileWalletAdapterSessionCommon.h"

FMobileWalletAdapterSessionCommon::FMobileWalletAdapterSessionCommon()
{
}

FMobileWalletAdapterSessionCommon::~FMobileWalletAdapterSessionCommon()
{
}

FMobileWalletAdapterSessionCommon* FMobileWalletAdapterSessionCommon::Construct()
{
	FMobileWalletAdapterSessionCommon* Object = new FMobileWalletAdapterSessionCommon();
	
	va_list Args = va_list();
	//va_start(Args, ClientTimeoutMs);
	// TODO: wrong signature!
	Object->PostConstruct("com/solana/mobilewalletadapter/common/protocol/FMobileWalletAdapterSessionCommon", "()V", Args);
	//va_end(Args);
	
	return Object;	
}

void FMobileWalletAdapterSessionCommon::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	FJavaClassObjectEx::PostConstruct(ClassName, CtorSig, Args);
}
