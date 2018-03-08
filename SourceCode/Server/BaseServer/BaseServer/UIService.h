/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: UIService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/21
* Description:服务器界面服务

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/21		   1.0		  build this module
lihl		2018/3/5		   1.0		  重写服务，添加UI命令
************************************************************************/
#pragma once

#include <unordered_map>

enum UICommand
{
	kStart,			//开始
	kShutdown,		//关闭服务器
	kUpdateConfig,	//更新配置
	kHelp,			//帮助
	kInvalid,		//无效命令	
	//...			//随时添加
};

//命令内容
struct UICmdData
{
	UICommand cmd;		//命令
	schar param[256];	//自定义参数
};

class CUIService : public ITraceView
{
public:
	CUIService();
	~CUIService();

	//继承 ITraceView
public:
	//窗口输出
	void PrintOnView(const wchar* wsMsg, ulong color = 0) override;

	//服务控制
public:
	//开始
	bool Start();
	//结束
	void Shutdown();
	//可服务状态
	bool Servable() const { return m_bRunning; }

	//功能函数
public:
	//画布GUI帧刷新
	//命令无效则返回false
	bool CanvasUpdate(UICmdData &Cmd);
	//追加显示内容
	bool AppendMsg(const wchar* wsMsg);
	//显示帮助
	void ShowHelp();

	//内部函数
protected:

	//成员变量
private:
	//运行状态
	bool m_bRunning;
	//命令库
	const std::unordered_map<std::string, UICommand> m_CmdLibraries;
};

