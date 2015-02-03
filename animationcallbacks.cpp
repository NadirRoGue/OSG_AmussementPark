#include "animationcallbacks.h"
#include "carrousel.h"
#include "bumpercars.h"
#include "world.h"

using namespace osg;
using namespace amussementpark;

//#############################################################################

AnimationCallBack::AnimationCallBack() {
}

void AnimationCallBack::operator()(Node *node, NodeVisitor *nv) {
	
	MovableObject *m = static_cast<MovableObject*>(node);
	m->updateAnimation();
	traverse(node, nv);
}

//#############################################################################

CameraUpdateCallBack::CameraUpdateCallBack(){
}

void CameraUpdateCallBack::operator()(Node *node, NodeVisitor *nv) {
	
	World::getInstance().updateCamera();
	traverse(node, nv);
}
