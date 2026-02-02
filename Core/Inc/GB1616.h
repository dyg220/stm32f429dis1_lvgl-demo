// ------------------  汉字字模的数据结构定义 ------------------------ //
#ifndef _GB1616_H_
#define _GB1616_H_

struct typFNT_GB16 // 汉字字模数据结构
{
    unsigned char Index[2]; // 汉字内码索引
    unsigned char Msk[32];  // 点阵码数据
};

extern const struct typFNT_GB16 codeGB_16[];

#endif
		 