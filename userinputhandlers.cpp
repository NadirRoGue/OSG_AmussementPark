#include <cstdlib>
#include <iostream>

#include <osgUtil/LineSegmentIntersector>

#include "userinputhandlers.h"
#include "carrousel.h"
#include "world.h"

using namespace osg;
using namespace amussementpark;

ToggleNodeKeyboardHandler::ToggleNodeKeyboardHandler(Geode *g, osgGA::GUIEventAdapter::KeySymbol k) {
	geode = g;
	defaultMask = geode->getNodeMask();
	key = k;
}

BOOLEAN ToggleNodeKeyboardHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

	switch (ea.getEventType()) {
		case osgGA::GUIEventAdapter::KEYDOWN: {
			if(ea.getKey() == key) {
				if(geode->getNodeMask() == 0)
					geode->setNodeMask(defaultMask);
				else
					geode->setNodeMask(0);
			}
			break;
		}
	}

	return false;
}


//##################################################################

DebuggerKeyboardHandler::DebuggerKeyboardHandler(Geode *g):
	ToggleNodeKeyboardHandler(g, osgGA::GUIEventAdapter::KEY_F3) {
}

//##################################################################

HelpMenuKeyboardHandler::HelpMenuKeyboardHandler(Geode *g):
	ToggleNodeKeyboardHandler(g, osgGA::GUIEventAdapter::KEY_F1) {
	g->setNodeMask(0);
}

//##################################################################

UserInputHandler::UserInputHandler(ManipulableObject *o, CHAR key) {
	obj = o;
	actionKey = key;
}

BOOLEAN UserInputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	
	switch(ea.getEventType()) {
		case osgGA::GUIEventAdapter::KEYDOWN: {
			CHAR curKey = ea.getUnmodifiedKey();
			if(curKey == actionKey) {
				if(ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL)
					obj->onCtrlKeyPress();	// Ctrl + actionKey pressed
				else
					obj->onKeyPress();	// actionKey pressed
			}
			break;
		}
	}
	
	return false;
}

//##################################################################

MouseInputHandler::MouseInputHandler() {
}

BOOLEAN MouseInputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	switch(ea.getEventType()) {
		case osgGA::GUIEventAdapter::RELEASE: {
			if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
				osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
				if ( viewer ) {
					osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
						new osgUtil::LineSegmentIntersector(
						osgUtil::Intersector::WINDOW, ea.getX(), ea.getY()
					);

					osgUtil::IntersectionVisitor iv( intersector.get() );
					iv.setTraversalMask( ~0x1 );
					viewer->getCamera()->accept( iv );

					if ( intersector->containsIntersections() ) {
						osgUtil::LineSegmentIntersector::Intersection result = 
							*(intersector->getIntersections().begin());

						osg::Drawable *drawable = result.drawable;
						if(drawable == NULL || drawable->getNumParents() == 0)
							return false;

						osg::Node *parent = result.drawable->getParent(0);
						ManipulableObject *obj = NULL;
						while(parent != NULL && parent->getNumParents() > 0 
							&& (obj = dynamic_cast<ManipulableObject*>(parent)) == NULL) {
							parent = parent->getParent(0);
						}

						if(obj != NULL)
							obj->onMouseClick();
					}
				}
			}
			break;
		}
	}

	return false;
}

//##################################################################

CameraSwitchHandler::CameraSwitchHandler() {
}

BOOLEAN CameraSwitchHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	if(ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN) {
		CHAR key = ea.getKey();
		if(key >= '0' && key <= '9') {
			INT value = atoi(&key);
			World::getInstance().tryToSwitchCamera(value);
		}
	}
}
