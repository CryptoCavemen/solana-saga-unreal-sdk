//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Android/Defines.h"

#if PLATFORM_ANDROID
#include "SeedVaultWallet.h"
#include "SeedVault.h"
#include "Android/SigningResponseWrapper.h"
#include "Android/PublicKeyResponseWrapper.h"
#include "Android/ArrayList.h"
#include "Android/JavaUtils.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidPlatform.h"

using namespace SeedVault;

extern "C"
{
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onAuthorizeSeedSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jlong AuthToken)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Seed authorized: AuthToken = %lld"), AuthToken);

		AsyncTask(ENamedThreads::GameThread, [AuthToken] {
			USeedVaultWallet::AuthorizeSeedSuccess.Execute(AuthToken);
			USeedVaultWallet::AuthorizeSeedSuccess.Unbind();
			USeedVaultWallet::AuthorizeSeedFailure.Unbind();
		});
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onAuthorizeSeedFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Seed authorization failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::AuthorizeSeedFailure.Execute(ErrorMessage);
			USeedVaultWallet::AuthorizeSeedSuccess.Unbind();
			USeedVaultWallet::AuthorizeSeedFailure.Unbind();
		});
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeedSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jlong AuthToken)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Seed created: AuthToken = %lld"), AuthToken);

		AsyncTask(ENamedThreads::GameThread, [AuthToken] {
			USeedVaultWallet::CreateSeedSuccess.Execute(AuthToken);
			USeedVaultWallet::CreateSeedSuccess.Unbind();
			USeedVaultWallet::CreateSeedFailure.Unbind();
		});
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onCreateSeedFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Seed creation failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::CreateSeedFailure.Execute(ErrorMessage);
			USeedVaultWallet::CreateSeedSuccess.Unbind();
			USeedVaultWallet::CreateSeedFailure.Unbind();
		});
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onImportSeedSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jlong AuthToken)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Seed imported: AuthToken = %lld"), AuthToken);

		AsyncTask(ENamedThreads::GameThread, [AuthToken] {
			USeedVaultWallet::ImportSeedSuccess.Execute(AuthToken);
			USeedVaultWallet::ImportSeedSuccess.Unbind();
			USeedVaultWallet::ImportSeedFailure.Unbind();
		});
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onImportSeedFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Seed import failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::ImportSeedFailure.Execute(ErrorMessage);
			USeedVaultWallet::ImportSeedSuccess.Unbind();
			USeedVaultWallet::ImportSeedFailure.Unbind();
		});
	}

	TArray<FSigningResponse> JSigningResponsesToFSigningResponses(jobject JSigningResponses)
	{
		TArray<FSigningResponse> FSigningResponses;
		
		auto SigningResponses = FArrayList::MakeFromExistingObject(JSigningResponses);

		int32 NumSigningResponses = SigningResponses->Size();
		for (int32 ResponseIndex = 0; ResponseIndex < NumSigningResponses; ResponseIndex++)
		{
			auto SigningResponse = FSigningResponseWrapper::MakeFromExistingObject(**SigningResponses->Get(ResponseIndex));
			FSigningResponse FSigningResponse;

			auto Signatures = SigningResponse->GetSignatures();
			int32 NumSignatures = Signatures->Size();
			for (int32 SignatureIndex = 0; SignatureIndex < NumSignatures; SignatureIndex++)
			{
				auto JSignature = Signatures->Get(SignatureIndex);
				TArray<uint8> Signature = FJavaUtils::JByteArrayToTArray((jbyteArray)**JSignature);
				FSigningResponse.Signatures.Add(Signature);
			}
			
			auto ResolvedDerivationPaths = SigningResponse->GetResolvedDerivationPaths();
			int32 NumResolvedDerivationPaths = ResolvedDerivationPaths->Size();
			for (int32 PathIndex = 0; PathIndex < NumResolvedDerivationPaths; PathIndex++)
			{
				auto JPath = ResolvedDerivationPaths->Get(PathIndex);
				FString Path = FJavaUtils::JUriToString(**JPath);
				FSigningResponse.ResolvedDerivationPaths.Add(Path);
			}
			
			FSigningResponses.Add(FSigningResponse);
		}

		return FSigningResponses;
	}

	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onSignTransactionsSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jobject signingResponses)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Signed transactions successfully"));

		TArray<FSigningResponse> FSigningResponses = JSigningResponsesToFSigningResponses(signingResponses);
		
		AsyncTask(ENamedThreads::GameThread, [FSigningResponses] {
			USeedVaultWallet::SignTransactionsSuccess.Execute(FSigningResponses);
			USeedVaultWallet::SignTransactionsSuccess.Unbind();
			USeedVaultWallet::SignTransactionsFailure.Unbind();
		});		
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onSignTransactionsFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Transactions signing failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::SignTransactionsFailure.Execute(ErrorMessage);
			USeedVaultWallet::SignTransactionsSuccess.Unbind();
			USeedVaultWallet::SignTransactionsFailure.Unbind();
		});
	}

	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onSignMessagesSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jobject signingResponses)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Signed messages successfully"));
		
		TArray<FSigningResponse> FSigningResponses = JSigningResponsesToFSigningResponses(signingResponses);

		AsyncTask(ENamedThreads::GameThread, [FSigningResponses] {
			USeedVaultWallet::SignMessagesSuccess.Execute(FSigningResponses);
			USeedVaultWallet::SignMessagesSuccess.Unbind();
			USeedVaultWallet::SignMessagesFailure.Unbind();
		});			
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onSignMessagesFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Messages signing failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::SignMessagesFailure.Execute(ErrorMessage);
			USeedVaultWallet::SignMessagesSuccess.Unbind();
			USeedVaultWallet::SignMessagesFailure.Unbind();
		});
	}

	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onRequestPublicKeysSuccess(JNIEnv* LocalJNIEnv, jobject LocalThis, jobject publicKeyResponses)
	{
		UE_LOG(LogSeedVault, Log, TEXT("Public keys request succeeded"));

		TArray<FPublicKeyResponse> FPublicKeyResponses;
		
		auto PublicKeyResponses = FArrayList::MakeFromExistingObject(publicKeyResponses);

		int32 NumPublicKeyResponses = PublicKeyResponses->Size();
		for (int32 ResponseIndex = 0; ResponseIndex < NumPublicKeyResponses; ResponseIndex++)
		{
			auto PublicKeyResponse = FPublicKeyResponseWrapper::MakeFromExistingObject(**PublicKeyResponses->Get(ResponseIndex));

			FPublicKeyResponse FPublicKeyResponse;
			FPublicKeyResponse.PublicKey = PublicKeyResponse->GetPublicKey();
			FPublicKeyResponse.PublicKeyEncoded = PublicKeyResponse->GetPublicKeyEncoded();
			FPublicKeyResponse.ResolvedDerivationPath = PublicKeyResponse->GetResolvedDerivationPath();
					
			FPublicKeyResponses.Add(FPublicKeyResponse);
		}

		AsyncTask(ENamedThreads::GameThread, [FPublicKeyResponses] {
			USeedVaultWallet::GetPublicKeysSuccess.Execute(FPublicKeyResponses);
			USeedVaultWallet::GetPublicKeysSuccess.Unbind();
			USeedVaultWallet::GetPublicKeysFailure.Unbind();
		});			
	}
	
	JNI_METHOD void Java_com_solanamobile_unreal_WalletJavaHelper_onRequestPublicKeysFailure(JNIEnv* LocalJNIEnv, jobject LocalThis, jstring errorMessage)
	{
		FString ErrorMessage = FJavaHelper::FStringFromLocalRef(LocalJNIEnv, errorMessage);
		UE_LOG(LogSeedVault, Error, TEXT("Public keys request failed: %s"), *ErrorMessage);
		
		AsyncTask(ENamedThreads::GameThread, [ErrorMessage] {
			USeedVaultWallet::GetPublicKeysFailure.Execute(ErrorMessage);
			USeedVaultWallet::GetPublicKeysSuccess.Unbind();
			USeedVaultWallet::GetPublicKeysFailure.Unbind();
		});
	}
}

#endif