package data;

import java.awt.Point;
import java.util.Comparator;
import java.util.Observable;

import datamethod.ComponentController;
import datamethod.RandomID;

public abstract class Component extends Observable {
	protected int ID;
	private int z;
    private Rect area;
    private Point lt,rb;
    final public boolean isContain(Point pt) {
		if(lt.x<pt.x&&pt.x<rb.x &&
			lt.y<pt.y&&pt.y<rb.y)
			return true;
		return false;
	}
    final public void setZ(int _z) {
    	z=_z;
    }
    final public int ID(){
    	return ID;
    }
    final public void setLT(Point _lt) {
    	lt.setLocation(_lt);
    	area.setH(rb.y-lt.y);
    	area.setW(rb.x-lt.x);
    }
    final public void setLT(int x,int y) {
    	lt.setLocation(x,y);
    	area.setH(rb.y-lt.y);
    	area.setW(rb.x-lt.x);
    }
    final public void setRB(Point _rb) {
    	rb.setLocation(_rb);
    	area.setH(rb.y-lt.y);
    	area.setW(rb.x-lt.x);
    }
    final public void setRB(int x,int y) {
    	rb.setLocation(x,y);
    	area.setH(rb.y-lt.y);
    	area.setW(rb.x-lt.x);
    }
    public int size() {
    	return area.size();
    }
	public Point lt() {
		return lt;
	}
	public Point rb() {
		return rb;
	}
	final protected void throwLabel(){
		setChanged();
		notifyObservers();
	}
	public Component(Point lt,int w, int h){
		z = 1000000;
    	ComponentController.goTop(this);
    	ID = RandomID.makeID(this);
    	area = new Rect(w,h);
	}
	public Component(Point lt,Point rb){
		z = 1000000;
    	ComponentController.goTop(this);
    	ID = RandomID.makeID(this);
    	area = new Rect(lt,rb);
	}
	
    public Component(int x,int y,int w,int h) {
    	z = 1000000;
    	ComponentController.goTop(this);
    	ID = RandomID.makeID(this);
    	area = new Rect(w,h);
    	lt=new Point(x,y);
    	rb=new Point(x+w,y+h);
    }
    public static class Compare implements Comparator<Component> {
		@Override
		public int compare(Component arg0, Component arg1) {
			// TODO Auto-generated method stub
			int z1 = arg0.z;
			int z2 = arg1.z;
			return z1 < z2 ? -1 : z1 > z2 ? 1:0;
		}
	}
	abstract void action() ;
}
