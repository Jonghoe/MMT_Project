#pragma once
#include"Label.h"
class LabelController{
	std::vector<Label*> mvLbVc;
	MarkList* ML;
	void mvCreateLabel(const Blob&);
	void mvChangeLabel(const Blob&);
	void mvDeleteLabel();
	void mvInitChanged();
	Label* mvFindLabel(const Blob&);
public:
	const std::vector<Label*> mGetVc()const{ return mvLbVc; }
	LabelController() :mvLbVc(){ ML = MarkList::mMakeMarkList(); }
	int mSize(){ return mvLbVc.size(); }
	void mModify(const std::vector<Blob>& vc);
};