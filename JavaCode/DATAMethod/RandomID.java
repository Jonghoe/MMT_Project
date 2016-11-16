package datamethod;

import java.util.Random;

import data.Component;
import data.Model;

public class RandomID {
	static final private int Limit = 10000;
	static private Object[] list=new Object[Limit];
	static public void Init(){
		for(int i=0;i<Limit;++i)
			list[i]=null;
	}
	static public synchronized boolean release(Object o){
		
		if(o instanceof Component){
			Component com = (Component)o;
			list[com.ID()]=null;
			return true;
		}
		return false;
	}
	static public int makeID(Object o){
		int ID=0;
		Random r = new Random();
		for(ID=0;list[ID]!=null;ID=r.nextInt()%Limit);
		list[ID]=o;
		return ID;
	}
}
