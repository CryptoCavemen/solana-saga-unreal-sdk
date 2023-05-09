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
	UWalletAdapterClient* Client, FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
	UWalletAdapterClientAsyncAction* BlueprintNode = NewObject<UWalletAdapterClientAsyncAction>();
	BlueprintNode->Client = Client;
	BlueprintNode->StartAuthorize(IdentityUri, IconUri, IdentityName, Cluster);
	return BlueprintNode;
}

UWalletAdapterClientAsyncAction* UWalletAdapterClientAsyncAction::Reauthorize(UWalletAdapterClient* Client, FString IdentityUri, FString IconUri, FString IdentityName, FString AuthToken)
{
	UWalletAdapterClientAsyncAction* BlueprintNode = NewObject<UWalletAdapterClientAsyncAction>();
	BlueprintNode->Client = Client;
	BlueprintNode->StartReauthorize(IdentityUri, IconUri, IdentityName, AuthToken);
	return BlueprintNode;
}

UWalletAdapterClientAsyncAction* UWalletAdapterClientAsyncAction::Deauthorize(UWalletAdapterClient* Client, FString AuthToken)
{
	UWalletAdapterClientAsyncAction* BlueprintNode = NewObject<UWalletAdapterClientAsyncAction>();
	BlueprintNode->Client = Client;
	BlueprintNode->StartDeauthorize(AuthToken);
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

void UWalletAdapterClientAsyncAction::StartAuthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& Cluster)
{
	AsyncTask(ENamedThreads::AnyThread, [this, IdentityUri, IconUri, IdentityName, Cluster]
	{
		if (Client->Authorize(IdentityUri, IconUri, IdentityName, Cluster))
			OnSuccess();
		else
			OnError();
	});		
}

void UWalletAdapterClientAsyncAction::StartReauthorize(const FString& IdentityUri, const FString& IconUri, const FString& IdentityName, const FString& AuthToken)
{
	AsyncTask(ENamedThreads::AnyThread, [this, IdentityUri, IconUri, IdentityName, AuthToken]
	{
		if (Client->Reauthorize(IdentityUri, IconUri, IdentityName, AuthToken))
			OnSuccess();
		else
			OnError();
	});
}

void UWalletAdapterClientAsyncAction::StartDeauthorize(const FString& AuthToken)
{
	AsyncTask(ENamedThreads::AnyThread, [this, AuthToken]
	{
		if (Client->Deauthorize(AuthToken))
			OnSuccess();
		else
			OnError();
	});	
}
