#pragma once
#include <Arduino.h>
#include <ELMduino.h>
#include <SoftwareSerial.h>

#include "../../include/pins.h"

class elm_comm {
  public:
    void init();
    void tick();

    bool isInitialized() { return hasInitializedElm; }
    void initializeElm();

  private:
    SoftwareSerial *btser;
    ELM327 *elm;

    bool hasInitializedElm = false;

    unsigned long lastLoop = 0;
};
