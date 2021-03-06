// BaseServer.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BaseService.h"

//全局变量
CBaseService g_BaseService;	//基础服务

//开启内存检测
void EnableMemLeakCheck()
{
	int temFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	temFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(temFlag);
}

//控制台窗口消息回调
BOOL WINAPI RoutineHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
		break;
	case CTRL_BREAK_EVENT:
		break;
	case CTRL_CLOSE_EVENT:
		g_BaseService.Shutdown();
		break;
	case CTRL_LOGOFF_EVENT:
		break;
	case CTRL_SHUTDOWN_EVENT:
		break;
	default:
		break;
	}
	return TRUE;
}

int DelayExit()
{
	std::cout << "服务器已停止，输入任意键退出：";
	char e;
	std::cin >> e;
	return 0;
}

//程序入口
int main()
{
	EnableMemLeakCheck();
	//_CrtSetBreakAlloc(1753);

	//窗口消息捕获
	if (FALSE == SetConsoleCtrlHandler(RoutineHandler, TRUE))
	{
		return DelayExit();
	}

	if (!g_BaseService.Start())
	{
		g_BaseService.Shutdown();
		return DelayExit();
	}

	//服务器主工作线程循环运转
	while (g_BaseService.Servable())
	{
		g_BaseService.Update();
	}

	//关闭服务器
	g_BaseService.Shutdown();
	SetConsoleCtrlHandler(RoutineHandler, FALSE);

    return 0;
}

