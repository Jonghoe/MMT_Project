package DATAMethod;

import java.awt.Point;
import java.util.ArrayList;

import DATA.Label;
import DATA.Rect;

public class RectMethod {
	static public Rect Create(ArrayList<Label> p){
		double l=p.get(0).GetPT().getX(),t=p.get(0).GetPT().getY(),
				b=p.get(0).GetPT().getY(),r=p.get(0).GetPT().getX();
		double x,y,xLen,yLen,len;
		for(Label lb :p){
			x = lb.GetPT().getX();
			y = lb.GetPT().getY();
			if(x<l)
				l=x;
			else if(r<x)
				r=x;
			if(y<t)
				t=y;
			else if(b<y)
				b=y;
		}
		xLen = r-l;
		yLen = b-t;
		if(xLen>yLen)
			len=xLen;
		else
			len =yLen;
		Point lt=new Point();
		lt.setLocation(0, 0);
		Point rb = new Point();
		rb.setLocation(len, len);
		return new Rect(lt,rb);
	}
}
