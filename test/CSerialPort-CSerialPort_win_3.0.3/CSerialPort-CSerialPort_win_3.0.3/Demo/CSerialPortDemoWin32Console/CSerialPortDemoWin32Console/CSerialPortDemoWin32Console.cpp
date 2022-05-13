// CSerialPortDemoWin32Console.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "tchar.h"

#include "SerialPort.h"
#include <iostream>

using namespace std;
using namespace itas109;


string str1 = " ";
class mySlot : public has_slots<>
{
public:
	void OnSendMessage(unsigned char* str, int port, int str_len)
	{
		bool flag = true;
		//cout << "tt" << endl;
		for (int i = 0;i < str_len;i++) {
			/*uint8_t ch = str[i];
			printf("%02X ", ch & 0xFF);*/
			if (str[i] != '\r') {
				str1 += str[i];
				/*if (isdigit(str[i]) || str[i]=='-' || str[i] == '+') {
					str1 += str[i];
				}*/
			}
			else {
				flag = false;
				break;
			}
		}
		//memcpy(buff, str, str_len);
		//std::cout << "port : " << port << ", str_len : " << str_len << ", data : " << str << endl;
		if (!flag) {
			/*if (str1.size() > 1 && ((str1[0] == '0' && str1[1] == '0' && str1[2] != '0') || str1[0] == '0' && str1[1] != '0')) {
				cout << str1 << endl;
			}*/
			string strr = "";
			int count = 0;
			bool flag = false;
			for (int i = 0;i < str1.size();i++) {
				if (str1[i] == '+' || str1[i] == '-') {
					strr += str1[i];
					flag = true;
					count++;
				}
				if ((str1[i] != '+' && str1[i] != '-') && flag) {
					if (str1[i] != '*') {
						strr += str1[i];
						if (count == 3) {
							strr += '.';
						}
						count++;
					}
				}
			}
			cout << str1 << endl;
			cout << strr << endl;
			/*if (str1.size() == 0) {
				cout << "please try again" << endl;
			}*/
			str1 = " ";
		}
		else {
			cout << "outrange,please try again" << endl;
		}
	};
};





int _tmain(int argc, _TCHAR* argv[])
{
	CSerialPort m_SerialPort;
	mySlot receive;

	HWND m_handle = GetConsoleWindow();

	int port = 3;

	int i = m_SerialPort.InitPort(m_handle, port);
	m_SerialPort.StartMonitoring();

	std::cout << "port : " << port << ", IsOpened : " << m_SerialPort.IsOpened() << endl;
	/*while (1) {
		
#ifdef _SEND_DATA_WITH_SIGSLOT
		m_SerialPort.sendMessageSignal.connect(&receive, &mySlot::OnSendMessage);
		

#endif

		TCHAR temp[256] = _T("%01#RMD**\r");
		size_t len = _tcsclen(temp) + 1;;
		char* m_str = NULL;
		size_t* converted = 0;
		m_str = new char[len];
#ifdef UNICODE
		wcstombs_s(converted, m_str, len, temp, _TRUNCATE);
#else
		m_str = temp;
#endif
		m_SerialPort.WriteToPort(m_str, len);
		Sleep(10 * 100);
	}*/
#ifdef _SEND_DATA_WITH_SIGSLOT
	
	/*while (1) {
		mySlot receive;
		cout << "running" << endl;
		m_SerialPort.sendMessageSignal.connect(&receive, &mySlot::OnSendMessage);
		Sleep(10*100);
	}*/
	m_SerialPort.sendMessageSignal.connect(&receive, &mySlot::OnSendMessage);
#endif

	TCHAR temp[256] = _T("%01#RMD**\r");
	size_t len = _tcsclen(temp) + 1;;
	char* m_str = NULL;
	size_t* converted = 0;
	m_str = new char[len];
#ifdef UNICODE
	wcstombs_s(converted, m_str, len, temp, _TRUNCATE);
#else
	m_str = temp;
#endif
	/*while (1) {
		m_SerialPort.WriteToPort(m_str, len);
		Sleep(10 * 100);
	}*/
	m_SerialPort.WriteToPort(m_str, len);

	//while (1);
	system("pause");
	return 0;
}

