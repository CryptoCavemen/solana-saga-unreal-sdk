// Fill out your copyright notice in the Description page of Project Settings.


#include "TestUserWidget.h"
#include "SolanaWallet.h"
#include "Wallet.h"

void UTestUserWidget::OnButtonClicked()
{
	//FSolanaWalletModule& SolanaWallet = FModuleManager::GetModuleChecked<FSolanaWalletModule>(TEXT("SolanaWallet"));

	//FString Action = TEXT("android.intent.action.VIEW");
	//FString Uri = TEXT("https://www.ya.ru/");
	//SolanaWallet.StartActivity(Action, Uri);
	//SolanaWallet.RunTest();
	
	UWallet::CreateSeed(EWalletContractV1::PURPOSE_SIGN_SOLANA_TRANSACTION);
}
