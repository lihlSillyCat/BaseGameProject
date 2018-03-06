#pragma once
#include "IAppInfo.h"
class CAppInfo : public IAppInfo
{
public:
	CAppInfo();
	~CAppInfo();

	//�̳� IAppInfo
public:
	//�汾��Ϣ
	const AppVersion& Version() override;

	//����������л���
	DevelopmentEnvironment Environment() override;

	//�����Ϣ
	uint GetComponents(IComponent* pComponents[], uint nSize) override;

protected:
	AppVersion m_AppVersion;
};

