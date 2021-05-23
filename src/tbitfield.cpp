/// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)
		throw ("NEGATIVE");
	BitLen = len;
	if (len % (sizeof(TELEM) * 8) == 0)
		MemLen = len / (sizeof(TELEM) * 8);
	else
		MemLen = (len / (sizeof(TELEM) * 8)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	if (pMem != NULL)
	{
		delete[]pMem;
		pMem = NULL;
	}
}

void TBitField::CheckIndex(const int value) const
{
	if (value > BitLen || value <= -1)
		throw ("INDEX_OUT_OF_RANGE");
}

int TBitField::GetMemIndex(const int n) const 
{
	CheckIndex(n);
		return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const 
{
	CheckIndex(n);
	return 1 << (n % (sizeof(TELEM) * 8));
}



int TBitField::GetLength(void) const 
{
	return BitLen;
}

void TBitField::SetBit(const int n) 
{
	CheckIndex(n);
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) 
{
	CheckIndex(n);
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const 
{
	CheckIndex(n);
	return (pMem[GetMemIndex(n)] & GetMemMask(n));
}



TBitField& TBitField::operator=(const TBitField& bf)
{
	if (pMem == NULL)
		throw "EROR";
	if (this != &bf)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const 
{

	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
				return 0;
		return 1;
	}
	return 0;
}

int TBitField::operator!=(const TBitField& bf) const 
{
	return (*this == bf) ^ 1;
}

TBitField TBitField::operator|(const TBitField& bf) 
{
	int btlen = BitLen;
	if (bf.BitLen > btlen)
		btlen = bf.BitLen;
	TBitField tmp(btlen);
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] = bf.pMem[i] | pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) 
{
	int btlen = BitLen;
	if (bf.BitLen > btlen)
		btlen = bf.BitLen;
	TBitField tmp(btlen);
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] = bf.pMem[i] & pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) 
{
	TBitField tmp(*this);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i))
			tmp.ClrBit(i);
		else
			tmp.SetBit(i);
	}
	return tmp;
}



istream& operator>>(istream& istr, TBitField& bf) 
{
	char sign;
	int i = 1;
	istr >> sign;
	while (((sign == '0') || (sign == '1')) && (i < bf.BitLen - 1))
	{
		if (sign == '0')
			bf.ClrBit(i);
		else
			bf.SetBit(i);
		istr >> sign;
		i++;
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) 
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i))
			ostr << 1;
		else
			ostr << 0;
		if ((i + 1) % (8 * sizeof(TELEM)) == 0)
			ostr << " ";
	}
	ostr << endl;
	return ostr;
}
