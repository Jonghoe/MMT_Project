package data;

import java.awt.Point;

public class Rect implements Size{
	int width;
	int height;
	
	public Rect(int w,int h){
		width = w;
		height = h;
	}
	public Rect(Point _lt,Point _rb){
		width = _rb.x - _lt.x;
		height = _rb.y-_lt.y;
	}
	public void setW(int w){
		width=w;
	}
	public void setH(int h){
		height=h;
	}
	@Override
	public int size() {
		// TODO Auto-generated method stub
		return width* height;
	}
}
