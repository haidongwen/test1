#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Windows.h"
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include <opencv2\opencv.hpp>
#include <time.h>

using namespace std;
using namespace cv;

LONG nPort = -1;

volatile int gbHandling = 3;
Rect select1, select2;

bool select_flag = false;

//解码回调 视频为YUV数据(YV12)，音频为PCM数据
void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
	if (gbHandling)
	{
		gbHandling--;
		return;
	}

	long lFrameType = pFrameInfo->nType;
	if (lFrameType == T_YV12)
	{

		Mat pImg(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
		Mat src(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, pBuf);
		
		cvtColor(src, pImg, CV_YUV2BGR_YV12);
		line(pImg, Point(10,20), Point(30,20), Scalar(0, 0, 255), 2, 8, 0);
		/*Sleep(-1);
		imshow("IPCamera", pImg);
		waitKey(1);*/

	}

	gbHandling = 3;

}

//解码回调 视频为YUV数据(YV12)，音频为PCM数据
void CALLBACK DecCBFun1(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
	if (gbHandling)
	{
		gbHandling--;
		return;
	}

	long lFrameType = pFrameInfo->nType;
	if (lFrameType == T_YV12)
	{

		Mat pImg(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
		Mat src(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, pBuf);
		
		cvtColor(src, pImg, CV_YUV2BGR_YV12);
		Mat img = pImg(Range(10, 30), Range(10, 30));
		Sleep(-1);
		imshow("IPCamera", img);
		waitKey(1);

	}

	gbHandling = 3;

}


///实时流回调
void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头

		if (!PlayM4_GetPort(&nPort))  //获取播放库未使用的通道号
		{
			break;
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}

			if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 10 * 1024 * 1024)) //打开流接口
			{
				break;
			}

			if (!PlayM4_Play(nPort, NULL)) //播放开始
			{
				break;
			}
			if (!PlayM4_SetDecCallBack(nPort, DecCBFun))
			{
				break;
			}
		}
		break;
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				cout << "error" << PlayM4_GetLastError(nPort) << endl;
				break;
			}
		}
		break;
	default: //其他数据
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}
}

void CALLBACK fRealDataCallBack1(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头

		if (!PlayM4_GetPort(&nPort))  //获取播放库未使用的通道号
		{
			break;
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}

			if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 10 * 1024 * 1024)) //打开流接口
			{
				break;
			}

			if (!PlayM4_Play(nPort, NULL)) //播放开始
			{
				break;
			}
			if (!PlayM4_SetDecCallBack(nPort, DecCBFun1))
			{
				break;
			}
		}
		break;
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				cout << "error" << PlayM4_GetLastError(nPort) << endl;
				break;
			}
		}
		break;
	default: //其他数据
		if (dwBufSize > 0 && nPort != -1)
		{
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}
}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT:    //预览时重连
		printf("----------reconnect--------%d\n", time(NULL));
		break;
	default:
		break;
	}
}

void CALLBACK g_cbDrawFun(LONG, HDC, DWORD);
void CALLBACK g_cbDrawFun(LONG lRealPlayHandle, HDC hDC, DWORD dwUser)
{
    //TextOut(hDC,400,400,_T("人脸识别")，4)；
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));      //画笔
	RECT rt;
	HWND hWnd = ::FindWindow(NULL, "Mywindow");
	/*HWND hWnd = WindowFromDC(hDC);*/
	GetWindowRect(hWnd, &rt);
	//int width = GetSystemMetrics(SM_CXFULLSCREEN); //获取最大化窗体的显示区域宽度
	//int height = GetSystemMetrics(SM_CYFULLSCREEN);//获取最大化窗体的显示区域高度
	int width = rt.right - rt.left;   //窗口的宽度
	int height = rt.bottom - rt.top;  //窗口的高度
	/*int width = GetDeviceCaps(hDC, DESKTOPHORZRES);
	int height = GetDeviceCaps(hDC, DESKTOPVERTRES);*/
	SelectObject(hDC, hPen);
	MoveToEx(hDC, width/5, height/2, NULL);
	LineTo(hDC, width / 2, height / 2);
	MoveToEx(hDC, width / 5, height / 2, NULL);
	LineTo(hDC, width / 5, height / 2-20);
	MoveToEx(hDC, width / 5, height / 2-20, NULL);
	LineTo(hDC, width / 2, height / 2 - 20);
	MoveToEx(hDC, width / 2, height / 2 - 20, NULL);
	LineTo(hDC, width / 2, height / 2 );
	MoveToEx(hDC, width / 5, height / 2, NULL);
	LineTo(hDC, width / 7, height / 2);
	//line(hDC,Point(10,20),Point(30,20), Scalar(0, 0, 255), 2, 8, 0);
	ReleaseDC(NULL, hDC);
	/*cout << "client width:" << rt.right << std::endl;*/
	/*std::cout << "client width:" << width << std::endl;
	std::cout << "client height:" << height << std::endl;*/
	return;
}

struct point
{
	double x, y;
};

double row1 = 0, row2 = 0, col1 = 0, col2 = 0;
//POINT ptB = { 0, 0 };

void on_Mouse(int event, int x, int y, int flags, void* param)
{

	Point p1, p2;
	POINT ptB;
	
	if (event == EVENT_LBUTTONDOWN) //按下左键
	{
		select1.x = x;
		select1.y = y;
		LPPOINT xy = &ptB;    //位置变量
		GetCursorPos(xy);
		row1 = xy->x;
		col1 = xy->y;
		cout << row1 << ";"<<col1 << endl;
		HWND hWnd = ::FindWindow(NULL, "Mywindow");
		ScreenToClient(hWnd, xy);
		row1 = xy->x;
		col1 = xy->y;
		cout << row1 << ";" << col1 << endl;
		select_flag = false;
	}
	if (event == EVENT_LBUTTONUP)  //放开左键
	{
		select2.x = x;
		select2.y = y;
		//p1 = Point(select1.x, select1.y);
		//p2 = Point(x, y);
		p1 = Point(select1.x, select1.y);
		p2 = Point(select2.x, select2.y);
		//HWND h = GetForegroundWindow();
		// 获取窗口标题

		LPPOINT xy = &ptB;    //位置变量
		GetCursorPos(xy);
		RECT rt;
		HWND hWnd = ::FindWindow(NULL, "Mywindow");
		ScreenToClient(hWnd, xy);
		/*GetWindowRect(hWnd, &rt);
		cout << rt.left << endl;
		cout << rt.top << endl;*/
		row2 = xy->x;
		col2 = xy->y;
		cvNamedWindow("Mywindow2", 0);
		cvResizeWindow("Mywindow2", 640, 480);
		//cvNamedWindow("Mywindow1", 0);
		HWND  h2 = (HWND)cvGetWindowHandle("Mywindow2");
		LONG pWidth, pHeight;
		PlayM4_GetPictureSize(0, &pWidth, &pHeight);
		cout << pWidth << "aa" << pHeight << endl;
		float ratiorow, ratiocol;
		ratiorow = float(pWidth) /1024 ;
		ratiocol =  float(pHeight) / 768;
		cout << ratiorow << "," << ratiocol << endl;
		RECT strSrcRect = { 0 };
		strSrcRect.left = row1*ratiorow;
		strSrcRect.top = col1*ratiocol;
		strSrcRect.right = row2*ratiorow;
		strSrcRect.bottom = col2*ratiocol;
		cout << strSrcRect.left << ";" << strSrcRect.top << ";" << strSrcRect.right << ";" << strSrcRect.bottom << endl;
		
		PlayM4_SetDisplayRegion(0, 1, &strSrcRect, h2, 1);
		/*system("pause");*/
	}
	else if (event == EVENT_MBUTTONDOWN)  //按下中键
	{
		select_flag = true;
	}
}



void main()
{

	//---------------------------------------
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);


	//---------------------------------------
	// 注册设备
	LONG lUserID, lUserID1;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo, struDeviceInfo1;
	lUserID = NET_DVR_Login_V30("192.168.1.101", 8000, "admin", "YNEBXT", &struDeviceInfo);
	lUserID1 = NET_DVR_Login_V30("192.168.1.162", 8000, "admin", "YGIJLS", &struDeviceInfo1);
	if (lUserID < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}

	//---------------------------------------
	//设置异常消息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//---------------------------------------
	//启动预览并设置回调数据流
	LONG lRealPlayHandle, lRealPlayHandle1, lRealPlayHandle2;
	cvNamedWindow("Mywindow", 0);
	cvResizeWindow("Mywindow", 1024, 768);
	//cvNamedWindow("Mywindow1", 0);
	//cvNamedWindow("IPCamera", 0);
	setMouseCallback("Mywindow", on_Mouse, 0);
	HWND  h = (HWND)cvGetWindowHandle("Mywindow");
	//HWND  h1 = (HWND)cvGetWindowHandle("Mywindow1");
	
	if (h == 0)
	{
		cout << "窗口创建失败" << endl;
	}


	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = h;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1;           //预览通道号
	struPlayInfo.dwStreamType = 1;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode = 0;         //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP


	NET_DVR_PREVIEWINFO struPlayInfo1 = { 0 };
	//struPlayInfo1.hPlayWnd = h1;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo1.lChannel = 2;           //预览通道号
	struPlayInfo1.dwStreamType = 1;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo1.dwLinkMode = 0;         //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

	


	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, fRealDataCallBack, NULL);
	//lRealPlayHandle1 = NET_DVR_RealPlay_V40(lUserID1, &struPlayInfo1, fRealDataCallBack, NULL);
	NET_DVR_RigisterDrawFun(lRealPlayHandle, g_cbDrawFun, NULL);
	//NET_DVR_RigisterDrawFun(lRealPlayHandle1, g_cbDrawFun, NULL);

	
	/*参数说明：
		参数1： nPort表示已经获取的播放库内部分配的端口号 ；
		参数2： 设定需要显示区域序号为1；
		参数3： &strSrcRect表示设置需要显示的区域（相对原始图像）；
		参数4： hDestWnd表示显示窗口句柄；
		参数5： 1表示打开显示区域；*/
	
	

	if (lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		printf("%d\n", NET_DVR_GetLastError());
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}

	NET_DVR_PREVIEWINFO struPlayInfo2 = { 0 };
	//struPlayInfo2.hPlayWnd = h1;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo2.lChannel = 1;           //预览通道号
	struPlayInfo2.dwStreamType = 1;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo2.dwLinkMode = 0;         //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
	/*while (true)
	{
		if (select_flag)
		{
			lRealPlayHandle2 = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo2, fRealDataCallBack1, NULL);
		}
	}*/

	waitKey();

	Sleep(-1);
	//---------------------------------------
	//关闭预览
	NET_DVR_StopRealPlay(lRealPlayHandle);
	//NET_DVR_StopRealPlay(lRealPlayHandle1);
	//注销用户
	NET_DVR_Logout(lUserID);
	NET_DVR_Logout(lUserID1);
	//释放SDK资源
	NET_DVR_Cleanup();



	return;
}

