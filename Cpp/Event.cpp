#include"Event.h"
#include"MyFunction.h"
using namespace std;
using namespace cv;
void Event::mSetTilt(Point& c, Event& e){
	Point FL = e.mvLoc - c;
	Point NL = mvLoc - c;
	double degree = acos(FL.dot(NL) / (FL.dot(FL)*NL.dot(NL)));
	Point tmpP = RotatePoint(degree,FL);
	if (tmpP == FL)
		mvTilt = degree;
	else
		mvTilt = -degree;
}