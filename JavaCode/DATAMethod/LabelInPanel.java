package DATAMethod;

import java.util.ArrayList;

import DATA.Label;
import DATA.Panel;

public class LabelInPanel {
	static public int FindPanel(ArrayList<Panel>p,Label l){
		ArrayList<Panel> Possible=new ArrayList<Panel>();
		for(int i=0;i<p.size();++i)
			if(p.get(i).IsGet(l))
				Possible.add(p.get(i));
		Possible.sort(null);
		return Possible.get(0).GetID();	
	}
	
}
