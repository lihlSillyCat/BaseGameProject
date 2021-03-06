// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "KernelExport.h"
#include "Trace.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

//创建Trace对象，日志服务
extern "C" __declspec(dllexport) 
ITraceService* CreateServiceObject(const wchar* wsModuleGUID)
{
	if (wcscmp(wsModuleGUID, kGUID_KERNEL[KernelModule::kTrace]) != 0)
	{
		return nullptr;
	}

	CTrace* pService = new CTrace();

	return pService;
}
