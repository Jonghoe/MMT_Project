#pragma once
#include"Common.h"
#include"Label.h"
#include"MarkList.h"
#include"Blob.h"
class Blob2Label{
	std::vector<Label*> mvLbVc;
	MarkList* mvML;
	void mvSetLabel(Label* l);
public:
	Blob2Label();
	Label* operator()(const Blob& b, std::vector<Label*> vc);
};
