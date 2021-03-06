/****************************************************************************************************************************
   defines.h
   For ESP32 boards

   Blynk_Async_ESP32_BT_WF is a library, using AsyncWebServer instead of (ESP8266)WebServer for inclusion of both ESP32 
   Blynk BT/BLE and WiFi libraries. Then select either one or both at runtime.
   
   Based on and modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
   Built by Khoi Hoang https://github.com/khoih-prog/Blynk_Async_ESP32_BT_WF
   Licensed under MIT license

   Version: 1.0.6

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.6   K Hoang      25/08/2020 Initial coding to use (ESP)AsyncWebServer instead of (ESP8266)WebServer. 
                                    Bump up to v1.0.16 to sync with BlynkESP32_BT_WF v1.0.6.
 *****************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#ifndef ESP32
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#define BLYNK_PRINT               Serial

#define ESP32_BLE_WF_DEBUG        true

#define DOUBLERESETDETECTOR_DEBUG     true
#define BLYNK_WM_DEBUG                3

// Not use #define USE_SPIFFS  => using EEPROM for configuration data in WiFiManager
// #define USE_SPIFFS    false => using EEPROM for configuration data in WiFiManager
// #define USE_SPIFFS    true  => using SPIFFS for configuration data in WiFiManager
// Those above #define's must be placed before #include <BlynkSimpleEsp32_Async_WFM.h>

#define USE_SPIFFS                  true
//#define USE_SPIFFS                  false

#if (!USE_SPIFFS)
  // EEPROM_SIZE must be <= 2048 and >= CONFIG_DATA_SIZE (currently 172 bytes)
  #define EEPROM_SIZE    (2 * 1024)
  // EEPROM_START + CONFIG_DATA_SIZE must be <= EEPROM_SIZE
  #define EEPROM_START   0
#endif

// Force some params in Blynk, only valid for library version 1.0.1 and later
#define TIMEOUT_RECONNECT_WIFI                    10000L
#define RESET_IF_CONFIG_TIMEOUT                   true
#define CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET    5
// Those above #define's must be placed before #include <BlynkSimpleEsp32_Async_WFM.h>

//#define BLYNK_USE_BT_ONLY      true
#define BLYNK_USE_BT_ONLY      false

#include <BlynkSimpleEsp32_Async_BT_WF.h>

#define USE_BLYNK_WM          true
//#define USE_BLYNK_WM          false

#if USE_BLYNK_WM
  #warning Please select 1.3MB+ for APP (Minimal SPIFFS (1.9MB APP, OTA), HugeAPP(3MB APP, NoOTA) or NoOA(2MB APP)
  #include <BlynkSimpleEsp32_Async_WFM.h>  
#else
  #include <BlynkSimpleEsp32_Async_WF.h>
  
  String cloudBlynkServer = "account.duckdns.org";
  //String cloudBlynkServer = "192.168.2.110";
  #define BLYNK_SERVER_HARDWARE_PORT    8080
  char ssid[] = "SSID";
  char pass[] = "PASSPASS";

  // WiFi Blynk token
  char WiFi_auth[]  = "WF_token";
  
  // BT Blynk token, not shared between BT and WiFi
  char BT_auth[]    = "BT_token";
  
  // BLE Blynk token, not shared between BT and WiFi
  char BLE_auth[]    = "BLE_token";  
#endif

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define HOST_NAME   "ASYNC-ESP32-BLE-WF"

#endif      //defines_h
