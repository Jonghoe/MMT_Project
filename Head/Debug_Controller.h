#pragma once
#include"MVC_Patter.h"
#include<opencv.hpp>
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델 < -- > 뷰어
// 싱글레톤 사용
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
