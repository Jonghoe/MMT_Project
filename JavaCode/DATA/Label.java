package DATA;

import java.awt.Point;

public class Label {
	Point p;
	int LID,MID,PID;
	public Label(String info){
		p = new Point(Integer.parseInt(info.substring(0, 4)),Integer.parseInt(info.substring(4, 7)));
		LID = Integer.parseInt(info.substring(7, 9));
		LID = Integer.parseInt(info.substring(9, 11));
		LID = Integer.parseInt(info.substring(11,12));
	}
	public Label(){
		p=new Point(0,0);
		LID = 1;
		MID = 1;
	}
	public Point GetPT(){return p;}
	public void SetPID(int p){PID = p;}
	public int GetPID(){return PID;}
}
