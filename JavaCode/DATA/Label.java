package DATA;

import java.awt.Point;

public class Label {
	Point p;
	int LID,markID,PID;
	//xxxxyyyLIDmI
	public Label(String info){
		p = new Point(Integer.parseInt(info.substring(0, 4)),Integer.parseInt(info.substring(4, 7)));
		LID = Integer.parseInt(info.substring(7, 9));
		markID = Integer.parseInt(info.substring(9, 11));
	}
	public Label(){
		p=new Point(0,0);
		LID = 1;
		markID = 1;
	}
	public Point GetPT(){return p;}
	public void SetPID(int p){PID = p;}
	public int GetPID(){return PID;}
}
