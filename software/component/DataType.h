/**
  ******************************************************************************
  * @file    DataType.h 
  * @author  西小岑
  * @version V1.0.0
  * @date    2013.10.01
  * @brief   DataType Define module
  ******************************************************************************
  * @attention
  Copyright (c) 2013 --- 2019 西小岑. All rights reserved.
  声明：
  1、此模块作者（西小岑）保留所有权限
  2、此模块仅供参考学习、研究、交流使用
  3、如用于商业目的，需要征求作者同意
  4、作者仅提供源代码，可以从“github.com”上下载，但不限于此渠道
  5、对此模块的使用请自行验证、测试、
  6、对于使用本模块造成的任何损失，作者（西小岑）不承担任何责任。
  7、如将此模块集成于你的代码中，必须保留此声明
  8、如不同意上述条款，请不要使用此模块
  ******************************************************************************
**/
#ifndef DATATYPE_H
#define DATATYPE_H


#define OSTASK								


#define PI2 								(6.28318530717959)
#define PI 									(3.1415926)

#define YES									1
#define NO									0
#define NEW_SUCCESS							1
#define NEW_FAILURE							0


#define PIN_STATUS_LOW						0
#define PIN_STATUS_HIGH						1


#define MySetBit(A,BITS)					( A |=  (1<<BITS))
#define MyClearBit(A,BITS)					( A &= ~(1<<BITS))
#define MyCheckBit(A,BITS)					(( 1&(A>>BITS))>0?1:0)
#define Max(a,b)							((a>b?a:b))
#define Min(a,b)							((b>a?a:b))

//#define TRUE					1
//#define FALSE					0

    /* exact-width signed integer types */
typedef   signed          char I8;
typedef   signed short     int I16;
typedef   signed           int I32;
typedef   signed       __int64 I64;

    /* exact-width unsigned integer types */
typedef unsigned          char U8;
typedef unsigned short     int U16;
typedef unsigned           int U32;
typedef unsigned       __int64 U64;

typedef   signed          char i8;
typedef   signed short     int i16;
typedef   signed           int i32;
typedef   signed       __int64 i64;

    /* exact-width unsigned integer types */
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;
typedef unsigned       __int64 u64;



#endif 



