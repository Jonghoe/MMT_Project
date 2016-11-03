package DATA;

import java.awt.Point;

public class MTTEvent {
	private int MID;
	private Point pt;
	private EventType type;
	private boolean consumed;
	private double rate;
	public MTTEvent(int id,Point p,EventType t){
		type =t;
		consumed=false;
		MID = id;
		pt = p;
		rate=0;
	}
	public MTTEvent(int pID, double r, EventType t) {
		// TODO Auto-generated constructor stub
		type =t;
		consumed=false;
		MID = pID;
		pt = null;
		rate=r;
	}
	public MTTEvent(int pID, EventType t) {
		// TODO Auto-generated constructor stub
		type =t;
		consumed=false;
		MID = pID;
	}
	private void SetType(){;}
	private EventType GetType(){return type;}
	public boolean GetConsumed(){return consumed;}
	public void SetConsumed(boolean c){consumed = c;}
	public double GetRate(){return rate;}
	public Point GetPoint(){return pt;}
	public int GetMID(){return MID;}
	 
}
