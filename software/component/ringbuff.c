/**
  ******************************************************************************
  * @file    ringbuff.h 
  * @author  ��С�
  * @version V1.2.0
  * @date    2019.12.25
  * @brief   ringbuff manage module
  ******************************************************************************
  * @attention
  Copyright (c) 2019 --- 2025 ��С�. All rights reserved.
  ������
  1����ģ�飨ringbuff������ƻ��λ���ģ��/��ˮ���棬���ߣ���Сᯣ���������Ȩ��
  2����ģ������ο�ѧϰ���о�������ʹ��
  3����������ҵĿ�ģ���Ҫ��������ͬ��
  4�����߽��ṩԴ���룬���Դӡ�github.com�������أ��������ڴ�����
  5���Դ�ģ���ʹ����������֤�����ԡ�
  6������ʹ�ñ�ģ����ɵ��κ���ʧ�����ߣ���Сᯣ����е��κ����Ρ�
  7���罫��ģ�鼯������Ĵ����У����뱣��������
  8���粻ͬ����������벻Ҫʹ�ô�ģ��
  ******************************************************************************
  ******************************************************************************
  * �������ͻ��ڡ�DataType.h������
  * ��ģ����Ӳ���޹ء�����������޹ء���һ����ɶ�����ģ�飬��Ϊһ���м�������������
  * ����������ģ�飬��Ҫ�ṩһ������Ĺ����ܣ�˼·�ǽ�һ��һά���ݻ�����֯��һ�����ε��߼��ṹ��
  * ��Ҫ���ܣ�
  * A����ʼ����������������������뻺��Խ�����
  * B���򻺴�д�����ݣ��������ֽ�����ֽ����ַ�����
  * C���ӻ����ȡ���ݣ�������ȡ���ֽڡ����ֽ���ȫ��������
  * D����ѯ����״̬������գ�������������ǿ�
  * E����ѯ������пռ��С
  * F����ѯ����ʹ������������װ�ж������ݣ�
  * G�����軺��������������������ָ�����ʼ״̬��
  * ------------------------------------------------------------
  * 
  * 
  * ------------------------------------------------------------
  * ע�⣺
  * 1����ģ���������ܹ��������˼·����ģ��Ϊ�м�㣩
  * 2��������ƣ���ȡ����֮�󻺴��Ƿ�������ܿ�---������߲����㣨����ԭ�����ݣ�֮���ɺ���д��������滻��
  * 3��������֮���޷�д���µ����ݣ������ȡ�����е����ݻ��߸�λ��ʵ��
  * ------------------------------------------------------------
  * �ϲ�Ͳ���ú���
  * GetRingbuffFreeSize();GetRingbuffStatus();
  * ------------------------------------------------------------
  * �ϲ���ú�����
  * GetRingbuffUseSize();resetRingBuff();
  * readOneMemberFromRingbuff()
  * CalRingbuffUsedPercent();
  * �Ͳ���ú�����
  * RingbuffInit();writeOneMemberToRingbuff();writeNMemberToRingbuff();
  * ------------------------------------------------------------
*/
#include "includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/********************************************************************
�������ܣ���ʼ�����λ���ʵ��
��ڲ�����Ringbuff�������ʵ����buff�����ַ��BuffByteSize�����С,MemberSize�����Ա��С 
clearFlag�����־����ȡ����֮���Ƿ����㻺�棩
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void ringbuffInit(RingbuffMgr_t *Ringbuff,void *buff,u16 BuffByteSize,u8 MemberSize,u8 clearFlag)
{
	Ringbuff->UsePercent=0;
	Ringbuff->MemberSize=MemberSize;
	Ringbuff->clearEnableFlag=clearFlag;
	Ringbuff->pBuff=buff;
	Ringbuff->usedBytes=0;
	Ringbuff->inBuffPointer=0;
	Ringbuff->OutBuffPointer=0;
	Ringbuff->status=RingbuffEmpty;
	Ringbuff->totalByteSize=BuffByteSize;
	Ringbuff->freeBytesSize=BuffByteSize;
	memset(Ringbuff->pBuff,RING_BUFF_WATERMARK,Ringbuff->totalByteSize);
}
/********************************************************************
�������ܣ���ȡ���λ���״̬
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
u8 getRingbuffStatus(RingbuffMgr_t *Ringbuff)
{
	return Ringbuff->status;
}
/********************************************************************
�������ܣ���ȡ���λ������ݴ�С
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
u16 getRingbuffUseSize(RingbuffMgr_t *Ringbuff)
{
	return Ringbuff->usedBytes;
}
/********************************************************************
�������ܣ���ȡ���λ���ʣ��ռ��С
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
u16 getRingbuffFreeSize(RingbuffMgr_t *Ringbuff)
{
	return Ringbuff->freeBytesSize;
}
/********************************************************************
�������ܣ����ö�ȡ��������֮���Ƿ����㻺�����ݵı�־
��ڲ�����flag=TRUEʱ��ȡ�������������Ӧ���棬��������
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void setRingbuffClearFlag(RingbuffMgr_t *Ringbuff,u8 flag)
{
	Ringbuff->clearEnableFlag=flag;
}
/********************************************************************
�������ܣ���ȡ���λ���ʣ��ռ��С
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void resetRingBuff(RingbuffMgr_t *Ringbuff)
{
	Ringbuff->UsePercent=0;
	Ringbuff->inBuffPointer=0;
	Ringbuff->OutBuffPointer=0;
	Ringbuff->status=RingbuffEmpty;
	Ringbuff->usedBytes=0;
	Ringbuff->freeBytesSize=Ringbuff->totalByteSize;
	if(Ringbuff->clearEnableFlag==TRUE)
	{
		memset(Ringbuff->pBuff,RING_BUFF_WATERMARK,Ringbuff->totalByteSize);
	}
}
/********************************************************************
�������ܣ��ӻ��λ����ȡһ���ֽ�
��ڲ�����Ringbuff�������ʵ��,*data�������ݱ����ַ
��    ע���ޡ�
********************************************************************/
RingbuffOperateResult_t readOneMemberFromRingbuff(RingbuffMgr_t *Ringbuff,void *data)
{
	if(Ringbuff->pBuff == NULL)return RingbuffOpNotInit;
	if(Ringbuff->usedBytes>0)
	{
		Ringbuff->usedBytes-=Ringbuff->MemberSize;
		Ringbuff->freeBytesSize+=Ringbuff->MemberSize;
		if(Ringbuff->clearEnableFlag==TRUE)	{
			memcpy(data,(u8*)Ringbuff->pBuff+Ringbuff->OutBuffPointer,Ringbuff->MemberSize);
			memset((u8*)Ringbuff->pBuff+Ringbuff->OutBuffPointer,0,Ringbuff->MemberSize);
			Ringbuff->OutBuffPointer+=Ringbuff->MemberSize;
		}
		else{
			memcpy(data,(u8*)Ringbuff->pBuff+Ringbuff->OutBuffPointer,Ringbuff->MemberSize);
			Ringbuff->OutBuffPointer+=Ringbuff->MemberSize;
		}
		if(Ringbuff->OutBuffPointer >= Ringbuff->totalByteSize)Ringbuff->OutBuffPointer=0;
		return RingbuffOpSucess;
	}
	else{
		Ringbuff->usedBytes=0;
		Ringbuff->inBuffPointer=0;
		Ringbuff->OutBuffPointer=0;
		Ringbuff->status=RingbuffEmpty;
		return RingbuffOpFailure;
	}
}
/********************************************************************
�������ܣ�дһ�����ݳ�Ա�����λ���
��ڲ�����Ringbuff�������ʵ����dataд������
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
RingbuffOperateResult_t writeOneMemberToRingbuff(RingbuffMgr_t *Ringbuff,void *data)
{
	if(Ringbuff->pBuff==NULL)return RingbuffOpNotInit;
	if(Ringbuff->usedBytes<Ringbuff->totalByteSize)
	{
		Ringbuff->usedBytes+=Ringbuff->MemberSize;
		Ringbuff->freeBytesSize-=Ringbuff->MemberSize;
		memcpy((u8*)Ringbuff->pBuff+Ringbuff->inBuffPointer,data,Ringbuff->MemberSize);
		Ringbuff->inBuffPointer+=Ringbuff->MemberSize;
		
		if(Ringbuff->inBuffPointer>=Ringbuff->totalByteSize)Ringbuff->inBuffPointer=0;
		if(Ringbuff->usedBytes==Ringbuff->totalByteSize)Ringbuff->status=RingbuffFull;
		else Ringbuff->status=RingbuffNotFull;
		return RingbuffOpSucess;
	}
	else 
	{
		Ringbuff->status=RingbuffFull;
		return RingbuffOpFailure;
	}
}
/********************************************************************
�������ܣ�дN����Ա�����λ���
��ڲ�����Ringbuff�������ʵ����pbuff��д������ָ�룬writeSize��д���ݴ�С
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
RingbuffOperateResult_t writeNMemberToRingbuff(RingbuffMgr_t *Ringbuff,void *pbuff,u16 WriteByteSize)
{
	u16 i;
	if(Ringbuff->pBuff==NULL)return RingbuffOpNotInit;
	if(Ringbuff->freeBytesSize >= WriteByteSize)	
	{
		Ringbuff->usedBytes  += WriteByteSize;
		Ringbuff->freeBytesSize -= WriteByteSize;
		for(i=0;i<WriteByteSize;i++)
		{
			memcpy((u8*)Ringbuff->pBuff+Ringbuff->inBuffPointer,(u8*)pbuff+i,1);
			Ringbuff->inBuffPointer++;
			if(Ringbuff->inBuffPointer >= Ringbuff->totalByteSize)Ringbuff->inBuffPointer = 0;		
		}
		if(Ringbuff->usedBytes==Ringbuff->totalByteSize)Ringbuff->status=RingbuffFull;
		else Ringbuff->status=RingbuffNotFull;
		return RingbuffOpSucess;
	}
	else 
	{
		Ringbuff->status=RingbuffFull;
		return RingbuffOpFailure;
	}
}

/********************************************************************
�������ܣ����㻺��ʹ�ðٷֱ�ֵ
��ڲ�����Ringbuff�������ʵ��
��    �أ��ٷֱ�ֵ��0---100����
��    ע���ޡ�
********************************************************************/
u8 CalRingbuffUsedPercent(RingbuffMgr_t *Ringbuff)
{
	u8 temp=0;
	u16 i,UseBytes=0;
	for(i=0;i<Ringbuff->totalByteSize;i++)
	{
		temp=*((u8*)Ringbuff->pBuff+i);
		if(temp!=RING_BUFF_WATERMARK)UseBytes++;
	}
	temp=(u8)((float)UseBytes*100/(float)Ringbuff->totalByteSize);
	//printf("ringbuff UseBytes %d totalByteSize=%d\r\n",UseBytes,Ringbuff->totalByteSize);
	Ringbuff->UsePercent=temp;
	return temp;
}
/********************************************************************
�������ܣ���ӡ�������ʵ����ر�������������
��ڲ�����Ringbuff�������ʵ����printDataFlag=TRUEʱ��ӡ�������������ݣ�ÿ16�ֽ�һ�У�������к�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void printRingBuff(RingbuffMgr_t *Ringbuff,u8 printDataFlag)
{
	#ifdef DEBUG
		u16 i,line=0;
		printf(" ----------------------------------- \r\n");
		if(Ringbuff->pBuff==NULL)printf("ringbuff Ringbuff not init \r\n");
		printf("ringbuff UsePercent %d%% \r\n",Ringbuff->UsePercent);
		printf(" totalByteSize:%d usedBytes:%d freeBytesSize:%d ",
						Ringbuff->totalByteSize,Ringbuff->usedBytes,Ringbuff->freeBytesSize);
		printf(" inBuffPointer:%d OutBuffPointer:%d ",Ringbuff->inBuffPointer,Ringbuff->OutBuffPointer);
		if(Ringbuff->status==RingbuffEmpty)printf(" status:RingbuffEmpty \r\n");
		if(Ringbuff->status==RingbuffFull)printf(" status:RingbuffFull \r\n");
		if(Ringbuff->status==RingbuffNotFull)printf(" status:RingbuffNotFull \r\n");
		if(printDataFlag==TRUE)
		{
			printf(" -------------- ringbuf data -------------- \r\n");
			if(Ringbuff->MemberSize==1)
			{
				for(i=0;i<Ringbuff->totalByteSize;i++)
				{
					if(i%16==0)printf("\r\n line:%d ",line++);
					printf(" %02X",*((u8*)Ringbuff->pBuff+i));
				}
			}
			else if(Ringbuff->MemberSize==2)
			{
				for(i=0;i<Ringbuff->totalByteSize/Ringbuff->MemberSize;i++)
				{
					if(i%16==0)printf("\r\n line:%d ",line++);
					printf(" %04X",*((u16*)Ringbuff->pBuff+i));
				}
			}
			else if(Ringbuff->MemberSize==4)
			{
				for(i=0;i<Ringbuff->totalByteSize/Ringbuff->MemberSize;i++)
				{
					if(i%16==0)printf("\r\n line:%d ",line++);
					printf(" %08X",*((u32*)Ringbuff->pBuff+i));
				}
			}
			printf(" \r\n");
		}
	#endif
}
/********************************************************************
�������ܣ�
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
#if 1
#define U1_RXBUFF_SIZE	64
#define U1_TXBUFF_SIZE	32
RingbuffMgr_t u1RxBuffMgr,u1TxBuffMgr;
#define RING_BUFF_DATA_TYPE		u8
RING_BUFF_DATA_TYPE U1RXbuff[U1_RXBUFF_SIZE],U1TXbuff[U1_TXBUFF_SIZE];
void ringbuffTest(void)
{
	u8 UsePercent;
	RING_BUFF_DATA_TYPE i,data,buff[40];
	RING_BUFF_DATA_TYPE cmdline1[]={0x0A,0,10,0x12,0xab,1,2,3,4,5,6,7,0xDA};
	RING_BUFF_DATA_TYPE cmdline2[]={8,9,0x0A,0,8,0x15,0xEA,1,2,3,6,7,0xDA,5,6};
	u16 temp,FrameLength;
	ringbuffInit(&u1RxBuffMgr,U1RXbuff,sizeof(U1RXbuff),sizeof(RING_BUFF_DATA_TYPE),TRUE);
	ringbuffInit(&u1TxBuffMgr,U1TXbuff,sizeof(U1TXbuff),sizeof(RING_BUFF_DATA_TYPE),TRUE);
	writeNMemberToRingbuff(&u1RxBuffMgr,(void *)cmdline1,sizeof(cmdline1));
	writeNMemberToRingbuff(&u1RxBuffMgr,(void *)cmdline2,sizeof(cmdline2));
	data=0x56;i=0;
	printf(" ------------------ ��ˮ������� 1------------------\r\n");
	writeOneMemberToRingbuff(&u1RxBuffMgr,&data);
	printRingBuff(&u1RxBuffMgr,TRUE);
	while(readOneMemberFromRingbuff(&u1RxBuffMgr,&data)==RingbuffOpSucess)
	{
		printf(" %02X",data);i++;
		if(i%16==0)printf("\r\n");
	}
	printf("\r\n");
	UsePercent=CalRingbuffUsedPercent(&u1RxBuffMgr);
	printf("UsePercent=%d%%\r\n",UsePercent);
	printRingBuff(&u1RxBuffMgr,TRUE);
	printf(" ------------------ ��ˮ������� 2------------------\r\n");
}
#endif
//---------------------------------------------------------------------







