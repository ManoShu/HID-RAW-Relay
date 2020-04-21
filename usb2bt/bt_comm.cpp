#include "bt_comm.hpp"

#include <SoftwareSerial.h>

#include "bt_reporter.hpp"
#include "hid_relay.hpp"

#define rxPin 4
#define txPin 5

BTComm Bluetooth;

// set up a new serial port
SoftwareSerial btSerial =  SoftwareSerial(rxPin, txPin);

void BTComm::Init(long speed)
{
  btSerial.begin(speed);
}

void BTComm::SendKeyboard(uint8_t len, uint8_t *buf)
{
  auto needToTriggerTheDamnKey = false;
  for (size_t i = 0; i < len; i++)
  {
    auto theByte = (uint8_t)buf[i];

    if (theByte == 135)
    {
      uint8_t mod = (uint8_t)buf[0];

      auto leftShiftPressed = mod == 2;
      auto rightShiftPressed = mod == 32;

      if (leftShiftPressed || rightShiftPressed)
      {
        needToTriggerTheDamnKey = true;
        buf[i] = 0;
      }
    }
  }

  if (needToTriggerTheDamnKey)
  {
    SendByte(0xfd);
    SendByte(9); // Len
    SendByte(0x1);
    SendByte(5); //Ctrl + alt
    SendByte(0x0);
    SendByte(26); //W
    SendByte(0x0);
    SendByte(0x0);
    SendByte(0x0);
    SendByte(0x0);
    SendByte(0x0);
  }

  SendByte(0xfd);
  SendByte(len + 1);
  SendByte(0x1);
  for (size_t i = 0; i < len; i++)
  {
    auto theByte = (uint8_t)buf[i];

    //HACK: VERY SPECIAL CASE
    if (theByte == 135)
    {
      //key pad slash
      theByte = 84;
    }

    SendByte(theByte);
  }
}

void BTComm::SendMouse(int8_t btn, int8_t x, int8_t y, int8_t scroll)
{
  SendByte(0xfd);
  SendByte(0x05);
  SendByte(0x02);
  SendSByte(btn);
  SendSByte(x);
  SendSByte(y);
  SendSByte(scroll);
}

void BTComm::SendMedia(uint8_t lowByte, uint8_t highByte)
{
  SendByte(0xfd);
  SendByte(0x03);
  SendByte(0x03);
  SendByte(lowByte);
  SendByte(highByte);
}

void BTComm::SendByte(uint8_t b)
{
  btSerial.write(b);
}

void BTComm::SendSByte(int8_t b)
{
  btSerial.write(b);
}

void BTComm::CheckReports()
{
  while (btSerial.available())
  {
    BTReporter.Update(btSerial.read(), BTComm::ReportMessageReceived);
  }
}

static void BTComm::ReportMessageReceived(uint8_t len, uint8_t* buf)
{
  Relay.HandleHIDReport(len, buf);
}
