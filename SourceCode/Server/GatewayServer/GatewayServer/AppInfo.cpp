#include "AppInfo.h"



CAppInfo::CAppInfo()
{
}


CAppInfo::~CAppInfo()
{
}

//�汾��Ϣ
const AppVersion& CAppInfo::Version()
{
	return m_AppVersion;
}

//����������л���
DevelopmentEnvironment CAppInfo::Environment()
{
	return DevelopmentEnvironment::kDevelopment;
}

//�����Ϣ
uint CAppInfo::GetComponents(IComponent* pComponents[], uint nSize)
{
	return 0;
}
