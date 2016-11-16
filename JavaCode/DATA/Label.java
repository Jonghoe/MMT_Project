package data;

import java.awt.Point;

public class Label {
	private Point center;
	private int LID,markID,ID;
	private boolean consumed; 
	//xxxxyyyLIDmI
	public Label(String info){
		center = new Point(Integer.parseInt(info.substring(0, 4)),Integer.parseInt(info.substring(4, 7)));
		LID = Integer.parseInt(info.substring(7, 9));
		markID = Integer.parseInt(info.substring(9, 11));
	}
	public Label(){
		center=new Point(50,50);
		LID = 1;
		markID = 1;
	}
	public void setConsumed(boolean b){consumed=b;}
	public boolean Consumed(){return consumed;}
	public Point center(){return center;}
	public void setPID(int p){ID = p;}
	public int ID(){return ID;}
}
