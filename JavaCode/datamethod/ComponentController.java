package datamethod;

import java.util.Collections;
import java.util.LinkedList;

import data.Component;
import data.Model;
import data.Panel;

public class ComponentController {
	static private LinkedList<Component> zList = new LinkedList<Component>();
	
	final static private void modifyPanelZ(){
		Collections.sort(zList,new Component.Compare());
		int z=500;
		if(zList.size()>0)
			for(Component p :zList)
				p.setZ(z--);
	}
	final static public void goTop(Component p){
		Collections.sort(zList,new Component.Compare()); 
		if(zList.contains(p)){
			zList.remove(p);
			zList.addFirst(p);
		}
		else
			zList.addFirst(p);
		modifyPanelZ();
	}
	final static public synchronized boolean release(Component p){
		if(zList.contains(p)){
			zList.remove(p);
			return true;
		}
		else
			return false;
	}
}
