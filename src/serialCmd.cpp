#include "serialCmd.h"

void SerialCommand::showHelp() {
    Serial.println(F("\nUseage: $>[command] [parameter]"));
    Serial.println(F("Commands: help, view, ssid, passwd, key, city, reset, apply, exit"));
    Serial.println(F("help: show this texts, e.g. $>help"));
    Serial.println(F("view: show current parameters, e.g. $>view"));
    Serial.println(F("ssid[n0-7]: update n's WIFI SSID, e.g. $>ssid1 wifi_ssid"));
    Serial.println(F("passwd[n0-7]: update n's WIFI Password, e.g. $>passwd3 wifi_password"));
    Serial.println(F("key: update OpenWeather API Key, e.g. $>key api_key"));
    Serial.println(F("\tUse your own API key by signing up for a free developer"));
    Serial.println(F("\taccount at https://openweathermap.org/"));
    Serial.println(F("city: update City Name, e.g. $>city city_name"));
    Serial.println(F("\tYour home city See: http://bulk.openweathermap.org/sample/"));
    Serial.println(F("reset: Reboot machine, e.g. $>reset"));
    Serial.println(F("apply: Apply updated parameters to EEPROM, e.g. $>apply"));
    Serial.println(F("exit: Return to boot process, e.g. $>exit"));
    Serial.println();
    Serial.flush();
}

void SerialCommand::showCurrents() {
    Serial.println(F("\nCurrent -"));
    Serial.println(F("\tWIFI SSID / passwd: "));

    for(int i=0; i<Credentials::maxSSIDs; ++i) {
        if(!credentials_.SSID[i].isEmpty()) {
            Serial.print("\t");
            Serial.println(String(i) + ": " + credentials_.SSID[i] + " / " + credentials_.passwd[i]);
        }
    }
    Serial.print(F("\tCity Name: "));
    Serial.println(credentials_.City);
    Serial.print(F("\tOpenWeather API Key: "));
    Serial.println(credentials_.API_key);
    Serial.println();
    Serial.flush();
}

String SerialCommand::getInput() {
    Serial.print(F("$>"));

    constexpr int maxBufferSize = 64;
    char buf[maxBufferSize + 1];
    buf[maxBufferSize] = 0;

    bool enter = false;
    while (!enter) {
        int rxlen = Serial.available();
        if (rxlen > 0) {
            int rlen = rxlen > maxBufferSize ? maxBufferSize : rxlen;
            rlen = Serial.readBytes(buf, rlen);

            for (int i = 0; i < rlen; ++i) {
                if (31 >= buf[i]) {
                    buf[i] = 0;
                    enter = true;
                    break;
                }
            }
        }
        delay(100);
    }
    String input(buf);
    input.trim();
    Serial.println(input);

    return input;
}

bool SerialCommand::getYesNo() {
    constexpr int maxBufferSize = 4;
    char buf[maxBufferSize];

    bool enter = false;
    while (!enter) {
        int rxlen = Serial.available();
        if (rxlen > 0) {
            int rlen = rxlen > maxBufferSize ? maxBufferSize : rxlen;
            rlen = Serial.readBytes(buf, rlen);

            for (int i = 0; i < rlen; ++i) {
                if (31 >= buf[i]) {
                    buf[i] = 0;
                    enter = true;
                    break;
                }
            }
        }
    }
    String input(buf);
    input.trim();
    input.toLowerCase();
    if (input == "y" || input == "yes") {
        Serial.println(F("yes"));
        return true;
    }
    Serial.println(F("no"));
    return false;
}

void SerialCommand::updateSSID(int id, String text) {
    text.trim();
    Serial.print(F("Do you want to update WIFI SSID["));
    Serial.println(String(id) + "] to " + text + " ? (y/n) ");
    Serial.flush();
    if (getYesNo()) {
        credentials_.SSID[id] = text;
    }
}

void SerialCommand::updatePASSWD(int id, String text) {
    text.trim();
    Serial.print(F("Do you want to update WIFI PASSWORD["));
    Serial.println(String(id) + "] to " + text + " ? (y/n) ");
    Serial.flush();
    if (getYesNo()) {
        credentials_.passwd[id] = text;
    }
}

void SerialCommand::updateKEY(String text) {
    text.trim();
    Serial.print(F("Do you want to update OpenWeather API key to "));
    Serial.print(text);
    Serial.println(F(" ? (y/n) "));
    Serial.flush();
    if (getYesNo()) {
        credentials_.API_key = text;
    }
}

void SerialCommand::updateCITY(String text) {
    text.trim();
    Serial.print(F("Do you want to update CITY to "));
    Serial.print(text);
    Serial.println(F(" ? (y/n) "));
    Serial.flush();
    if (getYesNo()) {
        credentials_.City = text;
    }
}

void SerialCommand::applyUpdates() {
    showCurrents();
    Serial.println(F("Do you want to apply updates to EEPROM? (y/n) "));
    Serial.flush();
    if (getYesNo()) {
        credentials_.write();
    }
}

int isNum(char c) {
    return '0' <= c && c <= '9' ? c - '0' : -1;
}
int SerialCommand::cmd() {
    String input = getInput();
    int length = input.length();
    if (4 == length && input.startsWith("help")) {
        showHelp();
        return HELP;
    }
    if (4 == length && input.startsWith("view")) {
        showCurrents();
        return VIEW;
    }
    if (5 < length && input.startsWith("ssid") && 0 <= isNum(input[4]) && input[5] == ' ') {
        updateSSID(isNum(input[4]), input.substring(6));
        return SSID;
    }
    if (7 < length && input.startsWith("passwd") && 0 <= isNum(input[6]) && input[7] == ' ') {
        updatePASSWD(isNum(input[6]), input.substring(8));
        return PASSWD;
    }
    if (4 < length && input.startsWith("key ")) {
        updateKEY(input.substring(4));
        return KEY;
    }
    if (5 < length && input.startsWith("city ")) {
        updateCITY(input.substring(5));
        return CITY;
    }
    if (5 == length && input.startsWith("reset")) {
        ESP.restart();
        return RESET;
    }
    if (5 == length && input.startsWith("apply")) {
        applyUpdates();
        return APPLY;
    }
    if (4 == length && input.startsWith("exit"))
        return EXIT;
    return NONE;
}

void SerialCommand::loop() {
    while (Serial.available() > 0) {
        Serial.read();
    }

    showCurrents();
    showHelp();
    while (EXIT != cmd());
}