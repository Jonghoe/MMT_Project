#pragma once
#include"MVCPatter.h"
#include<opencv.hpp>
// �ܼ��� ������� �ϱ� ���� ���� Ŭ����
// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > �� < -- > ���
// �̱۷��� ���
class DebugController{
private:
	static DebugController* Alive;
	int mvKey;
	void* mvConnected[2];
	bool mvKeyPush;
	DebugController(){ mvConnected[0] = false; mvConnected[1] = false; mvKeyPush = false; }
	~DebugController(){  }
public:
	static DebugController* mMakeController(){
		if (Alive != NULL)	return Alive;
		Alive = new DebugController();
		return Alive;
	}
	void mDeleteController(){
		if (Alive == NULL)
			return;
		delete this;
	}
	void mConnect(MVC type,void* Vp){
		mvConnected[type] = Vp; 
	}
	void mPushKey(int Key){
		if (Key == -1)
			return;
		this->mvKey = Key;
		this->mvKeyPush = true;
	}
	int mPutKey(){
		return this->mvKey; 
	}
	bool mKeyPushed(){
		if (this->mvKeyPush){
			this->mvKeyPush = false;
			return true;
		}
		return false;
	}
};
