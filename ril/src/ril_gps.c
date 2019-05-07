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
 *   ril_gps.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The module implements gps related APIs.
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
#include "ril_gps.h"
#include "ril.h"
#include "ril_util.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"

static s32 ATResponse_GPSRead_Hdlr(char* line, u32 len, void* userData);
static CB_GPSCMD     callback_GPSCMD = NULL;

static s32 ATResponse_GPSRead_Hdlr(char* line, u32 len, void* userData)
{
	char* p1 = NULL;
	char* p2 = NULL;
	char* head = Ql_RIL_FindString(line, len, "+QGNSSRD:"); //continue wait
	char strTmp[10];

	if(head)
	{
	    Ql_strncat((char*)userData,line,Ql_strlen(line));
		return  RIL_ATRSP_CONTINUE;
	}
    else
    {
        head = Ql_RIL_FindString(line, len, "$GNRMC");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }

        head = Ql_RIL_FindString(line, len, "$GNVTG");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }

        head = Ql_RIL_FindString(line, len, "$GNGGA");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }

        head = Ql_RIL_FindString(line, len, "$GNGSA");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
        head = Ql_RIL_FindString(line, len, "$GPGSA");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
        head = Ql_RIL_FindString(line, len, "$GLGSA");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
        head = Ql_RIL_FindString(line, len, "$BDGSA");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }

        head = Ql_RIL_FindString(line, len, "$GNGSV");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
        head = Ql_RIL_FindString(line, len, "$GPGSV");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
        head = Ql_RIL_FindString(line, len, "$GLGSV");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
        head = Ql_RIL_FindString(line, len, "$BDGSV");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }

        head = Ql_RIL_FindString(line, len, "$GNGLL");
        if(head)
        {
            Ql_strncat((char*)userData,line,Ql_strlen(line));
        }
    }

    head = Ql_RIL_FindString(line, len, "+CME ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{  
		return  RIL_ATRSP_FAILED;
	}

	head = Ql_RIL_FindLine(line, len, "OK"); // find <CR><LF>OK<CR><LF>, <CR>OK<CR>£¬<LF>OK<LF>
	if(head)
	{
		return  RIL_ATRSP_SUCCESS;
	}

	head = Ql_RIL_FindLine(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>£¬<LF>ERROR<LF>
	if(head)
	{
		return  RIL_ATRSP_FAILED;
	}

	return RIL_ATRSP_CONTINUE; //continue wait
}


s32 RIL_GPS_Open(Enum_GPS_Power_Mode gps_power_mode)
{
	char strAT[50] = {"\0"};
    u16  atLength = 0;

	if (gps_power_mode != GPS_POWER_MODE_OFF && gps_power_mode != GPS_POWER_MODE_ON)
	{
		return QL_RET_ERR_INVALID_PARAMETER;
	}

	Ql_memset(strAT, 0x0, sizeof(strAT));

	atLength = Ql_sprintf(strAT, "AT+QGNSSC=%d\r",gps_power_mode);

	return Ql_RIL_SendATCmd(strAT, atLength, NULL, NULL, 0);
}

s32 RIL_GPS_Read(u8 *item, u8 *rdBuff)
{
	char strAT[50] = {"\0"};
    u16  atLength = 0;
    s32  ret = -1;
    u8   readBuff[1000];

	Ql_memset(strAT, 0x0, sizeof(strAT));
    Ql_memset(readBuff, 0x0, sizeof(readBuff));

    if(0 == Ql_strncmp(item, "ALL", Ql_strlen(item)))
    {
        atLength = Ql_sprintf(strAT, "AT+QGNSSRD?");
    }
    else
    {
        atLength = Ql_sprintf(strAT, "AT+QGNSSRD=\"NMEA/%s\"\r", item);
    }

	ret = Ql_RIL_SendATCmd(strAT, atLength, ATResponse_GPSRead_Hdlr, readBuff, 0);

    if(RIL_ATRSP_SUCCESS == ret)
    {
        Ql_strcpy((char*)rdBuff,readBuff);
    }

    return ret;
}


s32 RIL_GPS_DEBUG_Enable(Enum_GPS_Debug_Mode gps_debug_mode)
{
	char strAT[50] = {"\0"};
    u16  atLength = 0;
	s32  ret = -1;

	if (gps_debug_mode != GPS_DEBUG_MODE_DISABLE && gps_debug_mode != GPS_DEBUG_MODE_ENABLE)
	{
		return QL_RET_ERR_INVALID_PARAMETER;
	}

	Ql_memset(strAT, 0x0, sizeof(strAT));

	atLength = Ql_sprintf(strAT, "AT+QGNSSDB=%d\r",gps_debug_mode);

	ret =  Ql_RIL_SendATCmd(strAT, atLength, NULL, NULL, 0);
	return ret;
}


void OnURCHandler_GPSCMD(const char* strURC, void* reserved)
{
	char urcHead[] = "\r\n+QGNSSCMD:\0";
        
 	if ( NULL != callback_GPSCMD )
 	{
		if( Ql_StrPrefixMatch(strURC, urcHead) )
		{
			callback_GPSCMD(strURC);
		}
	}
}


s32 RIL_GPS_CMD_Send(u8 cmdType, u8 *cmdStr, CB_GPSCMD cb_GPSCMD_hdl)
{
	s32 ret = RIL_AT_FAILED;
	char strAT[200]; 

    if (cmdType !=0 || NULL == cmdStr)
    {
        return RIL_AT_INVALID_PARAM;
    }

    callback_GPSCMD = cb_GPSCMD_hdl;
    
	Ql_memset( strAT, 0, sizeof(strAT) );
	Ql_sprintf( strAT, "AT+QGNSSCMD=%d,\"%s\"\r", cmdType, cmdStr);
	ret = Ql_RIL_SendATCmd( strAT, Ql_strlen(strAT), NULL, NULL, 0 ) ;
    
    return ret;
}



