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
	// 이벤트를 받는동안 대기
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
		for(Component c:components)				// 하위 컴포넌트 release
			release(c);

		if(this instanceof KFrame)				// 자기 자신 release 
			ret|= OS.Instance().release(this);// KFrame일 경우 OS에서도 release
		ret |= ComponentController.release(this);// this의 레퍼런스를 가지고 있는놈들에게서 release
		ret |= RandomID.release(this);			
		deleteObservers();						// 등록한 옵저버 삭제
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
	// 리스너에게 이벤트 쓰로우
	protected void throwEventtoListener(){
		for (EventListener listener : eventListeners)
			listener.action(events);
	}
	/*	HandDrag,Release,Press 외 고수준 이벤트 생성에 대해선 아직 미정
	 * private void makeEvent(){
		ArrayList<Thread> threadList = new ArrayList<Thread>();
		//각 패널들을 한 쓰레드로 이벤트를 생성
		for(Component c: components){
			int ID = c.ID();
			Thread th = new Thread(new EventBundleMethod(ID,events,preEvents,curEvents));
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
	}*/
	
	
	// 이벤트 추출
	private void findHandEvent(){
		MTTEvent tmp=null;
		if(prevLabels!=null)
		for(Label l:prevLabels){
			tmp = EventFactory.newEvent(EventType.KJH_HAND_RELEASE,l, curLabels);
			if(tmp != null)
				events.add(tmp);
		}
		for(Label l:curLabels){
			//PRESS 검색
			tmp = EventFactory.newEvent(EventType.KJH_HAND_PRESS,l, prevLabels);
			if(tmp != null)
				events.add(tmp);
			tmp = null;
			//DRAG 검색
			tmp = EventFactory.newEvent(EventType.KJH_HAND_DRAG,l, curLabels);
			if(tmp != null)
				events.add(tmp);
		}
	}
	private void makeEvent(){
		//Hand 이벤트 검색
		findHandEvent();
	}
	
	@Override
	// update 신호가오면 this가 가지고 있는 컴포넌트들의 area,z를 이용하여 
	// 각각의 label을 가지는 component의 id로 label의 id를 설정한다
	// 그리고 각 label의 맞는 id로 라벨들을 던져준다.
	// 그 후 남은 라벨들을 이용하여 event를 만든다.
	// 만든 이벤트를 리스너들에게 던진다.
	public void update(Observable o, Object arg) {
		// TODO Auto-generated method stub
		if( o instanceof Model ){
			setLabelComponent();					// 하위 component에 맞는 label id설정
			throwLabel();							// label의 id에 맞는 component로 라벨을 던짐
			Collections.copy(prevLabels, curLabels);// 이전 라벨 컨테이너에 현재 라벨 컨테이너 내용 복사
			curLabels.clear();						// 현재 라벨 컨테어니 내용 지움
			for(Label lb: ((Model)o).curLabels){	// this.ID와 일치하는 라벨을 추가 
				if(lb.ID()==ID)					//
					curLabels.add(lb);				//
			}
			events.clear();						// 현재 이벤트 컨테이너 내용 지움 
			makeEvent();							// 이벤트 생성
			throwEventtoListener();					// 이벤트 리스너에게 이벤트 던짐
		}
	}
	@Override
	final public void run(){
		action();
		release();
	}
	// 모델들의 행동
	protected abstract void action();
	// 리스너 추가
	final public void add(EventListener el){eventListeners.add(el);}
	final public Component component(int idx){return components.get(idx);}
	public Model(int x,int y,int w, int h){
		super(x,y,w,h);
		init();
	}
}

