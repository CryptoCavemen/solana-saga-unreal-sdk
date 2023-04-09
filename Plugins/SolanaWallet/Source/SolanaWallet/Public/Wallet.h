// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID && USE_ANDROID_JNI
#include "Android/AndroidJNI.h"
#endif

#include "Defines.h"
#include "Wallet.generated.h"

USTRUCT(BlueprintType)
struct FIntent
{
	GENERATED_BODY()

#if PLATFORM_ANDROID && USE_ANDROID_JNI
	jobject JObject;
	FIntent(jobject InJObject) : JObject(InJObject) {}
#else
	void* JObject;
	FIntent(void* InJObject) : JObject(InJObject) {}
#endif

	FIntent() : JObject(nullptr) {}
	FIntent(const FIntent& Other) : JObject(Other.JObject) {}
};

UENUM(BlueprintType)
enum class EWalletContractV1 : uint8
{
	PURPOSE_SIGN_SOLANA_TRANSACTION = 0,
};

/**
 * Wrapper for com/solanamobile/seedvault/Wallet.java. 
 */
UCLASS()
class SOLANAWALLET_API UWallet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static const char* GetClassName();
	/** initialize java objects and cache them for further usage. called when the module is loaded */
	static void Initialize();
protected:
	static void StartActivityForResult(FIntent Intent, int32 RequestCode);
	
public:
	UFUNCTION(BlueprintCallable)
	static FIntent CreateSeed(EWalletContractV1 Purpose);
	static void OnCreateSeed(int64 AuthToken);
};