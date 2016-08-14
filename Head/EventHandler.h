#pragma once
#include<vector>
#include<string>
#include"Label.h"
#include"Event.h"
using namespace std;

class EventHandler{
private:
	static EventHandler* Alive;
	vector<Label*> mvEventlist;
public:
	EventHandler():mvEventlist(){}
	static EventHandler* mMakeEventHandler(){
		if (Alive == nullptr)
			Alive = new EventHandler();
		return Alive;
	}
	void mPush(Label* e){
		mvEventlist.push_back(e);
	}
};