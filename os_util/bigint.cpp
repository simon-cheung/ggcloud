#include "ospch.h"
#include "bigint.h"

using namespace oo;

UINT128::UINT128()
{
	SetValue((unsigned int)0);
}

UINT128::UINT128(bool bFill)
{
	if( bFill )
	{
		m_uData[0] = (unsigned int)-1;
		m_uData[1] = (unsigned int)-1;
		m_uData[2] = (unsigned int)-1;
		m_uData[3] = (unsigned int)-1;
	}
	else
		SetValue((unsigned int)0);
}

UINT128::UINT128(unsigned int uValue)
{
	SetValue(uValue);
}

UINT128::UINT128(const unsigned char *pbyValueBE)
{
	SetValueBE(pbyValueBE);
}

UINT128::UINT128(const UINT128 &uValue, unsigned int uNumBits)
{
	// Copy the whole ULONGs
	unsigned int uNumULONGs = uNumBits / 32;
	for (unsigned int iIndex=0; iIndex<uNumULONGs; iIndex++)
		m_uData[iIndex] = uValue.m_uData[iIndex];

	// Copy the remaining bits
	for (unsigned int iIndex=(32*uNumULONGs); iIndex<uNumBits; iIndex++)
		SetBitNumber(iIndex, uValue.GetBitNumber(iIndex));

	// Pad with random bytes (Not seeding based on time to allow multiple different ones to be created in quick succession)
	for (unsigned int iIndex=uNumBits; iIndex<128; iIndex++)
		SetBitNumber(iIndex, (rand()%2));
}

UINT128& UINT128::SetValue(const UINT128 &uValue)
{
	m_uData[0] = uValue.m_uData[0];
	m_uData[1] = uValue.m_uData[1];
	m_uData[2] = uValue.m_uData[2];
	m_uData[3] = uValue.m_uData[3];
	return *this;
}

UINT128& UINT128::SetValue(unsigned int uValue)
{
	m_uData[0] = 0;
	m_uData[1] = 0;
	m_uData[2] = 0;
	m_uData[3] = uValue;
	return *this;
}

UINT128& UINT128::SetValueBE(const unsigned char *pbyValueBE)
{
	SetValue((unsigned int)0);
	for (int iIndex=0; iIndex<16; iIndex++)
		m_uData[iIndex/4] |= ((unsigned int)pbyValueBE[iIndex]) << (8*(3-(iIndex%4)));
	return *this;
}

boost::mt19937 RandomGen::boostrnd(time(NULL));

uint16 RandomGen::get16Random(uint16 nmin, uint16 nmax)
{
    boost::uniform_int<unsigned short> any(nmin, nmax);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<unsigned short> > gen(boostrnd, any);	

    return gen();
}

uint32 RandomGen::get32Random(uint32 nmin, uint32 nmax)
{
    boost::uniform_int<unsigned int> any(nmin, nmax);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<unsigned int> > gen(boostrnd, any);	

    return gen();
}

uint8 RandomGen::get8Random()
{
    boost::uniform_int<unsigned char> any(0, 0xff);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<unsigned char> > gen(boostrnd, any);	

    return gen();
}

UINT128& UINT128::SetValueRandom()
{
    m_uData[0] = RandomGen::get32Random();
    m_uData[1] = RandomGen::get32Random();
    m_uData[2] = RandomGen::get32Random();
    m_uData[3] = RandomGen::get32Random();

	return *this;
}


unsigned int UINT128::GetBitNumber(unsigned int uBit) const
{
	if (uBit > 127)
		return 0;
	int iLongNum = uBit / 32;
	int iShift = 31 - (uBit % 32);
	return ((m_uData[iLongNum] >> iShift) & 1);
}

UINT128& UINT128::SetBitNumber(unsigned int uBit, unsigned int uValue)
{
	int iLongNum = uBit / 32;
	int iShift = 31 - (uBit % 32);
	m_uData[iLongNum] |= (1 << iShift);
	if (uValue == 0)
		m_uData[iLongNum] ^= (1 << iShift);
	return *this;
}

UINT128& UINT128::Xor(const UINT128 &uValue)
{
	for (int iIndex=0; iIndex<4; iIndex++)
		m_uData[iIndex] ^= uValue.m_uData[iIndex];
	return *this;
}

UINT128& UINT128::XorBE(const unsigned char *pbyValueBE)
{
	return Xor(UINT128(pbyValueBE));
}

void UINT128::ToHexString(std::string& str) const
{
    str.clear();
    std::stringstream ss;
    ss.precision(8);

    for (int iIndex=0; iIndex < 4; iIndex++)
	{
        ss << std::hex << m_uData[iIndex];
	}
    str = ss.str();
}

std::string UINT128::ToHexString() const
{
    std::stringstream ss;
    ss.precision(8);

    for (int iIndex=0; iIndex < 4; iIndex++)
	{
        ss << std::hex << m_uData[iIndex];
	}
    return ss.str();	
}

void UINT128::ToBinaryString(std::string& str, bool bTrim) const
{
    std::stringstream ss;

	int iBit;
	for (int iIndex=0; iIndex<128; iIndex++)
	{
		iBit = GetBitNumber(iIndex);
		if ((!bTrim) || (iBit != 0))
		{
            ss << iBit;
			bTrim = false;
		}
	}
    str = ss.str();
    if(str.empty())
        str = "0";
}

#if defined(_M_IX86) && (_MSC_FULL_VER > 13009037)
#pragma intrinsic(_byteswap_ulong)
#endif
void UINT128::ToByteArray(unsigned char *pby) const
{
#if defined(_M_IX86) && (_MSC_FULL_VER > 13009037)
	((unsigned int*)pby)[0] = _byteswap_ulong(m_uData[0]);
	((unsigned int*)pby)[1] = _byteswap_ulong(m_uData[1]);
	((unsigned int*)pby)[2] = _byteswap_ulong(m_uData[2]);
	((unsigned int*)pby)[3] = _byteswap_ulong(m_uData[3]);
#else

	for (int iIndex=0; iIndex<16; iIndex++)
		pby[iIndex] = (unsigned char)(m_uData[iIndex/4] >> (8*(3-(iIndex%4))));
#endif
}

int UINT128::CompareTo(const UINT128 &uOther) const
{
	for (int iIndex=0; iIndex<4; iIndex++)
	{
		if (m_uData[iIndex] < uOther.m_uData[iIndex])
			return -1;
		if (m_uData[iIndex] > uOther.m_uData[iIndex])
			return 1;
	}
	return 0;
}

int UINT128::CompareTo(unsigned int uValue) const
{
	if ((m_uData[0] > 0) || (m_uData[1] > 0) || (m_uData[2] > 0) || (m_uData[3] > uValue))
		return 1;
	if (m_uData[3] < uValue)
		return -1;
	return 0;
}

UINT128& UINT128::Add(const UINT128 &uValue)
{
	if (uValue == 0)
		return *this;
	int64 iSum = 0;
	for (int iIndex=3; iIndex>=0; iIndex--)
	{
		iSum += m_uData[iIndex];
		iSum += uValue.m_uData[iIndex];
		m_uData[iIndex] = (unsigned int)iSum;
		iSum = iSum >> 32;
	}
	return *this;
}

UINT128& UINT128::Add(unsigned int uValue)
{
	if (uValue == 0)
		return *this;
	Add(UINT128(uValue));
	return *this;
}

UINT128& UINT128::Subtract(const UINT128 &uValue)
{
	if (uValue == 0)
		return *this;
	int64 iSum = 0;
	for (int iIndex=3; iIndex>=0; iIndex--)
	{
		iSum += m_uData[iIndex];
		iSum -= uValue.m_uData[iIndex];
		m_uData[iIndex] = (unsigned int)iSum;
		iSum = iSum >> 32;
	}
	return *this;
}

UINT128& UINT128::Subtract(unsigned int uValue)
{
	if (uValue == 0)
		return *this;
	Subtract(UINT128(uValue));
	return *this;
}

/* Untested
UINT128& UINT128::Div(unsigned int uValue)
{
	unsigned int uBit, uRemain = 0;
	for (i = 0; i < 128; i++)
	{
		uBit = GetBitNumber(0);
		uRemain <<= 1;
		if (uBit)
			uRemain |= 1;
		ShiftLeft(1);
		if (uRemain >= uValue)
		{
			uRemain -= uValue;
			SetBitNumber(127, 1);
		}
	}
	return *this;
}
*/

UINT128& UINT128::ShiftLeft(unsigned int uBits)
{
	if ((uBits == 0) || (CompareTo(0) == 0))
		return *this;
	if (uBits > 127)
	{
		SetValue((unsigned int)0);
		return *this;
	}

	unsigned int uResult[] = {0,0,0,0};
	int iIndexShift = (int)uBits / 32;
	int64 iShifted = 0;
	for (int iIndex=3; iIndex>=iIndexShift; iIndex--)
	{
		iShifted += ((int64)m_uData[iIndex]) << (uBits % 32);
		uResult[iIndex-iIndexShift] = (unsigned int)iShifted;
		iShifted = iShifted >> 32;
	}
	for (int iIndex=0; iIndex<4; iIndex++)
		m_uData[iIndex] = uResult[iIndex];
	return *this;
}

const unsigned char* UINT128::GetData() const
{
	return (unsigned char*)m_uData;
}

unsigned char* UINT128::GetDataPtr() const
{
	return (unsigned char*)m_uData;
}

unsigned int UINT128::Get32BitChunk(int iVal) const
{
	return m_uData[iVal];
}

void UINT128::operator+  (const UINT128 &uValue)
{
	Add(uValue);
}
void UINT128::operator-  (const UINT128 &uValue)
{
	Subtract(uValue);
}
void UINT128::operator=  (const UINT128 &uValue)
{
	SetValue(uValue);
}
bool UINT128::operator<  (const UINT128 &uValue) const
{
	return (CompareTo(uValue) <  0);
}
bool UINT128::operator>  (const UINT128 &uValue) const
{
	return (CompareTo(uValue) >  0);
}
bool UINT128::operator<= (const UINT128 &uValue) const
{
	return (CompareTo(uValue) <= 0);
}
bool UINT128::operator>= (const UINT128 &uValue) const
{
	return (CompareTo(uValue) >= 0);
}
bool UINT128::operator== (const UINT128 &uValue) const
{
	return (CompareTo(uValue) == 0);
}
bool UINT128::operator!= (const UINT128 &uValue) const
{
	return (CompareTo(uValue) != 0);
}

void UINT128::operator+  (unsigned int uValue)
{
	Add(uValue);
}
void UINT128::operator-  (unsigned int uValue)
{
	Subtract(uValue);
}
void UINT128::operator=  (unsigned int uValue)
{
	SetValue(uValue);
}
bool UINT128::operator<  (unsigned int uValue) const
{
	return (CompareTo(uValue) <  0);
}
bool UINT128::operator>  (unsigned int uValue) const
{
	return (CompareTo(uValue) >  0);
}
bool UINT128::operator<= (unsigned int uValue) const
{
	return (CompareTo(uValue) <= 0);
}
bool UINT128::operator>= (unsigned int uValue) const
{
	return (CompareTo(uValue) >= 0);
}
bool UINT128::operator== (unsigned int uValue) const
{
	return (CompareTo(uValue) == 0);
}
bool UINT128::operator!= (unsigned int uValue) const
{
	return (CompareTo(uValue) != 0);
}

