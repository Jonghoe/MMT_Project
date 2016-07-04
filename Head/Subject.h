#pragma once
#include"Observer.h"
class Subject
{
public:
	virtual void mRegistOb(Observer*)=0;
	virtual void mUnRegistOb(Observer*)=0;
	virtual void mNotify()=0;
};