#include <Arduino.h>

const uint16_t SAMPLES = 1024; //This value MUST ALWAYS be a power of 2
double vReal[SAMPLES];
double vImag[SAMPLES];
const double SAMPLING_FREQ = 1024.0 / 46.0e-3;// 24000;
const int PIN_ADC = GPIO_NUM_36;

const float MAG_2_MIDI = 127.0 / 680000.0; // Este valor depende de la frecuencia de muestreo.

const long BAUDRATE = 115200;       // Baudrte for serial com
const long MIDI_BAUDRATE = 31250;    // Baudrate for MIDI port
#define SerialMidi Serial2

const double MAGNITUDE_THRESHOLD = 5.0;

struct nota_t  {
    int notaMidi;
    int notaMidiOld;
    double peakFreq;
    double Magnitude;  
};