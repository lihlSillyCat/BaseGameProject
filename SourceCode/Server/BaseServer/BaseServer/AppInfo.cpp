#include "AppInfo.h"



CAppInfo::CAppInfo()
{
}


CAppInfo::~CAppInfo()
{
}

//版本信息
const AppVersion& CAppInfo::Version()
{
	return m_AppVersion;
}

//软件开发运行环境
DevelopmentEnvironment CAppInfo::Environment()
{
	return DevelopmentEnvironment::kDevelopment;
}

//组件信息
uint CAppInfo::GetComponents(IComponent* pComponents[], uint nSize)
{
	return 0;
}
