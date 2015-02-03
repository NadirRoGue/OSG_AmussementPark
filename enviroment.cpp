#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/Light>
#include <osg/LightModel>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <cmath>

#include "enviroment.h"
#include "util.h"

using namespace osg;
using namespace std;
using namespace amussementpark;

//##########################################################################################

const INT ShadowHolder::RECEIVE_SHADOW_MASK = 0x1;
const INT ShadowHolder::CAST_SHADOW_MASK = 0x2;

ShadowHolder::ShadowHolder() {
	setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
        setCastsShadowTraversalMask(CAST_SHADOW_MASK);

        ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
        setShadowTechnique(sm.get());

        int mapres = 4096;
        sm->setTextureSize(Vec2s(mapres,mapres));
}

void ShadowHolder::applyMaskToGroup(Group *group, INT mask) {
	for(unsigned i = 0; i < group->getNumChildren(); i++)
		group->getChild(i)->setNodeMask(mask);
}

//##########################################################################################

Floor::Floor(DOUBLE width, DOUBLE tileWidth):
	Geode(*(new TiledSurface(width, width, 0.0, tileWidth, tileWidth, false))) {

	//Image* image = new Image(*TextureTable::getInstance().getTexture( string("cesped.png") ));
	Image *image = osgDB::readImageFile( string("textures/cesped.png") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
             	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }

	getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
}

//##########################################################################################

Bush::Bush(DOUBLE x, DOUBLE y, DOUBLE rotation) {
	setMatrix(Matrix::rotate(inDegrees(rotation), Vec3(0,0,1)) * Matrix::translate(x,y,0));

	ref_ptr<Geode> geode = new Geode;

	ref_ptr<ShapeDrawable> bushShape = new ShapeDrawable(new Box(Vec3(0,0,0.25), 0.5));
	bushShape->setColor(Vec4(255,255,255,1));
	
	geode->addDrawable(bushShape);

	Image *image = osgDB::readImageFile( string("textures/bush.png") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
             	geode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }

	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	addChild(geode);
}

//##########################################################################################

Sky::Sky(DOUBLE domeRadius) {
	
	ref_ptr<ShapeDrawable> sky = new ShapeDrawable(new Sphere(Vec3(0.0f,0.0f,0.0f), domeRadius));
	sky->setColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	addDrawable(sky);

	//Image* image = new Image(*TextureTable::getInstance().getTexture( textureName ));
	Image *image = osgDB::readImageFile( string("textures/cielodia.jpeg") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
            	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }

	getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
}

//##########################################################################################

Sun::Sun(DOUBLE zValue) {

	DOUBLE angle = 80.0;
	DOUBLE y = cos( (60.0 * M_PI) / 180.0 ) * (zValue - 1);
	DOUBLE z = sin( (60.0 * M_PI) / 180.0 ) * (zValue - 1);

	ref_ptr<Light> sunLight = getLight();
    	sunLight->setPosition( osg::Vec4( 0.0f, y, z, 0.0f ) );
    	sunLight->setAmbient( osg::Vec4( 0.6f, 0.6f, 0.6f, 1.0f ) );
	sunLight->setDiffuse( osg::Vec4( 1, 1, 1, 1.0f ) );
	sunLight->setLightNum(0);

    	setLight( sunLight );
    	setLocalStateSetModes( osg::StateAttribute::ON );
    	getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

    	LightModel* lightModel = new osg::LightModel;
    	lightModel->setAmbientIntensity(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
	getOrCreateStateSet()->setAttribute(lightModel);
}

//##########################################################################################

StreetLamp::StreetLamp(DOUBLE x, DOUBLE y, DOUBLE scale) {
	setMatrix(Matrix::translate(x/scale,y/scale,0));

	ref_ptr<Geode> geode = (Geode*)osgDB::readNodeFile("meshes/farol.obj");
	geode->setNodeMask(ShadowHolder::CAST_SHADOW_MASK);	

	addChild(geode);
}
