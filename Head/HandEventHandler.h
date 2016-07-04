#pragma once
class HandEvent{
protected:
	virtual void mvAction() = 0;
	virtual void mOClick() = 0;
	virtual void mDClick() = 0;
	virtual void mDrag() = 0;
	virtual void mJoom() = 0;
	virtual void mRotate() = 0;
};