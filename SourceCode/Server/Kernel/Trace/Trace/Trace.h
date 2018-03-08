#pragma once

#include "TraceManager.h"

#define LOG_BUFFER_SIZE	2048	//������־�����С

//��־��ɫ
enum TraceColor
{
	kLogColor = RGB(255, 255, 255),			//��־
	kDebugColor = RGB(255, 255, 255),		//DEBUG��־
	kInfoColor = RGB(255, 255, 255),		//��Ϣ
	kWarningColor = RGB(255, 255, 255),		//����
	kAssertionColor = RGB(255, 255, 255),	//����
	kErrorColor = RGB(255, 255, 255),		//����
	kCriticalColor = RGB(255, 255, 255),	//�ٽ�㡢�ؼ�Σ����
	kExceptionColor = RGB(255, 255, 255),	//�쳣
	kCustomizeColor = RGB(255, 255, 255),	//�û��Զ��壨Ĭ��ֵ��
};

class CTrace : public ITraceService
{
public:
	explicit CTrace();
	~CTrace();

	//ITraceService �̳�
public:
	//��������
	bool Start(ITraceView* pView, const wchar* wsLogFileName) override;
	//ֹͣ����
	//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	void Shutdown() override;
	//����״̬
	bool Serviceable() override;

	//ITraceService::ITrace �̳�
public:
	//��־���
	void Log(TraceLevel enLevel, const wchar* wsMsg) override;
	void LogFormat(TraceLevel enLevel, const wchar* wsFormat, ...) override;

	//��ӡ��־
	void Log(const wchar* wsMsg) override;
	void LogFormat(const wchar* wsFormat, ...) override;

	//��ӡDEBUG��־
	void LogDebug(const wchar* wsMsg) override;
	void LogDebugFormat(const wchar* wsFormat, ...) override;

	//��ӡ��Ϣ
	void LogInfo(const wchar* wsMsg) override;
	void LogInfoFormat(const wchar* wsFormat, ...) override;

	//��ӡ����
	void LogWarning(const wchar* wsMsg) override;
	void LogWarningFormat(const wchar* wsFormat, ...) override;

	//��ӡ����
	void LogAssertion(const wchar* wsMsg) override;
	void LogAssertionFormat(const wchar* wsFormat, ...) override;

	//��ӡ����
	void LogError(const wchar* wsMsg) override;
	void LogErrorFormat(const wchar* wsFormat, ...) override;

	//��ӡ�ؼ�Σ��
	virtual void LogCritical(const wchar* wsMsg) override;
	virtual void LogCriticalFormat(const wchar* wsFormat, ...) override;

	//��ӡ�쳣
	void LogException(const wchar* wsMsg) override;
	void LogExceptionFormat(const wchar* wsFormat, ...) override;

	//��ӡ�Զ�����Ϣ
	void LogCustom(ulong color, const wchar* wsMsg) override;
	void LogCustomFormat(ulong color, const wchar* wsFormat, ...) override;

	//���붨λ
	void LogLocate(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsMsg) override;
	void LogLocateFormat(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsFormat, ...) override;

	//���붨λ��__FILE__,__LINE__ Ϊchar, �˴��ṩchar�汾��
	void LogLocate(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsMsg) override;
	void LogLocateFormat(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsFormat, ...) override;

	//���ܺ���
public:
	//��ȡ��ɫ
	COLORREF GetColor(TraceLevel enLevel) const;
	//��ӡ��־
	void PrintLog(TraceLevel enLevel, COLORREF color, const wchar* wsMsg);

	//�ڲ���Ա
private:
	ITraceView* m_pView;
	CTraceManager m_TraceMgr;
	//���ݽ���������
	wchar m_wsBuffer[LOG_BUFFER_SIZE];
	wchar m_wsPrintBuffer[LOG_BUFFER_SIZE];
	//��־�ַ�������
	static wchar m_wsTraceLevel[TraceLevel::kTraceLevelNum][16];
	//��д������֤�̰߳�ȫ
	SRWLOCK m_SRWLock;
};
