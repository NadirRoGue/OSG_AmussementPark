#include <osg/Material>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include "threadmil.h"
#include "animationcallbacks.h"

using namespace std;
using namespace osg;
using namespace amussementpark;

//#############################################################################

RingBar::RingBar(Vec3 center, DOUBLE xLen, DOUBLE yLen, DOUBLE zLen) {
	ref_ptr<ShapeDrawable> shape = new ShapeDrawable(new Box(center, xLen, yLen, zLen));
	addDrawable(shape);

	ref_ptr<Material> material = new Material;
	material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.68,0.65,0.41,0));
	material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.93,0.9,0.66,0));
	material->setSpecular(Material::FRONT_AND_BACK, Vec4(1.0,1.0,1.0,0));
	material->setShininess(Material::FRONT_AND_BACK, 30.0f);
	getOrCreateStateSet()->setAttributeAndModes( material, StateAttribute::ON );

	Image *image = osgDB::readImageFile( string("textures/madera.png") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
            	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }
}

//#############################################################################

ThreadmilRing::ThreadmilRing(DOUBLE radius, DOUBLE numSides) {

	DOUBLE deltaAngle = (360.0/numSides);
	DOUBLE halfDelta = deltaAngle / 2.0;
	DOUBLE angle = 0.0;

	DOUBLE y = 0.0;

	DOUBLE radiusPow2 = radius*radius;
	DOUBLE width = sqrt(radiusPow2+radiusPow2-(2*radiusPow2*cos((M_PI * deltaAngle)/180.0)));

	ref_ptr<Geode> joints = new Geode;

	for(INT i = 0; i < numSides; i++) {
		DOUBLE x = (cos((M_PI * angle)/180.0) * radius);
		DOUBLE z = (sin((M_PI * angle)/180.0) * radius);

		DOUBLE sphereX = (cos((M_PI * (angle + halfDelta))/180.0) * (radius + 0.25));
		DOUBLE sphereZ = (sin((M_PI * (angle + halfDelta))/180.0) * (radius + 0.25));

		DOUBLE radX = (sphereX / 2.0);
		DOUBLE radZ = (sphereZ / 2.0);

		ref_ptr<RingBar> shapeRad = new RingBar(Vec3(0,0,0), radius*2.0,0.05,0.05);
		ref_ptr<MatrixTransform> rotationRad = new MatrixTransform(Matrix::rotate(inDegrees(angle + halfDelta),Vec3(0,1,0)));
		rotationRad->addChild(shapeRad);
		addChild(rotationRad);

		ref_ptr<ShapeDrawable> shapeSphere = new ShapeDrawable(new Sphere(Vec3(sphereX, 0.0, sphereZ), 0.2));
		joints->addDrawable(shapeSphere);
		
		ref_ptr<MatrixTransform> rotation = new MatrixTransform(Matrix::rotate(inDegrees(-angle),Vec3(0,1,0)) * Matrix::translate(x, y, z));
		ref_ptr<RingBar> rb = new RingBar(Vec3(0,0,0), 0.2, 0.2, width);
		rotation->addChild(rb);
		addChild(rotation);
		addChild(joints);
		angle += deltaAngle;
	}
}

//#############################################################################

Cabinas::Cabinas(FLOAT r, INT l) {    	
	FLOAT angulo = 0.0;	
	lados = l;
	radio = r;

	DOUBLE deltaAngulo = (360.0 / lados);
	transf1 = new MatrixTransform();
		
	for(INT i = 0; i < lados; i++)
	{
		FLOAT x = cos(inDegrees(angulo)) * (radio / 2);
		FLOAT y = sin(inDegrees(angulo)) * (radio / 2);

		ref_ptr<ShapeDrawable> cuboS = new ShapeDrawable(new Box(Vec3(0,0,0), 1, 0.7, 1));
		ref_ptr<Geode> cubo = new Geode;
		cubo->addDrawable(cuboS);

		ref_ptr<MatrixTransform> translation = new MatrixTransform(Matrix::translate(Vec3(x, 0.0, y)));
		translation->addChild(transf1);

		transf1->addChild(cubo);

		transf2 = new MatrixTransform(Matrix::scale(0.15f, 3.0f, 0.15f));
		
		translation->addChild(transf2);	
		transf2->addChild(cubo);

		addChild(translation);
		
		angulo += deltaAngulo;						
	}

	Image *image = osgDB::readImageFile( string("textures/cabinas_noria.jpg") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
            	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }		
}

MatrixTransform * Cabinas::getAnimationCabina() {
	return transf1;
}

//#############################################################################

Estructura::Estructura()
{
	//tres cubos por estructura
	cuboEstructura1 = new Geode;
	cuboEstructura2 = new Geode;
	cuboEstructura3 = new Geode;

	ref_ptr<ShapeDrawable> s1 = new ShapeDrawable(new Box(Vec3(0,0,0), 1));
	ref_ptr<ShapeDrawable> s2 = new ShapeDrawable(new Box(Vec3(0,0,0), 1));
	ref_ptr<ShapeDrawable> s3 = new ShapeDrawable(new Box(Vec3(0,0,0), 1));

	cuboEstructura1->addDrawable(s1);
	cuboEstructura2->addDrawable(s2);
	cuboEstructura3->addDrawable(s3);

	transf3 = new MatrixTransform;
	transf4 = new MatrixTransform;
	
	transf1 = new MatrixTransform;
	transf1->setMatrix(Matrix::rotate(inDegrees(45.0), 0.0, 1.0, 0.0));		

	transf2 = new MatrixTransform;
	transf2->setMatrix(Matrix::rotate(inDegrees(-45.0), 0.0, 1.0, 0.0));	

	//scale
	transf3 = new MatrixTransform;
	transf3->setMatrix(Matrix::scale(5.0f, 0.45f, 0.45f));

	transf4 = new MatrixTransform;
	transf4->setMatrix(Matrix::translate(Vec3(-1.58, 0.0, -1.83)));

	transf5 = new MatrixTransform;
	transf5->setMatrix(Matrix::scale(7.f, 0.45f, 0.45f));

	transf6 = new MatrixTransform;
	transf6->setMatrix(Matrix::translate(Vec3(-3.16, 0.0, 0.0)));

	//palos oblicuos
	this->addChild(transf1);
	transf1->addChild(transf3);		
	transf3->addChild(cuboEstructura2);

	this->addChild(transf6);	
	transf6->addChild(transf2);
	transf2->addChild(transf3);	
	transf3->addChild(cuboEstructura3);	
	
	//palo horizontal
	transf4->addChild(transf5);		
	transf5->addChild(cuboEstructura1);		
	this->addChild(transf4);

	transf_esf = new MatrixTransform;
	transf_esf ->setMatrix(Matrix::translate(Vec3(-1.55, 0.0, 1.7)));

	sesfera = new ShapeDrawable(new Sphere(Vec3(0,0,0), 0.5));
	esfera = new Geode;
	esfera->addDrawable(sesfera);

	transf_esf->addChild(esfera);
	this->addChild(transf_esf);

	Image *image = osgDB::readImageFile( string("textures/metal2.jpg") );
	if ( image )
        {
		ref_ptr<Texture2D> tex2d = new Texture2D( image );
            	getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d);
        }
}

//#############################################################################

ThreadmilInstance::ThreadmilInstance() {

	ref_ptr<MatrixTransform> up1 = new MatrixTransform(Matrix::translate(0,0,2));
	ref_ptr<MatrixTransform> moveBase = new MatrixTransform(Matrix::translate(1.5,0,0));

	ref_ptr<MatrixTransform> left = new MatrixTransform(Matrix::translate(0,-1.5, 0));
	ref_ptr<MatrixTransform> right = new MatrixTransform(Matrix::translate(0, 1.5, 0));

	ref_ptr<MatrixTransform> up2 = new MatrixTransform(Matrix::translate(0,0,2));
	up1->addChild(up2);

	ringMovement = new MatrixTransform(Matrix::rotate(inDegrees(speed), Vec3(0,1,0)));
	up2->addChild(ringMovement);

	enabled = true;
	speed = 0.0;
	speedIncrease = 1.0;
	
	ref_ptr<MatrixTransform> pos = new MatrixTransform(Matrix::translate(0, -1, 0));
	ref_ptr<ThreadmilRing> ring = new ThreadmilRing(3, 8);
	pos->addChild(ring);

	ringMovement->addChild(pos);	
	
	ref_ptr<MatrixTransform> pos2 = new MatrixTransform(Matrix::translate(0, 1, 0));
	ref_ptr<ThreadmilRing> ring2 = new ThreadmilRing(3, 8);
	pos2->addChild(ring2);

	ringMovement->addChild(pos2);

	cabinas = new Cabinas(6.0, 8);
	ringMovement->addChild(cabinas);

	ref_ptr<Estructura> estructura1 = new Estructura;
	ref_ptr<Estructura> estructura2 = new Estructura;
	moveBase->addChild(left);
	left->addChild(estructura1);
	moveBase->addChild(right);
	right->addChild(estructura2);
	up1->addChild(moveBase);

	addChild(up1);
	setUpdateCallback(new AnimationCallBack);
}

void ThreadmilInstance::onKeyPress() {
	if(speedIncrease < 360.0)
		speedIncrease += 1.0;
}

void ThreadmilInstance::onCtrlKeyPress() {
	if(speedIncrease >= 1.0)
		speedIncrease -= 1.0;
}

void ThreadmilInstance::onMouseClick() {
	enabled = !enabled;
}

void ThreadmilInstance::updateAnimation() {
	if(enabled) {
		speed += speedIncrease;
		cabinas->getAnimationCabina()->setMatrix(Matrix::rotate(inDegrees(-speed), Vec3(0,1,0)));
		ringMovement->setMatrix(Matrix::rotate(inDegrees(speed), Vec3(0,1,0)));
	}
}
