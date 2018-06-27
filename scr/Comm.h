/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Comm.h
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
#ifndef __COMM_H__

	#define __COMM_H__
	/*-----�궨��---------------------*/	
	#define  COMM_RX_BUF_SIZE     200
	#define  COMM_TX_BUF_SIZE     200
	#define  UART_T();			  P2|=0x01;  	//P1.0
	#define  UART_R();			  P2&=0xfe;  	//

	#define BAUDRATE        	9600           	// Baud rate of UART in bps
	#define COMM_NO_ERR			0
	#define COMM_RX_EPT			1
	#define COMM_TX_FULL		2
	#define COMM_TX_EPT			3
	#define	COMM_STOP			1
	#define COMM_TRAN			0
	#define	ISR_EN()		    EA=1
	#define	ISR_DIS()		    EA=0
		
	#define	COMM_485							//485ͨ�ź궨��
	/*-----�ṹ���������Ͷ���---------*/
typedef struct {
				    int16u  RingBufRxCtr;
				    int8u  *RingBufRxInPtr;
				    int8u  *RingBufRxOutPtr;
				    int8u   RingBufRx[COMM_RX_BUF_SIZE];
				    int16u  RingBufTxCtr;
				    int8u  *RingBufTxInPtr;
				    int8u  *RingBufTxOutPtr;
				    int8u   RingBufTx[COMM_TX_BUF_SIZE];
				} COMM_RING_BUF;
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void  Comm_HardwareInit (void);
	void  Comm_SoftwareInit (void);
	void  Comm_SendSta (void);
	void  Comm_SendStop (void);
	int8u Comm_GetChar (int8u *err);
	int8u Comm_PutChar (int8u c);
	int8u Comm_GetTxChar (int8u *err);
	void  Comm_PutRxChar (int8u c);
	void  Comm_TimerInit (void);
	void  Comm_TimerProc (void);
	int8u Comm_StateGet (void);
	void  Comm_PutStr (int8u* ptr,int8u n);
	void  Sub_SendVer(void);
	void Comm_ClrRxBuf (void);
#endif
/*-----------------------------------*/
#ifdef   __COMM_C__
#define  COMM_EXT
#else
#define	 COMM_EXT  extern
#endif
/*-------------��������--------------*/
COMM_EXT COMM_RING_BUF	xdata	CommBuf;
COMM_EXT	bdata	bit		CommRxFlag;		//ͨ�Ž��ձ�־λ
COMM_EXT	bdata	bit		CommTxFlag;		//ͨ�ŷ��ͱ�־λ
COMM_EXT	xdata	int8u	Commsetup;		//ͨ�ŷ��ͱ�־λ

