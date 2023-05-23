//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "MobileWalletAdapterUseCase.h"
#include "Network/RequestManager.h"
#include "Network/RequestUtils.h"

DEFINE_LOG_CATEGORY(LogWalletAdapterUseCase);


void UMobileWalletAdapterUseCase::SignTransaction(UWalletAdapterClient* Client, const FSuccessCallback& Success, const FFailCallback& Fail)
{
	check(Client);

	FRequestData* Request = FRequestUtils::RequestBlockHash();
	Request->Callback.BindLambda([Client, Success](FJsonObject& ResponseJsonObject)
	{
		FString BlockHash = FRequestUtils::ParseBlockHashResponse(ResponseJsonObject);
		UE_LOG(LogWalletAdapterUseCase, Log, TEXT("Block Hash = %s"), *BlockHash);

		//Client->SignTransaction()
		
		AsyncTask(ENamedThreads::GameThread, [Success]
		{
			Success.ExecuteIfBound();
		});		
	});
	Request->ErrorCallback.BindLambda([Fail](const FText& FailureReason)
	{		
		FString ErrorMessage = FString::Printf(TEXT("Failed to request a block hash: %s"), *FailureReason.ToString());
		UE_LOG(LogWalletAdapterUseCase, Error, TEXT("%s"), *ErrorMessage);
		AsyncTask(ENamedThreads::GameThread, [Fail, ErrorMessage]
		{
			Fail.ExecuteIfBound(ErrorMessage);
		});		
	});
	
	FRequestManager::SendRequest(Request);
	
	//Client->PublicKey
}
