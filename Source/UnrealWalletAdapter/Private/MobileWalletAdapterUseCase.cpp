//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "MobileWalletAdapterUseCase.h"

#include "Crypto/Base58.h"
#include "Network/RequestManager.h"
#include "Network/RequestUtils.h"

DEFINE_LOG_CATEGORY(LogWalletAdapterUseCase);

const uint8 MEMO_TRANSACTION_TEMPLATE[] = { 
    0x01, // 1 signature required (fee payer)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // First signature (fee payer account)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, // 1 signature required (fee payer)
    0x00, // 0 read-only account signatures
    0x01, // 1 read-only account not requiring a signature
    0x02, // 2 accounts
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Fee payer account public key
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x4a, 0x53, 0x5a, 0x99, 0x29, 0x21, 0x06, // Memo program v2 account address
    0x4d, 0x24, 0xe8, 0x71, 0x60, 0xda, 0x38, 0x7c,
    0x7c, 0x35, 0xb5, 0xdd, 0xbc, 0x92, 0xbb, 0x81,
    0xe4, 0x1f, 0xa8, 0x40, 0x41, 0x05, 0x44, 0x8d,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Recent blockhash (placeholder)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, // program ID (index into list of accounts)
    0x01, // 1 account
    0x00, // account index 0
    0x14, // 20 byte payload
    0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, // "hello world "
    0x72, 0x6c, 0x64, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 8-digit random suffix
};

#define SIGNATURE_OFFSET			1
#define SIGNATURE_LEN				64
#define HEADER_OFFSET				65
#define ACCOUNT_PUBLIC_KEY_OFFSET	69
#define ACCOUNT_PUBLIC_KEY_LEN		32
#define BLOCKHASH_OFFSET			133
#define BLOCKHASH_LEN				32
#define SUFFIX_DIGITS_OFFSET		181
#define SUFFIX_DIGITS_LEN			8


TArray<uint8> CreateMemoLegacyTransaction(const TArray<uint8>& PublicKey, const FString& LatestBlockHash)
{
	checkf(PublicKey.Num() == ACCOUNT_PUBLIC_KEY_LEN, TEXT("Invalid public key length for a Solana transaction"));
	checkf(LatestBlockHash.Len() == BLOCKHASH_LEN, TEXT("Invalid blockhash length for a Solana transaction"));

	TArray<uint8> LatestBlockHash58;
	LatestBlockHash58.Append((const uint8*)TCHAR_TO_ANSI(*LatestBlockHash), LatestBlockHash.Len());
	FBase58::EncodeBase58(LatestBlockHash58);

	TArray<uint8> Transaction;
	Transaction.Append(MEMO_TRANSACTION_TEMPLATE, sizeof(MEMO_TRANSACTION_TEMPLATE));
	FMemory::Memcpy(Transaction.GetData() + ACCOUNT_PUBLIC_KEY_OFFSET, PublicKey.GetData(), PublicKey.Num());
	FMemory::Memcpy(Transaction.GetData() + BLOCKHASH_OFFSET, LatestBlockHash58.GetData(), LatestBlockHash58.Num());
	
	for (int32 I = 0; I < SUFFIX_DIGITS_LEN; I++)	
		Transaction.GetData()[SUFFIX_DIGITS_OFFSET + I] = '0' + FMath::RandHelper(10);
		
	//Log.d(TAG, "Created memo transaction for publicKey(base58)=${Base58EncodeUseCase(publicKey)}, latestBlockhash(base58)=${Base58EncodeUseCase(latestBlockhash)}")
	UE_LOG(LogWalletAdapterUseCase, Log, TEXT("Created memo transaction"));
	
	return Transaction;
}

void UMobileWalletAdapterUseCase::SignTransaction(UWalletAdapterClient* Client, const FSuccessCallback& Success, const FFailureCallback& Failure)
{
	check(Client);

	FRequestData* Request = FRequestUtils::RequestBlockHash();
	Request->Callback.BindLambda([Client, Success, Failure](FJsonObject& ResponseJsonObject)
	{
		FString BlockHash = FRequestUtils::ParseBlockHashResponse(ResponseJsonObject);
		UE_LOG(LogWalletAdapterUseCase, Log, TEXT("Block Hash = %s"), *BlockHash);
		
		TArray<FSolanaTransaction> Transactions;
		Transactions.Add(FSolanaTransaction(CreateMemoLegacyTransaction(Client->PublicKey, BlockHash)));
		
		Client->SignTransactions(Transactions,
			UWalletAdapterClient::FSignSuccessDelegate::CreateLambda([Client, Success](const TArray<FSolanaTransaction>& SignedTransactions)
			{
				AsyncTask(ENamedThreads::GameThread, [Success]
				{
					Success.ExecuteIfBound();
				});
			}),
			UWalletAdapterClient::FFailureDelegate::CreateLambda([Failure](const FString& ErrorMessage)
			{
				AsyncTask(ENamedThreads::GameThread, [Failure, ErrorMessage]
				{
					Failure.ExecuteIfBound(ErrorMessage);
				});
			}));		
	});
	Request->ErrorCallback.BindLambda([Failure](const FText& FailureReason)
	{		
		FString ErrorMessage = FString::Printf(TEXT("Failed to request a block hash: %s"), *FailureReason.ToString());
		UE_LOG(LogWalletAdapterUseCase, Error, TEXT("%s"), *ErrorMessage);
		AsyncTask(ENamedThreads::GameThread, [Failure, ErrorMessage]
		{
			Failure.ExecuteIfBound(ErrorMessage);
		});		
	});
	
	FRequestManager::SendRequest(Request);
}
