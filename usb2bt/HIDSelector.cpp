#include "HIDSelector.hpp"

#include <usbhid.h>

#include "hid_relay.hpp"

// Return true for the interface we want to hook into
bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto)
{
  return true;
  //return iface == 0;

  //  Serial.print("iface ");
  //  Serial.print(iface);
  //  Serial.print(" proto ");
  //  Serial.println(proto);
  //  return true;
}

// Will be called for all HID data received from the USB interface
void HIDSelector::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
  if (len && buf)  
  {
    const uint8_t ID_TYPE_KEYBAORD = 0b00;
    const uint8_t ID_TYPE_MOUSE = 0b01;
    const uint8_t ID_TYPE_MOUSE_COMBO = 0b10;
    const uint8_t ID_TYPE_MEDIA = 0b11;
    const uint8_t ID_TYPE_COMMAND = 0b100;    
    
    uint8_t id = (uint8_t)buf[0];

    if (ep == 1)
    //if(id == ID_TYPE_KEYBAORD)
    {
      Relay.HandleKeyboard(len, buf);
    }
    //else if (ep == 2 && len == 3)
    else if ((ep == 2 && id == ID_TYPE_MEDIA) || (ep == 2 && len == 3))
    {
      Relay.HandleMedia(len, buf);
    }
    //else if (ep == 2 && len == 2)
    else if(ep == 2 && id == ID_TYPE_COMMAND)
    {
      Relay.HandleCommand(len, buf);
    }
    //else if (ep == 2)
    else if(ep == 2 && (id == ID_TYPE_MOUSE || id == ID_TYPE_MOUSE_COMBO))
    {
      Relay.HandleMouse(len, buf);
    }
    else
    {
      Relay.HandleRawHIDData(ep, is_rpt_id, len, buf);
    }

    //TODO: Is this needed?
    //    Notify(PSTR("\r\n"), 0x80);
    //    for (uint8_t i = 0; i < len; i++) {
    //      D_PrintHex<uint8_t > (buf[i], 0x80);
    //      Notify(PSTR(" "), 0x80);
    //    }

  }
}
