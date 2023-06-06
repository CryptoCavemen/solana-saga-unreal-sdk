![sms-unreal](https://github.com/CryptoCavemen/solana-saga-unreal-sdk/assets/1970424/ac1b281a-6be1-4deb-af71-52170fc3f970)

# solana-saga-unreal-sdk

## Repository stage: `pre-release`
This repository is still pre-release. There are a few show stopping known issues. The following describes expected experience for each functionality in the example project:

### Wallet Adapter
* Authorize: Works
* Reauthorize: Works, but requires manual closing of wallet app
* Deauthorize: Works, but requires manual closing of wallet app
* Sign Transaction: Doesn't work yet, due to known bug

The above functionality lapses are mainly caused by Unreal suspending its' main game thread when the app is detected as going into background. Unreal's default implementation for HTTP/WebSocket requests runs all networking on that main thread. Once we work around that, basic functionality  should be there.

### Seed Vault
Currently, there is an integration in code but it has not yet been externalized to the example app.

## Running the example project
