/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  AdcDrv.c
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
#define  __ADC_DRV_C__
#include "_Include.h"

/***********************************************/
struct adc_entity xdata x_axis_adc;
struct adc_entity xdata y_axis_adc;
struct adc_entity xdata temp_adc;
//struct adc_entity xdata vdd_adc;
/*****************************************************/
static int8u    code    PinIdTab[13] = {   2,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1};
//static int8u    code    PinMuxTab[]  = {0x30,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a};
static int8u    code    PinMuxTab[]  = {0x30,0x11,0x12,0x11,0x12,0x11,0x12,0x11,0x12,0x11,0x12,0x11,0x12};
                        //AD����ͨ��ѡ��0x0a=P1.2,0x09=P1.1,0x30=temp,0x0b=Vdd
/*****************************************************/
int8u   idata   AdcSwitchId=0;
int8u   idata   AdcEndTag=0;
int8u   idata   AdcEndId;

static void Adc_SetInit (void);
static void Adc_VrefInit (void);
static void Adc_IsrInit (void);
static void AdcEntityInit(void);
//***********************************************************************/
// *��    �ܣ�ADC��ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Adc_Init (void)
{
    Adc_SetInit();
    Adc_VrefInit();
    Adc_IsrInit();
    AdcEntityInit();    //ADCʵ���ʼ��
}
//***********************************************************************/
void Adc_Enable (void)
{
    ADC0CN |= 0x10;     // enable ADC0      AD0EN = 1;
}
//***********************************************************************/
// *��    �ܣ���ȡX��ADCֵ
// *��    �ڣ�
// *��    �ڣ�X��ADCֵ�������˲���
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int16u Adc_GetXVal(void)
{
    return  (x_axis_adc.ftval);
}
//***********************************************************************/
// *��    �ܣ���ȡY��ADCֵ
// *��    �ڣ�
// *��    �ڣ�Y��ADCֵ�������˲���
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int16u Adc_GetYVal(void)
{
    return  (y_axis_adc.ftval);
}
//***********************************************************************/
// *��    �ܣ���ȡ�¶�ADCֵ
// *��    �ڣ�
// *��    �ڣ��¶�ADCֵ�������˲���
// *��    ע��
// *�������ʣ�
//***********************************************************************/
int16u Adc_GetTemp(void)
{
    return  (temp_adc.ftval);
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
/*
int16u Adc_GetVdd(void)
{
    return  (vdd_adc.ftval);
}
*/
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Adc_SetInit (void)
{   int8u SfrPage_ = SFRPAGE;


    SFRPAGE = ACTIVE_PAGE;
    ADC0MX  = 0x0c;     //0x0c
    ADC0CF |= 0x01;     // Set GAINEN = 1
    ADC0H   = 0x04;     // Load the ADC0GNH address
    ADC0L   = 0x6C;     // Load the upper byte of 0x6CA to
    ADC0H   = 0x07;     // Load the ADC0GNL address
    ADC0L   = 0xA0;     // Load the lower nibble of 0x6CA to
    ADC0H   = 0x08;     // Load the ADC0GNA address
    ADC0L   = 0x01;     // Set the GAINADD bit
    ADC0CF &= ~0x01;    // Set GAINEN = 0
    ADC0CN  = 0x80;     // ADC0 disabled, normal tracking,
    REF0CN  = 0x00;     // Enable on-chip VREF and buffer
    ADC0MX  = 0x11;     // Set ADC input to P1.2
    ADC0CF  = ((SYSCLK / 3000000) - 1) << 3;    // Set SAR clock to 3MHz
    SFRPAGE = SfrPage_;
}
//***********************************************************************/
static void Adc_VrefInit (void)
{
    REF0CN    = 0x37;       // Enable on-chip VREF = 2.2v and buffer    ok
}
//***********************************************************************/
static void Adc_IsrInit (void)
{
    EIE1 |= 0x04;           // Enable ADC0 conversion complete int.
}

//***********************************************************************/
// *��    �ܣ�ADCֵ�˲�
// *��    �ڣ�p_adc,ADC�ṹ��ָ�루X,Y,Temp);val,��ֵ
// *��    �ڣ�
// *��    ע��Ϊ�������Ӧ�ٶȣ������޷��˲�   ADCֵ�ײ��˲���X/Y/T�ĺ�������
// *�������ʣ�
//***********************************************************************/
static void AdcFilter (struct adc_entity *p_adc, int16u val)
{
    if(ValInRange(p_adc->ftval,val,(int16u)FILTER_NUM))                     //�޷��˲�
    {
        p_adc->ftval=SlideWinFilter (&(p_adc->arrpt),&(p_adc->arrcnt),
                                     p_adc->valarr,val,(int16u)FILTER_NUM); //�����˲�
    }
    else
    {
            p_adc->ftval=val;                                               //���ȱ仯��ֱ��ʹ�õ�ǰֵ
            p_adc->arrcnt=0;                                                //����д����
    }
}
//***********************************************************************/
// *��    �ܣ�X��ADC�˲�����
// *��    �ڣ�adcval��X���ADCֵ
// *��    �ڣ�
// *��    ע���˲�֮���ֱֵ��д��x_axis_adc.ftval��
// *�������ʣ�
//***********************************************************************/
/*void  XAdcFilter(int16u adcval)
{
    AdcFilter(&x_axis_adc,adcval);
}
*/
//***********************************************************************/
// *��    �ܣ�Y��ADC�˲�����
// *��    �ڣ�adcval��Y���ADCֵ
// *��    �ڣ�
// *��    ע���˲�֮���ֱֵ��д��y_axis_adc.ftval��
// *�������ʣ�
//***********************************************************************/
/*void  YAdcFilter(int16u adcval)
{
    AdcFilter(&y_axis_adc,adcval);
}
 */
//***********************************************************************/
// *��    �ܣ��¶�ADC�˲�����
// *��    �ڣ�adcval���¶ȵ�ADCֵ
// *��    �ڣ�
// *��    ע���˲�֮���ֱֵ��д��temp_adc.ftval��
// *�������ʣ�
//***********************************************************************/
/*void  TAdcFilter(int16u adcval)
{
    AdcFilter(&temp_adc,adcval);
}
*/

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
/*
void    VAdcFilter(int16u adcval)
{
    AdcFilter(&vdd_adc,adcval);
}
*/
//***********************************************************************/
// *��    �ܣ�ADCʵ���ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void AdcEntityInit(void)
{
    //---------X��
    x_axis_adc.num=0;                 //ADC����˿ں�
    x_axis_adc.arrpt=0;               //ADC��������λ��ָ��
    x_axis_adc.arrcnt=0;              //Ԫ�ظ���Ϊ0
    x_axis_adc.ftval=0;               //ADC�˲�֮���ֵ
//  x_axis_adc.Filter=XAdcFilter;     //�˲�����ָ���ʼ��
    x_axis_adc.next=&y_axis_adc;      //ADC�ṹ�������ʼ��
    //---------Y��
    y_axis_adc.num=1;
    y_axis_adc.arrpt=0;
    y_axis_adc.arrcnt=0;              //Ԫ�ظ���Ϊ0
    y_axis_adc.ftval=0;
//  y_axis_adc.Filter=YAdcFilter;
    y_axis_adc.next=&temp_adc;
    //------------�¶�
    temp_adc.num=2;
    temp_adc.arrpt=0;
    temp_adc.arrcnt=0;                //Ԫ�ظ���Ϊ0
    temp_adc.ftval=0;
//  temp_adc.Filter=TAdcFilter;
    temp_adc.next=(struct adc_entity *)0;  //�������
/*
    vdd_adc.num=3;
    vdd_adc.arrpt=0;
    vdd_adc.ftval=0;
    vdd_adc.Filter=VAdcFilter;
    vdd_adc.next=(struct adc_entity *)0;
*/
}
/*
void Adc_Init(struct adc_entity *adc_ent, int8u adc_num)
{
    adc_ent->num=adc_num;
    adc_ent->arrpt=0;
    adc_ent->ftval=0;
}
*/
//***********************************************************************/
// *��    �ܣ�ADCֵ�ı���Ͳɼ��˿�ѡ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Adc_SaveAndSwitch (int16u val)
{
    struct adc_entity *adc_ent;
    adc_ent=&x_axis_adc;                            //ָ��ADC�ṹ���������ʼԪ��
   if(PinIdTab[AdcSwitchId]==1||PinIdTab[AdcSwitchId]==0)
	  val=65540-val;								//2013520xinglong  ��������
	
    while(adc_ent!=(struct adc_entity *)0)          //��������
    {
        if(adc_ent->num==PinIdTab[AdcSwitchId])     //�Ƚ϶˿ں�
            break;                                  //���
        adc_ent=adc_ent->next;                      //����ȣ��Ƚ���һ��Ԫ��
    }
    if(adc_ent!=(struct adc_entity *)0)             //������Ԫ����������
//      adc_ent->Filter(val);                       //�˲�
        AdcFilter(adc_ent,val);

    AdcEndId=PinIdTab[AdcSwitchId];                 //����˿ں�
    AdcSwitchId++;                                  //ȡ������һ�˿ں�

    if(13==AdcSwitchId)AdcSwitchId=0;
    ADC0MX=PinMuxTab[AdcSwitchId];
    AdcEndTag=1;
}
//***********************************************************************/
// *��    �ܣ�AD�жϴ���
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Adc_Isr (void) interrupt INTERRUPT_ADC0_EOC//9
{
    static xdata int32u sum = 0;
    static xdata int16u cnt = 0;

    AD0INT=0;                                    // clear ADC0 conv. complete flag
    sum+=ADC0;
    if(++cnt==SUM_CNT)
    {
        Adc_SaveAndSwitch(sum/SUM_CNT1);
        sum=0;
        cnt=0;
    }
    else
        ADC0CN |= 0x10;
}
