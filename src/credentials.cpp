#include "credentials.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "utils.h"

#define FILE_CFG    "/credentials.json"

Credentials::Credentials() {
    if(!SPIFFS.begin()) {
        Serial.println(F("ERROR mounting SPIFFS file system"));
    }
    read();
}

Credentials::~Credentials() {
}

void Credentials::read() {
    String json = readSPIFFS(FILE_CFG);

    StaticJsonDocument<1024> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);
    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    API_key = doc["API_KEY"].as<const char*>();
    City = doc["City"].as<const char*>();
    uint8_t size = doc["WIFI"].size();
    for(int i=0; i<size; ++i) {        
        SSID[i] = doc["WIFI"][i]["SSID"].as<const char*>();
        passwd[i] = doc["WIFI"][i]["passwd"].as<const char*>();
    }
    FailCount = doc["FailCount"].as<int>();
}

void Credentials::write() {
    StaticJsonDocument<1024> doc;

    doc["API_KEY"] = API_key;
    doc["City"] = City;

    int idx = 0;
    for(int i=0; i<maxSSIDs; ++i) {
        if(!SSID[i].isEmpty()) {
            doc["WIFI"][idx]["SSID"] = SSID[i];

            if(!passwd[i].isEmpty())
                doc["WIFI"][idx]["passwd"] = passwd[i];

            ++idx;
        }
    }

    doc["FailCount"] = FailCount;

    String json;
    serializeJsonPretty(doc, json);

    writeSPIFFS(FILE_CFG, json);
}