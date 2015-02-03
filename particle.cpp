#include <osgParticle/RandomRateCounter>
#include <osgParticle/ConstantRateCounter>
#include <osgParticle/RadialShooter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/BoxPlacer>
#include <osgParticle/AccelOperator>
#include <osgParticle/ExplosionOperator>
#include <osg/ShapeDrawable>
#include <osgParticle/LinearInterpolator>
#include <osgParticle/ParticleSystemUpdater>
#include <osg/BlendFunc>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <osg/Geode>

#include "particle.h"

using namespace osg;
using namespace osgParticle;
using namespace std;
using namespace amussementpark;

//###################################################################

FireWorkParticle::FireWorkParticle(Vec4 startingColor) {
	setShape(Particle::POINT);

	setLifeTime(3);
	setSizeRange(rangef(10.0f, 10.0f));
	setAlphaRange(rangef(1.0f, 1.5f));

	DOUBLE r = startingColor.x() + 5;
	r = r > 255? 255 : r;
	DOUBLE g = startingColor.y() + 5;
	g = g > 255? 255 : g;
	DOUBLE b = startingColor.z() + 5;
	b = b > 255? 255 : b;

	Vec4 start(r,g,b,1);

	setColorRange(rangev4(start,startingColor));
	setColorInterpolator(new LinearInterpolator());
	setRadius(0.1f);    
	setMass(0.1f);    
}

//###################################################################

FireWorkEmitter::FireWorkEmitter(DOUBLE startTime) {
	ConstantRateCounter *counter = new ConstantRateCounter;
	counter->setNumberOfParticlesPerSecondToCreate(100);
	setCounter(counter);

	SectorPlacer *placer = new SectorPlacer;
	placer->setCenter(0, 0, 0);
	placer->setRadiusRange(0.0, 0.1);
	placer->setPhiRange(0, 2 * PI); // inDegrees(360.0)  
	setPlacer(placer);
	

	RadialShooter *shooter = new RadialShooter;
	shooter->setInitialSpeedRange(2, 2);
	shooter->setThetaRange(0, 2 * PI);
	shooter->setPhiRange(0, 2 * PI);
	setShooter(shooter);

	setEndless(false);
	setLifeTime(1.0);
	setResetTime(5.0);
	setStartTime(startTime);
}

//###################################################################

FireWorkProgram::FireWorkProgram(DOUBLE x, DOUBLE y, DOUBLE z) {

	ExplosionOperator *op1 = new ExplosionOperator;
	op1->setCenter(Vec3(x, y, z));
	op1->setRadius(2);
	op1->setMagnitude(5.0);
	op1->setEpsilon(1.0);
	op1->setSigma(1.0);
	addOperator(op1);

	AccelOperator *op2 = new AccelOperator;
	op2->setToGravity(0.2);
	//addOperator(op2);

	setEndless(false);
	setLifeTime(1.0);
	setResetTime(5.0);
}

//###################################################################

FireWorks::FireWorks(FireWorkEmitter *emitter, FireWorkProgram *program, Vec4 color) {

	FireWorkParticle fwp (color);
	setDefaultParticleTemplate(fwp);
	emitter->setParticleSystem(this);
	program->setParticleSystem(this);
}

//###################################################################

FireWorkInstance::FireWorkInstance(osg::Group *parent, DOUBLE x, DOUBLE y, DOUBLE z, Vec4 fireworkColor, DOUBLE startTime) {

	setMatrix(Matrix::rotate(inDegrees(-70.0), Vec3(1,0,0)) * Matrix::translate(x, y, z));

	ref_ptr<FireWorkEmitter> emitter = new FireWorkEmitter(startTime);
	ref_ptr<FireWorkProgram> program = new FireWorkProgram(x, y, z);

	ref_ptr<FireWorks> fireWorks = new FireWorks(emitter, program, fireworkColor);
	ref_ptr<Geode> geode = new Geode;
	geode->addDrawable(fireWorks);

	ref_ptr<Texture2D> text = new Texture2D(osgDB::readImageFile("textures/particle_original.rgb"));
	geode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, text);

	//geode->getOrCreateStateSet()->setMode(GL_BLEND, StateAttribute::ON);
    	BlendFunc *fn = new BlendFunc();
    	fn->setFunction(BlendFunc::SRC_ALPHA, BlendFunc::ONE);
    	//geode->getOrCreateStateSet()->setAttributeAndModes(fn, StateAttribute::ON);

	addChild(emitter);
	parent->addChild(program);
	parent->addChild(geode);

	ref_ptr<ParticleSystemUpdater> psu = new ParticleSystemUpdater;
	psu->addParticleSystem(fireWorks);
	parent->addChild(psu);
}
