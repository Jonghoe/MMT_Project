#pragma once
#include"MVCPatter.h"
#include<opencv.hpp>
// 단순히 디버깅을 하기 위해 만든 클래스
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델 < -- > 뷰어
// 싱글레톤 사용
class DebugController{
private:
	static DebugController* Alive;			//싱글레톤 기법을 위한 변수
	int mvKey;								//눌려진 키 값을 저장
	void* mvConnected[2];					//Viewer와 Model 객체 포인터
	bool mvKeyPush;							//키가 눌려졌는지 확인
	DebugController(){						//생성자
		mvConnected[0] = NULL;				//싱글레톤 처리를 위해 private설정
		mvConnected[1] = NULL;
		mvKeyPush = false; 
	}
	~DebugController(){  }
public:
	static DebugController* mMakeController(){//싱글레톤 생성 함수
		if (Alive != NULL)	return Alive;
		Alive = new DebugController();
		return Alive;
	}
	void mDeleteController(){				// 싱글레톤 파괴 함수
		if (Alive == NULL)
			return;
		delete this;
	}
	void mConnect(MVC type,void* Vp){		// Veiwer,Model 연결 함수
		mvConnected[type] = Vp; 
	}
	void mPushKey(int Key){					// 눌려진 키값을 저장
		if (Key == -1)						// mvKeyPush 변경
			return;
		this->mvKey = Key;
		this->mvKeyPush = true;
	}
	int mPutKey(){							// 키를 뱉어 낸다.
		return this->mvKey; 
	}
	bool mKeyPushed(){						// 키가 눌려졌는지 확인
		if (this->mvKeyPush){
			this->mvKeyPush = false;
			return true;
		}
		return false;
	}
};
