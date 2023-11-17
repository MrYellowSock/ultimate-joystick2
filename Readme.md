## [demo](https://www.youtube.com/watch?v=eldz6tahseq) 

# usb descriptor modification
- /middlewares/st/stm32_usb_device_library/class/hid/src/usbd_hid.c
    ```
    ninterface 0x00 -> 0x04
    ```
- /middlewares/st/stm32_usb_device_library/class/hid/inc/usbd_hid.h
    ```
    hid_mouse_report_desc_size -> 64 (same size as below array)
    ```
- /middlewares/st/stm32_usb_device_library/class/hid/src/usbd_hid.c
    ```c
    __align_begin static uint8_t hid_mouse_reportdesc[hid_mouse_report_desc_size] __align_end =
    {
            0x05, 0x01,                    // usage_page (generic desktop)
            0x09, 0x05,                    // usage (game pad)
            0xa1, 0x01,                    // collection (application)
            0x09, 0x01,                    //   usage (pointer)
            0xa1, 0x00,                    //   collection (physical)
            0x05, 0x09,                    //     usage_page (button)
            0x19, 0x01,                    //     usage_minimum (button 1)
            0x29, 0x08,                    //     usage_maximum (button 8)
            0x15, 0x00,                    //     logical_minimum (0)
            0x25, 0x01,                    //     logical_maximum (1)
            0x95, 0x08,                    //     report_count (8)
            0x75, 0x01,                    //     report_size (1)
            0x81, 0x02,                    //     input (data,var,abs)

            0x05, 0x09,                    //     usage_page (button)
            0x19, 0x09,                    //     usage_minimum (button 9)
            0x29, 0x10,                    //     usage_maximum (button 16)
            0x15, 0x00,                    //     logical_minimum (0)
            0x25, 0x01,                    //     logical_maximum (1)
            0x95, 0x08,                    //     report_count (8)
            0x75, 0x01,                    //     report_size (1)
            0x81, 0x02,                    //     input (data,var,abs)

            0x05, 0x01,                    //     usage_page (generic desktop)
            0x09, 0x30,                    //     usage (x)
            0x09, 0x31,                    //     usage (y)
            0x09, 0x33,                    //     usage (rx)
            0x09, 0x34,             	   //     usage (ry)
            0x15, 0x81,                    //     logical_minimum (-127)
            0x25, 0x7f,                    //     logical_maximum (127)
            0x75, 0x08,                    //     report_size (8)   param size 8 bit
            0x95, 0x04,                    //     report_count (4)	4 parameter x,y,rx,ry
            0x81, 0x02,                    //     input (data,var,abs)

            0xc0,                          //   end_collection
            0xc0                           // end_collection
    };
    ```

## generate code will override these. make sure to restore before compilation
