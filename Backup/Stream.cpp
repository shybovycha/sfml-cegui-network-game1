#include "Stream.h"

#ifdef WIN32
	#include <winsock2.h>
	#pragma comment(lib, "Ws2_32.lib")
#else
	#include <arpa/inet.h>
#endif

bool Stream::CheckSize(std::size_t Size)
{
	myIsValid = myIsValid && ((myReadPos + Size) <= myData.size());

	return myIsValid;
}

Stream::Stream() : myReadPos(0), myIsValid(true)
{
}

Stream::~Stream()
{
}

void Stream::Append(const void* Data, std::size_t SizeInBytes)
{
	if (Data && SizeInBytes > 0)
	{
		std::size_t Start =	myData.size();
		myData.resize(Start + SizeInBytes);
		memcpy(&myData[Start], Data, SizeInBytes);
	}
}

void Stream::Clear()
{
	myData.clear();
	myReadPos = 0;
	myIsValid = true;
}

const char* Stream::GetData() const
{
	if (!myData.empty())
		return &myData[0]; else
			return 0;
}

std::size_t Stream::GetDataSize() const
{
	return myData.size();
}

bool Stream::EndOfPacket() const
{
	return !(myReadPos < myData.size());
}

Stream::operator bool() const
{
	return myIsValid;
}

// Data reading operators
Stream& Stream::operator >>(bool& Data)
{
	unsigned char Value = 0;

	if (*this >> Value)
		Data = (Value != 0);

	return *this;
}

Stream& Stream::operator >>(char& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = *reinterpret_cast<const char*>(GetData() + myReadPos);
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(short& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = *reinterpret_cast<const short*>(GetData() + myReadPos);
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(unsigned short& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = *reinterpret_cast<const unsigned short*>(GetData() + myReadPos);
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(int& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = ntohl(*reinterpret_cast<const int*>(GetData() + myReadPos));
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(unsigned int& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = ntohl(*reinterpret_cast<const unsigned int*>(GetData() + myReadPos));
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(long& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = ntohl(*reinterpret_cast<const long*>(GetData() + myReadPos));
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(unsigned long& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = ntohl(*reinterpret_cast<const unsigned long*>(GetData() + myReadPos));
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(float& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = *reinterpret_cast<const float*>(GetData() + myReadPos);
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(double& Data)
{
	if (CheckSize(sizeof(Data)))
	{
		Data = *reinterpret_cast<const double*>(GetData() + myReadPos);
		myReadPos += sizeof(Data);
	}

	return *this;
}

Stream& Stream::operator >>(char* Data)
{
	unsigned int Length = 0;

	*this >> Length;

	if ((Length > 0) && CheckSize(Length))
	{
		memcpy(Data, GetData() + myReadPos, Length);
		Data[Length] = '\0';

		myReadPos += Length;
	}

	return *this;
}

Stream& Stream::operator >>(std::string& Data)
{
	unsigned int Length = 0;

	*this >> Length;

	if ((Length > 0) && CheckSize(Length))
	{
		Data.assign(GetData() + myReadPos, Length);

		myReadPos += Length;
	}

	return *this;
}

Stream& Stream::operator >>(wchar_t* Data)
{
	unsigned int Length = 0;	

	*this >> Length;

	if ((Length > 0) && CheckSize(Length * sizeof(int)))
	{
		for (unsigned int i = 0; i < Length; ++i)
		{
			unsigned int c;

			*this >> c;

			Data[i] = static_cast<wchar_t>(c);
		}

		Data[Length] = L'\0';
	}

	return *this;
}

Stream& Stream::operator >>(std::wstring& Data)
{
	unsigned int Length = 0;	

	*this >> Length;

	Data.clear();

	if ((Length > 0) && CheckSize(Length * sizeof(int)))
	{
		for (unsigned int i = 0; i < Length; ++i)
		{
			unsigned int c;

			*this >> c;

			Data += static_cast<wchar_t>(c);
		}
	}

	return *this;
}

// Data writing Stream::operators
Stream& Stream::operator <<(bool Data)
{
	*this << static_cast<char>(Data);
	return *this;
}

Stream& Stream::operator <<(char Data)
{
	Append(&Data, sizeof(Data));
	return *this;
}

Stream& Stream::operator <<(short Data)
{
	Append(&Data, sizeof(Data));
	return *this;
}

Stream& Stream::operator <<(unsigned short Data)
{
	Append(&Data, sizeof(Data));
	return *this;
}

Stream& Stream::operator <<(unsigned int Data)
{
	unsigned int ToWrite = htonl(Data);
	Append(&ToWrite, sizeof(ToWrite));
	return *this;
}

Stream& Stream::operator <<(int Data)
{
	int ToWrite = htonl(Data);
	Append(&ToWrite, sizeof(ToWrite));
	return *this;
}

Stream& Stream::operator <<(unsigned long Data)
{
	unsigned long ToWrite = htonl(Data);
	Append(&ToWrite, sizeof(ToWrite));
	return *this;
}

Stream& Stream::operator <<(long Data)
{
	long ToWrite = htonl(Data);
	Append(&ToWrite, sizeof(ToWrite));
	return *this;
}

Stream& Stream::operator <<(float Data)
{
	Append(&Data, sizeof(Data));
	return *this;
}

Stream& Stream::operator <<(double Data)
{
	Append(&Data, sizeof(Data));
	return *this;
}

Stream& Stream::operator <<(const char* Data)
{
	unsigned int Length = 0;

	for (const char* tmp = Data; *tmp != '\0'; ++tmp)
		Length++;

	*this << Length;

	Append(Data, Length * sizeof(char));

	return *this;
}

Stream& Stream::operator <<(const std::string& Data)
{
	unsigned int Length = static_cast<unsigned int>(Data.length());

	*this << Length;

	if (Length > 0)
		Append(Data.c_str(), Length * sizeof(std::string::value_type));

	return *this;
}

Stream& Stream::operator <<(const wchar_t* Data)
{
	unsigned int Length = 0;

	for (const wchar_t* tmp = Data; *tmp != L'\0'; ++tmp)
		Length++;

	*this << Length;

	for (const wchar_t* tmp = Data; *tmp != L'\0'; ++tmp)
		*this << static_cast<int>(*tmp);

	return *this;
}

Stream& Stream::operator <<(const std::wstring&	Data)
{
	unsigned int Length = static_cast<unsigned int>(Data.size());
	*this << Length;

	if (Length > 0)
	{
		for (std::wstring::const_iterator c = Data.begin(); c != Data.end(); ++c)
			*this << static_cast<int>(*c);
	}

	return *this;
}