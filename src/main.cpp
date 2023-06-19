#include <Arduino.h>
#include "config.h"
#include "midiConfig.h"
#include "fftAux.h"
#include "serialFunc.h"

// WiFi and OTA
#include "wifiFunctions.h"

// RTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

fftAux FFTAux(SAMPLES, SAMPLING_FREQ);
SerialFuncClass sf; // Create an instance of the serialFunc class
nota_t nota = {0, 0, 0, 0};

hw_timer_t *My_timer = NULL;
bool adquisitionCompleted = false; 

void serviciosWiFienCore1(void * params){
    Serial.printf("Generando Servicios WiFi desde Core %d\n",xPortGetCoreID());
    OverTheAirUpdate();
    while(true){}
}


void IRAM_ATTR  onTimer()
{
    digitalWrite(GPIO_NUM_17, HIGH);
    static int i = 0;
    FFTAux.vReal[i] = static_cast<double>(analogRead(PIN_ADC));
    if(i < SAMPLES){
        i++;
    }
    else{
        i = 0;
        adquisitionCompleted = true;
        timerAlarmDisable(My_timer);
    }

    digitalWrite(GPIO_NUM_17, LOW);
}

void timerInit(){
    My_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(My_timer, &onTimer, true);
    timerAlarmWrite(My_timer, 100, true);
    timerAlarmEnable(My_timer);     
}

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
    pinMode(GPIO_NUM_17, OUTPUT);
    digitalWrite(GPIO_NUM_17, HIGH);
    xTaskCreatePinnedToCore(serviciosWiFienCore1, "WiFi en Core 1", 10000, NULL, 1, NULL, 1);
    timerInit();
    Serial.println("Setup finished!");
}

void loop(){
    
    //Adquire();
    if(adquisitionCompleted){
        FFTAux.DCRemoval();
        FFTAux.ComputeFFT();
        nota.peakFreq = FFTAux.FindPeak();
        nota.Magnitude = MAG_2_MIDI * FFTAux.FindMag();
        nota.notaMidi = FFTAux.findNote(nota.peakFreq);
        
        

        if ((nota.notaMidi != nota.notaMidiOld) && (nota.Magnitude > MAGNITUDE_THRESHOLD)){
            //if(Magnitude > MAGNITUDE_THRESHOLD) sf.sendMIDI(SerialMidi, NOTE_ON, 0, 127, MIDI_CHANNEL);
            //if(Magnitude > MAGNITUDE_THRESHOLD) sf.serialInfo(peakFreq, Magnitude, notaMidi);
            nota.notaMidiOld = nota.notaMidi;
            //sf.serialSendFFT(FFTAux.vReal,SAMPLES);
            sf.sendMIDI(SerialMidi, NOTE_ON, 0, 127, MIDI_CHANNEL);
            sf.serialInfo(nota.peakFreq, nota.Magnitude, nota.notaMidi);
        }

        adquisitionCompleted = false;
        timerAlarmEnable(My_timer);
        
    }
     
}