#include <Arduino.h>
#include "config.h"
#include "midiConfig.h"
#include "fftAux.h"
#include "serialFunc.h"

#include "esp_adc_cal.h"




#define DEFAULT_VREF    1100        // Valor de referencia por defecto en mV (para ADC de 12 bits)



fftAux FFTAux(SAMPLES, SAMPLING_FREQ);
SerialFuncClass sf; // Create an instance of the serialFunc class

nota_t nota = {0, 0, 0, 0};



void Adquire(){
    digitalWrite(GPIO_NUM_17, HIGH);
    for(int i = 0; i < SAMPLES; i++)
    {
      FFTAux.vReal[i] = static_cast<double>(adc1_get_raw(ADC1_CHANNEL_0));
      FFTAux.vImag[i] = 0;
    }
    digitalWrite(GPIO_NUM_17, LOW);
}

void setup(){
    delay(2000);
    Serial.begin(BAUDRATE);
    //SerialMidi.begin(MIDI_BAUDRATE);
    pinMode(GPIO_NUM_17, OUTPUT);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
    Serial.println("Ready");

    Serial.println("Setup finished!");
}

void loop(){

    Adquire();
    FFTAux.DCRemoval();
    FFTAux.ComputeFFT();
    nota.peakFreq = FFTAux.FindPeak();
    nota.Magnitude = MAG_2_MIDI * FFTAux.FindMag();
    nota.notaMidi = FFTAux.findNote(nota.peakFreq);
    sf.serialInfo(nota.peakFreq, nota.Magnitude, nota.notaMidi, 2000);

    if ((nota.notaMidi != nota.notaMidiOld) && (nota.Magnitude > MAGNITUDE_THRESHOLD)){
        //if(Magnitude > MAGNITUDE_THRESHOLD) sf.sendMIDI(SerialMidi, NOTE_ON, 0, 127, MIDI_CHANNEL);
        //if(Magnitude > MAGNITUDE_THRESHOLD) sf.serialInfo(peakFreq, Magnitude, notaMidi);
        nota.notaMidiOld = nota.notaMidi;
        //sf.serialSendFFT(FFTAux.vReal,SAMPLES);
        //sf.sendMIDI(SerialMidi, NOTE_ON, 0, 127, MIDI_CHANNEL);
        sf.serialInfo(nota.peakFreq, nota.Magnitude, nota.notaMidi);
    }

}