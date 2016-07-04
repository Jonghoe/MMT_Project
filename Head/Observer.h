#pragma once
//옵저버 패턴
class Observer
{
public :
	virtual void mvUpdate(void* info) = 0;

};