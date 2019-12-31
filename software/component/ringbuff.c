/**
  ******************************************************************************
  * @file    ringbuff.h 
  * @author  西小岑
  * @version V1.2.0
  * @date    2019.12.25
  * @brief   ringbuff manage module
  ******************************************************************************
  * @attention
  Copyright (c) 2019 --- 2025 西小岑. All rights reserved.
  声明：
  1、此模块（ringbuff），简称环形缓存模块/流水缓存，作者（西小岑）保留所有权限
  2、此模块仅供参考学习、研究、交流使用
  3、如用于商业目的，需要征求作者同意
  4、作者仅提供源代码，可以从“github.com”上下载，但不限于此渠道
  5、对此模块的使用请自行验证、测试、
  6、对于使用本模块造成的任何损失，作者（西小岑）不承担任何责任。
  7、如将此模块集成于你的代码中，必须保留此声明
  8、如不同意上述条款，请不要使用此模块
  ******************************************************************************
  ******************************************************************************
  * 数据类型基于《DataType.h》定义
  * 此模块与硬件无关、与其他软件无关、是一个完成独立的模块，作为一个中间层软件独立存在
  * 不依赖其他模块，主要提供一个缓存的管理功能，思路是将一个一维数据缓存组织成一个环形的逻辑结构。
  * 主要功能：
  * A、初始化缓存管理器，将管理器与缓存对接起来
  * B、向缓存写入数据（包括单字节与多字节两种方法）
  * C、从缓存读取数据（包括读取单字节、多字节与全部读出）
  * D、查询缓存状态：缓存空，缓存满，缓存非空
  * E、查询缓存空闲空间大小
  * F、查询缓存使用量（即缓存装有多少数据）
  * G、重设缓存各个变量（各个变量恢复到初始状态）
  * ------------------------------------------------------------
  * 
  * 
  * ------------------------------------------------------------
  * 注意：
  * 1、此模块符合三层架构程序设计思路（此模块为中间层）
  * 2、清零控制：读取数据之后缓存是否清零可受控---清零或者不清零（保持原样数据，之后由后面写入的数据替换）
  * 3、缓存满之后：无法写入新的数据，必须读取缓存中的数据或者复位该实例
  * ------------------------------------------------------------
  * 上层低层调用函数
  * GetRingbuffFreeSize();GetRingbuffStatus();
  * ------------------------------------------------------------
  * 上层调用函数：
  * GetRingbuffUseSize();resetRingBuff();
  * readOneMemberFromRingbuff()
  * CalRingbuffUsedPercent();
  * 低层调用函数：
  * RingbuffInit();writeOneMemberToRingbuff();writeNMemberToRingbuff();
  * ------------------------------------------------------------
*/
#include "includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/********************************************************************
函数功能：初始化环形缓存实例
入口参数：Ringbuff缓存管理实例，buff缓存地址，BuffByteSize缓存大小,MemberSize缓存成员大小 
clearFlag清零标志（读取数据之后是否清零缓存）
返    回：无。
备    注：无。
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
函数功能：获取环形缓存状态
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
u8 getRingbuffStatus(RingbuffMgr_t *Ringbuff)
{
	return Ringbuff->status;
}
/********************************************************************
函数功能：获取环形缓存数据大小
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
u16 getRingbuffUseSize(RingbuffMgr_t *Ringbuff)
{
	return Ringbuff->usedBytes;
}
/********************************************************************
函数功能：获取环形缓存剩余空间大小
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
u16 getRingbuffFreeSize(RingbuffMgr_t *Ringbuff)
{
	return Ringbuff->freeBytesSize;
}
/********************************************************************
函数功能：设置读取缓存数据之后是否清零缓存数据的标志
入口参数：flag=TRUE时读取数据立即清零对应缓存，否则不清零
返    回：无。
备    注：无。
********************************************************************/
void setRingbuffClearFlag(RingbuffMgr_t *Ringbuff,u8 flag)
{
	Ringbuff->clearEnableFlag=flag;
}
/********************************************************************
函数功能：获取环形缓存剩余空间大小
入口参数：无。
返    回：无。
备    注：无。
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
函数功能：从环形缓存读取一个字节
入口参数：Ringbuff缓存管理实例,*data读出数据保存地址
备    注：无。
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
函数功能：写一个数据成员到环形缓存
入口参数：Ringbuff缓存管理实例，data写入数据
返    回：无。
备    注：无。
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
函数功能：写N个成员到环形缓存
入口参数：Ringbuff缓存管理实例，pbuff待写入数据指针，writeSize待写数据大小
返    回：无。
备    注：无。
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
函数功能：计算缓存使用百分比值
入口参数：Ringbuff缓存管理实例
返    回：百分比值（0---100）。
备    注：无。
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
函数功能：打印缓存管理实例相关变量及缓存数据
入口参数：Ringbuff缓存管理实例，printDataFlag=TRUE时打印出缓存所有数据，每16字节一行，并标记行号
返    回：无。
备    注：无。
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
函数功能：
入口参数：无。
返    回：无。
备    注：无。
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
	printf(" ------------------ 流水缓存测试 1------------------\r\n");
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
	printf(" ------------------ 流水缓存测试 2------------------\r\n");
}
#endif
//---------------------------------------------------------------------







