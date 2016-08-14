#include"DetectingModel.h"
#include"DebugViewer.h"
#include<typeinfo>
using namespace std;
//#define DEBUG
#define _MAIN_  int main(){ MAIN m;m.Do();}
class MAIN{
public:
	MAIN(){}
	void Do(){
		DetectingModel* DM = DetectingModel::mMakeModel();
#ifdef DEBUG
		DebugViewer* DV = DebugViewer::mMakeViewer();
		DebugController* DC = DebugController::mMakeController();
		DV->mConnect(MVC::C, DC);
		DV->mConnect(MVC::M, DM);

		DM->mConnect(MVC::C, DC);
		DM->mConnect(MVC::V, DV);

		DC->mConnect(MVC::M, DM);
		DC->mConnect(MVC::V, DV);
#endif

		while (DM->mAction()){
#ifdef DEBUG
			DV->mShowViewer();
#else if

#endif
		}
	}
};