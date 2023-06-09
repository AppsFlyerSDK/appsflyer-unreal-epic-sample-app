#pragma once
#include "RequestData.h"
#include <iostream>
#include <string>

#define UI UI_ST
// THIRD_PARTY_INCLUDES_START
#include "openssl/evp.h"
#include "openssl/hmac.h"
// THIRD_PARTY_INCLUDES_END
#undef UI

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

class CAppsflyerEpicModule
{
public:
	// This method receives your api key and app id,
	// and initializes the AppsFlyer Connector
	void Init(const char *devkey, const char *appID);
	// Sends "first open/session" request to AppsFlyer.
	void Start(bool skipFirst = false);
	// This method receives an event name and json object and sends an in-app event to AppsFlyer.
	void LogEvent(std::string event_name, std::string event_parameters);
	// Get AppsFlyer's unique device ID.
	std::string GetAppsFlyerUID();
	// This method receives a date string and returns true if the game folder modification date is older than the date string.
	bool IsInstallOlderThanDate(std::string datestring);

private:
	const char *devkey;
	const char *appID;
	friend CAppsflyerEpicModule *AppsflyerEpicModule();
	CAppsflyerEpicModule();
	void SendHTTPReq(FHttpRequestRef pRequest, uint64 context);
	RequestData buildRequestData();
};

const int64 FIRST_OPEN_REQUEST = 100;
const int64 SESSION_REQUEST = 101;
const int64 INAPP_EVENT_REQUEST = 102;

CAppsflyerEpicModule *AppsflyerEpicModule();