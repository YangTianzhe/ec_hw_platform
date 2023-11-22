//
// Created by ytz20 on 2023/11/15.
//

#ifndef EC_HW_PLATFORM_REMOTECONTROL_H
#define EC_HW_PLATFORM_REMOTECONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#define RC_CH_VALUE_ZERO  (1024)
#define RC_CH_VALUE_RANGE (660)

#define RC_MOUSE_VALUE_ZERO  (0)
#define RC_MOUSE_VALUE_RANGE (32768)

typedef enum
{
    RC_SW_UP=1,
    RC_SW_MID=3,
    RC_SW_DOWN=2
}RCSwitchValueType;


/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)

#define RC_FRAME_LENGTH 18u
/* ----------------------- Data Struct ------------------------------------- */
typedef struct
{
    struct
    {
        float ch0;  //右摇杆 横
        float ch1;  //右摇杆 纵
        float ch2;  //左摇杆 横
        float ch3;  //左摇杆 纵
        RCSwitchValueType s1;   //左开关位
        RCSwitchValueType s2;   //右开关位
    }rc;
    struct
    {
        float x;
        float y;
        float z;
        uint8_t press_l;
        uint8_t press_r;
    }mouse;
    struct
    {
        uint16_t v;
    }key;
}__attribute__((packed)) RC_Ctl_t;


void RemoteControlInit(void);
void RemoteControlDataReceive(void);
void RemoteControlDataUtilize(void);

#ifdef __cplusplus
};
#endif

#endif //EC_HW_PLATFORM_REMOTECONTROL_H
