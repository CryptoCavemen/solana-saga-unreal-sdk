//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "Wallet.h"

#if PLATFORM_ANDROID
#include "Android/Cursor.h"
#include "Android/ArrayList.h"
#include "Android/JavaUtils.h"
#include "Android/JavaClassObjectWrapper.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"

FJavaClassMethod FWallet::AuthorizeSeedMethod;
FJavaClassMethod FWallet::CreateSeedMethod;
FJavaClassMethod FWallet::ImportSeedMethod;
FJavaClassMethod FWallet::SignTransactionMethod;
FJavaClassMethod FWallet::SignMessageMethod;
FJavaClassMethod FWallet::RequestPublicKeysMethod;
FJavaClassMethod FWallet::GetAccountsMethod;
FJavaClassMethod FWallet::GetAccountMethod;
FJavaClassMethod FWallet::UpdateAccountNameMethod;
FJavaClassMethod FWallet::DeauthorizeSeedMethod;
FJavaClassMethod FWallet::HasUnauthorizedSeedsForPurposeMethod;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(FWallet, "com/solanamobile/seedvault/Wallet")
	AuthorizeSeedMethod = GetClassStaticMethod("authorizeSeed", "(I)Landroid/content/Intent;");
	CreateSeedMethod = GetClassStaticMethod("createSeed", "(I)Landroid/content/Intent;");
	ImportSeedMethod = GetClassStaticMethod("importSeed", "(I)Landroid/content/Intent;");
	SignTransactionMethod = GetClassStaticMethod("signTransaction", "(JLandroid/net/Uri;[B)Landroid/content/Intent;");
	SignMessageMethod = GetClassStaticMethod("signMessage", "(JLandroid/net/Uri;[B)Landroid/content/Intent;");
	RequestPublicKeysMethod = GetClassStaticMethod("requestPublicKeys", "(JLjava/util/ArrayList;)Landroid/content/Intent;");
	GetAccountsMethod = GetClassStaticMethod("getAccounts", "(Landroid/content/Context;J[Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)Landroid/database/Cursor;");
	GetAccountMethod = GetClassStaticMethod("getAccount", "(Landroid/content/Context;JJ[Ljava/lang/String;)Landroid/database/Cursor;");
	UpdateAccountNameMethod = GetClassStaticMethod("updateAccountName", "(Landroid/content/Context;JJLjava/lang/String;)V");	
	DeauthorizeSeedMethod = GetClassStaticMethod("deauthorizeSeed", "(Landroid/content/Context;J)V");
	HasUnauthorizedSeedsForPurposeMethod = GetClassStaticMethod("hasUnauthorizedSeedsForPurpose", "(Landroid/content/Context;I)Z");
END_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC

FJavaClassObjectWrapperPtr FWallet::AuthorizeSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, AuthorizeSeedMethod, Purpose);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(AuthorizeSeedMethod, Purpose);
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;	
}

FJavaClassObjectWrapperPtr FWallet::CreateSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, CreateSeedMethod, Purpose);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(CreateSeedMethod, Purpose);
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}

FJavaClassObjectWrapperPtr FWallet::ImportSeed(int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, ImportSeedMethod, Purpose);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(ImportSeedMethod, Purpose);
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}

FJavaClassObjectWrapperPtr FWallet::SignTransaction(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Transaction, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, SignTransactionMethod,
			AuthToken,
			!DerivationPath.IsEmpty() ? *FJavaUtils::GetJUri(DerivationPath) : nullptr,
			*FJavaUtils::GetByteArray(Transaction));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(SignTransactionMethod,
			AuthToken,
			!DerivationPath.IsEmpty() ? *FJavaUtils::GetJUri(DerivationPath) : nullptr,
			*FJavaUtils::GetByteArray(Transaction));
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}

FJavaClassObjectWrapperPtr FWallet::SignMessage(int64 AuthToken, const FString& DerivationPath, const TArray<uint8>& Message, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, SignMessageMethod,
			AuthToken,
			!DerivationPath.IsEmpty() ? *FJavaUtils::GetJUri(DerivationPath) : nullptr,
			*FJavaUtils::GetByteArray(Message));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	else
	{
		jobject JIntent = CallStaticMethod<jobject>(SignMessageMethod,
			AuthToken,
			!DerivationPath.IsEmpty() ? *FJavaUtils::GetJUri(DerivationPath) : nullptr,
			*FJavaUtils::GetByteArray(Message));
		Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	
	return Intent;
}

FJavaClassObjectWrapperPtr FWallet::RequestPublicKey(int64 AuthToken, const FString& DerivationPath, TSharedPtr<FThrowable>* OutException)
{
	TArray<FString> DerivationPaths;
	DerivationPaths.Add(DerivationPath);
	return RequestPublicKeys(AuthToken, DerivationPaths, OutException);
}

FJavaClassObjectWrapperPtr FWallet::RequestPublicKeys(int64 AuthToken, const TArray<FString>& DerivationPaths, TSharedPtr<FThrowable>* OutException)
{
	FJavaClassObjectWrapperPtr Intent;

	TSharedRef<FArrayList> JDerivationPaths(FArrayList::Construct(nullptr));
	for (auto& DerivationPath : DerivationPaths)
		JDerivationPaths->Add(*FJavaUtils::GetJUri(DerivationPath));
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JIntent = CallThrowableStaticMethod<jobject>(JThrowable, RequestPublicKeysMethod, AuthToken, **JDerivationPaths);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Intent = MakeShareable(new FJavaClassObjectWrapper(JIntent));
	}
	else
	{	
		jobject JIntent = CallStaticMethod<jobject>(RequestPublicKeysMethod, AuthToken, **JDerivationPaths);
		Intent = FThrowable::CreateFromExisting(JIntent);
	}

	return Intent;
}

TSharedPtr<FCursor> FWallet::GetAccounts(FJavaClassObjectWrapperRef Context, int64 AuthToken, const TArray<FString>& Projection, TSharedPtr<FThrowable>* OutException)
{
	TSharedPtr<FCursor> Cursor;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JCursor = CallThrowableStaticMethod<jobject>(JThrowable, GetAccountsMethod, **Context, AuthToken, *FJavaUtils::GetJStringArray(Projection), nullptr, nullptr);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Cursor = FCursor::CreateFromExisting(JCursor);
	}
	else
	{	
		jobject JCursor = CallStaticMethod<jobject>(GetAccountsMethod, **Context, AuthToken, *FJavaUtils::GetJStringArray(Projection), nullptr, nullptr);
		Cursor = FCursor::CreateFromExisting(JCursor);
	}

	return Cursor;
}

TSharedPtr<FCursor> FWallet::GetAccounts(FJavaClassObjectWrapperRef Context, int64 AuthToken, const TArray<FString>& Projection,
                                         const FString& FilterOnColumn, const FString& Value, TSharedPtr<FThrowable>* OutException)
{
	TSharedPtr<FCursor> Cursor;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JCursor = CallThrowableStaticMethod<jobject>(JThrowable, GetAccountsMethod, **Context, AuthToken,
			*FJavaUtils::GetJStringArray(Projection), *FJavaUtils::GetJString(FilterOnColumn), *FJavaUtils::GetJString(Value));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Cursor = FCursor::CreateFromExisting(JCursor);
	}
	else
	{	
		jobject JCursor = CallStaticMethod<jobject>(GetAccountsMethod, **Context, AuthToken,
			*FJavaUtils::GetJStringArray(Projection), *FJavaUtils::GetJString(FilterOnColumn), *FJavaUtils::GetJString(Value));
		Cursor = FCursor::CreateFromExisting(JCursor);
	}

	return Cursor;	
}

TSharedPtr<FCursor> FWallet::GetAccount(FJavaClassObjectWrapperRef Context, int64 AuthToken, int64 Id, const TArray<FString>& Projection, TSharedPtr<FThrowable>* OutException)
{
	TSharedPtr<FCursor> Cursor;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JCursor = CallThrowableStaticMethod<jobject>(JThrowable, GetAccountsMethod, **Context, AuthToken, Id, *FJavaUtils::GetJStringArray(Projection));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Cursor = FCursor::CreateFromExisting(JCursor);
	}
	else
	{	
		jobject JCursor = CallStaticMethod<jobject>(GetAccountsMethod, **Context, AuthToken, Id, *FJavaUtils::GetJStringArray(Projection));
		Cursor = FCursor::CreateFromExisting(JCursor);
	}

	return Cursor;	
}

void FWallet::UpdateAccountName(FJavaClassObjectWrapperRef Context, int64 AuthToken, int64 Id, const FString& Name, TSharedPtr<FThrowable>* OutException)
{
	if (OutException)
	{
		jthrowable JThrowable;
		CallThrowableStaticMethod<jobject>(JThrowable, UpdateAccountNameMethod, **Context, AuthToken, Id, *FJavaUtils::GetJString(Name));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
	{	
		CallStaticMethod<jobject>(UpdateAccountNameMethod, **Context, AuthToken, Id, *FJavaUtils::GetJString(Name));
	}
}

bool FWallet::DeauthorizeSeed(FJavaClassObjectWrapperRef Context, int64 AuthToken, TSharedPtr<FThrowable>* OutException)
{
	if (OutException)
	{
		jthrowable JThrowable;
		CallThrowableStaticMethod<void>(JThrowable, DeauthorizeSeedMethod, **Context, AuthToken);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
	{
		CallStaticMethod<void>(DeauthorizeSeedMethod, **Context, AuthToken);
	}

	return *OutException ? false : true;
}

bool FWallet::HasUnauthorizedSeedsForPurpose(FJavaClassObjectWrapperRef Context, int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	bool Result;
	
	if (OutException)
	{
		jthrowable JThrowable;
		Result = CallThrowableStaticMethod<bool>(JThrowable, HasUnauthorizedSeedsForPurposeMethod, **Context, Purpose);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
	{
		Result = CallStaticMethod<bool>(HasUnauthorizedSeedsForPurposeMethod, **Context, Purpose);
	}
	
	return Result;
}

#endif
