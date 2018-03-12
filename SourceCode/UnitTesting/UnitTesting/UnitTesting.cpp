/************************************************************************
* Copyright(c) 2017-2037 做梦公司
* All rights reserved.
*
* license
*
* FileName: UnitTesting.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2017/10/20
* Description:单元测试类，用于测试整个工程所有功能及稳定性和效率等。

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/20			1.0		  build this module
************************************************************************/
#include "stdafx.h"
#include <iostream>
#include <thread>
#include <vector>
#include "UnitTesting.h"

//函数表
CUnitTesting::ExecFunction	CUnitTesting::m_FuncMap[UnitTestingItem::kUTMax] =
{
	&CUnitTesting::MinTest,
	&CUnitTesting::UnitTestMacros,	// UTMacros,	//宏命名测试
	&CUnitTesting::UnitTestThreadReadWrite, //UTThreadReadWrite,	//一个线程写，一个线程读
	&CUnitTesting::VirtualFunctionPtr, 
	&CUnitTesting::CharToWchar,
	&CUnitTesting::ReverseClass,
	&CUnitTesting::NewDel,
};

CUnitTesting::CUnitTesting()
{
}

CUnitTesting::~CUnitTesting()
{
}

//执行单元测试
void CUnitTesting::UnitTest(UnitTestingItem item)
{
	if (item < UnitTestingItem::kUTMax)
	{
		if (nullptr != m_FuncMap[item])
		{
			(this->*m_FuncMap[item])();
		}
	}
}

void Out(char* s)
{
	std::cout << s << std::endl;
}

//宏测试
void CUnitTesting::UnitTestMacros()
{
//	LOG_ASSERT(false, "AAA");
//	LOG_ASSERT(false, nullptr);
	//assert(false);
	//Out(__FILE__ "||" __FUNCTION__ "||" __LINE__ "||" __DATE__ "||" __TIME__);

	//std::cout << __FILE__ "||" __TIME__<< std::endl;
	//std::cout << __FILE__ "||"  __FUNCTION__ "||" << __LINE__ << "||" __DATE__ "||" __TIME__ << std::endl;

	//std::wcout << __FILEW__ TEXT("||")  __FUNCTIONW__ TEXT("||") << __LINE__ << /*TEXT("||") __DATEW__ TEXT("||") __TIME__ <<*/ std::endl;
	std::cout << __FILEW__ << std::endl;
	std::wcout << __FILEW__ << std::endl;
}

struct ThreadData
{
	bool run;
	int datalen;
	int data[32];
};
SRWLOCK m_SRWLock;

int ThreadFuncRead(void *pParam)
{
	ThreadData *data = (ThreadData*)pParam;
	while (data->run)
	{
		//::AcquireSRWLockShared(&m_SRWLock);
		std::cout << "read data len:" << data->datalen<<" data:";
		for (int i = 0; i < data->datalen; ++i)
		{
			std::cout << " " << data->data[i];
		}
		std::cout << std::endl;
		
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		//::ReleaseSRWLockShared(&m_SRWLock);

		//std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
	std::cout << "Read exit" << std::endl;
	return 0;
}

int ThreadFuncWrite(void *pParam)
{
	ThreadData *data = (ThreadData*)pParam;
	while (data->run)
	{
		//::AcquireSRWLockExclusive(&m_SRWLock);
	
		std::cout << "write data len:" << data->datalen << std::endl;

		if (data->datalen < array_size(data->data))
		{
			data->datalen++;
		}
		else
		{
			data->datalen = 0;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		//::ReleaseSRWLockExclusive(&m_SRWLock);

		//std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
	std::cout << "Write exit" << std::endl;
	return 0;
}

//一个线程写，一个线程读
void CUnitTesting::UnitTestThreadReadWrite()
{
	ThreadData data;
	data.run = true;
	data.datalen = 0;
	for (int i = 0; i < array_size(data.data); ++i)
	{
		data.data[i] = i;
	}
	::InitializeSRWLock(&m_SRWLock);

	std::thread thread_read(ThreadFuncRead, &data);
	std::thread thread_write(ThreadFuncWrite, &data);

	std::this_thread::sleep_for(std::chrono::seconds(5));
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

	data.run = false;

	thread_read.join();
	thread_write.join();

	std::cout << "main data len:" << data.datalen << " data:";
	for (int i = 0; i < data.datalen; ++i)
	{
		std::cout << " " << data.data[i];
	}
	std::cout << std::endl;

	std::cout << "main exit" << std::endl;
}


class father
{
public:
	virtual void func() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual int funcn() = 0;
	virtual int funcn1() = 0;
	virtual int funcn2() = 0;

	virtual ~father() {}
};

class child : public father
{
public:
	child() 
	{
		//这个语句会破坏类的虚函数表，傻逼才会写这样的代码来快速初始化数据
		//memset(this, 0, sizeof(*this));
	}
	~child() {}

public:
	void func() {}
	void func1() {}
	void func2() {}
	int funcn() { return 1; }
	int funcn1() { return 1; }
	int funcn2() { return 1; }

private:
	
};

//虚函数表
void CUnitTesting::VirtualFunctionPtr()
{
	father* pFather = new child();

	//这个语句会破坏类的虚函数表，傻逼才会写这样的代码来快速初始化数据
	//memset(pFather, 0, sizeof(*pFather));

	delete pFather;
}

void CUnitTesting::CharToWchar()
{
	char cstr[] = "123456789abcd";
	wchar ws[] = L"你好";

	wchar wstr[256] = L"";

	//"%hs" 用于转换char; %ls 用于转换 wchar
	wsprintf(wstr, L"%hs   %ls", cstr, ws);
	
	std::wcout << wstr << std::endl;
}

struct Note1
{
	int a;
	int b;
	float c;
	wchar ws[25];

};

struct Note2 : public Note1
{
	int d;
	virtual void HeiMan() = 0;
};

struct NoteEx
{
	char exa;
	double exdd;
};

struct Note3 : public Note2, public NoteEx
{
	int e;

	virtual void HeiMan() override
	{
		e = 9;
	}

	~Note3()
	{
		std::cout << "des note2" << std::endl;
	}
};


void ModNote1(Note1& note)
{
	note.a = 1;
	note.b = 2;
	note.c = 3.125f;
	wcscpy_s(note.ws, L"Mod修改了");
}

void ModNoteEx(NoteEx& ex)
{
	ex.exa = 'a';
	ex.exdd = 78.95;
}

//将父子类逆转换更改数据看虚函数表是否受损
void CUnitTesting::ReverseClass()
{
	Note3 note;
	//memset(&note, 0, sizeof(note));
	ModNoteEx(note);
	ModNote1(note);
	std::cout << note.d << note.ws << std::endl;
}

void CUnitTesting::NewDel()
{
	std::cout << sizeof(Note3) * 10 << std::endl;
	Note3* pNote = new Note3[10];

	//delete (pNote + 5);
	delete pNote;
	//delete[] pNote;
}

void CUnitTesting::MinTest()
{
	//std::vector<Note3> ns(100);
	//std::vector<int> ns(100);

	//for (auto itr = ns.begin(); itr != ns.end(); itr++)
	//{
	//	*itr = 5;
	//}

	size_t a = -1;

	int b = a;
}