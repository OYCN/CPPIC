#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#define PSUCC 1
#define PERR 0
#define PERR_INPUT -1
#define PERR_TIMEOUT -2
#define PERR_OTHER -3

#define PING_TIMEMAX 1000

//���ؽ��
typedef struct
{
	DWORD dwRoundTripTime;   //����ʱ��
	in_addr dwIPaddr;        //���ر��ĵ�IP��ַ
	int state;				//���������õ�״̬
	int iTTL;				//TTL��ʼֵ
	int size;				//�ֽ���
}FUN_RESULT;

//IP��ͷ
typedef struct
{
	unsigned char hdr_len : 4;         //4λͷ������
	unsigned char version : 4;         //4λ�汾��
	unsigned char tos;               //8λ��������
	unsigned short total_len;        //16λ�ܳ���
	unsigned short identifier;       //16λ��ʶ��
	unsigned short frag_and_flags;   //3λ��־��13λƬƫ��
	unsigned char ttl;               //8λ����ʱ��
	unsigned char protocol;          //8λ�ϲ�Э���
	unsigned short checksum;         //16λЧ���
	unsigned long sourceIP;          //32λԴIP��ַ
	unsigned long destIP;            //32λĿ��IP��ַ
}IP_HEADER;

//ICMP��ͷ
typedef struct
{
	BYTE type;     //8λ�����ֶ�
	BYTE code;     //8λ�����ֶ�
	USHORT cksum;  //16λЧ���
	USHORT id;     //16λ��ʶ��
	USHORT seq;    //16λ���к�
}ICMP_HEADER;


//���Ľ���ṹ
typedef struct
{
	USHORT usSeqNo;          //���к�
	DWORD dwRoundTripTime;   //����ʱ��
	in_addr dwIPaddr;        //���ر��ĵ�IP��ַ
	int state;				//���������õ�״̬
}DECODE_RESULT;

//��������Ч��ͺ���
USHORT checksum(USHORT* pBuf, int iSize);

//�����ݰ����н���
BOOL DecodeIcmpResponse(char* pBuf, int iPacketSize, DECODE_RESULT& DecodeResult, BYTE ICMP_ECHO_REPLY, BYTE ICMP_TIMEOUT);

FUN_RESULT Ping(const char* IpAddress);
