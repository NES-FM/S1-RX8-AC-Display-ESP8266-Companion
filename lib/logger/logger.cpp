#include "logger.h"

char _logger_ser_buf[LOGGER_BUFFLEN];
unsigned int _logger_ser_buf_idx = 0;

void logger_init() {
    Serial.begin( 115200 );
    Serial.println( "\r\n" );
    logln( "Starting up Arduino..." );
}

String logger_tick() {
    if ( Serial.available() > 0 ) {
        while ( Serial.available() > 0 ) {
            char inByte = Serial.read();
            // Message coming in (check not terminating character) and guard for over message size
            if ( inByte != '\n' && ( _logger_ser_buf_idx < LOGGER_BUFFLEN - 1 ) ) {
                // Add the incoming byte to our message
                _logger_ser_buf[_logger_ser_buf_idx] = inByte;
                _logger_ser_buf_idx++;

                Serial.print( inByte ); // INTENDED
            }
            // Full message received...
            else {
                _logger_ser_buf[_logger_ser_buf_idx] = '\0';

                // Reset for the next message
                _logger_ser_buf_idx = 0;
                return ( _logger_ser_buf );
            }
        }
    }
    return "";
}

const char *pathToFileName( const char *path ) {
    size_t i = 0;
    size_t pos = 0;
    char *p = (char *)path;
    while ( *p ) {
        i++;
        if ( *p == '/' || *p == '\\' ) {
            pos = i;
        }
        p++;
    }
    return path + pos;
}
