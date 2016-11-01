package DATAMethod; 
import java.util.ArrayList;

import DATA.*;

public class EventMethod {
	//KJH_JOOM_IN,KJH_JOOM_OUT,KJH_DESTROY,KJH_ROTATE
	static public MTTEvent MakeHandRelease(Label prev,ArrayList<Label>cur){
		//이전 라벨리스트와 현재 라벨 리스트를 비교하여 HADN_UP 이벤트 생성
		if(!cur.contains(prev))
			return new MTTEventRelease(prev.GetPID(),prev.GetPT());
			
		
		return null;
			
	}
	static public MTTEvent MakeHandPress(Label cur,ArrayList<Label>prev){
		//이전 라벨리스트와 현재 라벨 리스트를 비교하여 HADN_UP 이벤트 생성
		if(!prev.contains(cur))
			return new MTTEventPress(cur.GetPID(),cur.GetPT());

		return null;
	}
	static public MTTEvent MakeHandDrag(Label cur,ArrayList<Label>prev){
		if(!prev.contains(cur))
			return new MTTEventDrag(cur.GetPID(),cur.GetPT());
	
		return null;
	}
	static public MTTEvent MakeJoomIn(ArrayList<Label>prev,ArrayList<Label>cur,int PID){
		if(prev.size()<2 || cur.size()<2)
			return null;
		
		double prArea =RectMethod.Create(prev).GetArea();
		double curArea =RectMethod.Create(cur).GetArea();
		
		return new MTTEventJoomIn(PID,curArea/prArea);			
	}
	
	public static MTTEvent MakeJoomOut(ArrayList<Label> prev, ArrayList<Label> cur, int pID) {
		if(prev.size()<2 || cur.size()<2)
			return null;
		
		double prArea =RectMethod.Create(prev).GetArea();
		double curArea =RectMethod.Create(cur).GetArea();
		
		return new MTTEventJoomOut(pID,curArea/prArea);
	}
	public static MTTEvent MakeJoomRotate(ArrayList<Label> prev, ArrayList<Label> cur, int pID) {
		// TODO Auto-generated method stub
		return null;
	}
	public static MTTEvent MakeDestroy(ArrayList<Label> prev, ArrayList<Label> cur, int pID) {
		if(prev.size()<2 || cur.size()<2)
			return null;
		// TODO Auto-generated method stub
		double prArea =RectMethod.Create(prev).GetArea();
		double curArea =RectMethod.Create(cur).GetArea();
		if(prArea>curArea&& curArea<5.0)
			return  new MTTEventDestory(pID);
		return null;
	}
}


