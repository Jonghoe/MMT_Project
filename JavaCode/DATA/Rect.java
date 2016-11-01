package DATA;

import java.awt.Point;

public class Rect {
	private Point LT,RB;
	public Rect(Point lt,Point rb){
		LT=lt;
		RB=rb;
	}
	public Point GetLT(){return LT;}
	public Point GetRB(){return RB;}

	public double GetArea(){
		return (RB.getX()-LT.getX())*(RB.getY()-LT.getY());
	}
}
