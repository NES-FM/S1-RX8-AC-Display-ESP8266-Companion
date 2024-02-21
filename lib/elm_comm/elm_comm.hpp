#pragma once
#include <Arduino.h>
#include <ELMduino.h>
#include <SoftwareSerial.h>

#include "../../include/pins.h"

class elm_comm {
  public:
    void init();
    void tick();

  private:
    SoftwareSerial *btser;
    ELM327 *elm;

    void initializeElm();
    bool hasInitializedElm = false;

    unsigned long lastLoop = 0;
};
