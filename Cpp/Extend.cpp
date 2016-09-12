#include"Extend.h"
#include"Frame.h"
void Extend::operator()(void* i){
	Frame* fr = (Frame*)i;
	fr->mResize(col / row);
}