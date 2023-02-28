 
#include "MCP3021.h"
#include "I2C.h"
 
//Create instance
MCP3021::MCP3021(I2C & i2c, float supplyVoltage) : _i2c(i2c), _supplyVoltage(supplyVoltage)
{
}
 
//destroy instance
MCP3021::~MCP3021()
{
}
 
int MCP3021::read()
{
 
//You cannot write to an MCP3021, it has no writable registers.
//MCP3021 also requires an ACKnowledge between each byte sent, before it will send the next byte. So we need to be a bit manual with how we talk to it.
//It also needs an (NOT) ACKnowledge after the second byte or it will keep sending bytes (continuous sampling)
//
//From the datasheet.
//
//i2c.START
//Send 8 bit device/ part address to open conversation.   (See .h file for part explanation)
//read a byte (with ACK)
//read a byte (with NAK)
//i2c.STOP
 
 
  //  char data[2];
 
    _i2c.start();
    int acknowledged = _i2c.write(0b10011001); //send a byte to start the conversation. It should be acknowledged.
    _data[0] = _i2c.read(1);  //read a byte. acknowledge when we have it.
    _data[1] = _i2c.read(0);  //read the second byte. (n)acknowledge when we have it to stop the flow.
    _i2c.stop();
 
    //convert to 12 bit.
    short res;
    int _10_bit_var; // 2 bytes
    char _4_bit_MSnibble = _data[0]; // 1 byte, example 0000 1000
    char _6_bit_LSByte = _data[1];   // 1 byte, example 1111 0000
 
    _10_bit_var = ((0x0F & _4_bit_MSnibble) << 6) | (_6_bit_LSByte >> 2);   //example 100011110000
    res=_10_bit_var;
 
    return res >> 3;
 
}