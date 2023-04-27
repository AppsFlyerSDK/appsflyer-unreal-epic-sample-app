---
title: Unreal Epic
category: 6446526dddf659006c7ea807
order: 2
hidden: true
slug: unreal-epic
---

> Link to repository
> [GitHub](https://github.com/AppsFlyerSDK/appsflyer-unreal-epic-sample-app)

# AppsFlyer Unreal Epic Integration

> 🚧 OS-Restriction
>
> This sample code currently supports windows environment only.

## **Getting started with AppsFlyer Unreal Epic Integration**

AppsFlyer empowers marketers to make better decisions by providing game marketers with powerful tools to perform cross-platform attribution.
Game attribution requires the game to integrate an AppsFlyer SDK, which handles first open, consecutive sessions, and in-app events. For example, purchase events.

We recommend you use this sample app as a reference for integrating AppsFlyer SDK into your Unreal-Epic game.

<hr/>

### Prerequisite

1. Unreal Engine 4.2x
2. [EOS Subsystem](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/Online/EOS/) integrated within your UE4 (it's included within the UE4 3rd parties, there is no need to download it)
<hr/>

## **AppsflyerEpicModule - Interface**

`AppsflyerEpicModule.h`, included in the folder above, contains the required code and logic to connect to our servers and report events.

### `void start(const char* devkey, const char* appID)`

This method receives your api key and app id, and initializes the AppsFlyer Module (sends “first open/session” request to AppsFlyer).

_Usage_:

```
AppsflyerEpicModule()->start("DEV_KEY", "EPIC_APP_ID");
```

_Arguments_:

- DEV_KEY - retrieve the Dev key from the marketer or the [AppsFlyer HQ](https://support.appsflyer.com/hc/en-us/articles/211719806-App-settings-#general-app-settings).
- EPIC_APP_ID - you may find your app id on the [Epic Dev Portal](https://dev.epicgames.com/portal/en-US/).

### `void **logEvent**(std::string **event_name**, json **event_values**)`

This method receives an event name and json object and sends an in-app event to AppsFlyer.

_Usage_:

```
//set event name
std::string event_name = "af_purchase";
//set json string
std::string event_values = "{\"af_currency\":\"USD\",\"af_price\":6.66,\"af_revenue\":24.12}";
AppsflyerEpicModule()->logEvent(event_name, event_values);
```

## Running the Sample App

1. Open UE4 engine.
2. Choose New Project -> Games -> First Person
3. Choose C++ instead of Blueprints
4. Name the project AppsFlyerSample and press "create project".
5. Follow [the steps below](#implementing-appsflyer-into-your-own-epic-game)
6. Launch the sample app from the UE4 engine editor
7. After 24 hours, the dashboard will update and show organic/non-organic install and in-app events.

<hr/>

## **Implementing AppsFlyer into your own Epic game**

### Set Up

1. Make sure that Epic is in your UE4 third parties https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/Online/EOS/
2. Add the following definitions to `Config/DefaultEngine.ini`:
   (for reference please check `appsflyer-unreal-Epic-sample-app/AppsflyerEpicIntegrationFiles/DefaultEngine.ini` file)

```

[OnlineSubsystem]
DefaultPlatformService=EOSPlus

[OnlineSubsystemEOS]
bEnabled=true

[OnlineSubsystemEOSPlus]
bEnabled=true

[/Script/OnlineSubsystemEOS.NetDriverEOS]
bIsUsingP2PSockets=true
NetConnectionClassName="OnlineSubsystemEOS.NetConnectionEOS"

[/Script/OnlineSubsystemEOS.EOSSettings]
CacheDir=CacheDir
DefaultArtifactName=Confidential
TickBudgetInMilliseconds=0
bEnableOverlay=False
bEnableSocialOverlay=False
bShouldEnforceBeingLaunchedByEGS=False
TitleStorageReadChunkLength=0
+Artifacts=(ArtifactName="Confidential",ClientId="Confidential",ClientSecret="Confidential",ProductId="Confidential",SandboxId="Confidential",DeploymentId="Confidential",EncryptionKey="Confidential")
bUseEAS=False
bUseEOSConnect=True
bMirrorStatsToEOS=False
bMirrorAchievementsToEOS=False
bUseEOSSessions=True
bMirrorPresenceToEAS=False
```

Please replace the "Confidential" credentials with your EOS credentials.

3. In your unreal editor, go to plugins and activate "Online Subsystem Steam", then restart the editor.
4. Open your the project in your preferred C++ editor, then in `[YOUR-APP-NAME].Build.cs` file add `OpenSSL`, `OnlineSubsystem`, `OnlineSubsystemEOS` to your dependencies:

```
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OpenSSL", "OnlineSubsystem", "OnlineSubsystemEOS" });
```

5. In your Unreal Project files, under **"Source"** directory, create a new directory under the name **"AppsflyerSteamModule"**
6. Copy the following files from "appsflyer-unreal-epic-sample-app/AppsflyerEpicIntegrationFiles/AppsflyerEpicModule" to the new folder:

- AppsflyerModule.cpp
- AppsflyerEpicModule.cpp
- AppsflyerEpicModule.h
- DeviceID.h
- RequestData.h

7. Generate projecte files in order to add OpenSSL https://forums.unrealengine.com/t/how-to-use-included-openssl/670971/2
8. In the GameMode.h file, add StartPlay() function:

```UCLASS(minimalapi)
class AAppsFlyerSampleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAppsFlyerSampleGameMode();
	virtual void StartPlay() override;
};

```

9. Open "Source/AppsFlyerSample/AppsFlyerSampleGameMode.cpp" file
   Add the following include to your GameMode.cpp file:

```
#include "AppsflyerEpicModule/AppsflyerEpicModule.cpp"
```

And the following function:

```

void AEpicTestGameMode::StartPlay()
{
	Super::StartPlay();

	EOS_InitializeOptions SDKOptions;
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = NULL;
	SDKOptions.ReallocateMemoryFunction = NULL;
	SDKOptions.ReleaseMemoryFunction = NULL;
	SDKOptions.ProductName = "PRODUCT_NAME";
	SDKOptions.ProductVersion = "1.0";
	SDKOptions.Reserved = NULL;
	SDKOptions.SystemInitializeOptions = NULL;
	EOS_EResult res = EOS_Initialize(&SDKOptions);

	if (res == EOS_EResult::EOS_Success || res == EOS_EResult::EOS_AlreadyConfigured) {
		EOS_Platform_Options PlatformOptions;
		PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
		PlatformOptions.Reserved = NULL;
		PlatformOptions.bIsServer = false;
		PlatformOptions.EncryptionKey = NULL;
		PlatformOptions.OverrideCountryCode = NULL;
		PlatformOptions.OverrideLocaleCode = NULL;
		PlatformOptions.ProductId = "PRODUCT_ID";
		EOS_Platform_ClientCredentials cCreds;
		cCreds.ClientId = "CLIENT_ID";
		cCreds.ClientSecret = "CLIENT_SECRET";
		PlatformOptions.ClientCredentials = cCreds;
		PlatformOptions.SandboxId = "SANDBOX_ID";
		PlatformOptions.DeploymentId = "DEPLOYMENT_ID";
		PlatformOptions.EncryptionKey = "ENCRYPTION_KEY";
		EOS_HPlatform platform = EOS_Platform_Create(&PlatformOptions);

		// af module init
		AppsflyerEpicModule()->init("bFzaVu2iecN77po5mWMJuL", "butterflyer");
		// af send firstopen/session
		AppsflyerEpicModule()->start();

		//set event name
		std::string event_name = "af_purchase";
		//set json string
		std::string event_values = "{\"af_currency\":\"USD\",\"af_price\":6.66,\"af_revenue\":24.12}";
		// af send inapp event
		AppsflyerEpicModule()->logEvent(event_name, event_values);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("EOS FAIL"));
		return;
	}
}
```

Make sure to replace **DEV_KEY** and **EPIC_APP_ID** in the [`start`](#void-startconst-char-devkey-const-char-appid) function with your [app details](#App-Details) and to intiziallize the EOS SDK options

10. [Initialize](#void-startconst-char-devkey-const-char-appid) the AppsFlyer integration
11. Report [in-app events](#void-logeventstdstring-event_name-json-event_values)
