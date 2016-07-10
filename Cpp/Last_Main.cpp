#include"DetectingModel.h"
#include"DebugViewer.h"
#include<typeinfo>
using namespace std;
int main(){
	DebugViewer* DV = DebugViewer::mMakeViewer();
	DebugController* DC = DebugController::mMakeController();
	DetectingModel* DM = DetectingModel::mMakeModel();
	DV->mConnect(MVC::C, DC);
	DV->mConnect(MVC::M, DM);

	DM->mConnect(MVC::C, DC);
	DM->mConnect(MVC::V, DV);

	DC->mConnect(MVC::M, DM);
	DC->mConnect(MVC::V, DV);
	while (DM->mAction()){
		DV->mShowViewer();
	}
}