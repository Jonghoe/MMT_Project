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
		//���� ����� ���� LABEL �����͸� StringŸ������ �޴� �Լ�		
		AddLabel(new Label());
	}	
	private void AddLabel(Label l){
		//���� ������ ����Ʈ�� �� �߰�
		curList.add(l);
	}
	private void SetLabelPanel(){
		//���� �����Ӹ���Ʈ( ���� ���� �󺧵�)�� �г� ����
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
		//HandRelease �̺�Ʈ �˻�
		FindHandReleaseEvent();
		//HandPress �̺�Ʈ �˻�
		FindHandPressEvent();
		//HandDrag �̺�Ʈ �˻�
		FindHandDragEvent();
		//����� �̺�Ʈ���� �г� ������ �˻�
		//������ ��⸦ ���� ����Ʈ
		ArrayList<Thread> threadList = new ArrayList<Thread>();
		//�� �гε��� �� ������� �̺�Ʈ�� ����
		for(int i=0;i<panelList.size();++i){
			Thread th = new Thread(new EventBundleMethod(panelList.get(0).GetID(),eventList,prevList,curList));
			th.start();
			threadList.add(th);
		}
		//������ ���
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
		//������ ���� �����͸� ����
		while(true){
			//��������� ���� ������ ����
			GetDataString();
			Readable = false;
			//������ ������ �гΰ� ����
			SetLabelPanel();
			//�̺�Ʈ ����
			MakeEvent();
			//������� �̺�Ʈ �������鿡�� �Ѹ�
			notifyObservers();
			//�󺧸���� ����
			prevList = curList;
			curList= new ArrayList<Label>();
			Readable = true;
			
		}
	}
}
