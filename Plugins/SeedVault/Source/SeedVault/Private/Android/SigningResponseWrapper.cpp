//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SigningResponseWrapper.h"

#if PLATFORM_ANDROID
#include "ArrayList.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"

using namespace SeedVault;

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FSigningResponseWrapper, FJavaClassObjectWrapper, "com/solanamobile/seedvault/SigningResponse", "(Ljava/util/List;Ljava/util/List;)V")
	GetSignaturesMethod = GetClassMethod("getSignatures", "()Ljava/util/List;");
	GetResolvedDerivationPathsMethod = GetClassMethod("getResolvedDerivationPaths", "()Ljava/util/List;");	
END_IMPLEMENT_JAVA_CLASS_OBJECT

TSharedRef<FArrayList> FSigningResponseWrapper::GetSignatures()
{
	jobject JSignatures = CallMethod<jobject>(GetSignaturesMethod);
	auto Signatures = FArrayList::MakeFromExistingObject(JSignatures);	
	return Signatures;
}

TSharedRef<FArrayList> FSigningResponseWrapper::GetResolvedDerivationPaths()
{
	jobject JResolvedDerivationPaths = CallMethod<jobject>(GetResolvedDerivationPathsMethod);
	auto ResolvedDerivationPaths = FArrayList::MakeFromExistingObject(JResolvedDerivationPaths);	
	return ResolvedDerivationPaths;
}

#endif
