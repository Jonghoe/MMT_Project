package datamethod;

import java.util.ArrayList;

import data.EventType;
import data.Label;
import data.MTTEvent;

public class EventFactory {
	public static MTTEvent newEvent(EventType t,ArrayList<Label>prev,ArrayList<Label>cur,int ID){
		MTTEvent event=null;
		if(prev ==null||cur==null)
			return null;
		switch(t){
		case KJH_DESTROY:
			event = EventMethod.makeDestroy(prev, cur, ID);
			break;
		case KJH_JOOM_IN:
			event = EventMethod.makeJoomIn(prev, cur, ID);
			break;
		case KJH_JOOM_OUT:
			event = EventMethod.makeJoomOut(prev, cur, ID);
			break;
		case KJH_ROTATE:
			event = EventMethod.makeJoomRotate(prev, cur, ID);
			break;
		default:
			break;
		}
		return event;
	}
	
	public static MTTEvent newEvent(EventType t,Label single,ArrayList<Label>list){
		if(list ==null)
			return null;
		if(!list.contains(single))
			return new MTTEvent(single.ID(),single.center(),t);
		return null;
	}
	static private class EventMethod {
		//KJH_JOOM_IN,KJH_JOOM_OUT,KJH_DESTROY,KJH_ROTATE
		static public MTTEvent makeHandRelease(Label prev,ArrayList<Label>cur){
			//이전 라벨리스트와 현재 라벨 리스트를 비교하여 HADN_UP 이벤트 생성
			if(!cur.contains(prev))
				return new MTTEvent(prev.ID(),prev.center(),EventType.KJH_HAND_RELEASE);
			
			return null;
		}
		static public MTTEvent makeHandPress(Label cur,ArrayList<Label>prev){
			//이전 라벨리스트와 현재 라벨 리스트를 비교하여 HADN_UP 이벤트 생성
			if(!prev.contains(cur))
				return new MTTEvent(cur.ID(),cur.center(),EventType.KJH_HAND_PRESS);

			return null;
		}
		static public MTTEvent makeHandDrag(Label cur,ArrayList<Label>prev){
			if(!prev.contains(cur))
				return new MTTEvent(cur.ID(),cur.center(),EventType.KJH_HAND_DRAG);
		
			return null;
		}
		
		
		static public MTTEvent makeJoomIn(ArrayList<Label>prev,ArrayList<Label>cur,int ID){
			
			return null;			
		}
		
		static public MTTEvent makeJoomOut(ArrayList<Label> prev, ArrayList<Label> cur, int ID) {
			
			return null;
		}
		static public MTTEvent makeJoomRotate(ArrayList<Label> prev, ArrayList<Label> cur, int ID) {
			// TODO Auto-generated method stub
			return null;
		}
		static public MTTEvent makeDestroy(ArrayList<Label> prev, ArrayList<Label> cur, int ID) {
			
			return null;
		}
	}
}
