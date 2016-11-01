package DATA;

import java.awt.Point;

public class MTTEventRelease extends MTTEvent {

	public MTTEventRelease(int id,Point pt) {
		super(id,pt);
		// TODO Auto-generated constructor stub
	}

	@Override
	public double GetRate() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Point GetPoint() {
		// TODO Auto-generated method stub
		return pt;
	}

}
