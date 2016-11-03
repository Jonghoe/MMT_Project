package DATAMethod; 
import java.util.ArrayList;

import DATA.*;

public class EventMethod {
	//KJH_JOOM_IN,KJH_JOOM_OUT,KJH_DESTROY,KJH_ROTATE
	static public MTTEvent MakeHandRelease(Label prev,ArrayList<Label>cur){
		//이전 라벨리스트와 현재 라벨 리스트를 비교하여 HADN_UP 이벤트 생성
		if(!cur.contains(prev))
			return new MTTEvent(prev.GetPID(),prev.GetPT(),EventType.KJH_HAND_RELEASE);
			
		
		return null;
			
	}
	static public MTTEvent MakeHandPress(Label cur,ArrayList<Label>prev){
		//이전 라벨리스트와 현재 라벨 리스트를 비교하여 HADN_UP 이벤트 생성
		if(!prev.contains(cur))
			return new MTTEvent(cur.GetPID(),cur.GetPT(),EventType.KJH_HAND_PRESS);

		return null;
	}
	static public MTTEvent MakeHandDrag(Label cur,ArrayList<Label>prev){
		if(!prev.contains(cur))
			return new MTTEvent(cur.GetPID(),cur.GetPT(),EventType.KJH_HAND_DRAG);
	
		return null;
	}
	static public MTTEvent MakeJoomIn(ArrayList<Label>prev,ArrayList<Label>cur,int PID){
		if(prev.size()<2 || cur.size()<2)
			return null;
		
		double prArea =RectMethod.Create(prev).GetArea();
		double curArea =RectMethod.Create(cur).GetArea();
		
		return new MTTEvent(PID,curArea/prArea,EventType.KJH_JOOM_IN);			
	}
	
	static public MTTEvent MakeJoomOut(ArrayList<Label> prev, ArrayList<Label> cur, int pID) {
		if(prev.size()<2 || cur.size()<2)
			return null;
		
		double prArea =RectMethod.Create(prev).GetArea();
		double curArea =RectMethod.Create(cur).GetArea();
		
		return new MTTEvent(pID,curArea/prArea,EventType.KJH_JOOM_OUT);
	}
	static public MTTEvent MakeJoomRotate(ArrayList<Label> prev, ArrayList<Label> cur, int pID) {
		// TODO Auto-generated method stub
		return null;
	}
	static public MTTEvent MakeDestroy(ArrayList<Label> prev, ArrayList<Label> cur, int pID) {
		if(prev.size()<2 || cur.size()<2)
			return null;
		// TODO Auto-generated method stub
		double prArea =RectMethod.Create(prev).GetArea();
		double curArea =RectMethod.Create(cur).GetArea();
		if(prArea>curArea&& curArea<5.0)
			return  new MTTEvent(pID,EventType.KJH_DESTROY);
		return null;
	}
}


