#ifndef ENVIROMENT_H_
#define ENVIROMENT_H_

#include <string>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>

#include <osg/Group>
#include <osg/LightSource>
#include <osg/MatrixTransform>

#include "macro.h"

namespace amussementpark {

	class ShadowHolder: public osgShadow::ShadowedScene {
		public:
			static const INT RECEIVE_SHADOW_MASK;
			static const INT CAST_SHADOW_MASK;

			ShadowHolder();
			void applyMaskToGroup(osg::Group *group, INT mask);
	};

	class Sky: public osg::Geode {
		public:
			Sky(DOUBLE dRadius);
	};
	
	class Floor: public osg::Geode {
		public:
			Floor(DOUBLE width, DOUBLE tileWidth);
	};

	class Bush: public osg::MatrixTransform {
		public:
			Bush(DOUBLE x, DOUBLE y, DOUBLE rotation);
	};

	class Sun: public osg::LightSource {
		public:
			Sun(DOUBLE zValue);	
	};

	class StreetLamp: public osg::MatrixTransform {
		public:
			StreetLamp(DOUBLE x, DOUBLE y, DOUBLE scale);
	};
}

#endif
