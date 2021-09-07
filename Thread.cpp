#include "stdafx.h"
#include "Thread.h"

//构造函数对类中的变量进行初始化
CThread::CThread()
{
	m_bExit = FALSE;
	m_dwParam = 0;
	m_hThread = NULL;
}

CThread::~CThread()
{
	if (!m_bExit)
	{
		Stop();
	}
}


void CThread::Start()
{
	DWORD dwThreadID=0;  //返回启动线程的ID
	HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, &dwThreadID);
	ASSERT(hThread);
	m_hThread = hThread;
}

void CThread::Stop()
{
	if (m_hThread)
	{
		m_bExit = true;
		::WaitForSingleObject(m_hThread, INFINITE); //等待线程是否完全退出
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

DWORD CThread::ThreadProc(LPVOID pParam)
{
	CThread* pThis = (CThread*)pParam;
	ASSERT(pThis);

	while (!pThis->m_bExit)
	{
		pThis->run();
	}
	return true;
}
void CThread::run()  //一个空实现
{
	Sleep(100);
}

void  CThread::SetThreadData(DWORD dwParam)
{
	if (m_dwParam != dwParam)
	{
		m_dwParam = dwParam;
	}
}
DWORD CThread::GetThreadData()
{
	return m_dwParam;
}