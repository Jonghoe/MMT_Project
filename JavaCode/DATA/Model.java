package data;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Observable;
import java.util.Observer;

import datamethod.ComponentController;
import datamethod.EventFactory;
import datamethod.LabelInComponent;
import datamethod.RandomID;
import middleprocess.OS;

public abstract  class Model extends Component implements Observer,Runnable{
	// �̺�Ʈ�� �޴µ��� ���
	protected boolean stop;
	protected ArrayList<EventListener> eventListeners;
	protected ArrayList<MTTEvent> events;
	protected ArrayList<Label> prevLabels; 
	protected ArrayList<Label> curLabels;
	protected ArrayList<Component> components;
	
	protected void setLabelComponent(){
		for(Label lb :curLabels)
			lb.setPID(LabelInComponent.findComponent(components, lb));
	}
	private boolean release(){
		boolean ret=false ;
		for(Component c:components)				// ���� ������Ʈ release
			release(c);

		if(this instanceof KFrame)				// �ڱ� �ڽ� release 
			ret|= OS.Instance().release(this);// KFrame�� ��� OS������ release
		ret |= ComponentController.release(this);// this�� ���۷����� ������ �ִ³�鿡�Լ� release
		ret |= RandomID.release(this);			
		deleteObservers();						// ����� ������ ����
		return ret;
	}
	protected boolean release(Component c){
		((Model)c).release();
		if(components.contains(c)){
			components.remove(c);
			return true;
		}
		return false;
	}

	public void add(Component c) throws Exception{
		if(!components.contains(c))
			components.add(c);
		this.addObserver((Model)c);
	}
	
	private void init(){
		curLabels = new ArrayList<Label>();
		prevLabels = new ArrayList<Label>();
		events=new ArrayList<MTTEvent>();
		eventListeners = new ArrayList<EventListener>();
		components = new ArrayList<Component>();
		stop =false;
	}
	// �����ʿ��� �̺�Ʈ ���ο�
	protected void throwEventtoListener(){
		for (EventListener listener : eventListeners)
			listener.action(events);
	}
	/*	HandDrag,Release,Press �� ����� �̺�Ʈ ������ ���ؼ� ���� ����
	 * private void makeEvent(){
		ArrayList<Thread> threadList = new ArrayList<Thread>();
		//�� �гε��� �� ������� �̺�Ʈ�� ����
		for(Component c: components){
			int ID = c.ID();
			Thread th = new Thread(new EventBundleMethod(ID,events,preEvents,curEvents));
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
	}*/
	
	
	// �̺�Ʈ ����
	private void findHandEvent(){
		MTTEvent tmp=null;
		if(prevLabels!=null)
		for(Label l:prevLabels){
			tmp = EventFactory.newEvent(EventType.KJH_HAND_RELEASE,l, curLabels);
			if(tmp != null)
				events.add(tmp);
		}
		for(Label l:curLabels){
			//PRESS �˻�
			tmp = EventFactory.newEvent(EventType.KJH_HAND_PRESS,l, prevLabels);
			if(tmp != null)
				events.add(tmp);
			tmp = null;
			//DRAG �˻�
			tmp = EventFactory.newEvent(EventType.KJH_HAND_DRAG,l, curLabels);
			if(tmp != null)
				events.add(tmp);
		}
	}
	private void makeEvent(){
		//Hand �̺�Ʈ �˻�
		findHandEvent();
	}
	
	@Override
	// update ��ȣ������ this�� ������ �ִ� ������Ʈ���� area,z�� �̿��Ͽ� 
	// ������ label�� ������ component�� id�� label�� id�� �����Ѵ�
	// �׸��� �� label�� �´� id�� �󺧵��� �����ش�.
	// �� �� ���� �󺧵��� �̿��Ͽ� event�� �����.
	// ���� �̺�Ʈ�� �����ʵ鿡�� ������.
	public void update(Observable o, Object arg) {
		// TODO Auto-generated method stub
		if( o instanceof Model ){
			setLabelComponent();					// ���� component�� �´� label id����
			throwLabel();							// label�� id�� �´� component�� ���� ����
			Collections.copy(prevLabels, curLabels);// ���� �� �����̳ʿ� ���� �� �����̳� ���� ����
			curLabels.clear();						// ���� �� ���׾�� ���� ����
			for(Label lb: ((Model)o).curLabels){	// this.ID�� ��ġ�ϴ� ���� �߰� 
				if(lb.ID()==ID)					//
					curLabels.add(lb);				//
			}
			events.clear();						// ���� �̺�Ʈ �����̳� ���� ���� 
			makeEvent();							// �̺�Ʈ ����
			throwEventtoListener();					// �̺�Ʈ �����ʿ��� �̺�Ʈ ����
		}
	}
	@Override
	final public void run(){
		action();
		release();
	}
	// �𵨵��� �ൿ
	protected abstract void action();
	// ������ �߰�
	final public void add(EventListener el){eventListeners.add(el);}
	final public Component component(int idx){return components.get(idx);}
	public Model(int x,int y,int w, int h){
		super(x,y,w,h);
		init();
	}
}

