#include "mbed.h"
#include "fsl_sai.h"
#include "USBAudio.h"
#include "USBMIDI.h"
#include "MCP23017.h"
#include "MCP3021.h"

//I2C j1(A1, A0);
//I2C j2(A4, A5);


USBAudio usba(true, 48000, 1, 192000, 2);
USBMIDI usbm;

DigitalOut led(LED_BLUE);


const _sai_config conf = {kSAI_BusI2S, kSAI_ModeSync, false, kSAI_MclkSourceSysclk, kSAI_BclkSourceOtherSai0, kSAI_Slave};




I2S_Type* base = (I2S_Type*)0x4002F000u;

uint8_t bufl[4];
uint8_t bufr[4];

const int lchannels[6] = {20,21,22,23,24,11};
const int rchannels[6] = {30,31,32,33,34,12};

DigitalIn j2_enable(D9);
DigitalIn j1_enable(D8);
DigitalOut audio_enable(D5);

Thread audio;
Thread midil;
Thread midir;



void audio_thread()
{
    SAI_RxInit(base,&conf);
    ThisThread::sleep_for(500ms);
    audio_enable.write(true);

    while (true) {
        uint32_t ldata = SAI_ReadData(base, 0);
        bufl[0] = ((ldata >> 24) & 0xFF);
        bufl[1] = ((ldata >> 16) & 0xFF);
        bufl[2] = ((ldata >> 8) & 0xFF);
        bufl[3] = (ldata & 0xFF);
        usba.write(bufl, 4);

        uint32_t rdata = SAI_ReadData(base, 0);
        bufr[0] = ((rdata >> 24) & 0xFF);
        bufr[1] = ((rdata >> 16) & 0xFF);
        bufr[2] = ((rdata >> 8) & 0xFF);
        bufr[3] = (rdata & 0xFF);
        usba.write(bufr, 4);

    }
}

/*
void midi_thread(I2C& i2c, DigitalIn *enable, const int *channels) {

    while (!enable->read()) {
        //nop
    }
    MCP23017 gpio(i2c, 0x40);
    MCP3021 adc(i2c, 5.0);
    int prev_mask = 0x00;
    double prev_pedal = 0.0;
    
    while (true) {
        char cur_mask = gpio.readRegister8(0x12);
        if ((cur_mask & 0x01) && !(prev_mask & 0x01)) {
            usbm.write(MIDIMessage::ControlChange(channels[0], 127));
        }
        if ((cur_mask & 0x02) && !(prev_mask & 0x02)) {
            usbm.write(MIDIMessage::ControlChange(channels[1], 127));
        }
        if ((cur_mask & 0x04) && !(prev_mask & 0x04)) {
            usbm.write(MIDIMessage::ControlChange(channels[2], 127));
        }
        if ((cur_mask & 0x08) && !(prev_mask & 0x08)) {
            usbm.write(MIDIMessage::ControlChange(channels[3], 127));
        }
        if ((cur_mask & 0x10) && !(prev_mask & 0x10)) {
            usbm.write(MIDIMessage::ControlChange(channels[4], 127));
        }
        double a = adc.read();
        if (a != prev_pedal) {
            usbm.write(MIDIMessage::ControlChange(channels[5], a));
        }
        prev_mask = cur_mask;
        prev_pedal = a;
    }
}

void midil_thread() {
    midi_thread(j1, &j1_enable, lchannels);
}
void midir_thread() {
    midi_thread(j2, &j2_enable, rchannels);
}
*/

int main() {
    audio.start(audio_thread);
//    midil.start(midil_thread);
//    midir.start(midir_thread);
    while(true) {
        //noop
        led.write(0);
        ThisThread::sleep_for(500ms);
        led.write(1);
        ThisThread::sleep_for(500ms);
    }
}