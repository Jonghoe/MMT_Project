package datamethod;

import java.util.ArrayList;

import data.EventType;
import data.Label;
import data.MTTEvent;
import data.Panel;

public class EventBundleMethod implements Runnable{
	int ID;
	ArrayList<Label> curList;
	ArrayList<Label> prevList;
	ArrayList<MTTEvent> eventList;
	public EventBundleMethod(int MID,ArrayList<MTTEvent> e,ArrayList<Label> c,ArrayList<Label> p){
		curList=c;
		prevList=p;
		ID = MID;
		eventList = e;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		MTTEvent event = null;
		event = EventFactory.newEvent(EventType.KJH_DESTROY,curList,prevList,ID);
		synchronized(this){
		if(event !=null)
			eventList.add(event);
		else{
			event = EventFactory.newEvent(EventType.KJH_JOOM_IN,curList,prevList,ID);
			if(event !=null)
				eventList.add(event);
			event = EventFactory.newEvent(EventType.KJH_JOOM_OUT,curList,prevList,ID);
			if(event !=null)
				eventList.add(event);
			event = EventFactory.newEvent(EventType.KJH_ROTATE,curList,prevList,ID);
			if(event !=null)
				eventList.add(event);
			}
		}
	}
}
