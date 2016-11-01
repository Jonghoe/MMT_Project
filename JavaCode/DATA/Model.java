package DATA;

import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

import DATAMethod.RandomID;
import MTT_MID.MID;

public class Model implements Observer{
	Observable sub;
	ArrayList<MTTEvent> eventList;
	Panel pl;
	int ID;
	Model(int id){
		sub = MID.CreateMID();
		sub.addObserver(this);
		pl = new Panel();
		ID = RandomID.MakeID(this);
		eventList=null;
	}
	private void ThrowEvent(MTTEvent e,ArrayList<MTTEvent> list){
		
	}
	@Override
	public void update(Observable o, Object arg) {
		// TODO Auto-generated method stub
		if( o instanceof MID ){
			eventList = ((MID)o).GetEventList();
			for(MTTEvent event :eventList){
				
			}
		}
	}

}
