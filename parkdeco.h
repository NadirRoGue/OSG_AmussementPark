#ifndef PARKDECO_H_
#define PARKDECO_H_

#include "abstracttypes.h"

namespace amussementpark {

	class TrashCan: public osg::Geometry {
		private:
			DOUBLE bigRadius;
			DOUBLE smallRadius;
			DOUBLE height;
		public:
			TrashCan(DOUBLE bigR, DOUBLE smallR, DOUBLE h);
			TrashCan(TrashCan &);
	};

	class TicketSellPoint: public osg::Geometry {
		private:
			DOUBLE xWidth;
			DOUBLE yWidth;
			DOUBLE height;
		public:
			TicketSellPoint(DOUBLE xW, DOUBLE yW, DOUBLE h);
			TicketSellPoint(DOUBLE w, DOUBLE h);
			TicketSellPoint(DOUBLE side);
			TicketSellPoint(TicketSellPoint &);
	};
}

#endif
