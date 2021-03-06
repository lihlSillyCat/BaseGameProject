// UnitTesting.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "UnitTesting.h"

void EnableMemLeakCheck()
{
	int temFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	temFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(temFlag);
}

int main()
{
	EnableMemLeakCheck();
	//_CrtSetBreakAlloc(202);

	CUnitTesting  UnitTesting;

	int input = -1;
	while (true)
	{
		std::cout << "Input unit testing code[0," << UnitTestingItem::kUTMax - 1 << "](others for exit):";
		std::cin >> input;
		if (0 <= input && UnitTestingItem::kUTMax > input)
		{
			std::cout << "----------------------Unit testing start!----------------------" << std::endl;
			std::cout << "" << std::endl;

			UnitTesting.UnitTest(static_cast<UnitTestingItem>(input));

			std::cout << "" << std::endl;
			std::cout << "----------------------Unit testing finished!-------------------" << std::endl;
		}
		else
		{
			break;
		}
	}

    return 0;
}

