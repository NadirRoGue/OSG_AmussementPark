#ifndef CARROUSEL_H_
#define CARROUSEL_H_

#include <string>
#include <vector>

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osg/LightSource>

#include "abstracttypes.h"

namespace amussementpark {


	//#######################################################################
	// Clases que contienen la geometría del tiovivo
	//#######################################################################

	class CarrouselRoof: public osg::ShapeDrawable {
		private:
			DOUBLE rad, height, yPos;

			void initialize();
		public:
			CarrouselRoof(DOUBLE rad, DOUBLE height, DOUBLE yPos);
			CarrouselRoof(CarrouselRoof &);	
	};

	class CarrouselBase: public osg::ShapeDrawable {
		private:
			DOUBLE rad, height, yPos;

			void initialize();
		public:
			CarrouselBase(DOUBLE rad, DOUBLE height, DOUBLE yPos);
			CarrouselBase(CarrouselBase &);
	};

	class HobbyHorse: public MovableObject {
		private:
			BOOLEAN enabled;
			DOUBLE curHeight;
			BOOLEAN goingUp;
			osg::MatrixTransform *animationMatrix;
		public:
			static const DOUBLE MAX_HEIGHT;
			static const DOUBLE MIN_HEIGHT;

			HobbyHorse(DOUBLE x, DOUBLE y, BOOLEAN startsUp, DOUBLE angleCorrection);
			void updateAnimation();
			void onKeyPress();
			void onCtrlKeyPress();
			void onMouseClick();
	};

	class CarrouselLight: public osg::LightSource {
		public:
			CarrouselLight(INT light, DOUBLE x, DOUBLE z);
	};

	//#######################################################################
	// Clases que representan una instancia del tiovivo en el mundo
	//#######################################################################

	class CarrouselUpperNode: public osg::Geode {
		public:
			CarrouselUpperNode(DOUBLE sizeFactor);
	};

	class CarrouselMiddleNode: public osg::Geode {
		public:
			CarrouselMiddleNode(DOUBLE sizeFactor);
	};

	class CarrouselBottomNode: public osg::Geode {
		public:
			CarrouselBottomNode(DOUBLE sizeFactor);
	};

	class CarrouselLightNode: public MovableObject {
		private:
			std::vector<osg::ShapeDrawable*> lights;
			INT currentIndex;
			INT currentCounter;
		public:
			CarrouselLightNode(DOUBLE sizeFactor);
			void onKeyPress();
			void onCtrlKeyPress();
			void onMouseClick();
			void updateAnimation();
	};

	class CarrouselInstance: public MovableObject {
		private:
			BOOLEAN enabled;
			DOUBLE rotationAngle, rotationSpeed;
			osg::MatrixTransform *rotation;

			void initializeWithoutLights(osg::MatrixTransform *cRoot, INT horseCount, DOUBLE sizeFactor);
		public:
			CarrouselInstance(DOUBLE xPos, DOUBLE yPos, INT horseCount, BOOLEAN addLights, DOUBLE sizeFactor=1.0);
			void updateAnimation();
			void onKeyPress();
			void onCtrlKeyPress();
			void onMouseClick();
	};
}

#endif
