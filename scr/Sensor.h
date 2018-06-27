/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Sensor.h
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
#include "_Type.h"
#ifndef __SENSOR_H__
#define __SENSOR_H__


/***********************************************************************/

	/*-----�궨��---------------------*/

	/*-----�ṹ���������Ͷ���---------*/
	
	struct sensor{
		int16u val;									//ԭʼֵ�������˲���
		float  ftval_f;								//�˲�֮���ֵ(�м������������һ���˲���Filter_1st)
		int16u ftval;								//�˲�֮���ֵ
		int16s oftval;   							//�²�֮���ֵ
		int32s ang;   								//�Ƕ�(�����²���
		struct temp_calib *tempcal;				    //ָ��temp_calib�ṹ��ָ��
		struct ang_calib *angcal;					//ָ��ang_calib�ṹ��ָ��
		void (*DataInit)();						    //����ָ��
		void (*Filter)(int16u val, int8u ftcnt);	//ָ���˲�����
		int16s (*GetTempOffset)(int16u temp);		// ָ���ȡ�²��ĽǶ�ֵ����
		void (*AngCalc)();							//����ָ��ָ��Ƕȼ��㺯��		 
	};

	/*-----���������ȣ�����---------*/
	
	/*-----��������-------------------*/
	void SnInit(void);    //
	
	void SnProc(void);    //
	
	int8u GetFilterCnt(void);
	void SetFilterCnt(int8u cnt);
	int32s Get_McuTempInCelsius(void);
	
	int16u Get_McuTemp(void);
	
	int16u GetXFtVal(void);    //һ���˲����ֵ
	int16u GetYFtVal(void);    //
	
	int16s GetXOftVal(void);   //�²�֮���ֵ
	int16s GetYOftVal(void);
	
	int32s GetXAng(void);     //�Ƕ�ֵ
	int32s GetYAng(void);


#endif
/***********************************************************************/
#ifdef   __SENSOR_C__
#define  SENSOR_EXT
#else
#define	 SENSOR_EXT  extern
#endif
SENSOR_EXT struct sensor xdata accel_x;		  //���ٶȴ�����X
SENSOR_EXT struct sensor xdata accel_y;		  //���ٶȴ�����Y