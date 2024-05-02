
#include "SystemConfig.h"

const HVLAS_LED_Config LED_ConfigArr[MAX_NO_OF_HVLAS_LED] = {
                    {eDeckPeriphery1,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_2, eFaulierMaskBit5},
                    {eDeckPeriphery2,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_2, eFaulierMaskBit6},
                    {eDeckPeriphery3,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_3, eFaulierMaskBit7},
                    {eDeckPeriphery4,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_3, eFaulierMaskBit8},
                    {eDeckPeriphery5,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_4, eFaulierMaskBit9},
                    {eDeckPeriphery6,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_4, eFaulierMaskBit10},
                    {eDeckPeriphery7,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_5, eFaulierMaskBit11},
                    {eDeckPeriphery8,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_5, eFaulierMaskBit12},
                    {eDeckPeriphery9,  A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_6, eFaulierMaskBit13},
                    {eDeckPeriphery10, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_6, eFaulierMaskBit14},
                    {eDeckPeriphery11, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_7, eFaulierMaskBit15},
                    {eDeckPeriphery12, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_7, eFaulierMaskBit16},
                    {eDeckPeriphery13, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_0, eFaulierMaskBit17},
                    {eDeckPeriphery14, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_1, UART_0, eFaulierMaskBit18},//PCM 1
                    {eDeckPeriphery15, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_2, UART_2, eFaulierMaskBit19},//14//PCM 2
                    {eDeckPeriphery16, A, LED_TYPE_NVG, LED_GROUP_DECK_PERIPHERY, PCM_2, UART_2, eFaulierMaskBit20},//15
                    
                    {eSternApproach17, B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_2, UART_3, eFaulierMaskBit5},//16 //Line up 1
                    {eSternApproach18, B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_2, UART_3, eFaulierMaskBit6},//line up 2
                    {eCommonLight19,   B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_3, UART_2, eFaulierMaskBit7},//PCM 3
                    {eSternApproach20, B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_2, UART_4, eFaulierMaskBit8},//line up 3
                    {eSternApproach21, B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_2, UART_4, eFaulierMaskBit9},//lin3 up 4
              /* Light 22, 23 to set before FATs and Dispatch*/
              //    {eSternApproach22, B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_3, UART_5, eFaulierMaskBit},//Extended line up 1
              //    {eSternApproach23, B, LED_TYPE_NVG, LED_GROUP_ASTERN_LIGHT, PCM_3, UART_5, eFaulierMaskBit},//Extended line up 2
                    
                    {eObliqueApproachPort25, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_2, UART_5, eFaulierMaskBit5},//line up 5
                    {eObliqueApproachPort26, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_2, UART_5, eFaulierMaskBit6},//line up 6
                    {eObliqueApproachPort27, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_2, UART_6, eFaulierMaskBit8},//line up 7
                    {eObliqueApproachPort28, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_2, UART_6, eFaulierMaskBit9},//line up 8
                    {eObliqueApproachPort29, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_3, UART_6, eFaulierMaskBit10},
                    {eObliqueApproachPort30, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_3, UART_7, eFaulierMaskBit11},
                    {eObliqueApproachPort31, C, LED_TYPE_NVG, LED_GROUP_OBLIQUE_PORT_LIGHT, PCM_3, UART_5, eFaulierMaskBit12},
                    
                    {eHangarReference32, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_6, eFaulierMaskBit5},
                    {eHangarReference33, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_6, eFaulierMaskBit6},
                    {eHangarReference34, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_7, eFaulierMaskBit7},
                    {eHangarReference35, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_7, eFaulierMaskBit8},//PCM 5
                    
                    {eSHRS40, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_2, eFaulierMaskBit13},//PCM 5
                    {eSHRS41, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_2, eFaulierMaskBit14},
                    {eSHRS42, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_2, eFaulierMaskBit15},
                    {eSHRS43, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_2, eFaulierMaskBit16},
                    {eSHRS44, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_0, eFaulierMaskBit17},
                    {eSHRS45, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_0, eFaulierMaskBit18},
                    {eSHRS46, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_0, eFaulierMaskBit19},
                    
                    {eHorizonReference47, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_5, eFaulierMaskBit20},
                    {eHorizonReference48, D, LED_TYPE_NVG, LED_GROUP_SHRS, PCM_5, UART_5, eFaulierMaskBit21},

                    {eGoIndication49, L,LED_TYPE_NVG, LED_GROUP_STOPGO_LIGHT, PCM_4, UART_0, eFaulierMaskBit8},//PCM 3
                    {eStopIndication50, L, LED_TYPE_NVG, LED_GROUP_STOPGO_LIGHT, PCM_4, UART_0, eFaulierMaskBit9},//PCM 3
                    
                    {eServiceFlood51, E, LED_TYPE_NVG, LED_GROUP_SERVICE_FLOOD, PCM_5, UART_3, eFaulierMaskBit5},
                    {eServiceFlood52, E, LED_TYPE_NVG, LED_GROUP_SERVICE_FLOOD, PCM_5, UART_3, eFaulierMaskBit6},
                    {eServiceFlood53, E, LED_TYPE_NVG, LED_GROUP_SERVICE_FLOOD, PCM_5, UART_4, eFaulierMaskBit7},
                    {eServiceFlood54, E, LED_TYPE_NVG, LED_GROUP_SERVICE_FLOOD, PCM_5, UART_4, eFaulierMaskBit8},
                    
                    {eDeckSurfaceIllumination55, F, LED_TYPE_NVG, LED_GROUP_DECK_SURFACE_ILLUM, PCM_4, UART_4, eFaulierMaskBit5},
                    {eDeckSurfaceIllumination56, F, LED_TYPE_NVG, LED_GROUP_DECK_SURFACE_ILLUM, PCM_4, UART_4, eFaulierMaskBit6},
                    {eDeckSurfaceIllumination57, F, LED_TYPE_NVG, LED_GROUP_DECK_SURFACE_ILLUM, PCM_4, UART_5, eFaulierMaskBit7},
                    {eDeckSurfaceIllumination58, F, LED_TYPE_NVG, LED_GROUP_DECK_SURFACE_ILLUM, PCM_4, UART_5, eFaulierMaskBit8},
                    
                    {eHangarWashDown59, G, LED_TYPE_NVG, LED_GROUP_HANGER_WASH_DOWN, PCM_3, UART_4, eFaulierMaskBit5},
                    {eHangarWashDown60, G, LED_TYPE_NVG, LED_GROUP_HANGER_WASH_DOWN, PCM_3, UART_4, eFaulierMaskBit6},
                    
                    {eHIFR61, H, LED_TYPE_NVG, LED_GROUP_HIFR, PCM_4, UART_2, eFaulierMaskBit5},//PCM 4
                    {eHIFR62, H, LED_TYPE_NVG, LED_GROUP_HIFR, PCM_4, UART_2, eFaulierMaskBit6},
                    {eHIFR63, H, LED_TYPE_NVG, LED_GROUP_HIFR, PCM_4, UART_3, eFaulierMaskBit7},
                    
                    {eLPD64, I, LED_TYPE_NVG, LED_GROUP_LPD, PCM_4, UART_6, eFaulierMaskBit2},
                    
                    {eHomingBeacon65, J, LED_TYPE_NVG, LED_GROUP_HOMING_BEACON, PCM_6, UART_3, eFaulierMaskBit5},
                    {eHomingBeacon66, J, LED_TYPE_NVG, LED_GROUP_HOMING_BEACON, PCM_6, UART_3, eFaulierMaskBit6},//PCM 6
                    
                    {eObstruction67, K, LED_TYPE_NVG, LED_GROUP_OBSTRUCTION, PCM_6, UART_2, eFaulierMaskBit5},
                    {eObstruction68, K, LED_TYPE_NVG, LED_GROUP_OBSTRUCTION, PCM_6, UART_2, eFaulierMaskBit6},
                    
                    {eGoIndication69, L, LED_TYPE_NVG, LED_GROUP_STOPGO_LIGHT, PCM_4, UART_7, eFaulierMaskBit8},
                    {eStopIndication70, L, LED_TYPE_NVG, LED_GROUP_STOPGO_LIGHT, PCM_4, UART_7, eFaulierMaskBit9},//PCM 4
                    
                    {eWaiveOff71, M, LED_TYPE_NVG, LED_GROUP_WAIVE_OFF, PCM_3, UART_3, eFaulierMaskBit2},
                    
                    {eObliqueApproachStbd72, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_2, UART_7, eFaulierMaskBit5},//line up 9
                    {eObliqueApproachStbd73, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_2, UART_7, eFaulierMaskBit6},//line up 10
                    {eObliqueApproachStbd74, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_2, UART_0, eFaulierMaskBit8},//line up 11
                    {eObliqueApproachStbd75, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_2, UART_0, eFaulierMaskBit9},//PCM 2 line up 12
                    {eObliqueApproachStbd76, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_3, UART_7, eFaulierMaskBit10},
                    {eObliqueApproachStbd77, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_3, UART_0, eFaulierMaskBit11},
                    {eObliqueApproachStbd78, N, LED_TYPE_NVG, LED_GROUP_OBLIQUE_STBD_LIGHT, PCM_3, UART_5, eFaulierMaskBit12},//PCM 3
                    
};
