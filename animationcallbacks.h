#ifndef ANIMATIONCALLBACKS_H_
#define ANIMATIONCALLBACKS_H_

#include <osg/NodeCallback>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/MatrixTransform>
#include <osg/Drawable>
#include <osgViewer/Viewer>
#include <osgText/Text>

#include <cstdlib>
#include <sstream>
#include <sys/time.h>

#include "macro.h"

namespace amussementpark {

	struct HUDDebuggerPosCallBack: public osg::Drawable::UpdateCallback {
		private:
			osgViewer::Viewer *viewer;
		public:
			HUDDebuggerPosCallBack(osgViewer::Viewer *view) {
				viewer = view;
			}

			virtual void update(osg::NodeVisitor *nv, osg::Drawable *node) {
				osgText::Text *t = static_cast<osgText::Text*>(node);
			
				osg::Vec3 eye, center, up;
				viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);

				std::ostringstream strs;
				strs << "Viewer x = " << eye.x() << "; y = " << eye.y() << "; z = " << eye.z();
				t->setText(strs.str());
			}
	};

	struct HUDDebuggerCallBack: public osg::Drawable::UpdateCallback {
		private:
			struct timeval start;
			INT updateCounter;
		public:
			HUDDebuggerCallBack() {
				gettimeofday(&start, NULL);
				updateCounter = 0;
			}

			DOUBLE updateFPS() {
				struct timeval now;
				gettimeofday(&now, NULL);
				
				DOUBLE timeNow = now.tv_sec + (now.tv_usec / 1000000.0);
				DOUBLE timeStart = start.tv_sec + (start.tv_usec / 1000000.0);
				DOUBLE dif = timeNow - timeStart;

				DOUBLE fps = 0.0;
				if(dif > 0.0)
					fps = 10.0 / dif;
				gettimeofday(&start, NULL);

				return fps;
			}

			virtual void update(osg::NodeVisitor *nv, osg::Drawable *node) {
				if(updateCounter > 10) {
					osgText::Text *t = static_cast<osgText::Text*>(node);
					std::ostringstream strs;
					strs << "Frames per second (FPS): " << (DWORD32)updateFPS();
					t->setText(strs.str());
					updateCounter = 0;
				} else
					updateCounter++;
			}
	};
	
	class AnimationCallBack: public osg::NodeCallback {
		public:
			AnimationCallBack();
			void operator()(osg::Node *node, osg::NodeVisitor *nv);
	};

	class CameraUpdateCallBack: public osg::NodeCallback {
		public:
			CameraUpdateCallBack();
			void operator()(osg::Node *node, osg::NodeVisitor *nv);
	};

	class ThreadMilWagonFixCallBack: public osg::NodeCallback {
		public:
			ThreadMilWagonFixCallBack();
			void operator()(osg::Node *node, osg::NodeVisitor *nv);
	};

	class LightSequenceCallBack: public osg::NodeCallback {
		public:
			LightSequenceCallBack();
			void operator()(osg::Node *node, osg::NodeVisitor *nv);
	};

	class SunMovementCallBack: public osg::NodeCallback {
		public:
			SunMovementCallBack();
			void operator()(osg::Node *node, osg::NodeVisitor *nv);
	};
}

#endif
