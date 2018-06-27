/**************************************************************************************************
  Filename:       comp.c
  Revised:        $Date: 2018/5/29 13:44:16 
  Revision:       $Revision: 001 $

  Description: This file contain some function of compass handle.
**************************************************************************************************/

/*********************************************************************
 * MACROS
 */
#define  __COMP_C__

/*********************************************************************
 * INCLUDES
 */

#include  "_Include.h"

/*********************************************************************
 * LOCAL VARIABLES
 */
int16s	xdata XGain;
int16s	xdata YGain;
int16s	xdata XOffset;
int16s	xdata YOffset;
float	xdata SinPhi;
float	xdata OneOverCosPhi;
static	int16s	xdata XAdcCalBuf[3];
static	int16s	xdata YAdcCalBuf[3];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
 
/*********************************************************************
 * @fn      Comp_HardwareInit
 *
 * @brief   Initialize each pin of compass.
 *
 * @param   void
 *
 * @return  void
 */
 void Comp_HardwareInit(void)
 {
		SS = 1;
		SCLK = 1;
		MOSI = 1;
		RESET = 0;
 	}
 	
 /*********************************************************************
 * @fn      Comp_SpiSta
 *
 * @brief  start spi.
 *
 * @param   void
 *
 * @return  void
 */
 void Comp_SpiSta (void)
 {
 		SS = 0;
 		RESET = 0;
 		SCLK = 0;
 		RESET = 1;
 		RESET = 0;
 	}
 	
/*********************************************************************
 * @fn      Comp_SpiBusyTest
 *
 * @brief   The test of busy.
 *
 * @param   void
 *
 * @return  The value of DRDY 0 bit,0 - busy
 */	
 int8u Comp_SpiBusyTest (void)
 {
 		return (DRDY&0x01);
 	}
 	
/*********************************************************************
 * @fn      Comp_SpiByteSend
 *
 * @brief   Send byte by spi.
 *
 * @param   the data that you want to send
 *
 * @return  void
 */
 void Comp_SpiByteSend (int8u val)
 {
 		int8u xdata i;
 		
 		for(i=0x80;i;i>>=1)
 		{
 				if(val&i)
 					MOSI = 1;
 				else
 					MOSI = 0;
 				SCLK = 1;
 				SCLK = 0;
 			}
 	}
 	
/*********************************************************************
 * @fn      Comp_SpiByteGet
 *
 * @brief   get data from spi.
 *
 * @param   void
 *
 * @return  the data of MISO
 */
int16s Comp_SpiByteGet (void)
{
		int16s xdata val = 0;
		int16u xdata i;
		
		SCLK = 0;
		for(i=0x8000;i;i>>=1)
		{
				SCLK = 1;
				if(MISO&0x01)
				{
						val |= i;
					}
				SCLK = 0;
			}
			
		return val;
	}
	
/*********************************************************************
 * @fn      Comp_SpiByteGet
 *
 * @brief   end of spi transfer.
 *
 * @param   void
 *
 * @return  void
 */
 void Comp_SpiEnd (void)
 {
 		SS = 1;
 	}
 	
/*********************************************************************
 * @fn      Comp_GetAdc
 *
 * @brief   get adc from sensor.
 *
 * @param   command of get adc
 *
 * @return  adc value
 */
int16s Comp_GetAdc (int8u cmd)
{
		int16s xdata val;
		
		Comp_SpiSta();
		Comp_SpiByteSend(cmd);
		while(!Comp_SpiBusyTest());
		val = Comp_SpiByteGet();
		Comp_SpiEnd();
		
		return val;
	}

/*********************************************************************
 * @fn      Comp_MidFilter
 *
 * @brief   Median filtering.
 *
 * @param   
 *
 * @return  
 */
 int16s Comp_MidFilter (int16s* buf)
 {
		int16s xdata min=buf[0];
		int16s xdata max=min;
		int16s xdata sum=max;
		
		if(min>buf[1])	min=buf[1];
		if(max<buf[1])	max=buf[1];
		sum+=buf[1];
		
		if(min>buf[2])	min=buf[2];
		if(max<buf[2])	max=buf[2];
		sum+=buf[2];
		
		return (sum-min-max);
 	 }
 	
/*********************************************************************
 * @fn      Comp_AdcCorrectS
 *
 * @brief   Median filtering.
 *
 * @param   
 *
 * @return  void
 */
void Comp_AdcCorrectS (int16s* x_ptr,int16s* y_ptr)
{	
		float xdata x,y;
		
		x = ((float)(*x_ptr - XOffset)) / XGain;
		y = ((float)(*y_ptr - YOffset)) / YGain;
		x = x - y * SinPhi;
		x = x * OneOverCosPhi;
		*x_ptr=(int16s)(x*XGain);
		*y_ptr=(int16s)(y*XGain);
 }

#define OFFSET (1440+90)

/*********************************************************************
 * @fn      Comp_AngCntProc
 *
 * @brief   Median filtering.
 *
 * @param   void
 *
 * @return  void
 */
void Comp_AngCntProc (void)
{	
	int16s xdata adc_x;
	int16s xdata adc_y;
	int8u  xdata tag;
	int16s xdata ang;
	
	adc_x=Comp_GetAdc(0x51);
	adc_y=Comp_GetAdc(0x52);
	Comp_AdcCorrectS(&adc_x,&adc_y);		//ÈíÓ²´ÅÐ£×¼
	if(0==adc_x)	adc_x=1;
	if(0==adc_y)	adc_y=1;
	if     ( (adc_x<0)&&(adc_y>0) )	{ tag=2;	adc_x=-adc_x;					}
	else if( (adc_x<0)&&(adc_y<0) )	{ tag=3;	adc_x=-adc_x;	adc_y=-adc_y;	}
	else if( (adc_x>0)&&(adc_y<0) )	{ tag=4;					adc_y=-adc_y;	}
	else                           	{ tag=1;									}
	ang=(int16s)((atan( (float) adc_x/adc_y))/3.14*1800);
	switch(tag)
	{	
		case 4:		ang=1800-ang;	break;
		case 3:		ang=1800+ang;	break;
		case 2:		ang=3600-ang;	break;
		case 1:		ang=ang;
	 }
	//ang=Comp_AdcSmooth_L(ang);
	CompVal=(OFFSET+CompOffset-ang/10)%360;
	
 }
 
/*********************************************************************
* @fn      Comp_CalInit
*
* @brief   calibration initialization.
*
* @param   void
*
* @return  void
*/
void Comp_CalInit (void)
{
	CompCalTag=1;
	
	XMax =-32767;
	YMax =-32767;
	XMin = 32767;
	YMin = 32767;
}
  
/*********************************************************************
* @fn      Comp_CalSampProc
*
* @brief   Median filtering.
*
* @param   void
*
* @return  void
*/
 void Comp_CalSampProc (void)
{	
	int16s xdata adc_x;
	int16s xdata adc_y;
	int8u  xdata i;
		
	for(i=0;i<3;i++)
	{	
		XAdcCalBuf[i]=Comp_GetAdc(0x51);
		YAdcCalBuf[i]=Comp_GetAdc(0x52);
	}
	adc_x=Comp_MidFilter(XAdcCalBuf);
	adc_y=Comp_MidFilter(YAdcCalBuf);
	if(adc_x > XMax) { 		        XMax = adc_x; }
	if(adc_x < XMin) { 			XMin = adc_x; }
	if(adc_y > YMax) { XAtYMax = adc_x;	YMax = adc_y; }
	if(adc_y < YMin) { XAtYMin = adc_x;	YMin = adc_y; }
	
 }
 
/*********************************************************************
* @fn      Comp_CalPrmtCnt
*
* @brief   Median filtering.
*
* @param   void
*
* @return  void
*/
void Comp_CalPrmtCnt (void)
{	
	float	xdata Phi0;
	float	xdata Phi1;
	float	xdata Phi;
	
	CompCalTag=0;
	
	XGain   = (XMax-XMin)>>1;
	YGain   = (YMax-YMin)>>1;
	XOffset = (XMax+XMin)>>1;
	YOffset = (YMax+YMin)>>1;
	Phi0  = asin( ((float)XAtYMax-XOffset)/XGain );
	Phi1  = asin( ((float)XAtYMin-XOffset)/XGain );
	Phi    = (Phi0-Phi1)/2.0f;
	SinPhi = sin(Phi);
	OneOverCosPhi = 1.0f/cos(Phi);
	
 }
 
/*********************************************************************
* @fn      Comp_CalDataInit
*
* @brief   Median filtering.
*
* @param   void
*
* @return  void
*/
void Comp_CalDataInit(void)
{
		XMax = -1000;
		XMin =  1000;
		YMax = -1000;
		YMin =  1000;
		XAtYMin    = 0;
		XAtYMax    = 0;
		CompOffset = 0;
	}