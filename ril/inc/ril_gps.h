/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2014
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ril_gps.h 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module declares GPS related APIs.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/
#ifndef __RIL_GPS_H__
#define __RIL_GPS_H__

#include "ql_type.h"

typedef enum
{
    GPS_POWER_MODE_OFF = 0,  //Power off GPS.
    GPS_POWER_MODE_ON        // Power on GPS.
}Enum_GPS_Power_Mode;


typedef enum
{
    GPS_DEBUG_MODE_DISABLE = 0,   //disable
    GPS_DEBUG_MODE_ENABLE         // enable.
}Enum_GPS_Debug_Mode;


/*****************************************************************
* Function:     RIL_GPS_Open
* 
* Description:
*               Power on/off GPS.
*
* Parameters:   gps_power_mode:[in]
*                      1: Power on GPS.
*                      0: Power off GPS.
* Return:        
*               QL_RET_OK indicates this function successes.
*               QL_RET_ERR_PARAM indicates parameter error.
*****************************************************************/
s32 RIL_GPS_Open(Enum_GPS_Power_Mode gps_power_mode);


/*****************************************************************
* Function:     RIL_GPS_DEBUG_Enable
* 
* Description:
*                Control NEMA statement output in debug port
*
* Parameters:   gps_debug_mode:[in]
*                      1: enable
*                      0: disable
* Return:        
*               QL_RET_OK indicates this function successes.
*               QL_RET_ERR_PARAM indicates parameter error.
*****************************************************************/
s32 RIL_GPS_DEBUG_Enable(Enum_GPS_Debug_Mode gps_debug_mode);


/*****************************************************************
* Function:     RIL_GPS_Read
* 
* Description:
*               Query the navigation information.
*
* Parameters:   item  :  [in] Pointer to the query item
*               rdBuff:  [out] Pointer to the information buffer
* Return:        
*               QL_RET_OK indicates this function successes.
*               QL_RET_ERR_PARAM indicates parameter error.
*****************************************************************/
s32 RIL_GPS_Read(u8 *item, u8 *rdBuff);

/*****************************************************************
* Function:     RIL_GPS_CMD_Send
* 
* Description:
*               This function is used to send NMEA to GPS module.
*
* Parameters:
*                <cmdType>:
*                       [IN] always 0 currently.
*                <cmdStr>:
*                       [IN] this string is an NMEA sentence.
*                <cb_GPSCMD_hdl>:
*                       [IN] callback function for QGPSCMD URC handle.
*
* Return:        
*                RIL_AT_SUCCESS,send AT successfully.
*                RIL_AT_FAILED, send AT failed.
*                RIL_AT_TIMEOUT,send AT timeout.
*                RIL_AT_BUSY,   sending AT.
*                RIL_AT_INVALID_PARAM, invalid input parameter.
*                RIL_AT_UNINITIALIZED, RIL is not ready, need to wait for MSG_ID_RIL_READY
*                                      and then call Ql_RIL_Initialize to initialize RIL.               
*****************************************************************/
typedef void (* CB_GPSCMD)(char *strURC);
s32 RIL_GPS_CMD_Send(u8 cmdType, u8 *cmdStr, CB_GPSCMD cb_GPSCMD_hdl);


#endif	//__RIL_GPS_H__

