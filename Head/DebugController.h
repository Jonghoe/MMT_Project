#pragma once
#include"MVCPatter.h"
#include<opencv.hpp>
// �ܼ��� ������� �ϱ� ���� ���� Ŭ����
// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > �� < -- > ���
// �̱۷��� ���
class DebugController{
private:
	static DebugController* Alive;			//�̱۷��� ����� ���� ����
	int mvKey;								//������ Ű ���� ����
	void* mvConnected[2];					//Viewer�� Model ��ü ������
	bool mvKeyPush;							//Ű�� ���������� Ȯ��
	DebugController(){						//������
		mvConnected[0] = NULL;				//�̱۷��� ó���� ���� private����
		mvConnected[1] = NULL;
		mvKeyPush = false; 
	}
	~DebugController(){  }
public:
	static DebugController* mMakeController(){//�̱۷��� ���� �Լ�
		if (Alive != NULL)	return Alive;
		Alive = new DebugController();
		return Alive;
	}
	void mDeleteController(){				// �̱۷��� �ı� �Լ�
		if (Alive == NULL)
			return;
		delete this;
	}
	void mConnect(MVC type,void* Vp){		// Veiwer,Model ���� �Լ�
		mvConnected[type] = Vp; 
	}
	void mPushKey(int Key){					// ������ Ű���� ����
		if (Key == -1)						// mvKeyPush ����
			return;
		this->mvKey = Key;
		this->mvKeyPush = true;
	}
	int mPutKey(){							// Ű�� ��� ����.
		return this->mvKey; 
	}
	bool mKeyPushed(){						// Ű�� ���������� Ȯ��
		if (this->mvKeyPush){
			this->mvKeyPush = false;
			return true;
		}
		return false;
	}
};
