#ifndef KEYBOARD_HANDLERS_H_
#define KEYBOARD_HANDLERS_H_

#include <osgGA/GUIEventHandler>
#include <osg/Node>
#include <osg/Geode>
#include <osg/MatrixTransform>

#include "abstracttypes.h"
#include "macro.h"

namespace amussementpark {

	class ToggleNodeKeyboardHandler: public osgGA::GUIEventHandler {
		private:
			osg::Node::NodeMask defaultMask;
			osg::ref_ptr<osg::Geode> geode;
			osgGA::GUIEventAdapter::KeySymbol key;
		public:
			ToggleNodeKeyboardHandler(osg::Geode *g, osgGA::GUIEventAdapter::KeySymbol k);
			virtual BOOLEAN handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	};

	class HelpMenuKeyboardHandler: public ToggleNodeKeyboardHandler {
		public:
			HelpMenuKeyboardHandler(osg::Geode *g);
	};

	class DebuggerKeyboardHandler: public ToggleNodeKeyboardHandler {
		public:
			DebuggerKeyboardHandler(osg::Geode *g);
	};

	class UserInputHandler: public osgGA::GUIEventHandler {
		private:
			ManipulableObject *obj;
			CHAR actionKey;
		public:
			UserInputHandler(ManipulableObject *o, CHAR key);
			virtual BOOLEAN handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	};

	class MouseInputHandler: public osgGA::GUIEventHandler {
		public:
			MouseInputHandler();
			virtual BOOLEAN handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	};

	class CameraSwitchHandler: public osgGA::GUIEventHandler {
		public:
			CameraSwitchHandler();
			virtual BOOLEAN handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	};
}

#endif
