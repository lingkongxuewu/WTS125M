/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  ModBusRtu.h
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
/***********************************************************************/
#ifndef __MODBUSRTU_H__
	#define __MODBUSRTU_H__
	#define MODBUS_CHECK_ENB	1
	#define	MAXSIZE		32
/*******�����ֶ���*************/
	#define READ		0x03	//��  
	#define	WRITE		0x10	//д
	
	#define	ANGX_ADDR				0x00		//r
	#define ANGY_ADDR				0x01		//r
	#define	ANGP_ADDR				0x02		//r
	#define	ANGX_OFFSET_ADDR		0x03		//r/w
	#define	ANGY_OFFSET_ADDR		0x04		//r/w	
	#define	ANGX_ALARM_ADDR			0x05		//r/w
	#define	ANGY_ALARM_ADDR			0x06		//r/w
	#define ANGP_ALARM_ADDR			0x07		//r/w
	#define SELF_ADDR				0x08		//r/w
	#define BAUD_ADDR				0x09		//r/w
	#define	ZERO_ADDR				0x0A		//w
//	#define RST_ADDR				0x0B		//w
//	#define FLASH
	
	#define	MR_READ_REGH(temp)	temp>>8
	#define MR_READ_REGL(temp)	temp
	#define	MR_WRITE_REGH(temp)	temp>>8
	#define MR_WRITE_REGL(temp)	temp
//	#define MR_READ_ADDRH()
	

/*********��������*********************/	
void ModRtu_RxProc (int8u* buf, int8u n);	//����ӿں���
void CustomPro(int8u* buf, int8u n);
void Int32s2val (char* s,int32s val);
#endif

/*-----------------------------------*/
#ifdef   __MODBUSRTU_C__
#define  MODBUSRTU_EXT
#else
#define	 MODBUSRTU_EXT  extern
#endif
/*-------------��������--------------*/
MODBUSRTU_EXT	xdata	int8u 	Addr;			//����ͨѶ��ַ
MODBUSRTU_EXT	int16u 	xdata	AngRange;		//�ǶȲ�����Χ 5 10 15 30 45 60 90 360
MODBUSRTU_EXT	xdata	int8u 	customAddr;			//����Э��ͨѶ��ַ
//MODBUSRTU_EXT	int32s xdata  	AngAlarmOffset;