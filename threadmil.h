#ifndef THREADMIL_H_
#define THREADMIL_H_

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>

#include "abstracttypes.h"
#include "macro.h"

namespace amussementpark {
	
	class RingBar: public osg::Geode {
		public:
			RingBar(osg::Vec3 center, DOUBLE xLen, DOUBLE yLen, DOUBLE zLen);
	};

	class ThreadmilRing: public osg::Group {
		public:
			ThreadmilRing(DOUBLE radius, DOUBLE numSides);
	};

	class Cabinas: public osg::Group {
		private:
			FLOAT radio;
			INT lados;
			osg::ref_ptr<osg::MatrixTransform> transf1;
			osg::ref_ptr<osg::MatrixTransform> transf2;
		public:
    			Cabinas(FLOAT radio, INT lados);
			osg::MatrixTransform * getAnimationCabina();  
	};

	class Estructura: public osg::Group {
		private:
			osg::ref_ptr<osg::Geode> cuboEstructura1;
			osg::ref_ptr<osg::Geode> cuboEstructura2;			
			osg::ref_ptr<osg::Geode> cuboEstructura3;
			osg::ref_ptr<osg::MatrixTransform> transf1;	//scale
			osg::ref_ptr<osg::MatrixTransform> transf2; //rotate
			osg::ref_ptr<osg::MatrixTransform> transf3;	//rotate2
			osg::ref_ptr<osg::MatrixTransform> transf4;	//rotate3	
			osg::ref_ptr<osg::MatrixTransform> transf5;	//rotate3
			osg::ref_ptr<osg::MatrixTransform> transf6;	//translate
			osg::ref_ptr<osg::ShapeDrawable> sesfera;
			osg::ref_ptr<osg::Geode> esfera;
			osg::ref_ptr<osg::MatrixTransform> transf_esf; //translate esfera juntura
		public:
    			Estructura();	  
	};

	class ThreadmilInstance: public MovableObject {
		private:
			DOUBLE speed;
			DOUBLE speedIncrease;
			BOOLEAN enabled;
			osg::MatrixTransform* ringMovement;
			osg::ref_ptr<Cabinas> cabinas;
		public:
			ThreadmilInstance();
			void onKeyPress();
			void onCtrlKeyPress();
			void onMouseClick();
			void updateAnimation();
	};
}

#endif
