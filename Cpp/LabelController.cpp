#include"LabelController.h"
#include"MyFunction.h"
#include<algorithm>
Label* LabelController::mvFindLabel(const Blob&b){
	vector<Label*>::iterator it;
	it = find_if(mvLbVc.begin(), mvLbVc.end(), BlobDup(b));
	if (it == mvLbVc.end())
		return nullptr;
	return *it;
}
void LabelController::mvInitChanged(){
	for (size_t i = 0; i < mvLbVc.size(); ++i)
		mvLbVc[i]->mSetChanged(false);
}
void LabelController::mvCreateLabel(const Blob&b){
	Label* newLabel = new Label(b);
	newLabel->mSetAngle(newLabel->mGetImg());
	newLabel->mSetMark(*ML);
	mvLbVc.push_back(newLabel);
}
void LabelController::mvChangeLabel(const Blob&b){
	vector<Label*>::iterator it;
	it = find_if(mvLbVc.begin(), mvLbVc.end(), BlobDup(b));
	if (it != mvLbVc.end()){
		(*it)->mChange(b);
		(*it)->mSetChanged(true);
	}
}
void LabelController::mvDeleteLabel(){
	for (size_t i = 0; i < mvLbVc.size();){
		if (mvLbVc[i]->mGetChanged())
			++i;
		else{
			Label* tmp = mvLbVc[i];
			swap(mvLbVc[i], mvLbVc[mvLbVc.size() - 1]);
			mvLbVc.pop_back();
			delete tmp;
		}

	}
}
void LabelController::mModify(const std::vector<Blob>& vc){
	mvInitChanged();
	for (size_t i = 0; i < vc.size(); ++i)
		if (!mvFindLabel(vc[i]))
			mvCreateLabel(vc[i]);
		else
			mvChangeLabel(vc[i]);
	mvDeleteLabel();
}
