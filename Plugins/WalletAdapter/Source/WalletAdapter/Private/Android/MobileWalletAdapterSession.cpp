#include "MobileWalletAdapterSession.h"

FMobileWalletAdapterSession::FMobileWalletAdapterSession()
{
}

FMobileWalletAdapterSession::~FMobileWalletAdapterSession()
{
}

FMobileWalletAdapterSession* FMobileWalletAdapterSession::Construct()
{
	FMobileWalletAdapterSession* Object = new FMobileWalletAdapterSession();
	
	va_list Args = va_list();
	//va_start(Args, ClientTimeoutMs);
	// TODO: wrong signature!
	Object->PostConstruct("com/solana/mobilewalletadapter/clientlib/protocol/MobileWalletAdapterSession", "()V", Args);
	//va_end(Args);
	
	return Object;	
}

void FMobileWalletAdapterSession::PostConstruct(const char* ClassName, const char* CtorSig, const va_list Args)
{
	FMobileWalletAdapterSessionCommon::PostConstruct(ClassName, CtorSig, Args);
}
