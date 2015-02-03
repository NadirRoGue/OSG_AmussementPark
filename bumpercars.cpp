#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <string>
#include <iostream>

#include "bumpercars.h"
#include "enviroment.h"
#include "util.h"
#include "animationcallbacks.h"
#include "world.h"

using namespace std;
using namespace osg;
using namespace amussementpark;

//###################################################################################

BumperCarsStructRoof::BumperCarsStructRoof() {

	ref_ptr<MatrixTransform> roofFixer = new MatrixTransform(Matrix::scale(0.4,0.4,0.4) * Matrix::rotate(inDegrees(90.0), Vec3(1,0,0)));

	ref_ptr<Geode> roofG = new Geode;
	roofG->addDrawable(new Geometry(*MeshTable::getInstance().getMesh( string("tejado_coches_choque.ply") )));
	
	//ref_ptr<Geode> roofG = (Geode*)osgDB::readNodeFile("meshes/tejado_coches_choque.ply");

	roofFixer->addChild(roofG);

	ref_ptr<Geode> roofSupport = new Geode;
	roofSupport->addDrawable(new ShapeDrawable(new Box(Vec3(0.0,0.5,0.25), 15.0, 1.0, 0.5)));

	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.4,0.4,0.4,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.8,0.8,0.8,0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);
	roofSupport->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );
	roofG->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	addChild(roofFixer);
	addChild(roofSupport);
}

//###################################################################################

BumperCarsTrack::BumperCarsTrack(DOUBLE xWidth, DOUBLE yWidth) {
	ref_ptr<Geode> baseHolder = new Geode;
	ref_ptr<ShapeDrawable> base = new ShapeDrawable(new Box(Vec3(0,0,0.15), xWidth, yWidth, 0.3));
	baseHolder->addDrawable(base);

	ref_ptr<TiledSurface> surface = new TiledSurface(xWidth, yWidth, 0.31, 1.0, 1.0, false);


	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.4,0.4,0.4,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.8,0.8,0.8,0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);

	baseHolder->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );
	surface->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );
	
	//Image* image = new Image(*TextureTable::getInstance().getTexture( string("metal2.jpg") ));
	Image *image = osgDB::readImageFile( string("textures/metal2.png") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
             	surface->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }

	addChild(baseHolder);
	addChild(surface);
}

//###################################################################################

BumperCarStructBorder::BumperCarStructBorder(DOUBLE x, DOUBLE y, DOUBLE z, DOUBLE xWidth, DOUBLE yWidth, DOUBLE heading) {
	ref_ptr<ShapeDrawable> box = new ShapeDrawable(new Box(Vec3(x,y,z + (0.5/2.0)), xWidth, yWidth, 0.5));
	box->setColor(Vec4(1,1,1,1));
	addDrawable(box);

	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.4,0.4,0.4,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.8,0.8,0.8,0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);
	getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	//Image* image = new Image(*TextureTable::getInstance().getTexture( string("madera_bordes.jpg") ));
	Image *image = osgDB::readImageFile( string("textures/madera_bordes.jpg") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
             	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }

	setNodeMask(ShadowHolder::RECEIVE_SHADOW_MASK | ShadowHolder::CAST_SHADOW_MASK);

	this->x = x;
	this->y = y;
	this->heading = heading;
	this->radio = xWidth / 2.0;
	this->widthParam = yWidth;
}

//###################################################################################

BumperCar::BumperCar(DOUBLE x, DOUBLE y, Vec4 carColor):
	StaticCameraAttachment(-0.55, -0.55, 0.52, -30.0) {

	animationMatrix = new MatrixTransform;

	ref_ptr<Geode> geode = new Geode;
	ref_ptr<Geometry> bumperGeo = new Geometry(*(MeshTable::getInstance().getMesh( string("coche.ply"))));

	ref_ptr<Vec4Array> color = new Vec4Array;
	color->push_back(carColor);
	bumperGeo->setColorArray(color);
	bumperGeo->setColorBinding(Geometry::BIND_OVERALL);

	geode->addDrawable(bumperGeo);

	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(carColor.x() - 0.2,carColor.y() - 0.2,carColor.z() - 0.2,0));
	material->setDiffuse(Material::FRONT_AND_BACK, carColor);
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);
	geode->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	geode->setNodeMask(ShadowHolder::RECEIVE_SHADOW_MASK | ShadowHolder::CAST_SHADOW_MASK);
	
	setUpdateCallback(new AnimationCallBack());

	animationMatrix->addChild(geode);
	addChild(animationMatrix);

	heading = 0.0;
	this->x = x;
	this->y = y;
	radio = 1.0;
	enabled = true;
	speed = 0.1;
	rotationSpeed = 1.0;

	World::getInstance().addCameraAttach(this);
}

void BumperCar::updateAnimation() {
	
	if(enabled) {
		if(collides())
			heading += rotationSpeed;
		else {
			x += (cos((M_PI * (heading - 90.0)) / 180.0) * speed);
			y += (sin((M_PI * (heading - 90.0)) / 180.0) * speed);
		}
		animationMatrix->setMatrix(Matrix::rotate(inDegrees(heading), Vec3(0,0,1)) * Matrix::translate(x, y, 0.31));
	}
}

void BumperCar::onKeyPress() {
	if(speed < 0.3)
		speed += 0.1;
	if(rotationSpeed < 360.0)
		rotationSpeed += 1.0;
}

void BumperCar::onCtrlKeyPress() {
	if(speed > 0.0) {
		speed -= 0.5;
		rotationSpeed -= 1.0;
	}
}

void BumperCar::onMouseClick() {
	enabled = !enabled;
}

DOUBLE BumperCar::getX() {
	Node *node = animationMatrix->getChild(0);
	MatrixList matrices = node->getWorldMatrices();
	Vec3 center = matrices.empty()? animationMatrix->getChild(0)->getBound().center() :
		animationMatrix->getChild(0)->getBound().center() * matrices[0];
	return center.x();
}

DOUBLE BumperCar::getY() {
	Node *node = animationMatrix->getChild(0);
	MatrixList matrices = node->getWorldMatrices();
	Vec3 center = matrices.empty()? animationMatrix->getChild(0)->getBound().center() :
		animationMatrix->getChild(0)->getBound().center() * matrices[0];

	return center.y();
}

DOUBLE BumperCar::getZ() {
	return 0.31;
}

DOUBLE BumperCar::getHeading() {
	return (heading - 90.0);
}

//###################################################################################

BumperCarsBuild::BumperCarsBuild(INT cars) {
	// Parametros editables
	const DOUBLE xW = 14.0, yW = 6.0;
	const DOUBLE borderWidth = 0.35;
	// Fin parametros editables

	ref_ptr<BumperCarsTrack> track = new BumperCarsTrack(xW, yW);
	track->setNodeMask(ShadowHolder::RECEIVE_SHADOW_MASK);
	addChild(track);

	vector<BumperCarStructBorder*> feedBack;

	createRoof(xW, yW, borderWidth);
	createBorders(xW, yW, borderWidth, feedBack);
	createDeco(xW, yW, borderWidth);
	createCars(xW, yW, cars, feedBack);
}

void BumperCarsBuild::createRoof(DOUBLE xW, DOUBLE yW, DOUBLE borderFix) {
	ref_ptr<MatrixTransform> roofPosFixer = new MatrixTransform(Matrix::translate(Vec3(0.0, (yW/2.0) + borderFix, 0.0)));
	ref_ptr<BumperCarsStructRoof> roof = new BumperCarsStructRoof;
	roofPosFixer->addChild(roof);

	addChild(roofPosFixer);
}

void BumperCarsBuild::createBorders(DOUBLE xW, DOUBLE yW, DOUBLE borderWidth, vector<BumperCarStructBorder*> & feedBack) {
	DOUBLE xLeftBorder = -(xW / 2.0) - (borderWidth/2.0);
	DOUBLE yLeftBorder = 0.0;
	ref_ptr<BumperCarStructBorder> leftB = new BumperCarStructBorder(xLeftBorder, yLeftBorder, 0.0, borderWidth, yW, 180.0);
	addChild(leftB);
	feedBack.push_back(leftB);
	
	DOUBLE xUpperBorder = 0.0;
	DOUBLE yUpperBorder = (yW / 2.0) + (borderWidth / 2.0);
	ref_ptr<BumperCarStructBorder> upperB = new BumperCarStructBorder(xUpperBorder, yUpperBorder, 0.0, xW, borderWidth, 90.0);
	addChild(upperB);
	feedBack.push_back(upperB);

	DOUBLE xRightBorder = (xW / 2.0) + (borderWidth / 2.0);
	DOUBLE yRightBorder = 0.0;
	ref_ptr<BumperCarStructBorder> rightB = new BumperCarStructBorder(xRightBorder, yRightBorder, 0.0, borderWidth, yW, 0.0);
	addChild(rightB);
	feedBack.push_back(rightB);

	DOUBLE xBottomBorder = 0.0;
	DOUBLE yBottomBorder = -(yW / 2.0) - (borderWidth / 2.0);
	ref_ptr<BumperCarStructBorder> bottomB = new BumperCarStructBorder(xBottomBorder, yBottomBorder, 0.0, xW, borderWidth, 270.0);
	addChild(bottomB);
	feedBack.push_back(bottomB);
}

void BumperCarsBuild::createDeco(DOUBLE xW, DOUBLE yW, DOUBLE bW) {
	DOUBLE xUpperLeft = -((xW / 2.0) + (bW / 2.0));
	DOUBLE yUpperLeft = (yW / 2.0) + (bW / 2.0);

	DOUBLE xBottomLeft = xUpperLeft;
	DOUBLE yBottomLeft = -yUpperLeft;

	DOUBLE xUpperRight = -xUpperLeft;
	DOUBLE yUpperRight = yUpperLeft;

	DOUBLE xBottomRight = xUpperRight;
	DOUBLE yBottomRight = yBottomLeft;

	ref_ptr<ShapeDrawable> boxUL = new ShapeDrawable( new Box(Vec3(xUpperLeft, yUpperLeft, 0.25), bW, bW, 0.5) );
	ref_ptr<ShapeDrawable> boxBL = new ShapeDrawable( new Box(Vec3(xBottomLeft, yBottomLeft, 0.25), bW, bW, 0.5) );
	ref_ptr<ShapeDrawable> boxUR = new ShapeDrawable( new Box(Vec3(xUpperRight, yUpperRight, 0.25), bW, bW, 0.5) );
	ref_ptr<ShapeDrawable> boxBR = new ShapeDrawable( new Box(Vec3(xBottomRight, yBottomRight, 0.25), bW, bW, 0.5) );

	ref_ptr<Geode> deco = new Geode;
	deco->addDrawable(boxUL);
	deco->addDrawable(boxBL);
	deco->addDrawable(boxUR);
	deco->addDrawable(boxBR);

	DOUBLE r = 100.0 / 255.0;
	DOUBLE g = 149.0 / 255.0;
	DOUBLE b = 237.0 / 255.0;

	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(r - 0.2, g - 0.2, b - 0.2,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(r, g, b, 0.0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);
	deco->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	addChild(deco);
}

void BumperCarsBuild::createCars(DOUBLE xW, DOUBLE yW, INT cars, vector<BumperCarStructBorder*> &feedBack) {

	DOUBLE xFix = -(xW/2.0);
	DOUBLE yFix = -(yW/2.0);

	DOUBLE xPos = 1.5, yPos = 1.5;
	DOUBLE incr = 1.5;
	const DOUBLE maxYPos = yW - incr;
	const DOUBLE maxXPos = xW - incr;

	Vec4 colors [] = {
		Vec4(1,0,0,1),
		Vec4(0,1,0,1),
		Vec4(0,1,1,1),
		Vec4(1,1,0.3,1)
	};

	for(INT i = 0; i < cars; i++) {
		ref_ptr<BumperCar> car = new BumperCar(xFix + xPos, yFix + yPos, colors[i % 4]);
		addChild(car);
		carsV.push_back(car);
		
		xPos += incr;
		if(xPos > maxXPos)
			xPos = incr;
		yPos += incr;
		if(yPos > maxYPos)
			yPos = incr;
	}

	// Actualizar la knownlist para las colisiones
	for(DWORD32 i = 0; i < carsV.size(); i++) {
		for(DWORD32 j = 0; j < carsV.size(); j++) {
			if(j != i)
				carsV[i]->addToKnownList(carsV[j]);
		}
		for(DWORD32 j = 0; j < feedBack.size(); j++) {
			carsV[i]->addToKnownList(feedBack[j]);
		}
	}
}

void BumperCarsBuild::onKeyPress() {
	INT i = 0;
	while(i < carsV.size()) {
		carsV[i]->onKeyPress();
		i++;
	}
}

void BumperCarsBuild::onCtrlKeyPress() {
	INT i = 0;
	while(i < carsV.size()) {
		carsV[i]->onCtrlKeyPress();
		i++;
	}

}

void BumperCarsBuild::onMouseClick() {
	INT i = 0;
	while(i < carsV.size()) {
		carsV[i]->onMouseClick();
		i++;
	}
}
