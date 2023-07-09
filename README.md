---
title: Unreal Epic
category: 6446526dddf659006c7ea807
order: 2
hidden: false
slug: unreal-epic
---

> Link to repository  
> [GitHub](https://github.com/AppsFlyerSDK/appsflyer-unreal-epic-sample-app)

## AppsFlyer Unreal Epic SDK integration

AppsFlyer empowers gaming marketers to make better decisions by providing powerful tools to perform cross-platform attribution.

Game attribution requires the game to integrate the AppsFlyer SDK that records first opens, consecutive sessions, and in-app events. For example, purchase events.
We recommend you use this sample app as a reference for integrating the AppsFlyer SDK into your Unreal Epic game. **Note**: The sample code that follows is currently only supported in a Windows environment.

### Prerequisites

- Unreal Engine 4.2x.
- [EOS Subsystem](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/Online/EOS/) integrated within your UE4 (usually it’s included within the UE4 third-parties and there’s no need to download it).

<hr/>

## AppsflyerEpicModule - Interface

`AppsflyerEpicModule.h`, included in the `appsflyer-unreal-epic-sample-app/AppsflyerEpicIntegrationFiles/AppsflyerEpicModule` folder, contains the required code and logic to connect to AppsFlyer servers and report events.

### Init

This method receives your API key and app ID and initializes the AppsFlyer Module.

**Method signature**

```c++
void Init(const char* devkey, const char* appID)
```

**Usage**:

```c++
AppsflyerEpicModule()->Init(<< DEV_KEY >>, << EPIC_APP_ID >>);
```

<span id="app-details">**Arguments**:</span>

- `EPIC_APP_ID`: Your Epic app id.
- `DEV_KEY`: Get from the marketer or [AppsFlyer HQ](https://support.appsflyer.com/hc/en-us/articles/211719806-App-settings-#general-app-settings).

### Start

This method sends first open/session requests to AppsFlyer.

**Method signature**

```c++
void Start(bool skipFirst = false)
```

**Usage**:

```c++
// without the flag
AppsflyerEpicModule()->Start();

// with the flag
bool skipFirst = [SOME_CONDITION];
AppsflyerEpicModule()->Start(skipFirst);
```

### LogEvent

This method receives an event name and JSON object and sends in-app events to AppsFlyer.

**Method signature**

```c++
void LogEvent(std::string event_name, json event_parameters)
```

**Usage**:

```c++
//set event name
std::string event_name = "af_purchase";
//set json string
std::string event_parameters = "{\"af_currency\":\"USD\",\"af_price\":6.66,\"af_revenue\":24.12}";
AppsflyerEpicModule()->LogEvent(event_name, event_parameters);
```

### GetAppsFlyerUID

Get AppsFlyer's unique device ID. The SDK generates an AppsFlyer unique device ID upon app installation. When the SDK is started, this ID is recorded as the ID of the first app install.

**Method signature**

```c++
std::string GetAppsFlyerUID()
```

**Usage**:

```c++
AppsflyerEpicModule()->GetAppsFlyerUID();
```

### IsInstallOlderThanDate

This method receives a date string and returns true if the game folder modification date is older than the date string. The date string format is: "2023-January-01 23:12:34"

**Method signature**

```c++
bool IsInstallOlderThanDate(std::string datestring)
```

**Usage**:

```c++
// the modification date in this example is "2023-January-23 08:30:00"

// will return false
bool dateBefore = AppsflyerSteamModule()->IsInstallOlderThanDate("2023-January-01 23:12:34");

// will return true
bool dateAfter = AppsflyerSteamModule()->IsInstallOlderThanDate("2023-April-10 23:12:34");

// example usage with skipFirst:
bool isInstallOlderThanDate = AppsflyerSteamModule()->IsInstallOlderThanDate("2023-April-10 23:12:34");
AppsflyerSteamModule()->Start(isInstallOlderThanDate);
```

## Running the sample app

1. Open the UE4 engine.
2. Choose **New Project** > **Games** > **First Person**.
3. Choose C++ (instead of Blueprints).
4. Name the project `AppsFlyerSample` and click **Create project**.
5. Follow the [instructions to implement AppsFlyer in your Epic game](#implementing-appsflyer-in-your-epic-game).
6. Launch the sample app from the UE4 engine editor.
7. After 24 hours, the dashboard updates and shows organic and non-organic installs and in-app events.

## **Implementing AppsFlyer in your Epic game**

### Setup

1. Make sure that Epic is in your UE4 third parties. [Learn more](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/Online/EOS/)
2. Add the following definitions to `Config/DefaultEngine.ini` while replacing the `confidential` credentials with your EOS credentials. For reference, see the `appsflyer-unreal-Epic-sample-app/AppsflyerEpicIntegrationFiles/DefaultEngine.ini` file.

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

3. In your Unreal editor, go to **Plugins**, activate **Online Subsystem EOS**, and then restart the editor.
4. Open the project in your preferred C++ editor and in the `[YOUR-APP-NAME].Build.cs` file, add `OpenSSL`, `OnlineSubsystem`, and `OnlineSubsystemEOS` to your dependencies and `HTTP` as a private dependency:

```c#
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OpenSSL", "OnlineSubsystem", "OnlineSubsystemEOS" });
PrivateDependencyModuleNames.Add("HTTP");
```

5. In your Unreal Project files, under the **Source** directory, create a new directory named **AppsflyerEpicModule**.
6. Copy the following files from `appsflyer-unreal-epic-sample-app/AppsflyerEpicIntegrationFiles/AppsflyerEpicModule` to the new folder:

   - AppsflyerModule.cpp
   - AppsflyerEpicModule.cpp
   - AppsflyerEpicModule.h
   - DeviceID.h
   - RequestData.h

7. Generate project files in order to add OpenSSL. [Learn more](https://forums.unrealengine.com/t/how-to-use-included-openssl/670971/2)
8. In the `GameMode.h` file, add the `StartPlay()` function:

```c++
UCLASS(minimalapi)
class AAppsFlyerSampleGameMode : public AGameModeBase
{
 GENERATED_BODY()

public:
 AAppsFlyerSampleGameMode();
 virtual void StartPlay() override;
};

```

9. Open the `Source/AppsFlyerSample/AppsFlyerSampleGameMode.cpp` file and add the following include to your GameMode.cpp file.

```c++
#include "AppsflyerEpicModule/AppsflyerEpicModule.cpp"
#include <ThirdParty/EOSSDK/SDK/Include/eos_init.h>
#include <ThirdParty/EOSSDK/SDK/Include/eos_sdk.h>
#include <EOSSDK/SDK/Include/eos_userinfo.h>
using Super = AGameModeBase;
```

10. Add the following function, making sure to replace `DEV_KEY` and `EPIC_APP_ID` in the [`init`](#init) function with your [app details](#app-details) and initialize the EOS SDK with your EOS details.

```c++
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
  AppsflyerEpicModule()->Init(<< DEV_KEY >>, << EPIC_APP_ID >>);
  // af send firstopen/session
  AppsflyerEpicModule()->Start();

  //set event name
  std::string event_name = "af_purchase";
  //set json string
  std::string event_parameters = "{\"af_currency\":\"USD\",\"af_price\":6.66,\"af_revenue\":24.12}";
  // af send inapp event
  AppsflyerEpicModule()->LogEvent(event_name, event_parameters);
 }
 else {
  UE_LOG(LogTemp, Warning, TEXT("EOS FAIL"));
  return;
 }
}
```

11. Report [in-app events](#logevent)

## Resetting the attribtion

Delete the `appsflyer_info` file:

![Delete the](https://files.readme.io/d43d8ce-image.png)
