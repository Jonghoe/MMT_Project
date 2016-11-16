package lowprocess;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Observable;

import javax.swing.JFrame;

public class Sample_Input extends Observable implements Input{
	private int i;
	String data;
	ArrayList<String> datas;
	private JFrame f;
	public Sample_Input(int x,int y){
		datas = new ArrayList<String>();
		f = new JFrame();
		f.setTitle("InputWindow");
		f.setVisible(true);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setSize(x,y);
		i=0;
		f.addMouseListener(new MouseListener(){
			@Override
			public void mouseClicked(MouseEvent e) {
			
			}

			@Override
			public void mousePressed(MouseEvent e) {
				// TODO Auto-generated method stub
				//xxxxyyyLIDmI
				// TODO Auto-generated method stub
				data = "";
				String x=Integer.toString(e.getX());
				String y=Integer.toString(e.getY());
				String LID = Integer.toString(i);
				String MID ="00";
				for (int j=0; j<4-x.length();++j)
					data+="0";
				data+=x;
				for (int j=0; j<3-y.length();++j)
					data+="0";
				data+=y;
				for (int j=0; j<3-LID.length();++j)
					data+="0";
				data+=LID;
				data+=MID;
				setChanged();
				notifyObservers();
				System.out.println("Label info Throw!!: "+data);
				i= (i+1)%150+100;
				
			}

			@Override
			public void mouseReleased(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}
			
		});
	}
	@Override
	public ArrayList<String> sends() {
		// TODO Auto-generated method stub
		return datas;
	}
	@Override
	public String send() {
		// TODO Auto-generated method stub
		return data;
	}
	public static void main(String []args){
		Sample_Input A = new Sample_Input(1280,900);
	}
}
