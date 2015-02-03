#include <cmath>
#include <iostream>

#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/Material>
#include <osg/Matrix>
#include <osg/LightModel>
#include <osgDB/ReadFile>

#include "carrousel.h"
#include "util.h"
#include "animationcallbacks.h"
#include "userinputhandlers.h"

using namespace osg;
using namespace std;
using namespace amussementpark;

//################################################################################

CarrouselRoof::CarrouselRoof(DOUBLE rad, DOUBLE height, DOUBLE yPos):
	rad(rad),height(height),yPos(yPos) {
	initialize();
}

CarrouselRoof::CarrouselRoof(CarrouselRoof &o):
	rad(o.rad),height(o.height),yPos(o.yPos) {
	initialize();
}

void CarrouselRoof::initialize() {
	setShape(new Cone(Vec3(0.0,0.0,yPos),rad,height));
}

//################################################################################

CarrouselBase::CarrouselBase(DOUBLE rad, DOUBLE height, DOUBLE yPos):
	rad(rad),height(height),yPos(yPos) {
	initialize();
}

CarrouselBase::CarrouselBase(CarrouselBase &o):
	rad(o.rad),height(o.height),yPos(o.yPos) {
	initialize();
}

void CarrouselBase::initialize() {
	setShape(new Cylinder(Vec3(0.0f,0.0f, yPos),rad,height));
}

//################################################################################
//################################################################################

CarrouselUpperNode::CarrouselUpperNode(DOUBLE sizeFactor) {
	// Parametros editables
	DOUBLE roofRadius = 2.2 * sizeFactor;
	DOUBLE roofHeight = 0.8 * sizeFactor;

	// Parametros no editables;
	DOUBLE startingHeight = (1.525 * sizeFactor);
	DOUBLE defaultRoofHeight = startingHeight + (roofHeight / 4.0) + 0.2;

	ref_ptr<ShapeDrawable> baseRoof = new ShapeDrawable(new Cylinder(Vec3(0, 0, startingHeight + 0.1),roofRadius, 0.2));
	addDrawable(baseRoof);

	ref_ptr<CarrouselRoof> roof = new CarrouselRoof(roofRadius, roofHeight, defaultRoofHeight);
	roof->setColor(Vec4(1.0,1.0,1.0,1.0));
	addDrawable(roof);

	ref_ptr<ShapeDrawable> light1 = new ShapeDrawable(new Cylinder( Vec3(roofRadius / 2.0, 0, startingHeight - 0.025), 0.05, 0.05));
	ref_ptr<ShapeDrawable> light2 = new ShapeDrawable(new Cylinder( Vec3( -(roofRadius / 2.0), 0, startingHeight - 0.025), 0.05, 0.05));

	addDrawable(light1);
	addDrawable(light2);

	//Image* image = new Image(*TextureTable::getInstance().getTexture( string("carrouselRoof.jpg") ));
	Image *image = osgDB::readImageFile( string("textures/carrouselRoof.jpg") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
            	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }
}

//################################################################################

CarrouselLightNode::CarrouselLightNode(DOUBLE sizeFactor) {
	DOUBLE startingHeight = (1.525 * sizeFactor);
	DOUBLE roofRadius = 2.2 * sizeFactor;
	DOUBLE deltaAngle = 360.0 / 20;
	DOUBLE angle;

	ref_ptr<Geode> geode = new Geode;

	for(INT i = 0; i < 20; i++) {
		DOUBLE x = (cos((inDegrees(angle))) * (roofRadius + 0.005));
		DOUBLE y = (sin((inDegrees(angle))) * (roofRadius + 0.005));
		
		ref_ptr<ShapeDrawable> lightSupport = new ShapeDrawable(new Box(Vec3(x,y,startingHeight+0.1), 0.05,0.07,0.07));
		lightSupport->setColor(Vec4(255,255,255,1));
		geode->addDrawable(lightSupport);
		angle += deltaAngle;
		lights.push_back(lightSupport);
	}
	addChild(geode);
	setUpdateCallback(new AnimationCallBack);

	currentCounter = 0;
	currentIndex = 0;
}

void CarrouselLightNode::onKeyPress() {

}

void CarrouselLightNode::onCtrlKeyPress() {

}

void CarrouselLightNode::onMouseClick() {

}

void CarrouselLightNode::updateAnimation() {
	if(currentCounter == 30) {
		currentCounter = 0;
		for(INT i = 0; i < lights.size(); i++) {
			if((i % 2) == 0) {
				if(currentIndex == 0) {
					lights[i]->setColor(Vec4(0,0,0,1));
				} else {
					lights[i]->setColor(Vec4(0,255,0,1));
				}
			} else {
				if(currentIndex == 1) {
					lights[i]->setColor(Vec4(0,0,0,1));
				} else { 
					lights[i]->setColor(Vec4(255,0,0,1));
				}
			}
		}

		currentIndex = (currentIndex + 1) % 2;
 	} else 
		currentCounter++;
}

//################################################################################

CarrouselMiddleNode::CarrouselMiddleNode(DOUBLE sizeFactor) {
	// Parametros editables
	DOUBLE baseRadius = 2.0 * sizeFactor;
	DOUBLE axisBaseRadius = 0.4 * sizeFactor;
	DOUBLE axisRadius = 0.25 * sizeFactor;
	DOUBLE baseHeight = 0.075 * sizeFactor;
	DOUBLE axisBaseHeight = 0.15 * sizeFactor;
	DOUBLE axisHeight = 1.0 * sizeFactor;

	// Parametros no editables
	DOUBLE yPos = 0.15 * sizeFactor;
	DOUBLE halfBaseHeight = (baseHeight / 2.0);
	DOUBLE halfAxisBaseHeight = (axisBaseHeight / 2.0);
	DOUBLE halfAxisHeight = (axisHeight / 2.0);

	// Base
	yPos += halfBaseHeight;
	addDrawable(new CarrouselBase(baseRadius, baseHeight, yPos));

	// Base del eje
	yPos += halfBaseHeight + halfAxisBaseHeight;
	addDrawable(new CarrouselBase(axisBaseRadius, axisBaseHeight, yPos));

	// Eje
	yPos += halfAxisBaseHeight + halfAxisHeight;
	addDrawable(new CarrouselBase(axisRadius, axisHeight, yPos));

	// Base superior del eje
	yPos += halfAxisHeight + halfAxisBaseHeight;
	addDrawable(new CarrouselBase(axisBaseRadius, axisBaseHeight, yPos));
}

//################################################################################

CarrouselBottomNode::CarrouselBottomNode(DOUBLE sizeFactor) {
	// Parametros editables
	DOUBLE baseRadius = 2.2 * sizeFactor;
	DOUBLE baseHeight = 0.15 * sizeFactor;

	addDrawable(new CarrouselBase(baseRadius, baseHeight, baseHeight / 2.0));
}

//################################################################################

const DOUBLE HobbyHorse::MAX_HEIGHT = 0.40;
const DOUBLE HobbyHorse::MIN_HEIGHT = 0.0;

HobbyHorse::HobbyHorse(DOUBLE x, DOUBLE y, BOOLEAN startsUp, DOUBLE angleCorrection) {
	

	goingUp = !startsUp;
	enabled = true;

	ref_ptr<MatrixTransform> matrix = new MatrixTransform(Matrix::rotate(osg::inDegrees(-angleCorrection - 180.0), Vec3(0.0,0.0,1.0)) * Matrix::translate ( Vec3(x, y, 0.0) ));
	addChild(matrix);	

	// Limpiamos el color de las mallas
	ref_ptr<Vec4Array> color = new Vec4Array;
	color->push_back( Vec4(1.0,1.0,1.0,1.0) );

	// Añadimos la barra

	ref_ptr<Geode> barra = new Geode;
	Geometry *geomBarra = NULL;
	barra->addDrawable( (geomBarra = new Geometry(*MeshTable::getInstance().getMesh( string("barra_carrusel.ply") ) )) );

	//ref_ptr<Geode> barra = (Geode*)osgDB::readNodeFile("meshes/barra_carrusel.ply");
	//Geometry *geomBarra = barra->getDrawable(0)->asGeometry();

	geomBarra->setColorArray(color);
	geomBarra->setColorBinding(Geometry::BIND_OVERALL);
	// Añadimos un material a la barra
	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.68,0.65,0.41,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.93,0.9,0.66,0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);
	barra->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	matrix->addChild(barra);

	// Creamos la animación del caballo
	curHeight = startsUp? MAX_HEIGHT : MIN_HEIGHT;	
	animationMatrix = new MatrixTransform(Matrix::translate(Vec3(0.0,0.0,curHeight)));
	setUpdateCallback(new AnimationCallBack());

	// Añadimos el caballo a la animación (animamos el caballo) y lo añadimos al grafo

	ref_ptr<Geode> caballo = new Geode;
	Geometry *geomCaballo = NULL;
	caballo->addDrawable( (geomCaballo = new Geometry(*MeshTable::getInstance().getMesh( string("caballito_carrusel.ply") ) )) );

	//ref_ptr<Geode> caballo = (Geode*)osgDB::readNodeFile("meshes/caballito_carrusel.ply");
	//Geometry *geomCaballo = caballo->getDrawable(0)->asGeometry();

	geomCaballo->setColorArray(color);
	geomCaballo->setColorBinding(Geometry::BIND_OVERALL);

	//Image* image = new Image(startsUp? *TextureTable::getInstance().getTexture( string("plastico.jpg") ) :
	//	*TextureTable::getInstance().getTexture( string("madera.png") ));
	Image *image = startsUp ? osgDB::readImageFile( string("textures/plastico.jpg") ) : osgDB::readImageFile( string("textures/madera.png") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );

		osg::TexGen* texgen = new osg::TexGen;
	        texgen->setMode(osg::TexGen::OBJECT_LINEAR);

		caballo->getOrCreateStateSet()->setTextureAttributeAndModes(0, texgen, osg::StateAttribute::ON);
		caballo->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex2d);
        }

	animationMatrix->addChild(caballo);
	matrix->addChild(animationMatrix);
}

void HobbyHorse::updateAnimation() {
	if(enabled) {
		if(goingUp) {
			curHeight += 0.01;
			if(curHeight >= MAX_HEIGHT)
				goingUp = false;
		} else {
			curHeight -= 0.01;
			if(curHeight <= MIN_HEIGHT)
				goingUp = true;
		}	

		animationMatrix->setMatrix(Matrix::translate(Vec3(0.0,0.0,curHeight)));
	}
}

void HobbyHorse::onKeyPress() {
}

void HobbyHorse::onCtrlKeyPress() {

}

void HobbyHorse::onMouseClick() {
	enabled != enabled;
}

//################################################################################

CarrouselLight::CarrouselLight(INT light, DOUBLE x, DOUBLE z) {
	ref_ptr<Light> sunLight = getLight();
    	sunLight->setPosition( osg::Vec4( x, 0.0f, z, 1.0f ) );
    	sunLight->setAmbient( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	sunLight->setSpotCutoff(10.0f);
	sunLight->setLightNum(light);

    	setLight( sunLight );
    	setLocalStateSetModes( osg::StateAttribute::ON );
    	getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

    	LightModel* lightModel = new osg::LightModel;
    	lightModel->setAmbientIntensity(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
    	getOrCreateStateSet()->setAttribute(lightModel);
}

//################################################################################
//################################################################################

CarrouselInstance::CarrouselInstance(DOUBLE xPos, DOUBLE yPos, INT horseCount, BOOLEAN addLights, DOUBLE sizeFactor) {
	
	rotationSpeed = 0.6;
	enabled = true;
	rotationAngle = 0.0;
	ref_ptr<MatrixTransform> cRoot = new MatrixTransform(Matrix::translate( Vec3 (xPos, yPos, 0.0) ));
	addChild(cRoot);
	
	//if(addLights)
	initializeWithoutLights(cRoot, horseCount, sizeFactor);
}

void CarrouselInstance::initializeWithoutLights(osg::MatrixTransform *cRoot, INT horseCount, DOUBLE sizeFactor) {

	// Construimos el objeto 3D
	ref_ptr<CarrouselUpperNode> roof = new CarrouselUpperNode(sizeFactor);
	ref_ptr<CarrouselLightNode> lights = new CarrouselLightNode(sizeFactor);
	ref_ptr<CarrouselMiddleNode> carrouselStruct = new CarrouselMiddleNode(sizeFactor);
	ref_ptr<CarrouselBottomNode> base = new CarrouselBottomNode(sizeFactor);

	DOUBLE lightHeight = 1.525 * sizeFactor;
	DOUBLE radius = 2.2 * sizeFactor;

	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.4,0.4,0.4,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.6,0.6,0.6,0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);    

	carrouselStruct->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );
	base->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	// Lo animamos
	rotation = new MatrixTransform(Matrix::rotate(osg::inDegrees(0.0), Vec3(0,0,1)));
	setUpdateCallback(new AnimationCallBack());

	rotation->addChild(roof);
	rotation->addChild(lights);
	rotation->addChild(carrouselStruct);

	// Añadimos los caballitos
	DOUBLE angleFactor = (360.0 / (DOUBLE)horseCount);
	DOUBLE angle = 0.0;
	for(int i = 0; i < horseCount; i++) {
		DOUBLE x = cos( (M_PI * angle) / 180.0 ) * 1.8;
		DOUBLE y = sin( (M_PI * angle) / 180.0 ) * 1.8;
		ref_ptr<HobbyHorse> horse = new HobbyHorse(x, y, (i % 2 == 0), -angle);
		rotation->addChild(horse);
		angle += angleFactor;
	}

	cRoot->addChild(rotation);
	cRoot->addChild(base);
}

void CarrouselInstance::updateAnimation() {
	if(enabled) {
		rotationAngle += rotationSpeed;
		rotation->setMatrix(Matrix::rotate(osg::inDegrees(rotationAngle), Vec3(0,0,1)));
	}
}

void CarrouselInstance::onKeyPress() {
	if(rotationSpeed < 360.0)
		rotationSpeed += 0.3;
}

void CarrouselInstance::onCtrlKeyPress() {
	if(rotationSpeed > 0.0)
		rotationSpeed -= 0.3;
}

void CarrouselInstance::onMouseClick() {
	enabled = !enabled;
}
