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

//��ʼ
bool CUIService::Start()
{
	//����֧�֣�����ϵͳ��
	std::wcout.imbue(std::locale(""));

	//�����߳�
	m_bRunning = true;

	return true;
}

//����
void CUIService::Shutdown()
{
	m_bRunning = false;
}

//����GUI֡ˢ��
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

	//��������
	//...

	return true;
}

//׷����ʾ����
bool CUIService::AppendMsg(const wchar* wsMsg)
{
	return false;
}

//��ʾ����
void CUIService::ShowHelp()
{
	std::cout << "Here are commands:" << std::endl;
	for (auto itr = m_CmdLibraries.begin(); itr != m_CmdLibraries.end(); itr++)
	{
		std::cout << itr->first << std::endl;
	}
}

//�������
void CUIService::PrintOnView(const wchar* wsMsg, ulong color)
{
	std::wcout << wsMsg;
}
