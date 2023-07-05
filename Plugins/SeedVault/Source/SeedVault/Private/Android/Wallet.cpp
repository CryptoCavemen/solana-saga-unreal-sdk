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

FJavaClassStaticMethod FWallet::AuthorizeSeedMethod;
FJavaClassStaticMethod FWallet::CreateSeedMethod;
FJavaClassStaticMethod FWallet::ImportSeedMethod;
FJavaClassStaticMethod FWallet::SignTransactionMethod;
FJavaClassStaticMethod FWallet::SignMessageMethod;
FJavaClassStaticMethod FWallet::RequestPublicKeysMethod;
FJavaClassStaticMethod FWallet::GetAccountsMethod;
FJavaClassStaticMethod FWallet::GetAccountMethod;
FJavaClassStaticMethod FWallet::UpdateAccountNameMethod;
FJavaClassStaticMethod FWallet::GetAuthorizedSeedsMethod;
FJavaClassStaticMethod FWallet::GetAuthorizedSeedMethod;
FJavaClassStaticMethod FWallet::DeauthorizeSeedMethod;
FJavaClassStaticMethod FWallet::HasUnauthorizedSeedsForPurposeMethod;
FJavaClassStaticMethod FWallet::ResolveDerivationPathMethod;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT_STATIC(FWallet, "com/solanamobile/seedvault/Wallet")
	AuthorizeSeedMethod = GetClassStaticMethod(StaticClass, "authorizeSeed", "(I)Landroid/content/Intent;");
	CreateSeedMethod = GetClassStaticMethod(StaticClass, "createSeed", "(I)Landroid/content/Intent;");
	ImportSeedMethod = GetClassStaticMethod(StaticClass, "importSeed", "(I)Landroid/content/Intent;");
	SignTransactionMethod = GetClassStaticMethod(StaticClass, "signTransaction", "(JLandroid/net/Uri;[B)Landroid/content/Intent;");
	SignMessageMethod = GetClassStaticMethod(StaticClass, "signMessage", "(JLandroid/net/Uri;[B)Landroid/content/Intent;");
	RequestPublicKeysMethod = GetClassStaticMethod(StaticClass, "requestPublicKeys", "(JLjava/util/ArrayList;)Landroid/content/Intent;");
	GetAccountsMethod = GetClassStaticMethod(StaticClass, "getAccounts", "(Landroid/content/Context;J[Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)Landroid/database/Cursor;");
	GetAccountMethod = GetClassStaticMethod(StaticClass, "getAccount", "(Landroid/content/Context;JJ[Ljava/lang/String;)Landroid/database/Cursor;");
	UpdateAccountNameMethod = GetClassStaticMethod(StaticClass, "updateAccountName", "(Landroid/content/Context;JJLjava/lang/String;)V");
	GetAuthorizedSeedsMethod = GetClassStaticMethod(StaticClass, "getAuthorizedSeeds", "(Landroid/content/Context;[Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)Landroid/database/Cursor;");
	GetAuthorizedSeedMethod = GetClassStaticMethod(StaticClass, "getAuthorizedSeed", "(Landroid/content/Context;J[Ljava/lang/String;)Landroid/database/Cursor;");
	DeauthorizeSeedMethod = GetClassStaticMethod(StaticClass, "deauthorizeSeed", "(Landroid/content/Context;J)V");
	HasUnauthorizedSeedsForPurposeMethod = GetClassStaticMethod(StaticClass, "hasUnauthorizedSeedsForPurpose", "(Landroid/content/Context;I)Z");
	ResolveDerivationPathMethod = GetClassStaticMethod(StaticClass, "resolveDerivationPath", "(Landroid/content/Context;Landroid/net/Uri;I)Landroid/net/Uri;");
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
			*FJavaUtils::GetJStringArray(Projection),
			*FJavaUtils::GetJString(FilterOnColumn),
			*FJavaUtils::GetJString(Value));
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
		CallThrowableStaticMethod<void>(JThrowable, UpdateAccountNameMethod, **Context, AuthToken, Id, *FJavaUtils::GetJString(Name));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
	{	
		CallStaticMethod<void>(UpdateAccountNameMethod, **Context, AuthToken, Id, *FJavaUtils::GetJString(Name));
	}
}

TSharedPtr<FCursor> FWallet::GetAuthorizedSeeds(FJavaClassObjectWrapperRef Context, const TArray<FString>& Projection, TSharedPtr<FThrowable>* OutException)
{
	TSharedPtr<FCursor> Cursor;
	
	if (OutException)
	{
		jthrowable JThrowable;
		
		jobject JCursor = CallThrowableStaticMethod<jobject>(
			JThrowable,
			GetAuthorizedSeedsMethod,
			**Context,
			*FJavaUtils::GetJStringArray(Projection),
			nullptr,
			nullptr);
		
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Cursor = FCursor::CreateFromExisting(JCursor);
	}
	else
	{	
		jobject JCursor = CallStaticMethod<jobject>(
			GetAuthorizedSeedsMethod,
			**Context,
			*FJavaUtils::GetJStringArray(Projection),
			nullptr,
			nullptr);
		
		Cursor = FCursor::CreateFromExisting(JCursor);
	}

	return Cursor;		
}

TSharedPtr<FCursor> FWallet::GetAuthorizedSeeds(FJavaClassObjectWrapperRef Context, const TArray<FString>& Projection, const FString& FilterOnColumn, const FString& Value,
                                                TSharedPtr<FThrowable>* OutException)
{
	TSharedPtr<FCursor> Cursor;
	
	if (OutException)
	{
		jthrowable JThrowable;
		
		jobject JCursor = CallThrowableStaticMethod<jobject>(
			JThrowable,
			GetAuthorizedSeedsMethod,
			**Context,
			*FJavaUtils::GetJStringArray(Projection),
			*FJavaUtils::GetJString(FilterOnColumn),
			*FJavaUtils::GetJString(Value));
		
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Cursor = FCursor::CreateFromExisting(JCursor);
	}
	else
	{	
		jobject JCursor = CallStaticMethod<jobject>(
			GetAuthorizedSeedsMethod,
			**Context,
			*FJavaUtils::GetJStringArray(Projection),
			*FJavaUtils::GetJString(FilterOnColumn),
			*FJavaUtils::GetJString(Value));
		
		Cursor = FCursor::CreateFromExisting(JCursor);
	}

	return Cursor;		
}

TSharedPtr<FCursor> FWallet::GetAuthorizedSeed(FJavaClassObjectWrapperRef Context, int64 AuthToken, const TArray<FString>& Projection, TSharedPtr<FThrowable>* OutException)
{
	TSharedPtr<FCursor> Cursor;
	
	if (OutException)
	{
		jthrowable JThrowable;
		jobject JCursor = CallThrowableStaticMethod<jobject>(JThrowable, GetAuthorizedSeedMethod, **Context, AuthToken, *FJavaUtils::GetJStringArray(Projection));
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
		if (!JThrowable)
			Cursor = FCursor::CreateFromExisting(JCursor);
	}
	else
	{	
		jobject JCursor = CallStaticMethod<jobject>(GetAuthorizedSeedMethod, **Context, AuthToken, *FJavaUtils::GetJStringArray(Projection));
		Cursor = FCursor::CreateFromExisting(JCursor);
	}

	return Cursor;		
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

FString FWallet::ResolveDerivationPath(FJavaClassObjectWrapperRef Context, FString DerivationPath, int32 Purpose, TSharedPtr<FThrowable>* OutException)
{
	jobject JRetVal;
	if (OutException)
	{
		jthrowable JThrowable;
		JRetVal = CallThrowableStaticMethod<jobject>(JThrowable, ResolveDerivationPathMethod, **Context, *FJavaUtils::GetJUri(DerivationPath), Purpose);
		*OutException = JThrowable ? MakeShareable(FThrowable::Construct(JThrowable)) : nullptr;
	}
	else
	{
		JRetVal = CallStaticMethod<jobject>(ResolveDerivationPathMethod, **Context, *FJavaUtils::GetJUri(DerivationPath), Purpose);
	}

	return *OutException ? "" : FJavaUtils::JUriToString(JRetVal);	
}

#endif
