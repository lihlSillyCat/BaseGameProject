#pragma once
#include "IAppInfo.h"
class CAppInfo : public IAppInfo
{
public:
	CAppInfo();
	~CAppInfo();

	//继承 IAppInfo
public:
	//版本信息
	const AppVersion& Version() override;

	//软件开发运行环境
	DevelopmentEnvironment Environment() override;

	//组件信息
	uint GetComponents(IComponent* pComponents[], uint nSize) override;

protected:
	AppVersion m_AppVersion;
};

