// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "SharedDefine.h"
#include "KernelExport.h"
#include "CommonService.h"

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

//导出公共服务
extern "C" __declspec(dllexport)
ICommonService* CreateServiceObject(const wchar* wsModuleGUID)
{
	if (wcscmp(wsModuleGUID, kGUID_KERNEL[KernelModule::kCommon]) != 0)
	{
		return nullptr;
	}

	CCommonService* pServer = new CCommonService();

	return pServer;
}
