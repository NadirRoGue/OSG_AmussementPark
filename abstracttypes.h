#ifndef ABSTRACTTYPES_H_
#define ABSTRACTTYPES_H_

#include <string>
#include <vector>

#include <osg/Group>

#include "macro.h"

namespace amussementpark {

	class ManipulableObject: public osg::Group {
		public:
			ManipulableObject();
			virtual void onKeyPress() = 0;
			virtual void onCtrlKeyPress() = 0;
			virtual void onMouseClick() = 0;
	};

	class MovableObject: public ManipulableObject {
		public:
			MovableObject();
			virtual void updateAnimation() = 0;
	};

	class ICollidableObject {
		protected:
			std::vector<ICollidableObject*> knownList;
			DOUBLE x, y;
			DOUBLE heading;
			DOUBLE radio;
		public:
			ICollidableObject();
			~ICollidableObject();
			void addToKnownList(ICollidableObject *obj);
			virtual BOOLEAN collides() = 0;
			virtual BOOLEAN isBeingCollided(DOUBLE x, DOUBLE y) = 0;
	};

	class CircleCollidableObject: public ICollidableObject {
		public:
			CircleCollidableObject();
			BOOLEAN collides();
			BOOLEAN isBeingCollided(DOUBLE x, DOUBLE y);
	};

	class SquareCollidableObject: public ICollidableObject {
		protected:
			DOUBLE widthParam;
		public:
			SquareCollidableObject();
			BOOLEAN collides();
			BOOLEAN isBeingCollided(DOUBLE x, DOUBLE y);
	};

	class StaticCameraAttachment {
		private:
			DOUBLE xFix;
			DOUBLE yFix;
			DOUBLE zFix;
			DOUBLE headingFix;
		public:
			StaticCameraAttachment(DOUBLE xFix, DOUBLE yFix, DOUBLE zFix, DOUBLE headingFix);
			osg::Vec3 calcCenter();
			osg::Vec3 calcEye();
			osg::Vec3 calcUp();
			virtual DOUBLE getX() = 0;
			virtual DOUBLE getY() = 0;
			virtual DOUBLE getZ() = 0;
			virtual DOUBLE getHeading() = 0;
	};
}

#endif
