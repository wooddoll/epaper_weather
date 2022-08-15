#ifndef _CREDENTIALS_H_
#define _CREDENTIALS_H_

#include <Arduino.h>

class Credentials {
public:
    Credentials();
    ~Credentials();

    void read();
    void write();

public:
    static constexpr int maxSSIDs = 8;
    String SSID[maxSSIDs];
    String passwd[maxSSIDs];
    String API_key;
    String City;
    int FailCount;
};

#endif//_CREDENTIALS_H_