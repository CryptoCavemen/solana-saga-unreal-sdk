//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "LocalAssociateWithWalletAsyncAction.h"

ULocalAssociateWithWalletAsyncAction::ULocalAssociateWithWalletAsyncAction()
{
}

ULocalAssociateWithWalletAsyncAction* ULocalAssociateWithWalletAsyncAction::LocalAssociateWithWallet(UObject* WorldContextObject, FString UriPrefix)
{
	ULocalAssociateWithWalletAsyncAction* BlueprintNode = NewObject<ULocalAssociateWithWalletAsyncAction>();
	BlueprintNode->Start(UriPrefix);
	return BlueprintNode;
}

ULocalAssociateWithWalletAsyncAction* ULocalAssociateWithWalletAsyncAction::CloseLocalAssociationWithWallet(UObject* WorldContextObject, ULocalAssociationWithWallet* LocalAssociation)
{
	ULocalAssociateWithWalletAsyncAction* BlueprintNode = NewObject<ULocalAssociateWithWalletAsyncAction>();
	BlueprintNode->LocalAssociation = LocalAssociation;
	BlueprintNode->Close();
	return BlueprintNode;		
}

void ULocalAssociateWithWalletAsyncAction::OnSuccess()
{
	AsyncTask(ENamedThreads::GameThread, [this]
	{
		Success.Broadcast(LocalAssociation, ELocalAssociateWithWalletError::None);
		SetReadyToDestroy();
	});
}

void ULocalAssociateWithWalletAsyncAction::OnError(ELocalAssociateWithWalletError InError)
{
	AsyncTask(ENamedThreads::GameThread, [this, InError]
	{
		Fail.Broadcast(LocalAssociation, InError);
		SetReadyToDestroy();
	});	
}

void ULocalAssociateWithWalletAsyncAction::Start(FString UriPrefix)
{
	AsyncTask(ENamedThreads::AnyThread, [this, UriPrefix]
	{
		LocalAssociation = NewObject<ULocalAssociationWithWallet>();
		if (!LocalAssociation->OpenWallet(UriPrefix))
		{
			OnError(ELocalAssociateWithWalletError::NoCompatibleWallet);
			return;
		}
	
		if (!LocalAssociation->Start())
		{
			OnError(ELocalAssociateWithWalletError::AssociationFailed);
			return;
		}

		auto Client = LocalAssociation->GetMobileWalletAdapterClient();
		check(Client);
		
		OnSuccess();
	});	
}

void ULocalAssociateWithWalletAsyncAction::Close()
{
	AsyncTask(ENamedThreads::AnyThread, [this]
	{
		if (LocalAssociation && LocalAssociation->Close())
			OnSuccess();
		else
			OnError(ELocalAssociateWithWalletError::CloseFailed);
	});
	
}