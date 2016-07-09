#pragma once
#include"MVCPatter.h"
#include<opencv.hpp>
// 단순히 디버깅을 하기 위해 만든 클래스
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델 < -- > 뷰어
// 싱글레톤 사용
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
