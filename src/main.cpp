#include <Arduino.h>
#include "config.h"
#include "midiConfig.h"
#include "fftAux.h"
#include "serialFunc.h"


fftAux FFTAux(SAMPLES, SAMPLING_FREQ);
SerialFuncClass sf; // Create an instance of the serialFunc class

nota_t nota = {0, 0, 0, 0};

void Adquire(){
    for(int i = 0; i < SAMPLES; i++)
    {
      FFTAux.vReal[i] = static_cast<double>(analogRead(PIN_ADC));
      FFTAux.vImag[i] = 0;
    }
}

void setup(){
    delay(2000);
    Serial.begin(BAUDRATE);
    SerialMidi.begin(MIDI_BAUDRATE);
    Serial.println("Ready");
    
    Serial.println("Setup finished!");
}

void loop(){
    
    Adquire();
    FFTAux.DCRemoval();
    FFTAux.ComputeFFT();
    nota.peakFreq = FFTAux.FindPeak();
    nota.Magnitude = MAG_2_MIDI * FFTAux.FindMag();
    nota.notaMidi = FFTAux.findGuitarNote(nota.peakFreq);
    //sf.serialInfo(peakFreq, Magnitude, notaMidi, 2000);
    
    if ((nota.notaMidi != nota.notaMidiOld) && (nota.Magnitude > MAGNITUDE_THRESHOLD)){
        //if(Magnitude > MAGNITUDE_THRESHOLD) sf.sendMIDI(SerialMidi, NOTE_ON, 0, 127, MIDI_CHANNEL);
        //if(Magnitude > MAGNITUDE_THRESHOLD) sf.serialInfo(peakFreq, Magnitude, notaMidi);
        nota.notaMidiOld = nota.notaMidi;
        //sf.serialSendFFT(FFTAux.vReal,SAMPLES);
        sf.sendMIDI(SerialMidi, NOTE_ON, 0, 127, MIDI_CHANNEL);
        sf.serialInfo(nota.peakFreq, nota.Magnitude, nota.notaMidi);
    }
    
    
  
    
}