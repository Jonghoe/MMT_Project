package DATA;

import java.awt.Point;

public abstract class MTTEvent {
	int MID;
	Point pt;
	boolean Used;
	public MTTEvent(int id,Point p){
		Used=false;
		MID = id;
		pt = p;
	}
	public abstract double GetRate();
	public abstract Point GetPoint();
	public int GetMID(){return MID;}
	public String toString(){
		String s=new String("�̺�Ʈ�� �� ID: "+Integer.toString(MID)+"�̺�Ʈ�� Ư��: "+"NONE");
		return s;
	}
}
