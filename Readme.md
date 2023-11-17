# USB Descriptor Modification
- /Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c

    nInterface 0x00 -> 0x04
    
- /Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc/usbd_hid.h

    HID_MOUSE_REPORT_DESC_SIZE -> 64 (same size as below array)

- /Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c
    __ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] __ALIGN_END =
    {
            0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
            0x09, 0x05,                    // USAGE (Game Pad)
            0xa1, 0x01,                    // COLLECTION (Application)
            0x09, 0x01,                    //   USAGE (Pointer)
            0xa1, 0x00,                    //   COLLECTION (Physical)
            0x05, 0x09,                    //     USAGE_PAGE (Button)
            0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
            0x29, 0x08,                    //     USAGE_MAXIMUM (Button 8)
            0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
            0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
            0x95, 0x08,                    //     REPORT_COUNT (8)
            0x75, 0x01,                    //     REPORT_SIZE (1)
            0x81, 0x02,                    //     INPUT (Data,Var,Abs)

            0x05, 0x09,                    //     USAGE_PAGE (Button)
            0x19, 0x09,                    //     USAGE_MINIMUM (Button 9)
            0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
            0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
            0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
            0x95, 0x08,                    //     REPORT_COUNT (8)
            0x75, 0x01,                    //     REPORT_SIZE (1)
            0x81, 0x02,                    //     INPUT (Data,Var,Abs)

            0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
            0x09, 0x30,                    //     USAGE (X)
            0x09, 0x31,                    //     USAGE (Y)
            0x09, 0x33,                    //     USAGE (Rx)
            0x09, 0x34,             	   //     USAGE (Ry)
            0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
            0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
            0x75, 0x08,                    //     REPORT_SIZE (8)   param size 8 bit
            0x95, 0x04,                    //     REPORT_COUNT (4)	4 parameter x,y,Rx,Ry
            0x81, 0x02,                    //     INPUT (Data,Var,Abs)

            0xc0,                          //   END_COLLECTION
            0xc0                           // END_COLLECTION
    };

## Generate code will override these. Make sure to restore before compilation
