#pragma once
#include"MVC_Patter.h"
#include<opencv.hpp>
// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > �� < -- > ���
// �̱۷��� ���
class Debug_Controller{
private:
	static Debug_Controller* Alive;
	int mvKey;
	void* mvConnected[2];
	bool mvKey_Push;
	Debug_Controller(){ mvConnected[0] = false; mvConnected[1] = false; mvKey_Push = false; }
	~Debug_Controller(){  }
public:
	static Debug_Controller* mMake_Controller(){
		if (Alive != NULL)	return Alive;
		Alive = new Debug_Controller();
		return Alive;
	}
	void mDelete_Controller(){
		if (Alive == false)
			return;
		delete this;
	}
	void mConnect(MVC type,void* Vp){
		mvConnected[type] = Vp; 
	}
	void mPush_Key(int Key){
		if (Key == -1)
			return;
		this->mvKey = Key;
		this->mvKey_Push = true;
	}
	int mPut_Key(){
		return this->mvKey; 
	}
	bool mKey_Pushed(){
		if (this->mvKey_Push){
			this->mvKey_Push = false;
			return true;
		}
		return false;
	}
};
