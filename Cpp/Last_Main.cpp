#include"Detecting_Model.h"
#include"Debug_Viewer.h"
#include<typeinfo>
using namespace std;
int main(){
	Debug_Viewer* D_V = Debug_Viewer::mMake_Viewer();
	Debug_Controller* D_C = Debug_Controller::mMake_Controller();
	Detecting_Model* D_M = Detecting_Model::mMake_Model();
	D_V->mConnect(MVC::C, D_C);
	D_V->mConnect(MVC::M, D_M);

	D_M->mConnect(MVC::C, D_C);
	D_M->mConnect(MVC::V, D_V);

	D_C->mConnect(MVC::M, D_M);
	D_C->mConnect(MVC::V, D_V);
	while (!D_M->mExit()){
		D_M->mDetecting();
		D_V->mShow_Viewer();
	}
}