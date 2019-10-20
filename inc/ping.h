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

//返回结果
typedef struct
{
	DWORD dwRoundTripTime;   //返回时间
	in_addr dwIPaddr;        //返回报文的IP地址
	int state;				//函数返回用的状态
	int iTTL;				//TTL初始值
	int size;				//字节数
}FUN_RESULT;

//IP报头
typedef struct
{
	unsigned char hdr_len : 4;         //4位头部长度
	unsigned char version : 4;         //4位版本号
	unsigned char tos;               //8位服务类型
	unsigned short total_len;        //16位总长度
	unsigned short identifier;       //16位标识符
	unsigned short frag_and_flags;   //3位标志加13位片偏移
	unsigned char ttl;               //8位生存时间
	unsigned char protocol;          //8位上层协议号
	unsigned short checksum;         //16位效验和
	unsigned long sourceIP;          //32位源IP地址
	unsigned long destIP;            //32位目的IP地址
}IP_HEADER;

//ICMP报头
typedef struct
{
	BYTE type;     //8位类型字段
	BYTE code;     //8位代码字段
	USHORT cksum;  //16位效验和
	USHORT id;     //16位标识符
	USHORT seq;    //16位序列号
}ICMP_HEADER;


//报文解码结构
typedef struct
{
	USHORT usSeqNo;          //序列号
	DWORD dwRoundTripTime;   //返回时间
	in_addr dwIPaddr;        //返回报文的IP地址
	int state;				//函数返回用的状态
}DECODE_RESULT;

//计算网际效验和函数
USHORT checksum(USHORT* pBuf, int iSize);

//对数据包进行解码
BOOL DecodeIcmpResponse(char* pBuf, int iPacketSize, DECODE_RESULT& DecodeResult, BYTE ICMP_ECHO_REPLY, BYTE ICMP_TIMEOUT);

FUN_RESULT Ping(const char* IpAddress);
