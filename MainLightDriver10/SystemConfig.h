#ifndef SystemConfig_H
#define SystemConfig_H

#define MAX_NO_OF_HVLAS_LED 71 //STOP GO Lights added as 49 and 50

//enum for PCM
typedef enum 
{
  PCM_NONE,
  PCM_1,
  PCM_2,
  PCM_3,
  PCM_4,
  PCM_5,
  PCM_6,
}PCM_ID;

//enum for UART
typedef enum
{
  UART_0,
  UART_1,
  UART_2,
  UART_3,
  UART_4,
  UART_5,
  UART_6,
  UART_7,
  UART_8,
  UART_9,
}UART_ID;

//enum for Light ID
typedef enum
{
  eLightIDNone = 0,
  eDeckPeriphery1 = 1,
  eDeckPeriphery2 = 2,
  eDeckPeriphery3 = 3,
  eDeckPeriphery4 = 4,
  eDeckPeriphery5 = 5,
  eDeckPeriphery6 = 6,
  eDeckPeriphery7 = 7,
  eDeckPeriphery8 = 8,
  eDeckPeriphery9 = 9,
  eDeckPeriphery10 = 10,
  eDeckPeriphery11 = 11,
  eDeckPeriphery12 = 12,
  eDeckPeriphery13 = 13,
  eDeckPeriphery14 = 14,
  eDeckPeriphery15 = 15,
  eDeckPeriphery16 = 16,

  eSternApproach17 = 17,
  eSternApproach18 = 18,
  eCommonLight19 = 19,
  eSternApproach20 = 20,
  eSternApproach21 = 21,
  eSternApproach22 = 22,
  eSternApproach23 = 23,
  eSternApproach24 = 24,// Unused, for future use

  eObliqueApproachPort25 = 25,
  eObliqueApproachPort26 = 26,
  eObliqueApproachPort27 = 27,
  eObliqueApproachPort28 = 28,
  eObliqueApproachPort29 = 29,
  eObliqueApproachPort30 = 30,
  eObliqueApproachPort31 = 31,// Unused, for future use

  eHangarReference32 = 32,
  eHangarReference33 = 33,
  eHangarReference34 = 34,
  eHangarReference35 = 35,
  eHangarReference36 = 36,// Unused, for future use
  eHangarReference37 = 37,// Unused, for future use
  eHangarReference38 = 38,// Unused, for future use
  eHangarReference39 = 39,// Unused, for future use
  
  eSHRS40 = 40,
  eSHRS41 = 41,
  eSHRS42 = 42,
  eSHRS43 = 43,
  eSHRS44 = 44,
  eSHRS45 = 45,
  eSHRS46 = 46,

  eHorizonReference47 = 47,
  eHorizonReference48 = 48,

  eGoIndication49 = 49,
  eStopIndication50 = 50,

  eServiceFlood51 = 51,
  eServiceFlood52 = 52,
  eServiceFlood53 = 53,
  eServiceFlood54 = 54,

  eDeckSurfaceIllumination55 = 55,
  eDeckSurfaceIllumination56 = 56,
  eDeckSurfaceIllumination57 = 57,
  eDeckSurfaceIllumination58 = 58,

  eHangarWashDown59 = 59,
  eHangarWashDown60 = 60,

  eHIFR61 = 61,
  eHIFR62 = 62,
  eHIFR63 = 63,
  
  eLPD64 = 64, 

  eHomingBeacon65 = 65,
  eHomingBeacon66 = 66,

  eObstruction67 = 67,
  eObstruction68 = 68,

  eGoIndication69 = 69,
  eStopIndication70 = 70,

  eWaiveOff71 = 71,

  eObliqueApproachStbd72 = 72,
  eObliqueApproachStbd73 = 73,
  eObliqueApproachStbd74 = 74,
  eObliqueApproachStbd75 = 75,
  eObliqueApproachStbd76 = 76,
  eObliqueApproachStbd77 = 77,
  eObliqueApproachStbd78 = 78,// Unused, for future use
   
}enLightId;

//enum for Light Masking
typedef enum
{
  eFaulierMaskBit0  = 0x0001,
  eFaulierMaskBit1  = 0x0002,
  eFaulierMaskBit2  = 0x0004,
  eFaulierMaskBit3  = 0x0008,
  eFaulierMaskBit4  = 0x0010,
  eFaulierMaskBit5  = 0x0020,
  eFaulierMaskBit6  = 0x0040,
  eFaulierMaskBit7  = 0x0080,
  eFaulierMaskBit8  = 0x0100,
  eFaulierMaskBit9  = 0x0200,
  eFaulierMaskBit10  = 0x0400,
  eFaulierMaskBit11  = 0x0800,
  eFaulierMaskBit12  = 0x1000,
  eFaulierMaskBit13  = 0x2000,
  eFaulierMaskBit14  = 0x4000,
  eFaulierMaskBit15  = 0x8000,
  eFaulierMaskBit16  = 0x10000,
  eFaulierMaskBit17  = 0x20000,
  eFaulierMaskBit18  = 0x40000,
  eFaulierMaskBit19  = 0x80000,
  eFaulierMaskBit20  = 0x100000,
  eFaulierMaskBit21  = 0x200000,
  eFaulierMaskBit22  = 0x400000,
  eFaulierMaskBit23  = 0x800000,
}FAILMASK_ID;

//enum for Group ID
typedef enum
{
    A,//1 A- Deck Periphery Light  
    B,//2 B- Stern Approach Light
    C,//3 C- Oblique Approach Port Light (A single Common Light is also present)  
    D,//4 D- SHRS Light
    E,//5 E- Service Flood  Light
    F,//6 F- Deck Surface Illuminition Light
    G,//7 G- Hangar Wash Down Light
    H,//8 H- HIFR  Light 
    I,//9 I- LPD
    J,//10 J- Homing Beacon Light
    K,//11 K- Obstruction  Light
    L,//12 L- STOP/ GO Light
    M,//13 M- Waive Off Light
    N,//14 N- Oblique Approach Stbd Light
    O,//15 O- Group GPI Light
    LED_GROUP_IDMAX,
}LED_Group_ID;

//enum for LED Type
typedef enum
{
  LED_TYPE_NORMAL,
  LED_TYPE_NVG,
}LED_Type;

//enum for Group Name
typedef enum
{
    LED_GROUP_DECK_PERIPHERY,//1 A
    LED_GROUP_ASTERN_LIGHT,//2 B
    LED_GROUP_OBLIQUE_PORT_LIGHT,//3 C
    LED_GROUP_SHRS,//4 D
    LED_GROUP_SERVICE_FLOOD,//5 E
    LED_GROUP_DECK_SURFACE_ILLUM,//6 F
    LED_GROUP_HANGER_WASH_DOWN,//7 G
    LED_GROUP_HIFR,//8 H
    LED_GROUP_LPD,//9 I
    LED_GROUP_HOMING_BEACON,//10 J
    LED_GROUP_OBSTRUCTION,//11 K
    LED_GROUP_STOPGO_LIGHT,//12 L
    LED_GROUP_WAIVE_OFF,//13 M
    LED_GROUP_OBLIQUE_STBD_LIGHT,//14 N
    LED_GROUP_GPI,//15 O
    //LED_GROUP_ASTERN_COMMON_LIGHT,//16

    //LED_GROUP_MODE,//17
    LED_GROUP_MAX//16
//18
}LED_Group;

//enum for HVLAS LED Configuration
typedef struct
{
    enLightId  ui8LED_ID;
    LED_Group_ID ui8LED_GroupID;
    LED_Type eLED_Type;
    LED_Group eLED_Group;
    PCM_ID ePCM_ID;
    UART_ID eUART_ID;
    unsigned long ui32FailureStatusMask;
}HVLAS_LED_Config;

extern const HVLAS_LED_Config LED_ConfigArr[MAX_NO_OF_HVLAS_LED];
#endif
