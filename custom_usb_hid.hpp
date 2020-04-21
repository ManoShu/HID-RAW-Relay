#ifndef CUSTOM_USB_HID_HPP
#define CUSTOM_USB_HID_HPP

#include <Arduino.h>

class CustomUSBHID 
{
  public:
    int Init();
    void Update();
    void SendReport(uint8_t len, uint8_t* buf);
    

  private:
  
  
};

extern CustomUSBHID USB_HID;

#endif
