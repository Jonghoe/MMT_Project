package subclass;

import java.util.Observer;

public interface Observable {
	public void addObserver(Observer o);      // 옵저버 등록
    public void deleteObserver(Observer o);   // 옵저버 제거
    public void notifyObservers();            // 옵저버에 데이터 전달
}
