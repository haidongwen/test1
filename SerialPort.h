#pragma once
#include "ThreadComm.h"
class CSerialPort
{
public:
	CSerialPort();
	~CSerialPort();
public:
	BOOL OpenCommm(CString strComm); //打开串口 ,串口就是一个文件
	BOOL SetCommStata(DWORD dwBaudtate,BYTE byParity,BYTE byByteSize,BYTE byStopBits); //设置串口的参数
	BOOL SetupComm(DWORD dwInqueue, DWORD dwOutqueue);//设置输入输出缓冲区的大小
	BOOL PurgeComm(DWORD dwFlags);//清楚缓冲区
	BOOL SetCommMask(DWORD dwEvtMask); //设置串口的事件类型
	BOOL WriteFile(IN LPVOID lpBuffer, IN DWORD nNumberOfBytesToRead, OUT LPDWORD lpNumberOfBytesRead, IN LPOVERLAPPED lpOverlapped);
	BOOL ReadFile(OUT LPVOID lpBuffer, IN DWORD nNumberOfBytesToRead, OUT LPDWORD lpNumberOfBytesRead, IN LPOVERLAPPED lpOverlapped);
	BOOL ClearCommError(OUT LPDWORD lpErrors, OUT LPCOMSTAT lpStat); //读写完之后，把错误的标志给清楚掉
	BOOL GetOverlappedResult(IN LPOVERLAPPED lpOverlapped,OUT LPDWORD lpNumberOfBytesTransferred,IN BOOL bWait);  //判断数据有没有收发完成
	void CloseComm();
	void StartComm();


public:
	HANDLE m_hComm;
	CThreadComm m_threadComm;  //接收数据的线程的对象作为了串口对象的一个成员变量
};

