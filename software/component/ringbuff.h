/**
  ******************************************************************************
  * @file    ringbuff.h 
  * @author  alan
  * @version V1.2.0
  * @date    
  * @brief   ringbuff manage module
  ******************************************************************************
  * @attention
  ******************************************************************************
*/
#ifndef RINGBUFF_H
#define RINGBUFF_H

#include "includes.h"

#define RING_BUFF_WATERMARK	0xAA	//缓存水印

typedef enum
{
	RingbuffEmpty=0,		//缓存空
	RingbuffFull,			//缓存满
	RingbuffNotFull			//缓存非满
}RingbuffStatus_t;

typedef enum
{
	RingbuffOpSucess=0,		//缓存操作成功----读取写入缓存
	RingbuffOpFailure,		//缓存操作失败----读取写入缓存
	RingbuffOpNotInit
}RingbuffOperateResult_t;

typedef struct
{
	u8 UsePercent;			//缓存时候百分比
	u8 MemberSize;			//缓存成员大小：1字节、2字节、4字节
	u8 clearEnableFlag;		//读取缓存时此值为TRUE则清零缓存/
	void *pBuff;			//指向缓存的指针
	u8 status;				//缓存状态
	u16 inBuffPointer;		//缓存操作指针
	u16 OutBuffPointer;		//缓存操作指针
	u16 usedBytes;			//缓存计数器：已使用	
	u16 freeBytesSize;		//缓存计数器：剩余
	u16 totalByteSize;		//缓存计数器：总大小
}RingbuffMgr_t;

// -------------------------------------- 上层低层调用函数 --------------------------------------
u8 getRingbuffStatus(RingbuffMgr_t *Ringbuff);
u16 getRingbuffUseSize(RingbuffMgr_t *Ringbuff);
u16 getRingbuffFreeSize(RingbuffMgr_t *Ringbuff);
void setRingbuffClearFlag(RingbuffMgr_t *Ringbuff,u8 flag);
// -------------------------------------- 上层调用函数 --------------------------------------
void ringbuffInit(RingbuffMgr_t *Ringbuff,void *buff,u16 BuffByteSize,u8 MemberSize,u8 clearFlag);
void resetRingBuff(RingbuffMgr_t *Ringbuff);
RingbuffOperateResult_t readOneMemberFromRingbuff(RingbuffMgr_t *Ringbuff,void *data);
// -------------------------------------- 底层调用函数 --------------------------------------
RingbuffOperateResult_t writeOneMemberToRingbuff(RingbuffMgr_t *Ringbuff,void *data);
RingbuffOperateResult_t writeNMemberToRingbuff(RingbuffMgr_t *Ringbuff,void *pbuff,u16 WriteByteSize);
u8 CalRingbuffUsedPercent(RingbuffMgr_t *Ringbuff);
// -------------------------------------- 辅助测试功能 --------------------------------------

void printRingBuff(RingbuffMgr_t *Ringbuff,u8 printDataFlag);
void ringbuffTest(void);


#endif 



