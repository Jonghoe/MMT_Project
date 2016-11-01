package DATAMethod;

import java.util.ArrayList;

import DATA.EventType;
import DATA.Label;
import DATA.MTTEvent;

public class EventFactory {
	public static MTTEvent Product(EventType t,ArrayList<Label>prev,ArrayList<Label>cur,int PID){
		MTTEvent event=null;
		switch(t){
		case KJH_DESTROY:
			event = EventMethod.MakeDestroy(prev, cur, PID);
			break;
		case KJH_JOOM_IN:
			event = EventMethod.MakeJoomIn(prev, cur, PID);
			break;
		case KJH_JOOM_OUT:
			event = EventMethod.MakeJoomOut(prev, cur, PID);
			break;
		case KJH_ROTATE:
			event = EventMethod.MakeJoomRotate(prev, cur, PID);
			break;
		default:
			break;
		
		}
		return event;
	}
	public static MTTEvent Product(EventType t,Label single,ArrayList<Label>list){
		MTTEvent event=null;
		switch(t){
		case KJH_HAND_DRAG:
			event = EventMethod.MakeHandDrag(single, list);
			break;
		case KJH_HAND_PRESS:
			event = EventMethod.MakeHandPress(single, list);
			break;
		case KJH_HAND_RELEASE:
			event = EventMethod.MakeHandRelease(single, list);
			break;
		default:
			break;
		}
		return event;
	}
}
