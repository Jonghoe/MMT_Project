package DATA;

import java.awt.Point;
import java.util.Random;

import DATAMethod.RandomID;

public class Component {
	int ID;
	Point LT;
	public Component(){
		LT=new Point(0,0);
		ID = RandomID.MakeID(this);
	}
	public int GetID(){return ID;}
	public void SetLoc(Point pt){LT=pt;}
	public void SetLoc(int x,int y){LT=new Point(x,y);}
	public Point GetLoc(){return LT;}
}
