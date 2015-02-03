#ifndef WORLD_H_
#define WORLD_H_

#include <vector>

#include <osg/Group>
#include <osgViewer/Viewer>

#include "macro.h"
#include "abstracttypes.h"
#include "enviroment.h"

namespace amussementpark {

	class World {
		private:
			static World INSTANCE;

			INT curCamera;

			std::vector<StaticCameraAttachment*> cameraAttachments;
			osgViewer::Viewer viewer;			

			ShadowHolder * initializeEnviroment(osg::Group *root);
			void initializeFireworks(osg::Group *root);
			void initializeHUD(osg::Group *root);
			void createAndAddCarrousel(ShadowHolder *root);
			void createAndAddBumperCars(ShadowHolder *root);
			void createAndAddThreadmil(ShadowHolder *root);
		public:
			static World & getInstance();

			World();
			void addCameraAttach(StaticCameraAttachment *attach);
			void tryToSwitchCamera(INT index);
			void updateCamera();
			INT run();
	};
}

#endif
