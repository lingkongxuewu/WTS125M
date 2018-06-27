/**************************************************************************************************
  Filename:       compass.h
  Revised:        $Date: 2018-05-28 17:44 -0700 (Mon, 28 May 2018) $
  Revision:       $Revision: 16618 $
  
  Description:	compass operation function
*************************************************************************************************/
#ifndef __COMPASS_H__
#define __COMPASS_H__

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * FUNCTIONS
 */
void Comp_CalDataInit(void);
void Comp_HardwareInit(void);
void Comp_CalSampProc (void);
void Comp_AngCntProc (void);
void Comp_CalInit (void);
void Comp_CalPrmtCnt (void);
int16s Comp_GetAdc (int8u cmd);

/*********************************************************************
 * PIN DEFINE
 */ 
	sbit SCLK = P1^0;
	sbit MISO = P1^1;		//input
	sbit MOSI = P1^2;
	sbit SS		= P1^3;
	sbit DRDY = P1^4;  //input
	sbit RESET= P1^5;


#endif 