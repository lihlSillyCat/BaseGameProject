#include "stdafx.h"
#include "UIService.h"


CUIService::CUIService():
	m_bRunning(false),
	m_CmdLibraries{ {"start", UICommand::kStart},
					{"shutdown", UICommand::kShutdown},
					{"update config", UICommand::kUpdateConfig}, 
					{"help", UICommand::kHelp} }
{

}

CUIService::~CUIService()
{

}

//开始
bool CUIService::Start()
{
	//中文支持（本地系统）
	std::wcout.imbue(std::locale(""));

	//启动线程
	m_bRunning = true;

	return true;
}

//结束
void CUIService::Shutdown()
{
	m_bRunning = false;
}

//画布GUI帧刷新
bool CUIService::CanvasUpdate(UICmdData &Cmd)
{
	std::cout << "Please enter a command, Type \"help\" for more information." << std::endl;
	char strCmd[256];
	std::cin >> strCmd;

	auto itr = m_CmdLibraries.find(strCmd);
	if (itr == m_CmdLibraries.end())
	{
		Cmd.cmd = UICommand::kInvalid;
		return false;
	}

	Cmd.cmd = itr->second;

	//参数解析
	//...

	return true;
}

//追加显示内容
bool CUIService::AppendMsg(const wchar* wsMsg)
{
	return false;
}

//显示帮助
void CUIService::ShowHelp()
{
	std::cout << "Here are commands:" << std::endl;
	for (auto itr = m_CmdLibraries.begin(); itr != m_CmdLibraries.end(); itr++)
	{
		std::cout << itr->first << std::endl;
	}
}

//窗口输出
void CUIService::PrintOnView(const wchar* wsMsg, ulong color)
{
	std::wcout << wsMsg;
}
