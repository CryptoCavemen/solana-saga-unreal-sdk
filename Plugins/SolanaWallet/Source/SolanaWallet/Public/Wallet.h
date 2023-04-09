// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_ANDROID && USE_ANDROID_JNI
#include "Android/AndroidJNI.h"
#endif

// __RESHARPER__ is only defined while in IDE. Used to help editing this file with proper highlighting.
#ifdef __RESHARPER__
#include "C:\Users\Sergey\AppData\Local\Android\Sdk\ndk\25.1.8937393\toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\include\jni.h"
#endif

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
	UFUNCTION(BlueprintCallable)
	static int64 OnCreateSeedResult(int32 ResultCode, FIntent Intent);
};