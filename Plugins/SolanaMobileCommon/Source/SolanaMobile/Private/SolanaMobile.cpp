//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

#include "SolanaMobile.h"

DEFINE_LOG_CATEGORY(LogSolanaMobile);

void FSolanaMobileModule::StartupModule()
{
}

void FSolanaMobileModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSolanaMobileModule, SolanaMobile)