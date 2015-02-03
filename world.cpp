#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/Geode>
#include <osg/LightModel>
#include <osg/ClearNode>
#include <osg/GraphicsContext>
#include <osgUtil/Optimizer>
#include <osgGA/TrackballManipulator>
#include <osgParticle/ParticleSystemUpdater>
#include <osgDB/ReadFile>

#include <iostream>

#include "util.h"
#include "world.h"
#include "carrousel.h"
#include "bumpercars.h"
#include "threadmil.h"
#include "userinputhandlers.h"
#include "animationcallbacks.h"
#include "particle.h"

using namespace std;
using namespace osg;
using namespace amussementpark;

World World::INSTANCE;

World & World::getInstance() {
	return INSTANCE;
}

World::World() {
	MeshTable::getInstance().initialize();
	
	ref_ptr<Group> root = new Group;
	root->setUpdateCallback(new CameraUpdateCallBack);

	ref_ptr<ClearNode> clearNode = new ClearNode;
    	clearNode->setClearColor(Vec4(1.0f,1.0f,1.0f,1.0f));
    	root->addChild(clearNode);

	ShadowHolder * holder = initializeEnviroment(root);
	initializeHUD(root);
	initializeFireworks(root);
	createAndAddCarrousel(holder);
	createAndAddBumperCars(holder);
	createAndAddThreadmil(holder);
	osgUtil::Optimizer optimzer;
	optimzer.optimize( root );

	viewer.setSceneData( root );

	ref_ptr<osgGA::TrackballManipulator> m = new osgGA::TrackballManipulator;
	m->setHomePosition(Vec3(1.63,-47.4,9.5), Vec3(0,0,5), Vec3(0,0,1));
	viewer.setCameraManipulator(m);
	
	viewer.addEventHandler( new MouseInputHandler );
	viewer.addEventHandler(new CameraSwitchHandler);
}

void World::initializeHUD(Group *root) {
	GraphicsContext::WindowingSystemInterface* wsi = GraphicsContext::getWindowingSystemInterface(); 
	DWORD32 width, height;
	wsi->getScreenResolution(GraphicsContext::ScreenIdentifier(0), width, height); 
	HUDDebugText hud(width, height, &viewer);
	root->addChild(hud.getProjection());
}

ShadowHolder * World::initializeEnviroment(Group *root) {
	// Cielo
	ref_ptr<Sky> sky = new Sky(80.0);
	root->addChild(sky);

	// Sol
	ref_ptr<Sun> sun = new Sun(80.0);
	root->addChild(sun);

	// Escena con sombras
	ref_ptr<ShadowHolder> se = new ShadowHolder;
	sun->addChild(se);
	
	// Suelo
	ref_ptr<Floor> floor = new Floor(160.0, 2.0);
	floor->setNodeMask(ShadowHolder::RECEIVE_SHADOW_MASK);
	se->addChild(floor);

	ref_ptr<MatrixTransform> rescale = new MatrixTransform(Matrix::scale(0.03, 0.03, 0.03));

	ref_ptr<StreetLamp> f1 = new StreetLamp(0,0, 0.03);
	ref_ptr<StreetLamp> f2 = new StreetLamp(-6, -2, 0.03);
	ref_ptr<StreetLamp> f3 = new StreetLamp(7, -2, 0.03);
	rescale->addChild(f1);
	rescale->addChild(f2);
	rescale->addChild(f3);

	se->addChild(rescale);

	DOUBLE start = -10.0;
	for(INT i = 0; i < 20 * 2; i++) {
		ref_ptr<Bush> bush = new Bush(start,14,0);
		start += 0.5;
		se->addChild(bush);
	}

	start = 14.0;
	for(int i = 0; i < 20 * 2; i++) {
		ref_ptr<Bush> bush = new Bush(-10.0,start,0);
		start -= 0.5;
		se->addChild(bush);
	}
	
	return se;
}

void World::initializeFireworks(Group *root) {

	ref_ptr<FireWorkInstance> redFireworks = new FireWorkInstance(root, -1.5, 20, 20, Vec4(255,0,0,1), 0.0);
	ref_ptr<FireWorkInstance> greenFireworks = new FireWorkInstance(root, 0, 20, 21.5, Vec4(0,255,0,1), 0.8);
	ref_ptr<FireWorkInstance> blueFireworks = new FireWorkInstance(root, 1.5, 20, 20, Vec4(0,0,255,1), 0.5);
	ref_ptr<FireWorkInstance> yellowFirework = new FireWorkInstance(root, 0, 20, 18.5, Vec4(255,255, 100,1), 0.3);
	root->addChild(redFireworks);
	root->addChild(greenFireworks);
	root->addChild(blueFireworks);
	root->addChild(yellowFirework);
}

void World::createAndAddCarrousel(ShadowHolder *root) {
	ref_ptr<ManipulableObject> carrousel = new CarrouselInstance(-5.0, -5.0, 8, true);
	root->applyMaskToGroup(carrousel, ShadowHolder::CAST_SHADOW_MASK);
	root->addChild(carrousel);
	viewer.addEventHandler( new UserInputHandler(carrousel, 'q') );
}

void World::createAndAddBumperCars(ShadowHolder *root) {
	ref_ptr<MatrixTransform> posBumpers = new MatrixTransform(Matrix::rotate(inDegrees(-30.0), Vec3(0,0,1)) * Matrix::translate(Vec3(10.0,5.0,0.0)));
	ref_ptr<BumperCarsBuild> bumpers = new BumperCarsBuild(6);
	posBumpers->addChild(bumpers);
	root->applyMaskToGroup(posBumpers, ShadowHolder::CAST_SHADOW_MASK);
	root->addChild(posBumpers);
	viewer.addEventHandler(new UserInputHandler(bumpers, 'e'));
}

void World::createAndAddThreadmil(ShadowHolder *root) {
	ref_ptr<MatrixTransform> pos = new MatrixTransform(Matrix::rotate(inDegrees(30.0), Vec3(0,0,1)) * Matrix::translate(5, -10, 0));
	ref_ptr<ThreadmilInstance> threadmil = new ThreadmilInstance;
	pos->addChild(threadmil);
	root->addChild(pos);
	viewer.addEventHandler(new UserInputHandler(threadmil, 'w'));
}

void World::addCameraAttach(StaticCameraAttachment *attach) {
	cameraAttachments.push_back(attach);
}

void World::tryToSwitchCamera(INT index) {
	if(index >= 0 && index < cameraAttachments.size()) {
		curCamera = index;
		if(index == 0) {
			ref_ptr<osgGA::TrackballManipulator> m = new osgGA::TrackballManipulator;
			m->setHomePosition(Vec3(1.63,-47.4,9.5), Vec3(0,0,5), Vec3(0,0,1));
			viewer.setCameraManipulator(m);
			
		} else {
			StaticCameraAttachment *attach = cameraAttachments[index];
			viewer.setCameraManipulator(0);
			viewer.getCamera()->setViewMatrixAsLookAt(attach->calcEye(), attach->calcCenter(), attach->calcUp());
		}
	}
}

void World::updateCamera() {
	if(curCamera > 0) {
		StaticCameraAttachment *attach = cameraAttachments[curCamera];
		viewer.setCameraManipulator(0);
		viewer.getCamera()->setViewMatrixAsLookAt(attach->calcEye(), attach->calcCenter(), attach->calcUp());
	}
}

INT World::run() {
	INT result = viewer.run();
	MeshTable::getInstance().~MeshTable();
	return result;
}
