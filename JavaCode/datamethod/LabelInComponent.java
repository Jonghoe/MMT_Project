package datamethod;

import java.util.ArrayList;
import java.util.Collections;

import data.Component;
import data.Label;

public class LabelInComponent {
	 static public int findComponent(ArrayList<Component>components,Label l){
		ArrayList<Component> Possible=new ArrayList<Component>();
		for(int i=0;i<components.size();++i)
			if(components.get(i).isContain(l.center()))
				Possible.add(components.get(i));
		
		Collections.sort(Possible,new Component.Compare());
		if(Possible.size()>0)
			return Possible.get(0).ID();
		else
			return -1;
	}
}
