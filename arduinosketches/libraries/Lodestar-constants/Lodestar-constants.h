#include "Arduino.h"

#define TRANSMISSION_ALLOWED        0b10101010
#define TRANSMISSION_FORBIDDEN      0b01010101
#define CIGS_DATA_TIMEOUT           50000

// ----------- DATA LENGTH ---------- //
#define NUMBER_OF_DATA_POINTS       256
#define CIGS_DATA_LEN               4*NUMBER_OF_DATA_POINTS+1
#define CIGS_CELL_POS               CIGS_DATA_LEN-1
#define FORMATED_DATA_LEN           2*CIGS_DATA_LEN+8

// ----------- I2C ADDRESSES -------- //
#define NANO_1_ADDRESS               0x01
#define NANO_2_ADDRESS               0x02
#define GEIGER_ADDRESS               0x03

// ----------- NANO PINS ------------ //
#define VOLTAGE_PIN_1               A7
#define VOLTAGE_PIN_2               A1
#define VOLTAGE_PIN_3               A3

#define CURRENT_PIN_1               A6 
#define CURRENT_PIN_2               A0 
#define CURRENT_PIN_3               A2 

#define GATE_PIN_1                  9
#define GATE_PIN_2                  10
#define GATE_PIN_3                  11

// ----------- NANO CONSTANSTS -------//
#define SETTLING_TIME               25
#define INTEGRATING_DELAY           30

// ---------- MEGA PINS --------------//
#define SD_SS_PIN                   4
#define ETHERNET_SS_PIN             10

// ---------- UDp PORTS --------------//
#define REMOTE_PORT                 8888
#define LOCAL_PORT                  8888

struct CigsCell
{
    uint8_t voltagePin;
    uint8_t currentPin;
    uint8_t gatePin;
    uint8_t cellNumber;
};

