#define __CALMCUT_C__
#include "_Include.h"


static struct mcut_calib xdata *PMCUTCAL= &McuTcal;
static int16u xdata mtemp_buf[MTEMP_CAL_NUM];

//***********************************************************************/
// *��    �ܣ�����MCU�¶Ȳ��������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void McuT_SetCalCnt(struct mcut_calib *p_cal, int8u num)
{
    p_cal->calcnt=num;
}

//***********************************************************************/
// *��    �ܣ����¶�У׼ģ�����¶������ĳ��Ԫ��
// *��    �ڣ�
// *��    �ڣ��¶�ֵ
// *��    ע��
// *�������ʣ�
//***********************************************************************/

static int16s   McuT_RdTempVal(struct mcut_calib *p_cal, int8u posid)
{
    return (p_cal->mcu_temp[posid]);
}
//***********************************************************************/
// *��    �ܣ���MCU�¶�У׼ģ����ADC�����ĳ��Ԫ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static int16u   McuT_RdAdcVal(struct mcut_calib *p_cal, int8u posid)
{
    return (p_cal->ptempadc[posid]);
}

//***********************************************************************/
// *��    �ܣ������¶�У׼ģ�����¶������ĳ��Ԫ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void McuT_WrTempVal(struct mcut_calib *p_cal,int16s val, int8u posid)
{
    p_cal->mcu_temp[posid]=val;
}

//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����ADC�����ĳ��Ԫ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void McuT_WrAdcVal(struct mcut_calib *p_cal,int16u val, int8u posid)
{
    p_cal->ptempadc[posid]=val;
}

//***********************************************************************/
// *��    �ܣ�MCU�¶�A,K��������
// *��    �ڣ�p_cal,temp_calib�ṹ��ָ��(X,Y);
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Mcut_CalcPrm(struct mcut_calib *p_cal)
{
    int8u i;
	float aa;
    for(i=0;i<(p_cal->calcnt-1);i++)    						//�������е�A,Kֵ
    {
		aa=(float)(p_cal->mcu_temp[i+1]-p_cal->mcu_temp[i]);
        p_cal->prm_k[i]=((float)(p_cal->mcu_temp[i+1]-p_cal->mcu_temp[i])/(float)(p_cal->ptempadc[i+1]-p_cal->ptempadc[i]));
        p_cal->prm_a[i] = (float)(p_cal->mcu_temp[i]-p_cal->prm_k[i]*(p_cal->ptempadc[i]) );
    }
}








void SetMcuTCalCnt(int8u cnt)
{
     McuT_SetCalCnt(&McuTcal,cnt);    //����X���¶Ȳ��������
}

//***********************************************************************/
// *��    �ܣ���ȡMCU�¶�У׼�����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u GetMcuTCalCnt(void)
{
    return (McuTcal.calcnt);    //��ȡMCU�¶�У׼�����
}

//***********************************************************************/
// *��    �ܣ���MCU�¶�У׼���¶������ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��¶�ֵ
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int16s GetMcuTCalTempVal(int8u pos)
{
    return (McuT_RdTempVal(&McuTcal,pos));
}


//***********************************************************************/
// *��    �ܣ����¶�У׼ģ����X��ADC�����ĳ��Ԫ��
// *��    �ڣ�pos,�����е�λ��
// *��    �ڣ��Ƕ�0ʱ��ADCֵ
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
int16u GetMcuTCalAdcVal(int8u pos)
{
    return (McuT_RdAdcVal(&McuTcal,pos));
}

//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����X���¶������ĳ��Ԫ��
// *��    �ڣ�val���¶�ADֵ;pos,�����е�λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetMcuTCalTempVal(int16s val, int8u pos)
{
    McuT_WrTempVal(&McuTcal,val,pos);
}

//***********************************************************************/
// *��    �ܣ������¶�У׼ģ����X��ADC�����ĳ��Ԫ��
// *��    �ڣ�val,д��ĽǶ�0ʱ��ADֵ;pos,λ��
// *��    �ڣ�
// *��    ע����֤λ��������ķ�Χ��
//***********************************************************************/
void SetMcuTCalAdcVal(int16u val, int8u pos)
{
    McuT_WrAdcVal(&McuTcal,val,pos);
}

 

void  CalibMtInit(void)
{
	McuTcal.ptempadc=mtemp_buf;            //�¶Ȳ���ģ�����¶�ADֵ��������
    McuTcal.SetCalNum=SetMcuTCalCnt;     //�����¶�У׼����
    McuTcal.RdTempVal=GetMcuTCalTempVal; //���¶���������
    McuTcal.RdAdcVal=GetMcuTCalAdcVal;   //���¶�AD����
    McuTcal.WrTempVal=SetMcuTCalTempVal; //д�¶���������
    McuTcal.WrAdcVal=SetMcuTCalAdcVal;   //д�¶�AD��������
//  McuTcal.CalcPrm=McuTCalcPrm;            //�������A,K
}


//***********************************************************************/
// *��    �ܣ�MCU�¶ȱ궨Ĭ�����ݳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��Flash ��ʼ����ʱ�����
// *�������ʣ�
//***********************************************************************/
void MTCalDataInit()
{
    int8u i;
    SetMcuTCalCnt(5);          								//�����¶Ȳ��������
    for(i=0;i<MTEMP_CAL_NUM;i++)								//21
    {
        McuTcal.ptempadc[i] = 80*(i-MTEMP_CAL_NUM/2)+11200; //MCU���¶�AD
		if(i==0)
        {
		  McuTcal.mcu_temp[i] = 10;
		}
		McuTcal.mcu_temp[i+1] = McuTcal.mcu_temp[i]+3;
    }
/*	McuTcal.ptempadc[0]= 2854;
	McuTcal.ptempadc[1]= 2912;
	McuTcal.ptempadc[2]= 3201;
	McuTcal.ptempadc[3]= 3400;
	McuTcal.ptempadc[4]= 3629;

	McuTcal.mcu_temp[0]= -15;
	McuTcal.mcu_temp[1]= -5;
	McuTcal.mcu_temp[2]= 25;
	McuTcal.mcu_temp[3]= 45;
	McuTcal.mcu_temp[4]= 65;
*/
	McuTcal.ptempadc[0]= 11040;
	McuTcal.ptempadc[1]= 11860;
	McuTcal.ptempadc[2]= 12700;
	McuTcal.ptempadc[3]= 13544;
	McuTcal.ptempadc[4]= 14392;

	McuTcal.mcu_temp[0]= -15;
	McuTcal.mcu_temp[1]= -5;
	McuTcal.mcu_temp[2]= 25;
	McuTcal.mcu_temp[3]= 45;
	McuTcal.mcu_temp[4]= 65;

}



//***********************************************************************/
// *��    �ܣ���ȡMCU�¶�ֵ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
//float GetMt(int16u tadc)
//{
//    int8u i;
//    float offset;
//
//    for(i=0;i<(PMCUTCAL->calcnt-2);i++)
//    {
//        if(tadc<(PMCUTCAL->ptempadc[i+1]))    						//�����¶����²�ģ�����¶������е�λ��
//            break;
//    }
//    offset = (PMCUTCAL->prm_k[i]*tadc+PMCUTCAL->prm_a[i])*1000;    //��ȡ��Ӧ��K,Aֵ��������
//
//    return offset;
//}



//***********************************************************************/
// *��    �ܣ������¶Ƚ����������򣬲�����MCU�¶�ϵ��K,A
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void McuTCalcPrm ()
{

	Sub_BufBubble16s(PMCUTCAL->mcu_temp,PMCUTCAL->ptempadc,PMCUTCAL->calcnt);//����

	Mcut_CalcPrm(&McuTcal);											  			 //MCU T��������

}

//***********************************************************************/
// *��    �ܣ�д���¶Ȳ�������
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u McuT_CalValWr (int8u id,int16s temp,int16u tadc)
{
	if(id>=MTEMP_CAL_NUM)return FALSE;
	PMCUTCAL->WrTempVal(temp,id);       //д�궨���¶�ֵ
	PMCUTCAL->WrAdcVal(tadc,id);        //д��ǰ�궨���¶�ADֵ

	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�MCU�¶ȱ궨��ʼ
// *��    �ڣ�
// *��    �ڣ�
// *��    ע������ͨ����
// *�������ʣ�
//***********************************************************************/
int8u McuT_CalSta (void)
{
//	if(MenuId!=MOD_MOD0)return FALSE;
//	MenuId=MOD_MT_CAL;							  //��ΪMCU�¶ȱ궨���²�һ����ҵ���˴���ע�͵�
	return TRUE;
}
//***********************************************************************/
// *��    �ܣ�MCU�¶ȱ궨����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int8u McuT_CalStop (void)
{
//	if(MenuId!=MOD_MT_CAL)return FALSE;
//	MenuId=MOD_MOD0;						   //��ΪMCU�¶ȱ궨���²�һ����ҵ���˴���ע�͵�
	McuTCalcPrm();
	return TRUE;
}
//***********************************************************************/
int8u Mcut_CalSamp (int8u id,int16s temp,int16u tadc)
{
//	if(MenuId!=MOD_MT_CAL)
//	return FALSE;
	if(McuT_CalValWr(id,temp,tadc)==FALSE)
	return FALSE;
	Sub_SendMcuT(temp,tadc);
	return TRUE;
}