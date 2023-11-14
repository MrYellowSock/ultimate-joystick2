ถ้าโปรแกรมทำงานไม่ถูกต้อง
1. เปิดไฟล์ usbd_hid.c
2. ตรวจสอบว่า config เป็น gamepad 0x05 หรือยัง
3. หากยัง 0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
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
		0x09, 0x32,                    //     USAGE (Z)
		0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		0x75, 0x08,                    //     REPORT_SIZE (8)
		0x95, 0x03,                    //     REPORT_COUNT (3)
		0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		0xc0,                          //   END_COLLECTION
		0xc0                           // END_COLLECTION
4. แก้HID_MOUSE_REPORT_DESC_SIZE ใน define ให้เป็น 62U
https://www.youtube.com/watch?v=ELDz6TAHSEQ
