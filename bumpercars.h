#ifndef BUMPERCARS_H_
#define BUMPERCARS_H_

#include <vector>

#include <osg/Geode>

#include "abstracttypes.h"

namespace amussementpark {

	class BumperCarsStructRoof: public osg::Group {
		public:
			BumperCarsStructRoof();
	};

	class BumperCarsTrack: public osg::Group {
		public:
			BumperCarsTrack(DOUBLE xWidth, DOUBLE yWidth);
	};

	class BumperCarStructBorder: public osg::Geode, public SquareCollidableObject {
		public:
			BumperCarStructBorder(DOUBLE x, DOUBLE y, DOUBLE z, DOUBLE xWidth, DOUBLE yWidth, DOUBLE heading);
	};

	class BumperCar: public MovableObject, public CircleCollidableObject, public StaticCameraAttachment {
		private:
			BOOLEAN enabled;
			DOUBLE speed;
			DOUBLE rotationSpeed;
			osg::MatrixTransform *animationMatrix;
		public:
			BumperCar(DOUBLE x, DOUBLE y, osg::Vec4 carColor);
			void updateAnimation();
			void onKeyPress();
			void onCtrlKeyPress();
			void onMouseClick();
			DOUBLE getX();
			DOUBLE getY();
			DOUBLE getZ();
			DOUBLE getHeading();
	};

	class BumperCarsBuild: public ManipulableObject {
		private:
			std::vector<BumperCar*> carsV;

			void createRoof(DOUBLE xW, DOUBLE yW, DOUBLE borderFix);
			void createBorders(DOUBLE xW, DOUBLE yW, DOUBLE borderW, std::vector<BumperCarStructBorder*> &feedBack);
			void createDeco(DOUBLE xW, DOUBLE yW, DOUBLE bW);
			void createCars(DOUBLE xW, DOUBLE yW, INT cars, std::vector<BumperCarStructBorder*> &feedBack);
		public:
			BumperCarsBuild(INT cars);
			void onKeyPress();
			void onCtrlKeyPress();
			void onMouseClick();
	};
}

#endif
