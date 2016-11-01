package DATAMethod;

import java.util.Random;

import DATA.Component;
import DATA.Model;

public class RandomID {
	static final private int Limit = 10000;
	static private Model[]m;
	static private Component[]c;
	private static RandomID Singleton=null;
	private RandomID(){
		Singleton= this;
		c=new Component[Limit];
		m=new Model[Limit];
		for(int i=0;i<Limit;++i)
			c[i]=null;
		for(int i=0;i<Limit;++i)
			m[i]=null;
	}
	static public int MakeID(Object o){
		if(Singleton==null)
			return -1;
		int ID=0;
		Random r = new Random();
		if(o instanceof Component){
			for(ID=0;c[ID]==null;ID=r.nextInt()%Limit);
			c[ID]=(Component)o;
		}
		else if(o instanceof Model){
			for(ID=0;m[ID]==null;ID=r.nextInt()%Limit);
			m[ID]=(Model)o;
		}
		return ID;
	}
	static public RandomID Create(){
		if(Singleton == null)
			Singleton = new RandomID();
		return Singleton;
	}
}
