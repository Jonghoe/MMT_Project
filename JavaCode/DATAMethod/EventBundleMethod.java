package DATAMethod;

import java.util.ArrayList;

import DATA.EventType;
import DATA.Label;
import DATA.MTTEvent;
import DATA.Panel;

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
		event = EventFactory.Product(EventType.KJH_DESTROY,curList,prevList,ID);
		synchronized(this){
		if(event !=null)
			eventList.add(event);
		else{
			event = EventFactory.Product(EventType.KJH_JOOM_IN,curList,prevList,ID);
			if(event !=null)
				eventList.add(event);
			event = EventFactory.Product(EventType.KJH_JOOM_OUT,curList,prevList,ID);
			if(event !=null)
				eventList.add(event);
			event = EventFactory.Product(EventType.KJH_ROTATE,curList,prevList,ID);
			if(event !=null)
				eventList.add(event);
			}
		}
	}
}
