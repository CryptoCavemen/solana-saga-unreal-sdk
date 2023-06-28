//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SigningResponseWrapper.h"

#if PLATFORM_ANDROID

BEGIN_IMPLEMENT_JAVA_CLASS_OBJECT(FSigningResponseWrapper, FJavaClassObjectWrapper, "com/solanamobile/seedvault/SigningResponse", "(Ljava/util/List;Ljava/util/List;)V")
	GetSignaturesMethod = GetClassMethod("getSignatures", "()Ljava/util/List;");
	GetResolvedDerivationPathsMethod = GetClassMethod("getResolvedDerivationPaths", "()Ljava/util/List;");	
END_IMPLEMENT_JAVA_CLASS_OBJECT

TSharedRef<FArrayList> FSigningResponseWrapper::GetSignatures()
{
	jobject JSignatures = CallMethod<jobject>(GetSignaturesMethod);
	auto Signatures = FArrayList::CreateFromExisting(JSignatures);	
	return Signatures;
}

TSharedRef<FArrayList> FSigningResponseWrapper::GetResolvedDerivationPaths()
{
	jobject JResolvedDerivationPaths = CallMethod<jobject>(GetResolvedDerivationPathsMethod);
	auto ResolvedDerivationPaths = FArrayList::CreateFromExisting(JResolvedDerivationPaths);	
	return ResolvedDerivationPaths;
}

#endif
