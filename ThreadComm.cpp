#include "stdafx.h"
#include "ThreadComm.h"
#include "SerialcommunicationDlg.h"
#include "SerialPort.h"
#include "resource.h"	
#include <string>
using namespace std;
CThreadComm::CThreadComm()
{
}


CThreadComm::~CThreadComm()
{
}
bool falg = false;

void CThreadComm::run()
{
	Sleep(100);
	CSerialcommunicationDlg* pSerialcommunicationDlg = (CSerialcommunicationDlg*)AfxGetApp()->m_pMainWnd;   //获取对话框的对象
	if (NULL == pSerialcommunicationDlg)    //这几行不注释会显示IDD_SERIALCOMMUNICATION_DIALOG未识别的错误
	{
		return;
	}

	CSerialPort *pSerialPort = (CSerialPort*)GetThreadData();
	if (NULL == pSerialPort) return;  //判断指针是否存在

	//判断串口传入的有没有数据，只要有数据进来，串口的缓冲区里就会设置一个错误的标志位，接下来判断错误标志位的输入缓冲区到底有没有数据
	DWORD dwError = 0;
	COMSTAT comStat; //来接收错误的状态
	BOOL bRet = TRUE;
	//TCHAR recvTemp[512]; //定义接收缓冲区数据的临时变量
	char recvTemp[512];
	ZeroMemory(recvTemp, sizeof(recvTemp));

	char recvBuf[4096]; //定义接收缓冲区数据的实际变量
	//TCHAR recvBuf[4096]; //定义接收缓冲区数据的实际变量
	ZeroMemory(recvBuf, sizeof(recvBuf));

	DWORD dwRead = 0;
	int nLength = 0;
	

	pSerialPort->ClearCommError(&dwError, &comStat);
	if (comStat.cbInQue > 0)  //判断错误状态中的输入缓冲区（cbInQue）是否大于0,大于0说明有数据过来了，接下来就可以来接收数据
	{
		OVERLAPPED overlappedRead;
		ZeroMemory(&overlappedRead, sizeof(overlappedRead));//先把overlappedRead变量给清零
		overlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);   //第二个参数TRUE表示事件会自动触发，第三个参数FALSE表示原先的状态是有信号状态，还是无信号状态，FALSE表示无信号状态
			                        //如果有信号过来时，这个事件就会被触发
		
		/*OVERLAPPED m_osRead;
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		dwRead = min(dwRead, (DWORD)comStat.cbInQue);*/

		if (comStat.cbInQue < 512)
		{
			bRet = pSerialPort->ReadFile(recvTemp, comStat.cbInQue, &dwRead, &overlappedRead); //dwRead表示实际读了多少个
		}
		else
		{
			bRet = pSerialPort->ReadFile(recvTemp, 500, &dwRead, &overlappedRead);  //每次读500个字节
		}

		if (comStat.cbInQue >= dwRead)  //判断缓冲区的数值是否大于等于读取的数值，来判断是否读完
		{
			memcpy(recvBuf + nLength, recvTemp, dwRead);
			nLength += dwRead;
		}

		if (comStat.cbInQue == dwRead)  //判断是否读完，读完之后就显示出来
		{
			nLength = 0;
			CEdit* pEditRecv = (CEdit*)pSerialcommunicationDlg->GetDlgItem(IDC_EDIT_RECV);
			if (pEditRecv)
			{
				CString strRecv;
				pEditRecv->GetWindowText(strRecv);
				strRecv += recvBuf;
				strRecv += "\r\n";
				pEditRecv->SetWindowText(strRecv);
			}
			string DirectStr;
			string subDirectStr;
			DirectStr = recvTemp;
			subDirectStr=DirectStr.substr(0, DirectStr.find(DirectStr[0], 3));
			
		
			if (subDirectStr == "RZminus")
			{
				//pSerialcommunicationDlg->InitComboBox(); //按下发3个字符，松开发三个字符。
				falg = true;
			}


		}		


		if (!bRet)  //判断是否重叠，bRet返回FALSE说明数据没被读完
		{
			if (ERROR_IO_PENDING == GetLastError())  //判断数据是否被挂起，挂起代表还有数据
			{
				while (!bRet)
				{
					bRet = pSerialPort->GetOverlappedResult(NULL, &dwRead, TRUE);
					if (GetLastError() != ERROR_IO_INCOMPLETE)    //判断是否由于数据没有收完而被挂起
					{
						pSerialPort->ClearCommError(&dwError, &comStat);  //来清楚缓冲区内容
						break;
					}
				}
			}
		}
	}
	Sleep(50);  //延时50ms，让数据存入缓冲区
	if ((comStat.cbInQue == 0) && falg)
	{
		pSerialcommunicationDlg->InitComboBox();
		falg = false;
	}
		
}