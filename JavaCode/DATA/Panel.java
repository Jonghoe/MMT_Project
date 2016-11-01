package DATA;
import java.util.TreeMap;

public class Panel {
	int PID;
	TreeMap<String,Component> componentList; 
	Panel(){
		componentList=new TreeMap<String,Component>();
	}
	public void Add(Component c){componentList.put(Integer.toString(c.GetID()),c);}
	public void Delete(Component c){
		if(componentList.containsKey(Integer.toString(c.GetID())))
			componentList.remove(c);
	}
	public int GetID(){return PID;}
	public boolean IsGet(Label l){
		return true;
	}


}
