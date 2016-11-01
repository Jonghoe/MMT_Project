package DATA;

import java.awt.Point;

public class MTTEventJoomOut extends MTTEvent {
	double rate;

	public MTTEventJoomOut(int pID, double d) {
		super(pID,null);
		// TODO Auto-generated constructor stub
		rate = d;
	}

	@Override
	public double GetRate() {
		// TODO Auto-generated method stub
		return rate;
	}

	@Override
	public Point GetPoint() {
		// TODO Auto-generated method stub
		return null;
	}
}
