package highprocess;

import javax.swing.JFrame;

import middleprocess.OS;

public class OSWrapper extends JFrame{
	OS os ;
	OSWrapper(){
		os=OS.Instance();
		setTitle("MTTWindow");
		setVisible(true);
		setSize(os.rb().x-os.lt().x,os.rb().y-os.lt().y);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		new Thread(os).start();
	}
}
