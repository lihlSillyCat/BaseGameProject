#pragma once

#include <vector>
#include "Proactor.h"

class CCommonService : public ICommonService
{
public:
	CCommonService();
	~CCommonService();

	//继承 ICommonService
public:
	//启动服务
	bool Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment) override;
	//停止服务
	void Shutdown() override;
	//服务状态
	bool Serviceable() override { return m_bRunning; }
	//释放资源
	//调用后不得再使用该对象，因为模块内部会将所有资源释放。
	void Release() override;

	//获取一个主动器
	//参数 nConcurrentThreads：并发线程数。不建议用户填写该参数。若无特殊要求则使用默认值
	IProactor* CreateProactor(sint nConcurrentThreads = -1) override;

protected:
	bool m_bRunning;	//运转状态
	std::vector<CProactor*> m_Proactors;
};

