#ifndef __STREAM__
#define __STREAM__

#include <string>
#include <vector>

class Stream
{
	private:
		std::vector<char> myData;
		std::size_t myReadPos;
		bool myIsValid;

		bool CheckSize(std::size_t Size);

	public:
		Stream();

		virtual ~Stream();

		void Append(const void* Data, std::size_t SizeInBytes);
		void Clear();

		const char* GetData() const;
		std::size_t GetDataSize() const;
		bool EndOfPacket() const;

		operator bool() const;

		// Data reading operators
		Stream& operator >>(bool& Data);
		Stream& operator >>(char& Data);
		Stream& operator >>(short& Data);
		Stream& operator >>(unsigned short& Data);
		Stream& operator >>(int& Data);
		Stream& operator >>(unsigned int& Data);
		Stream& operator >>(long& Data);
		Stream& operator >>(unsigned long& Data);
		Stream& operator >>(float& Data);
		Stream& operator >>(double& Data);
		Stream& operator >>(char* Data);
		Stream& operator >>(std::string& Data);
		Stream& operator >>(wchar_t* Data);
		Stream& operator >>(std::wstring& Data);

		// Data writing operators
		Stream& operator <<(bool Data);
		Stream& operator <<(char Data);
		Stream& operator <<(short Data);
		Stream& operator <<(unsigned short Data);
		Stream& operator <<(unsigned int Data);
		Stream& operator <<(int Data);
		Stream& operator <<(unsigned long Data);
		Stream& operator <<(long Data);
		Stream& operator <<(float Data);
		Stream& operator <<(double Data);
		Stream& operator <<(const char* Data);
		Stream& operator <<(const std::string& Data);
		Stream& operator <<(const wchar_t* Data);
		Stream& operator <<(const std::wstring&	Data);
};

#endif