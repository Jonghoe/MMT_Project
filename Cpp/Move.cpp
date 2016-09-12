#include"Move.h"
#include"Frame.h"
void Move::operator()(void* i){
	Frame* fr = (Frame*)i;
	Point& c = fr->mGetCenter();
	c += Point(col, row);
}