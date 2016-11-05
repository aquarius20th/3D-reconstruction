#include"stdafx.h"
#include"MyMath.h"
#include<iostream>
using namespace std;



/***********************************************************************************************/
//名字：log2（）
//功能：以2为底的对数
/*参数：n:求n的以2为底的对数
/***********************************************************************************************/
int log2(unsigned long n)
{
	switch(n)
	{
	case 1:return 0;
	case 2:return 1;
	case 4:return 2;
	case 8:return 3;
	case 16:return 4;
	case 32:return 5;
	case 64:return 6;
	case 128:return 7;
	case 256:return 8;
	case 512:return 9;
	case 1024:return 10;
	case 2048:return 11;
	case 4096:return 12;
	case 8192:return 13;
	case 16384:return 14;
	case 32768:return 15;
	case 65536:return 16;
	case 131072:return 17;
    case 262144:return 18;
	case 524288:return 19;
	case 1048576:return 20;
	case 2097152:return 21;
	case 4194304:return 22;
	case 8388608:return 23;
	case 16777216:return 24;
	case 33554432:return 25;
	case 67108864:return 26;
	case 134217728:return 27;
	case 268435456:return 28;
	case 536870912:return 29;
	case 1073741824:return 30;
	case 2147483648:return 31;
	default:
		{
			cout<<"Overfull!"<<endl;
			exit(1);
		}
	}
}
/***********************************************************************************************/





/***********************************************************************************************/
//名字：pow2（）
//功能：求2的n次幂
/*参数：n:2的幂指数
/***********************************************************************************************/
unsigned int pow2(unsigned int n)
{
	if(n>31)
	{
		cout<<"Overfull!"<<endl;
		exit(1);
	}
	return 1<<n;
/*	switch(n)
	{
	case 0:return 1;
	case 1:return 2;
	case 2:return 4;
	case 3:return 8;
	case 4:return 16;
	case 5:return 32;
	case 6:return 64;
	case 7:return 128;
	case 8:return 256;
	case 9:return 512;
	case 10:return 1024;
	case 11:return 2048;
	case 12:return 4096;
	case 13:return 8192;
	case 14:return 16384;
	case 15:return 32768;
	case 16:return 65536;
	case 17:return 131072;
    case 18:return 262144;
	case 19:return 524288;
	case 20:return 1048576;
	case 21:return 2097152;
	case 22:return 4194304;
	case 23:return 8388608;
	case 24:return 16777216;
	case 25:return 33554432;
	case 26:return 67108864;
	case 27:return 134217728;
	case 28:return 268435456;
	case 29:return 536870912;
	case 30:return 1073741824;
	case 31:return 2147483648;
	default:
		{
			cout<<"Overfull!"<<endl;
			exit(1);
		}
	}*/
}
/***********************************************************************************************/