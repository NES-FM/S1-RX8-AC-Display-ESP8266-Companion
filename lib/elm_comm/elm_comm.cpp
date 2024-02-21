#include "elm_comm.hpp"

void elm_comm::init() {
    btser = new SoftwareSerial( PIN_BT_RX, PIN_BT_TX );
    btser->begin( 9600 );

    elm = new ELM327();

    hasInitializedElm = false;
}

void elm_comm::tick() {
    if ( millis() - lastLoop > 50 ) {
        if ( !hasInitializedElm ) {
            initializeElm();
            return;
        }

        unsigned long a = millis();

        float load = elm->engineLoad();
        float coolantTemp = elm->engineCoolantTemp();
        float rpm = elm->rpm();
        int32_t kph = elm->kph();
        float timingAdvance = elm->timingAdvance();
        float intakeAirTemp = elm->intakeAirTemp();
        float throttle = elm->throttle();
        float fuelLevel = elm->fuelLevel();
        float absLoad = elm->absLoad();
        float commandedAirFuelRatio = elm->commandedAirFuelRatio();
        float ambientAirTemp = elm->ambientAirTemp();
        uint8_t fuelType = elm->fuelType();
        float ethanolPercent = elm->ethanolPercent();
        float oilTemp = elm->oilTemp();
        float demandedTorque = elm->demandedTorque();
        float torque = elm->torque();
        uint16_t referenceTorque = elm->referenceTorque();

        Serial.printf( "load:%s,coolantTemp:%s,rpm:%s,kph:%d,timingAdvance:%s,intakeAirTemp:%s,throttle:%s,"
                       "fuelLevel:%s,absLoad:%s,commandedAirFuelRatio:%s,ambientAirTemp:%s,fuelType:%d,"
                       "ethanolPercent:%s,oilTemp:%s,demandedTorque:%s,torque:%s,referenceTorque:%d,deltaTime:%lu\r\n",
                       String( load ).c_str(), String( coolantTemp ).c_str(), String( rpm ).c_str(), (int)kph,
                       String( timingAdvance ).c_str(), String( intakeAirTemp ).c_str(), String( throttle ).c_str(),
                       String( fuelLevel ).c_str(), String( absLoad ).c_str(), String( commandedAirFuelRatio ).c_str(),
                       String( ambientAirTemp ).c_str(), (int)fuelType, String( ethanolPercent ).c_str(),
                       String( oilTemp ).c_str(), String( demandedTorque ).c_str(), String( torque ).c_str(),
                       (int)referenceTorque, millis() - a );

        lastLoop = millis();
    }
}

void elm_comm::initializeElm() { hasInitializedElm = elm->begin( *btser, true, 2000 ); }
