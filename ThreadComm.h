#pragma once
#include "Thread.h"
class CThreadComm :public CThread   //定义一个派生类CThreadComm
{
public:
	CThreadComm();
	~CThreadComm();

public:
	virtual void run();
};

