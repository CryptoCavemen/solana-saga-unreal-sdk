#include "MobileWalletAdapterClientBP.h"
#include "Android/GameActivity.h"
#include "Android/LocalAssociationIntentCreator.h"
#include "Android/LocalAssociationScenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif

UMobileWalletAdapterClientBP::UMobileWalletAdapterClientBP()
{
/*
#if PLATFORM_ANDROID
	if (!HasAnyFlags(RF_ClassDefaultObject))
		Wrapper = FMobileWalletAdapterClient::Construct(nullptr, DEFAULT_CLIENT_TIMEOUT_MS);
	else
		Wrapper = nullptr;
#endif
*/
}

UMobileWalletAdapterClientBP::~UMobileWalletAdapterClientBP()
{
/*	
#if PLATFORM_ANDROID	
	if (Wrapper)
		delete Wrapper;
#endif
*/
}

void UMobileWalletAdapterClientBP::LocalAssociateAndExecute(FString UriPrefix)
{
#if PLATFORM_ANDROID
	auto Activity = FGameActivity::MakeFromExistingObject(FAndroidApplication::GetGameActivityThis());
	
	auto LocalAssociation = FLocalAssociationScenario::MakeInstance(DEFAULT_CLIENT_TIMEOUT_MS);
	auto AssociationIntent = FLocalAssociationIntentCreator::CreateAssociationIntent(UriPrefix.IsEmpty() ? nullptr : &UriPrefix, LocalAssociation->GetPort(), *LocalAssociation->GetSession());

	Activity->StartActivityForResult(AssociationIntent, 55);

	const int64 LOCAL_ASSOCIATION_START_TIMEOUT_MS = 60000L; // LocalAssociationScenario.start() has a shorter timeout; this is just a backup safety measure	
	LocalAssociation->Start()->Get(LOCAL_ASSOCIATION_START_TIMEOUT_MS);

/*
	public static final String CLUSTER_MAINNET_BETA = "mainnet-beta";
	public static final String CLUSTER_TESTNET = "testnet";
	public static final String CLUSTER_DEVNET = "devnet";*
		
	private val CLUSTER_RPC_URI = Uri.parse("https://api.testnet.solana.com")
	private const val CLUSTER_NAME = ProtocolContract.CLUSTER_TESTNET
	private val IDENTITY = MobileWalletAdapterUseCase.DappIdentity(
		uri = Uri.parse("https://solanamobile.com"),
		iconRelativeUri = Uri.parse("favicon.ico"),
		name = "FakeDApp"
	)
*/
#endif
}

/*
void UMobileWalletAdapterClientBP::Authorize(FString IdentityUri, FString IconUri, FString IdentityName, FString Cluster)
{
#if PLATFORM_ANDROID
	Wrapper->Authorize(IdentityUri, IconUri, IdentityName, Cluster);
#endif
}
*/
