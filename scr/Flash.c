/*--------------------------------��*��*��*Ϣ-----------------------------
* �ļ�����  Flash.c
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
#define   __FLASH_C__
#include  "_Include.h"
/***********************************************************************/
static void Ram_CalDataInit (void);
static void Ram_CalData2Buf (int8u);
static void Ram_Buf2CalData (int8u);
static void FlashArea_User (void) ;
static int8u Flash_CheckSum (int8u *addr,int16u n);
static int8u FlashChk (void);
static  void  Ram_CalDataInit_User(void);
static  void Flash_PageErase (int16u addr);
static  void Flash_BufWr (int16u addr,char* buf,int16u n);
static  void Flash_BufRd (int16u addr,char* buf,int16u n);
static  void FlashUpdate (int16u addr,int8u* buf,int16u n);
static  void Flash_CalDataUpdate_User (int8u page,int16u addr,int8u* buf,int16u n);

//***********************************************************************/
// *��    �ܣ�Flash calibrate Init У׼���ݳ�ʼ��
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
void Flash_CalDataInit (void)
{
    int8u SfrPage_ = SFRPAGE;
    Flash_BufRd(FLASH_SYS,DispBuf,FLASH_BUF_NUM);               //Reading Form FLASH_SYS ��ϵͳ����
    if(OK != FlashChk())                                        //�ж�У���� 0x85
    {
        Ram_CalDataInit();                                      //�ڴ����ݳ�ʼ��
        Flash_CalDataUpdate();                                  //���ڴ������Update��Flash
    }
    else
    {
        Ram_Buf2CalData(1);                                     //��Flash������ϵͳ���ݱ��浽����ڴ�
        Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);          //Reading Form FLASH_USER ����ʱ����
        if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
        {
           FlashArea_User();                                    //�ж�USER����
        }
        else
        {
            Flash_BufRd(FLASH_USER,DispBuf,FLASH_BUF_NUM);      //ȡ���û����������
            if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
            {
               Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);
               Ram_Buf2CalData(2);                              //������ʱ��������ݱ��浽����ڴ�
               Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);      
            }													 //USER�������쳣�� ��TEMP ������ UPDATE ��USER ����		
            else
            {
                Flash_BufRd(FLASH_TEMP,DispBuf,FLASH_BUF_NUM);
                Ram_Buf2CalData(2);                             //������ʱ��������ݱ��浽����ڴ�
            }
         }

      }
	  Flash_CompDataInit();

    ACalXId=GetAngXCalCnt();                                    //��ȡx��У׼��
    ACalYId=GetAngYCalCnt();                                    //��ȡy��У׼��  

    MenuId=MOD_X_CAL;
    Adc_XCalStop();                                              //ͨ����Flash�����ݵ���У׼�������� ���A,K

    MenuId=MOD_Y_CAL;
    Adc_YCalStop();

    MenuId=MOD_T_CAL;
    Temp_CalStop();

	McuT_CalStop();
	MainTime4Cnt = SendSpeed[SendSpeedNum]; 					 

    SFRPAGE = CONFIG_PAGE;

    SBRL0   = -(SYSCLK / Braud[Braud_Num] / 2);             	//���ò�����   SFRPAGE=0x0F     SBRL0 = -(SYSCLK / BAUDRATE / 2);
    SFRPAGE = SfrPage_;
}

/*********************************************************************
* @fn      Flash_CompDataInit
*
* @brief   Compass data init.
*
* @param   void
*
* @return  void
*/
void Flash_CompDataInit (void)
{
    int8u SfrPage_ = SFRPAGE;
    Flash_BufRd(FLASH_12927,DispBuf,FLASH_BUF_NUM);               //Reading Form FLASH_12927
	if(OK != FlashChk())
	{
		Comp_CalDataInit();
		Ram_CalData2Buf(3);
		FlashUpdate(FLASH_12927,&DispBuf[0],FLASH_BUF_NUM);		   //��compass����
	}
	else
	{
		Ram_Buf2CalData(3);
	}
	SFRPAGE = CONFIG_PAGE;
}
//***********************************************************************/
// *��    �ܣ�Flash Update
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�� ϵͳ�û����� ��Ӧ�˲���
// *�������ʣ�
//***********************************************************************/
void Flash_CalDataUpdate (void)
{
    Ram_CalData2Buf(1);										   //para->buff->flash
    FlashUpdate(FLASH_SYS,&DispBuf[0],FLASH_BUF_NUM);          //��ϵͳ������Flash
    Ram_CalData2Buf(2);                                        //���û�������Flash
    FlashUpdate(FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);
    FlashUpdate(FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);
	Ram_CalData2Buf(3);
    FlashUpdate(FLASH_12927,&DispBuf[0],FLASH_BUF_NUM);		   //��compass����
}

void Flash_UserDataUpdate(void)
{

    Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);  
    Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);  

}
//***********************************************************************/
void FlashUpdate (int16u addr,int8u* buf,int16u n)
{
    bit  ea_temp = EA;
    EA    = 0;

    Flash_PageErase(addr);
    delay_ms(250);
    Flash_BufWr(addr,buf,n);

    EA = ea_temp;
}

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע�� Update Flash User/Temp
// *�������ʣ�
//***********************************************************************/
void  Flash_CalDataUpdate_User (int8u page,int16u addr,int8u* buf,int16u n)
{

    int8u sum;
    char code * data pread;                                 // FLASH read pointer
    pread = (char code *)(addr+1);

    sum = *pread;                                           // ȡ��FLASH �е�CheckSum
    Ram_CalData2Buf(page);                                  // ���������浽������

    if(sum == DispBuf[1])return;                            // �ж�ǰ���CheckSum �Ƿ����
    FlashUpdate(addr,buf,n);
}
//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Ram_CalDataInit (void)
{
    AngCalDataInit();          								//�Ƕ�У׼���ݳ�ʼ��
    TempCalDataInit();         								//�¶�У׼���ݳ�ʼ��
	MTCalDataInit();										//MCU�¶�У׼���ݳ�ʼ��
//	Comp_CalDataInit();										//����У�����ݳ�ʼ��
}
//************************��У׼���ݴ浽������***********************************************/
static void Ram_CalData2Buf (int8u page)
{   int8u i;
	int16u j;
    int16u k;
    if(page==1)                		//sys param page                               
    {
        for(i=0;i<ANG_CAL_NUM;i++)
        {

            *((int32s*)&DispBuf[i*12+2]) = GetAngXCalAngVal(i);
            *((int32s*)&DispBuf[i*12+6]) = GetAngYCalAngVal(i);
            *((int16u*)&DispBuf[i*12+10]) = GetAngXCalAdcVal(i);
            *((int16u*)&DispBuf[i*12+12]) = GetAngYCalAdcVal(i);    			 

        }
		j=ANG_CAL_NUM*12+FLASH_CK_NUM;
        for(i=0;i<SystemVerNum;i++)
        {
          *((int8u*)&DispBuf[j+i]) = SystemVer[i];
        }

        j=j+SystemVerNum;
        for(i=0;i<TEMP_CAL_NUM;i++)
        {

            *((int16u*)&DispBuf[j+i*6+0]) = GetTempXCalTempVal(i);   //MCU���¶�      
            *((int16s*)&DispBuf[j+i*6+2]) = GetTempXCalAdcVal(i);
            *((int16s*)&DispBuf[j+i*6+4]) = GetTempYCalAdcVal(i);
        }
		j=j+TEMP_CAL_NUM*6;
        *((int8u*)&DispBuf[j+0])  = GetAngXCalCnt();   //�Ƕ�У׼��������   
        *((int8u*)&DispBuf[j+1])  = GetTempXCalCnt();  //�¶Ȳ�����������
        *((int16u*)&DispBuf[j+2])  = AngRange;


        *((int32s*)&DispBuf[j+4])  = GetAngXSysZero();   //X��Բ����Ƕ�(�ڲ����㣩
        *((int32s*)&DispBuf[j+8])  = GetAngYSysZero();   //y��Բ����Ƕ�(�ڲ����㣩
		*((int8u*)&DispBuf[j+12])  = KeyEnb;

		*((int8u*)&DispBuf[j+13])  = GetMcuTCalCnt();    //MCU�¶�У׼��������
		for(i=0;i<MTEMP_CAL_NUM;i++)
		{
            *((int16s*)&DispBuf[j+14+i*4+0]) = GetMcuTCalTempVal(i);   //MCU���¶�      
            *((int16u*)&DispBuf[j+14+i*4+2]) = GetMcuTCalAdcVal(i);
		}

        k=j+64;

//        *((int16s*)&DispBuf[k+2]) = XMax;                                   //
//		*((int16s*)&DispBuf[k+4]) = XMin;
//		*((int16s*)&DispBuf[k+6]) = YMax;
//		*((int16s*)&DispBuf[k+8]) = YMin;
//		*((int16s*)&DispBuf[k+10]) = XAtYMin;
//		*((int16s*)&DispBuf[k+12]) = XAtYMax;
//		*((int16s*)&DispBuf[k+14]) = CompOffset;
//
//		k=k+20;
        for(;k<FLASH_BUF_NUM;k++)
        {
            DispBuf[k]=0;
        }                                                                       //ʣ��� ����0

        DispBuf[0] = FLASH_DATA_CHK;
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));   //����CheckSum 
    }
    else if(2==page)           				//user param page                                                 
    {
        *((int8u*)&DispBuf[2]) = SendActPosi;                                   //��������ORӦ����
        *((int8u*)&DispBuf[3]) = SendSpeedNum;                                  //���������ٶ�
        *((int8u*)&DispBuf[4])  = Braud_Num;         							//������
        *((int8u*)&DispBuf[5])  = GetFilterCnt();    							//�˲�ϵ��
		*((int16u*)&DispBuf[6]) = AlarmHighLow;
//        *((int32s*)&DispBuf[8]) = AngAlarmOffset;                               //�����ݲ�  HXL ��׷��12/09/10

        j = 12;

        *((int32s*)&DispBuf[j+0])  = GetAngXUsrZero() ;                         //X��Բ����Ƕ�(�û����㣩
        *((int32s*)&DispBuf[j+4]) = GetAngYUsrZero() ;                          //Y��Բ����Ƕ�(�û����㣩   

        *((int32s*)&DispBuf[j+8]) = GetAngXInst() ;                             //X��װ�Ƕ�
        *((int32s*)&DispBuf[j+12]) = GetAngYInst();                             //Y��װ�Ƕ�

        *((int32s*)&DispBuf[j+16]) = GetAngXSet();
        *((int32s*)&DispBuf[j+20]) = GetAngYSet();                              //�����Ƕ�  
        *((int32s*)&DispBuf[j+24]) = GetAngPSet();                              //�汨���Ƕ�  
		*((int32s*)&DispBuf[j+28]) = Addr;

		//*((int8u*)&DispBuf[j+32]) = Addr;
		j = 12+32;
		 *((int8u*)&DispBuf[j+0]) = customAddr;
		 *((int8u*)&DispBuf[j+1]) =Commsetup;
		 *((int8u*)&DispBuf[j+2]) = SetMcuTincelsius;		 //roben 2017-08-03
		 *((int32s*)&DispBuf[j+3]) = McuTAdcValWhileSet;
		  

		k=j+29+32;					 											//	Ԥ��32���ռ�׷���µı���
        for(;k<FLASH_BUF_NUM;k++)
        {
            DispBuf[k]=0;
        }


        DispBuf[0] = FLASH_DATA_CHK;
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));       
    }
	else if(3==page)
	{
        *((int16s*)&DispBuf[2]) = XMax;                                   //
		*((int16s*)&DispBuf[4]) = XMin;
		*((int16s*)&DispBuf[6]) = YMax;
		*((int16s*)&DispBuf[8]) = YMin;
		*((int16s*)&DispBuf[10]) = XAtYMin;
		*((int16s*)&DispBuf[12]) = XAtYMax;
		*((int16s*)&DispBuf[14]) = CompOffset;

		j=16;

		for(;j<FLASH_BUF_NUM;j++)
        {
            DispBuf[j]=0;
        }

		DispBuf[0] = FLASH_DATA_CHK;
        DispBuf[1] = Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2));

	}
}


//**************************************************************************/
//**************************�������������ݴ浽�ڴ�DispBuf to Ram ************/
//**************************************************************************/
static void Ram_Buf2CalData (int8u page)
{   int8u i;
	int16u j;
	int16u k;
    if(1==page)				//sys page
    {
        for(i=0;i<ANG_CAL_NUM;i++)                                              
        {
		/*
            Ang_Xcal.angarr[i] = *((int32s*)&DispBuf[i*12+2]);                  //��Flash���������ݴ浽����ڴ�
            Ang_Ycal.angarr[i] = *((int32s*)&DispBuf[i*12+6]);
            Ang_Xcal.adcarr[i] = *((int16u*)&DispBuf[i*12+10]);
            Ang_Ycal.adcarr[i] = *((int16u*)&DispBuf[i*12+12]);                 //
       	*/
            SetAngXCalAngVal(*((int32s*)&DispBuf[i*12+2]),i);
            SetAngYCalAngVal(*((int32s*)&DispBuf[i*12+6]),i);
            SetAngXCalAdcVal(*((int16u*)&DispBuf[i*12+10]),i);
            SetAngYCalAdcVal(*((int16u*)&DispBuf[i*12+12]),i);                  //

        }
		j=ANG_CAL_NUM*12+FLASH_CK_NUM;
        for(i=0;i<SystemVerNum;i++)
        {
          SystemVer[i] = *((int8u*)&DispBuf[j+i]);
        }


        j=j+SystemVerNum;                                                    
        for(i=0;i<TEMP_CAL_NUM;i++)                                            
        {
            SetTempXCalTempVal(*((int16u*)&DispBuf[j+i*6+0]),i);
            SetTempXCalAdcVal ( *((int16u*)&DispBuf[j+i*6+2]),i);
            SetTempYCalAdcVal ( *((int16u*)&DispBuf[j+i*6+4]),i);                     

        }
		j=j+TEMP_CAL_NUM*6;
        SetAngCalCnt(*((int8u*)&DispBuf[j+0]));    //�Ƕ�У׼��������   
        SetTempCalCnt(*((int8u*)&DispBuf[j+1]));   //�¶Ȳ�����������
        AngRange = *((int16u*)&DispBuf[j+2]);		//������Χ

		SetAngXSysZero(*((int32s*)&DispBuf[j+4]));
        SetAngYSysZero(*((int32s*)&DispBuf[j+8]));
		KeyEnb = *((int8u*)&DispBuf[j+12]);

		SetMcuTCalCnt(*((int8u*)&DispBuf[j+13]));    //MCU�¶�У׼��������
		for(i=0;i<MTEMP_CAL_NUM;i++)
		{
           SetMcuTCalTempVal(*((int16s*)&DispBuf[j+14+i*4+0]),i);   //MCU���¶�      
           SetMcuTCalAdcVal(*((int16u*)&DispBuf[j+14+i*4+2]),i);
		}

		k=j+64;
//		XMax =*((int16s*)&DispBuf[k+2]);                                   //
//		XMin =*((int16s*)&DispBuf[k+4]);
//		YMax =*((int16s*)&DispBuf[k+6]);
//		YMin =*((int16s*)&DispBuf[k+8]);
//		XAtYMin =*((int16s*)&DispBuf[k+10]);
//		XAtYMax =*((int16s*)&DispBuf[k+12]);
//		CompOffset =*((int16s*)&DispBuf[k+14]);
		
    }
    else if(2==page)		   //user page
    {
        SendActPosi         = *((int8u*)&DispBuf[2]);
        SendSpeedNum        = *((int8u*)&DispBuf[3]);
        Braud_Num 			= *((int8u*)&DispBuf[4]);      									//������
        SetFilterCnt(*((int8u*)&DispBuf[5]));    											//�˲�������������
		AlarmHighLow		= *((int16u*)&DispBuf[6]);
//        AngAlarmOffset      = *((int32s*)&DispBuf[8]);                                      //�����ݲ� 

        j = 12;

        SetAngXUsrZero(*((int32s*)&DispBuf[j+0]));
        SetAngYUsrZero(*((int32s*)&DispBuf[j+4]));

        SetAngXInst(*((int32s*)&DispBuf[j+8]));
        SetAngYInst(*((int32s*)&DispBuf[j+12]));                                             //

 //       SetAngXSet(*((int32s*)&DispBuf[j+16]));
 //       SetAngYSet(*((int32s*)&DispBuf[j+20]));
//        SetAngPSet(*((int32s*)&DispBuf[j+24]));
		Addr = *((int32s*)&DispBuf[j+28]) ;

		j=12+32;

		customAddr =  *((int8u*)&DispBuf[j+0]);
		Commsetup  =   *((int8u*)&DispBuf[j+1]) ; 
		SetMcuTincelsius = *((int8u*)&DispBuf[j+2]) ;   //roben 2017-08-03
		McuTAdcValWhileSet =  *((int32s*)&DispBuf[j+3]) ;
	
    }
	else if(3==page)
	{
		XMax =*((int16s*)&DispBuf[2]);                                   //
		XMin =*((int16s*)&DispBuf[4]);
		YMax =*((int16s*)&DispBuf[6]);
		YMin =*((int16s*)&DispBuf[8]);
		XAtYMin =*((int16s*)&DispBuf[10]);
		XAtYMax =*((int16s*)&DispBuf[12]);
		CompOffset =*((int16s*)&DispBuf[14]);
	}

}


//************************* CHECKSUM *************************************/
static int8u Flash_CheckSum (int8u *addr,int16u n)
{   int8u * data pread;
    int16u i;
    int8u  sum=0;
    pread = addr;
    for(i=0;i<n;i++)
    {
      sum += *(pread++);
    }
    return(sum);
}

//***********************************************************************/
// *��    �ܣ�ҳ����
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Flash_PageErase (int16u addr)
{   char xdata * data pwrite;               // FLASH write pointer
    bit  ea_temp = EA;                      // Preserve EA


    pwrite = (char xdata *) addr;
    EA = 0;                                 // Disable interrupts
    VDM0CN = 0xA0;                          // Enable VDD monitor and high threshold
    RSTSRC = 0x02;                          // Enable VDD monitor as a reset source
    FLKEY  = 0xA5;                          // Key Sequence 1
    FLKEY  = 0xF1;                          // Key Sequence 2
    PSCTL |= 0x03;                          // PSWE = 1; PSEE = 1
    VDM0CN = 0xA0;                          // Enable VDD monitor and high threshold
    RSTSRC = 0x02;                          // Enable VDD monitor as a reset source
    *pwrite = 0;                            // Initiate page erase
    PSCTL  &= ~0x03;                        // PSWE = 0; PSEE = 0
    EA = ea_temp;                           // Restore interrupts
}
//***********************************************************************/
// *��    �ܣ�Flash Write
// *��    �ڣ�addr - valid range is from 0x0000 to 0x7DFE for 32K devices
//            valid range is from 0x0000 to 0x3FFE for 16K devices
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Flash_BufWr (int16u addr,char* buf,int16u n)
{   char xdata * data pwrite;               // FLASH write pointer
    bit  ea_temp = EA;                      // Preserve EA
    int16u i;


    pwrite = (char xdata *) addr;
    EA = 0;                                 // Disable interrupts
    for(i=0;i<n;i++)
    {   VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold
        RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
        FLKEY  = 0xA5;                      // Key Sequence 1
        FLKEY  = 0xF1;                      // Key Sequence 2
        PSCTL |= 0x01;                      // PSWE = 1 which enables writes
        VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold
        RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
        *pwrite++=*buf++;
        PSCTL &= ~0x01;                     // PSWE = 0 which disable writes
    }
    EA = ea_temp;                           // Restore interrupts
}
//***********************************************************************/
// *��    �ܣ�Flash Read
// *��    �ڣ�addr - valid range is from 0x0000 to 0x7DFE for 32K devices
//            valid range is from 0x0000 to 0x3FFE for 16K devices
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void Flash_BufRd (int16u addr,char* buf,int16u n)
{   char code * data pread;                 // FLASH read pointer
    bit  ea_temp = EA;                      // Preserve EA
    int16u i;

    pread = (char code *) addr;
    EA    = 0;                              // Disable interrupts
    for(i=0;i<n;i++)
    {
      *buf++ = *pread++;
    }
    EA = ea_temp;                           // Restore interrupts
}

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static int8u FlashChk (void)
{   int8u temp;;
    temp = DispBuf[0];
    if( temp == FLASH_DATA_CHK)return OK;
    else
    return ERR;
}

//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/
static void FlashArea_User (void)
{
    Flash_BufRd(FLASH_USER,DispBuf,FLASH_BUF_NUM);                
    if((OK != FlashChk())||(DispBuf[1] != Flash_CheckSum((int8u*)(&DispBuf[2]),(FLASH_BUF_NUM-2))))
    {
      Ram_CalDataInit_User();                           //USER��CHK��CheckSum ������ ���ʼ�� TEMP OR USER ���������
      Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);
      Flash_CalDataUpdate_User (2,FLASH_USER,&DispBuf[0],FLASH_BUF_NUM);
    }
    else
    {
       Ram_Buf2CalData(2);                              //��USER��������ݴ浽�ڴ�
       Flash_CalDataUpdate_User (2,FLASH_TEMP,&DispBuf[0],FLASH_BUF_NUM);  
       													//USER��CHK��CheckSum ��û������ ��USER ������UPDATE ��TEMP ����
    }

}


//***********************************************************************/
// *��    �ܣ�
// *��    �ڣ�
// *��    �ڣ�
// *��    ע��
// *�������ʣ�
//***********************************************************************/

static  void  Ram_CalDataInit_User(void)
{

    SetAngXUsrZero(0);
    SetAngYUsrZero(0);	
 //   SetAngXSet(5000);			  					//X���û������ǲ���
 //   SetAngYSet(5000);			 					//Y���û������ǲ���
//    SetAngPSet(5000);            	
    SetAngXInst(0);
    SetAngYInst(0);
    Init_SystemPara();

}



