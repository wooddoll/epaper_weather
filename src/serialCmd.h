#ifndef _SERIAL_COMMAND_H_
#define _SERIAL_COMMAND_H_

#include <Arduino.h>
#include "credentials.h"

class SerialCommand {
public:
    SerialCommand(Credentials &credentials) : credentials_(credentials) {}
    ~SerialCommand() {}

    void showHelp();
    void showCurrents();

    String getInput();
    bool getYesNo();

    void updateSSID(int id, String text);
    void updatePASSWD(int id, String text);
    void updateKEY(String text);
    void updateCITY(String text);

    void applyUpdates();
    
    int cmd();
    void loop();

public:
    enum { NONE, HELP, VIEW, SSID, PASSWD, KEY, CITY, RESET, APPLY, EXIT };
    Credentials &credentials_;
};

#endif//_SERIAL_COMMAND_H_