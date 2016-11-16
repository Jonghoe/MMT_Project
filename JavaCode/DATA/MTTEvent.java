package data;

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
	
	private EventType Type(){return type;}
	public boolean Consumed(){return consumed;}
	public void setConsumed(boolean c){consumed = c;}
	public double Rate(){return rate;}
	public Point Point(){return pt;}
	public int MID(){return MID;} 
}
