#pragma once

class CThread
{
public:
	CThread();
	~CThread();
public:
	void Start();
	void Stop();

public:
	virtual void SetThreadData(DWORD dwParam); //设置线程中的数据
	virtual DWORD GetThreadData();            //获取传入线程中的串口指针
	virtual void run();
public:
	static DWORD ThreadProc(LPVOID pParam);  //线程的回调函数

public:
	HANDLE m_hThread;
	bool m_bExit; //线程是否退出的标志位
	DWORD m_dwParam;
};

