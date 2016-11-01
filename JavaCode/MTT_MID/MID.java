package MTT_MID;

import java.util.ArrayList;
import java.util.Observable;

import DATA.EventType;
import DATA.Label;
import DATA.MTTEvent;
import DATA.Panel;
import DATAMethod.EventBundleMethod;
import DATAMethod.EventFactory;
import DATAMethod.EventMethod;
import DATAMethod.LabelInPanel;


public class MID extends Observable implements Runnable {
	static private MID Sigleton = null;
	private ArrayList<Label> prevList; 
	private ArrayList<Label> curList; 
	private ArrayList<Panel> panelList;
	private ArrayList<MTTEvent> eventList;
	private boolean Readable;
	private MID(){
		Sigleton=this;
		Readable=true;
		panelList = new ArrayList<Panel>();
		curList = new ArrayList<Label>();
		eventList = new ArrayList<MTTEvent>();
		
	}
	
	private void GetDataString(){
		//소켓 통신을 통해 LABEL 데이터를 String타입으로 받는 함수		
		AddLabel(new Label());
	}	
	private void AddLabel(Label l){
		//현재 프레임 리스트에 라벨 추가
		curList.add(l);
	}
	private void SetLabelPanel(){
		//현재 프레임리스트( 새로 받은 라벨들)의 패널 조정
		for(int i=0;i<curList.size();++i){
			Label l = curList.get(i);
			l.SetPID(LabelInPanel.FindPanel(panelList, l));
		}
	}
	private void FindHandReleaseEvent(){
		MTTEvent tmp=null;
		for(Label l:prevList){
			tmp = EventFactory.Product(EventType.KJH_HAND_RELEASE,l, curList);
			if(tmp != null)
				eventList.add(tmp);
		}
	}
	private void FindHandPressEvent(){
		MTTEvent tmp=null;
		for(Label l:curList){
			tmp = EventFactory.Product(EventType.KJH_HAND_PRESS,l, prevList);
			if(tmp != null)
				eventList.add(tmp);
		}
	}
	private void FindHandDragEvent(){
		MTTEvent tmp=null;
		for(Label l:curList){
			tmp = EventFactory.Product(EventType.KJH_HAND_DRAG,l, curList);
			if(tmp != null)
				eventList.add(tmp);
		}
	}
	private void MakeEvent(){
		//HandRelease 이벤트 검색
		FindHandReleaseEvent();
		//HandPress 이벤트 검색
		FindHandPressEvent();
		//HandDrag 이벤트 검색
		FindHandDragEvent();
		//고수준 이벤트들은 패널 단위로 검색
		//쓰레드 대기를 위한 리스트
		ArrayList<Thread> threadList = new ArrayList<Thread>();
		//각 패널들을 한 쓰레드로 이벤트를 생성
		for(int i=0;i<panelList.size();++i){
			Thread th = new Thread(new EventBundleMethod(panelList.get(0).GetID(),eventList,prevList,curList));
			th.start();
			threadList.add(th);
		}
		//쓰레드 대기
		for(Thread t:threadList){
			try {
				t.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	static public MID CreateMID(){
		if(Sigleton ==null)
			return new MID();
		else
			return Sigleton;
	}
	public ArrayList<MTTEvent> GetEventList(){return eventList;}
	@Override
	public void run() {
		//소켓을 통해 데이터를 받음
		while(true){
			//소켓통신을 통해 데이터 생성
			GetDataString();
			Readable = false;
			//생성된 데이터 패널과 연결
			SetLabelPanel();
			//이벤트 생성
			MakeEvent();
			//만들어진 이벤트 옵저버들에게 뿌림
			notifyObservers();
			//라벨목록을 변경
			prevList = curList;
			curList= new ArrayList<Label>();
			Readable = true;
			
		}
	}
}
