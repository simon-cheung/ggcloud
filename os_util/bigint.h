#pragma once

namespace oo
{
    struct RandomGen
    {
        static boost::mt19937 boostrnd;

        static uint32 get32Random(uint32 nmin = 0, uint32 nmax = 0xffffffff);
        static uint16 get16Random(uint16 nmin = 0, uint16 nmax = 0xffff);
        static uint8 get8Random();
    };
    class UINT128
    {
    public:
        UINT128();
        UINT128(bool bFill);
        UINT128(unsigned int uValue);
        UINT128(const unsigned char *pbyValueBE);
        //Generates a new number, copying the most significant 'numBits' bits from 'value'.
        //The remaining bits are randomly generated.
        UINT128(const UINT128 &uValue, unsigned int uNumBits = 128);

        const unsigned char* GetData() const;
        unsigned char* GetDataPtr() const;
        /** Bit at level 0 being most significant. */
        unsigned int GetBitNumber(unsigned int uBit) const;
        int CompareTo(const UINT128 &uOther) const;
        int CompareTo(unsigned int uValue) const;

        void ToHexString(std::string& str) const;
        std::string ToHexString() const;

        void ToBinaryString(std::string& str, bool bTrim = false) const;
        void ToByteArray(unsigned char *pby) const;

        unsigned int Get32BitChunk(int iVal) const;
        UINT128& SetValue(const UINT128 &uValue);
        UINT128& SetValue(unsigned int uValue);
        UINT128& SetValueBE(const unsigned char *pbyValueBE);
        UINT128& SetValueRandom();
        UINT128& SetBitNumber(unsigned int uBit, unsigned int uValue);
        UINT128& ShiftLeft(unsigned int uBits);
        UINT128& Add(const UINT128 &uValue);
        UINT128& Add(unsigned int uValue);
        //UINT128& Div(unsigned int uValue);
        UINT128& Subtract(const UINT128 &uValue);
        UINT128& Subtract(unsigned int uValue);
        UINT128& Xor(const UINT128 &uValue);
        UINT128& XorBE(const unsigned char *pbyValueBE);
        void operator+  (const UINT128 &uValue);
        void operator-  (const UINT128 &uValue);
        void operator=  (const UINT128 &uValue);
        bool operator<  (const UINT128 &uValue) const;
        bool operator>  (const UINT128 &uValue) const;
        bool operator<= (const UINT128 &uValue) const;
        bool operator>= (const UINT128 &uValue) const;
        bool operator== (const UINT128 &uValue) const;
        bool operator!= (const UINT128 &uValue) const;
        void operator+  (unsigned int uValue);
        void operator-  (unsigned int uValue);
        void operator=  (unsigned int uValue);
        bool operator<  (unsigned int uValue) const;
        bool operator>  (unsigned int uValue) const;
        bool operator<= (unsigned int uValue) const;
        bool operator>= (unsigned int uValue) const;
        bool operator== (unsigned int uValue) const;
        bool operator!= (unsigned int uValue) const;
    private:
        unsigned int		m_uData[4];
    };
}