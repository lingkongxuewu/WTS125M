/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Main.h
* �桡����  v1.0
* ��  ����  100T˫�� ���Ϻ�İ汾
* �����ˣ�   
* ��  �ڣ�   
--------------------------------------------------------------------------
*��ǰ�汾:	v1.0
*�� �� ��:
*�ա�  ��:
----------------------------------�޸�����--------------------------------
1��
2��
3��
4��
------------------------------------------------------------------------*/
#include	"_Type.h"
//***********************************************************************/
#ifndef __MAIN_H__

	#define __MAIN_H__
	/*-----�궨��---------------------*/
	#define		SYSCLK 				(24000000/4)		// SYSCLK frequency in Hz
	#define		MOD_MOD0			0
	#define		MOD_X_CAL			1
	#define		MOD_Y_CAL			2
	#define		MOD_T_CAL			3
	#define		MOD_MT_CAL			4
	#define		TEMP_S				1		//0 = �������¶� 1 = MCU�¶�
	
	#define     CUSTOMER_XBY        0
	


	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
#endif
//***********************************************************************/
#ifdef   __MAIN_C__
#define  MAIN_EXT
#else
#define  MAIN_EXT  extern
#endif
/*-------------��������--------------*/
MAIN_EXT	xdata	int8u	MenuId;
MAIN_EXT	xdata	int16u	TempA;

MAIN_EXT	xdata	int16u	AlarmHighLow;//'H'�ߵ�ƽ������'L'�͵�ƽ����
//variable about compass 
MAIN_EXT	xdata int8u		CompCalTag;
MAIN_EXT	xdata int16s	XMax,YMax,XMin,YMin;
MAIN_EXT	xdata int16s	XAtYMin,XAtYMax;                
MAIN_EXT	xdata int16u	CompVal;				//���̶���
MAIN_EXT	xdata int8u		TxBuf[5];
MAIN_EXT	xdata int16u	CompOffset;				//ƫ����
MAIN_EXT	xdata int8u		SysPowOnTag;
#if CUSTOMER_XBY
MAIN_EXT xdata int16u	sensor_num;
#endif 
//***********************************************************************/
