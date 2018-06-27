/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  AdcDrv.h
* �桡����  v1.0
* ��  ����  ADC�ײ�����ģ��	100T˫�� ���Ϻ�İ汾
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
	#include "_Type.h"
	#ifndef __ADC_DRV_H__
	#define __ADC_DRV_H__
	#include <compiler_defs.h>
	#include <c8051F500_defs.h>


	/*-----�궨��---------------------*/
	#define	SUM_CNT			256
	#define	SUM_CNT1		16
	#define FILTER_NUM 		64
	#define FILTER_SUBB		300
	
	/*-----�ṹ���������Ͷ���---------*/
	struct adc_entity{
		int8u num;					  //ADC�˿ں�
		int8u arrpt;				  //��������ָ��
		int8u arrcnt;				  //��������Ԫ�ظ���
		int16u valarr[FILTER_NUM];	  //��������
		int16u ftval;	  			  //�˲����ֵ
		struct adc_entity *next;	  //ADC�������һ��Ԫ��
//		void (*Filter)(int16u adc_val);	//�˲�����ָ��
	};
	

	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Adc_Init (void);
	void Adc_Enable (void);	
	int16u Adc_GetXVal(void);
	int16u Adc_GetYVal(void);
	int16u Adc_GetTemp(void);
//	int16u Adc_GetVdd(void);

#endif
/***********************************************************************/
#ifdef   __ADC_DRV_C__
#define  ADC_DRV_EXT
#else
#define  ADC_DRV_EXT  extern
#endif
extern int8u idata	AdcEndTag;
extern int8u idata	AdcEndId;	
