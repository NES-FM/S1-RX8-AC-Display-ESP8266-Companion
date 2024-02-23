#include "main.hpp"

ESP8266WiFiMulti wifiMulti;
AsyncWebServer server( 80 );
bool connectioWasAlive = false;

elm_comm elm;

Scheduler ts;
Task initElm( 3000 * TASK_MILLISECOND, -1, &initElmCb, &ts, true );

espComm ard( Serial );

void setup( void ) {
    ard.init();

    // Create a file `include/secrets.h` with String wifi_aps[n][2] for n access points, with each
    // AP={"SSID", "PASSWORD"}
    for ( auto ap : wifi_aps ) {
        wifiMulti.addAP( ap[0].c_str(), ap[1].c_str() );
    }

    server.on( "/", HTTP_GET, []( AsyncWebServerRequest *request ) {
        request->send( 200, "text/plain", "Hi! This is ElegantOTA AsyncDemo." );
    } );

    ElegantOTA.begin( &server ); // Start ElegantOTA
    server.begin();

    elm.init();
}

bool monitorWiFi() {
    if ( wifiMulti.run() != WL_CONNECTED ) {
        if ( connectioWasAlive == true ) {
            connectioWasAlive = false;
            Serial.print( "Looking for WiFi " );
        }
        return false;
    } else if ( connectioWasAlive == false ) {
        connectioWasAlive = true;
        Serial.printf( "Wifi connected to %s with IP %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str() );
    }
    return true;
}

void loop( void ) {
    if ( monitorWiFi() ) {
        ElegantOTA.loop();
    }

    if ( Serial.available() ) {
        Serial.write( Serial.read() );
    }

    elm.tick();

    delay( 5 );
}

void initElmCb() {
    if ( !elm.isInitialized() )
        elm.initializeElm();
    else
        initElm.disable();
}

void logger_log_formatted_string( const char *format, ... ) {
    va_list args;
    va_start( args, format );

    char str[256];
    vsprintf( str, format, args );

    ard.debugData( str );
    va_end( args );
}
