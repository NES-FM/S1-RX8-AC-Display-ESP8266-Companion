#include "main.hpp"

ESP8266WiFiMulti wifiMulti;
AsyncWebServer server(80);
bool connectioWasAlive = false;

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Create a file `include/secrets.h` with String wifi_aps[n][2] for n access points, with each AP={"SSID", "PASSWORD"}
  for (auto ap : wifi_aps) {
    wifiMulti.addAP(ap[0].c_str(), ap[1].c_str());
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
}

bool monitorWiFi()
{
  if (wifiMulti.run() != WL_CONNECTED)
  {
    if (connectioWasAlive == true)
    {
      connectioWasAlive = false;
      Serial.print("Looking for WiFi ");
    }
    return false;
  }
  else if (connectioWasAlive == false)
  {
    connectioWasAlive = true;
    Serial.printf("Wifi connected to %s with IP %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  }
  return true;
}

void loop(void) {
  if (monitorWiFi()) {
    ElegantOTA.loop();
  }

  if (Serial.available()) {
    Serial.write(Serial.read());
  }

  delay(5);
}
