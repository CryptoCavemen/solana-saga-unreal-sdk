//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "WalletAdapterClientAsyncAction.h"

#include "WalletAdapterClient.h"

UWalletAdapterClientAsyncAction::UWalletAdapterClientAsyncAction()
{
}

UWalletAdapterClientAsyncAction* UWalletAdapterClientAsyncAction::Authorize(
	UObject* WorldContextObject, UWalletAdapterClient* Client, FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
	UWalletAdapterClientAsyncAction* BlueprintNode = NewObject<UWalletAdapterClientAsyncAction>();
	BlueprintNode->Client = Client;
	BlueprintNode->Start(IdentityUri, IconUri, IdentityName, Cluster);
	return BlueprintNode;
}

void UWalletAdapterClientAsyncAction::OnSuccess()
{
	AsyncTask(ENamedThreads::GameThread, [this]
	{
		Success.Broadcast(Client);
		SetReadyToDestroy();
	});	
}

void UWalletAdapterClientAsyncAction::OnError()
{
	AsyncTask(ENamedThreads::GameThread, [this]
	{
		Fail.Broadcast(Client);
		SetReadyToDestroy();
	});	
}

void UWalletAdapterClientAsyncAction::Start(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster)
{
	AsyncTask(ENamedThreads::AnyThread, [this, IdentityUri, IconUri, IdentityName, Cluster]
	{
		if (Client->Authorize(IdentityUri, IconUri, IdentityName, Cluster))
			OnSuccess();
		else
			OnError();
	});		
}
