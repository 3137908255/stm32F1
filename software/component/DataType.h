/**
  ******************************************************************************
  * @file    DataType.h 
  * @author  ��С�
  * @version V1.0.0
  * @date    2013.10.01
  * @brief   DataType Define module
  ******************************************************************************
  * @attention
  Copyright (c) 2013 --- 2019 ��С�. All rights reserved.
  ������
  1����ģ�����ߣ���Сᯣ���������Ȩ��
  2����ģ������ο�ѧϰ���о�������ʹ��
  3����������ҵĿ�ģ���Ҫ��������ͬ��
  4�����߽��ṩԴ���룬���Դӡ�github.com�������أ��������ڴ�����
  5���Դ�ģ���ʹ����������֤�����ԡ�
  6������ʹ�ñ�ģ����ɵ��κ���ʧ�����ߣ���Сᯣ����е��κ����Ρ�
  7���罫��ģ�鼯������Ĵ����У����뱣��������
  8���粻ͬ����������벻Ҫʹ�ô�ģ��
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



