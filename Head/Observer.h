#pragma once
//옵저버 패턴
class Observer
{
public :
	virtual void mUpdate(void* info) = 0;
};