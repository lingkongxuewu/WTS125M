/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Flash.h
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
#ifndef __FLASH_H__

	#define __FLASH_H__
	/*-----�궨��---------------------*/
//	#define		FLASH_DATA_CHK		0x85	



	#define FLASH_SYS       0xD600L            // For 64K Flash devices	
	#define FLASH_USER      0xD800L            // For 64K Flash devices
	#define	FLASH_TEMP      0xDA00L				// For 64K Flash devices ��ʱ����
	#define FLASH_12927		0xDC00L				// For 512B Flash compass data

	#define	FLASH_DATA_CHK	0x85			   //У����
// 	#define	NUM_PAGE0     	0x200// 1D0//          //Flash page=0 CheckSum ���ۼӴ���
//	#define	NUM_PAGE1     	0x200// 1fc//          //Flash page=1 CheckSum 	�ۼӴ���
	#define	FLASH_BUF_NUM   0x200//         	//FLASH BUF ��
	#define	FLASH_CK_NUM  	2


	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Flash_CalDataInit (void);
	void Flash_CalDataUpdate (void);
	void Flash_UserDataUpdate(void);
	void Flash_CompDataInit (void);

#endif

//***********************************************************************/
#ifdef   __FLASH_C__
#define  FLASH_EXT
#else
#define  FLASH_EXT  extern
#endif
/*-------------��������--------------*/
FLASH_EXT	xdata	char	DispBuf[FLASH_BUF_NUM];
//***********************************************************************/