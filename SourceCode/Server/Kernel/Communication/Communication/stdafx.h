// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

/*
Note
The Iphlpapi.h header file is required if an application is using the IP Helper APIs.When the Iphlpapi.h header file is required, the #include line for the Winsock2.h header this file should be placed before the #include line for the Iphlpapi.h header file.
The Winsock2.h header file internally includes core elements from the Windows.h header file, so there is not usually an #include line for the Windows.h header file in Winsock applications.If an #include line is needed for the Windows.h header file, this should be preceded with the #define WIN32_LEAN_AND_MEAN macro.For historical reasons, the Windows.h header defaults to including the Winsock.h header file for Windows Sockets 1.1.The declarations in the Winsock.h header file will conflict with the declarations in the Winsock2.h header file required by Windows Sockets 2.0.The WIN32_LEAN_AND_MEAN macro prevents the Winsock.h from being included by the Windows.h header.An example illustrating this is shown below.
*/
//防止Windows.h中Winsock1和我们使用的winsock2冲突
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif // !WIN32_LEAN_AND_MEAN

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <mswsock.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

// TODO: 在此处引用程序需要的其他头文件
//框架头文件
#include "SharedDefine.h"
#include "ITrace.h"
#include "ICommunication.h"
#include "singleton.h"
#include "IProactor.h"
#include "IAppInfo.h"

//本地头文件
#include "SharedRes.h"