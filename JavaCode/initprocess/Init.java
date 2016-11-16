package initprocess;

import datamethod.RandomID;
import middleprocess.OS;
 
public class Init {
	public static void main(String []args){
		OS os = OS.Instance();
		new Thread(os).start();
	}
}
