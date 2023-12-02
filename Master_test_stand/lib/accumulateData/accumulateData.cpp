#include "accumulateData.h"
#include <Arduino.h>
#include <../pins.h>
#include <Filters.h>
#include <Filters/Butterworth.hpp>
#include <driver/adc.h>
#include <esp_now.h>

unsigned long start_time;

typedef struct {
    uint32_t time;
    uint16_t ducer1;
    uint16_t ducer2;
    uint16_t ducer3;
    uint16_t ducer4;
    uint16_t ducer5;
    uint16_t ducer6;
    uint16_t ducer7;
    uint16_t ducer8;
    uint16_t therm1;
    uint16_t therm2;
    uint16_t therm3;
    uint16_t therm4;
    uint16_t therm5;
    uint16_t therm6;
    uint16_t loadcell;
} outgoingPacket; // 34 bytes
#define MAX_BUFFER_IDX 6
outgoingPacket outPacket[MAX_BUFFER_IDX];
uint8_t buffer_index;
uint8_t receiverAddr[] = {0x40, 0x22, 0xd8, 0x3c, 0x37, 0xfc}; // MAC Address of the MK1


// setup for butterworth filters
const double f_s = 100; // Hz
const double f_c = 4; // Hz
const double f_n = 2 * f_c / f_s;

const float MIN_CURRENT = 4e-3;
const float MAX_CURRENT = 20e-3;
const uint16_t RES_VALVE = 150;
const float correcting_factor = 1.27;       // correcting factor for the shitty ADC onboard the ESP32
uint16_t minVolt_420 = (uint16_t)(MIN_CURRENT * RES_VALVE / 3.3 * 8192 * correcting_factor);
uint16_t maxVolt_420 = constrain((uint16_t)(MAX_CURRENT * RES_VALVE / 3.3 * 8192 * correcting_factor), 0, 0x1fff);

// upper and lower bounds for sensors cuz of shitty ADC
const int MIN_PSI = 0;
const int MAX_PSI = 833; 
const int MIN_THRUST_LBS = 0;
const int MAX_THRUST_LBS = 92;


// TODO: get rid of useless variables
int ducer1_value;
int ducer2_value;
int ducer3_value;
int ducer4_value;
int ducer5_value;
int ducer6_value;
int ducer7_value;
int ducer8_value;

int loadcell_value;

auto ducer1_filter = butter<2>(f_n);
auto ducer2_filter = butter<2>(f_n);
auto ducer3_filter = butter<2>(f_n);
auto ducer4_filter = butter<2>(f_n);
auto ducer5_filter = butter<2>(f_n);
auto ducer6_filter = butter<2>(f_n);
auto ducer7_filter = butter<2>(f_n);
auto ducer8_filter = butter<2>(f_n);

auto therm1_filter = butter<2>(f_n);
auto therm2_filter = butter<2>(f_n);
auto therm3_filter = butter<2>(f_n);
auto therm4_filter = butter<2>(f_n);
auto therm5_filter = butter<2>(f_n);
auto therm6_filter = butter<2>(f_n);

auto loadcell_filter = butter<2>(f_n);

void setup_ADC() {
    adc1_config_width(ADC_WIDTH_BIT_13);
    adc1_config_channel_atten(TRANSDUCER1_PIN, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TRANSDUCER2_PIN, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TRANSDUCER3_PIN, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TRANSDUCER4_PIN, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(TRANSDUCER5_PIN, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(TRANSDUCER6_PIN, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(TRANSDUCER7_PIN, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(TRANSDUCER8_PIN, ADC_ATTEN_DB_11);
    // TODO:
    // adc1_config_channel_atten(THERMISTOR1_PIN, ADC_ATTEN_DB_11);
    // adc1_config_channel_atten(THERMISTOR2_PIN, ADC_ATTEN_DB_11);
    // adc1_config_channel_atten(THERMISTOR3_PIN, ADC_ATTEN_DB_11);
    // adc1_config_channel_atten(THERMISTOR4_PIN, ADC_ATTEN_DB_11);
    // adc2_config_channel_atten(THERMISTOR5_PIN, ADC_ATTEN_DB_11);
    // adc2_config_channel_atten(THERMISTOR6_PIN, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(LOADCELL_PIN, ADC_ATTEN_DB_11);
}

void set_start_time() {
    start_time = millis();
}

// void accumulate_data(void* parameter) { 
//     for (;;) {
//         // TODO: calibrate ADCs
//         // get time data
//         outPacket[buffer_index].time = millis() - start_time;

//         // get ducer values
//         outPacket[buffer_index].ducer1 = map(ducer1_filter(adc1_get_raw(TRANSDUCER1_PIN)), minVolt_420, maxVolt_420, 0, 1000);
//         outPacket[buffer_index].ducer2 = map(ducer2_filter(adc1_get_raw(TRANSDUCER2_PIN)), minVolt_420, maxVolt_420, 0, 1000);
//         outPacket[buffer_index].ducer3 = map(ducer3_filter(adc1_get_raw(TRANSDUCER3_PIN)), minVolt_420, maxVolt_420, 0, 1000);
//         outPacket[buffer_index].ducer4 = map(ducer4_filter(adc1_get_raw(TRANSDUCER4_PIN)), minVolt_420, maxVolt_420, 0, 1000);
//         adc2_get_raw(TRANSDUCER5_PIN, ADC_WIDTH_BIT_13, &ducer5_value);
//         outPacket[buffer_index].ducer5 = map(ducer5_filter(ducer5_value), minVolt_420, maxVolt_420, 0, 1000);
//         adc2_get_raw(TRANSDUCER6_PIN, ADC_WIDTH_BIT_13, &ducer6_value);
//         outPacket[buffer_index].ducer6 = map(ducer6_filter(ducer6_value), minVolt_420, maxVolt_420, 0, 1000);
//         adc2_get_raw(TRANSDUCER7_PIN, ADC_WIDTH_BIT_13, &ducer7_value);
//         outPacket[buffer_index].ducer7 = map(ducer7_filter(ducer7_value), minVolt_420, maxVolt_420, 0, 1000);
//         adc2_get_raw(TRANSDUCER8_PIN, ADC_WIDTH_BIT_13, &ducer8_value);
//         outPacket[buffer_index].ducer8 = map(ducer8_filter(ducer8_value), minVolt_420, maxVolt_420, 0, 1000);

//         // TODO: get thermistor values
//         // outPacket[buffer_index].therm1 = map(therm1_filter(adc1_get_raw(THERMISTOR1_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
//         // outPacket[buffer_index].therm2 = map(therm2_filter(adc1_get_raw(THERMISTOR2_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
//         // outPacket[buffer_index].therm3 = map(therm3_filter(adc1_get_raw(THERMISTOR3_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
//         // outPacket[buffer_index].therm4 = map(therm4_filter(adc1_get_raw(THERMISTOR4_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
//         // outPacket[buffer_index].therm5 = map(therm5_filter(adc2_get_raw(THERMISTOR5_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
//         // outPacket[buffer_index].therm6 = map(therm6_filter(adc2_get_raw(THERMISTOR6_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);

//         // get loadcell value
//         adc2_get_raw(LOADCELL_PIN, ADC_WIDTH_BIT_13, &loadcell_value);
//         outPacket[buffer_index].loadcell = map(loadcell_filter(loadcell_value), minVolt_420, maxVolt_420, 0, 1000);

//         Serial.printf("%lu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu\n",
//             outPacket[buffer_index].time,
//             outPacket[buffer_index].ducer1,
//             outPacket[buffer_index].ducer2,
//             outPacket[buffer_index].ducer3,
//             outPacket[buffer_index].ducer4,
//             outPacket[buffer_index].ducer5,
//             outPacket[buffer_index].ducer6,
//             outPacket[buffer_index].ducer7,
//             outPacket[buffer_index].ducer8,
//             outPacket[buffer_index].therm1,
//             outPacket[buffer_index].therm2,
//             outPacket[buffer_index].therm3,
//             outPacket[buffer_index].therm4,
//             outPacket[buffer_index].therm5,
//             outPacket[buffer_index].therm6,
//             outPacket[buffer_index].loadcell);

//         buffer_index++;
//         if (max_buffer_index == buffer_index) {
//             buffer_index = 0;
//             // TODO: print correct packet on serial
//             esp_now_send(receiverAddr, (uint8_t *) &outPacket, sizeof(outPacket));
//         }
//         vTaskDelay(1 / portTICK_PERIOD_MS);
//     }
// }

const int delay_rate = 2 / portTICK_PERIOD_MS; // TODO: change back to 5ms
void accumulate_data(void* parameter) { 
    for (;;) {
        outPacket[buffer_index].time = millis() - start_time;
        
        // get ducer values
        ducer1_value = constrain(adc1_get_raw(TRANSDUCER1_PIN), minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer1 = map(ducer1_filter(ducer1_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        ducer2_value = constrain(adc1_get_raw(TRANSDUCER2_PIN), minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer2 = map(ducer2_filter(ducer2_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        ducer3_value = constrain(adc1_get_raw(TRANSDUCER3_PIN), minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer3 = map(ducer3_filter(ducer3_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        ducer4_value = constrain(adc1_get_raw(TRANSDUCER4_PIN), minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer4 = map(ducer4_filter(ducer4_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        adc2_get_raw(TRANSDUCER5_PIN, ADC_WIDTH_BIT_13, &ducer5_value);
        ducer5_value = constrain(ducer5_value, minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer5 = map(ducer5_filter(ducer5_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        adc2_get_raw(TRANSDUCER6_PIN, ADC_WIDTH_BIT_13, &ducer6_value);
        ducer6_value = constrain(ducer6_value, minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer6 = map(ducer6_filter(ducer6_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        adc2_get_raw(TRANSDUCER7_PIN, ADC_WIDTH_BIT_13, &ducer7_value);
        ducer7_value = constrain(ducer7_value, minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer7 = map(ducer7_filter(ducer7_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);
        adc2_get_raw(TRANSDUCER8_PIN, ADC_WIDTH_BIT_13, &ducer8_value);
        ducer8_value = constrain(ducer8_value, minVolt_420, maxVolt_420);
        outPacket[buffer_index].ducer8 = map(ducer8_filter(ducer8_value), minVolt_420, maxVolt_420, MIN_PSI, MAX_PSI);

        // TODO: get thermistor values
        // outPacket[buffer_index].therm1 = map(therm1_filter(adc1_get_raw(THERMISTOR1_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
        // outPacket[buffer_index].therm2 = map(therm2_filter(adc1_get_raw(THERMISTOR2_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
        // outPacket[buffer_index].therm3 = map(therm3_filter(adc1_get_raw(THERMISTOR3_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
        // outPacket[buffer_index].therm4 = map(therm4_filter(adc1_get_raw(THERMISTOR4_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
        // outPacket[buffer_index].therm5 = map(therm5_filter(adc2_get_raw(THERMISTOR5_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);
        // outPacket[buffer_index].therm6 = map(therm6_filter(adc2_get_raw(THERMISTOR6_PIN)), thermMinVolt, thermMaxVolt, 0, 1000);

        // get loadcell value
        adc2_get_raw(LOADCELL_PIN, ADC_WIDTH_BIT_13, &loadcell_value);
        loadcell_value = constrain(loadcell_value, minVolt_420, maxVolt_420);
        outPacket[buffer_index].loadcell = map(loadcell_filter(loadcell_value), minVolt_420, maxVolt_420, MIN_THRUST_LBS, MAX_THRUST_LBS);
        // Serial.printf("%lu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu\n",
        //     outPacket[buffer_index].time,
        //     outPacket[buffer_index].ducer1,
        //     outPacket[buffer_index].ducer2,
        //     outPacket[buffer_index].ducer3,
        //     outPacket[buffer_index].ducer4,
        //     outPacket[buffer_index].ducer5,
        //     outPacket[buffer_index].ducer6,
        //     outPacket[buffer_index].ducer7,
        //     outPacket[buffer_index].ducer8,
        //     outPacket[buffer_index].therm1,
        //     outPacket[buffer_index].therm2,
        //     outPacket[buffer_index].therm3,
        //     outPacket[buffer_index].therm4,
        //     outPacket[buffer_index].therm5,
        //     outPacket[buffer_index].therm6,
        //     outPacket[buffer_index].loadcell);
        

        buffer_index++;    
        if (buffer_index == 6) {
            buffer_index = 0;
            
            esp_now_send(receiverAddr, (uint8_t *) &outPacket, sizeof(outPacket));

        }
        vTaskDelay(delay_rate);
    }
}