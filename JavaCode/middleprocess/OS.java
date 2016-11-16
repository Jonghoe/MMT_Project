package middleprocess;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Observable;
import data.Component;
import data.KFrame;
import data.Label;
import data.Model;
import datamethod.RandomID;
import lowprocess.Input;
import lowprocess.Sample_Input;
import subclass.Receiver;

final public class OS extends Model implements Receiver{
	static private OS singleton = new OS(500,500);
	private Input input;
	boolean readable=true;
	ArrayList<String> datas; 
	private OS(int w, int h) {		
		super(0, 0, w, h);
		// TODO Auto-generated constructor stub
		RandomID.Init();
		datas = new ArrayList<String>();
		Sample_Input tmp = new Sample_Input(w,h);
		input = tmp;
		tmp.addObserver(this);
	}
	
	
	static public OS Instance(){
		return singleton;
	}
	public void add(Component c) throws Exception{
		Exception e = new Exception();
		if(c instanceof KFrame)
			components.add(c);
		throw e;
	}
	public void update(Observable o, Object arg) {
		if(readable){
			stop= true;
			receive();
			if(datas.size()>0)
			for(String s:datas)
				System.out.println("I got data: "+s);
			stop=false;
		}
	}
	@Override
	public void receive() {
		// TODO Auto-generated method stub
		ArrayList<String> tmp = input.sends();
		if(tmp.size()>0)
			Collections.copy(datas, tmp);
		else
			datas.add(input.send());
	}

	private void dataToLabel(){
		if(datas.size()>0){
			for(String s : datas)
				curLabels.add(new Label(s));
		}
			
	}
	private boolean transmiting(){
		int count=0;
		for(Label lb : curLabels)
			if(lb.ID()!= ID&&!lb.Consumed())
				++count;	
		if(count ==0 )
			return true;
		return false;
	}
	protected void action() {
		// TODO Auto-generated method stub
		while(true)	
			if(!stop){
				readable = false;			// 데이터 입력 비활성화			
				dataToLabel();
				setLabelComponent();		// 라벨 설정			
				throwLabel();				// 라벨 전송
				curLabels.clear();			// 라벨 지움
				datas.clear();				// 데이터 지움
				readable = true;			// 데이터 입력 활성화
				System.out.println("I`m working");
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
	}
}
