#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <osgParticle/ParticleSystem>
#include <osgParticle/Particle>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osg/MatrixTransform>

#include "macro.h"

namespace amussementpark {

	class FireWorkParticle: public osgParticle::Particle {
		public:
			FireWorkParticle(osg::Vec4 startingColor);
	};

	class FireWorkEmitter: public osgParticle::ModularEmitter {
		public:
			FireWorkEmitter(DOUBLE startTime);
	};

	class FireWorkProgram: public osgParticle::ModularProgram {
		public:
			FireWorkProgram(DOUBLE x, DOUBLE y, DOUBLE z);
	};

	class FireWorks: public osgParticle::ParticleSystem {
		public:
			FireWorks(FireWorkEmitter *emitter, FireWorkProgram *prog, osg::Vec4 color);
	};

	class FireWorkInstance: public osg::MatrixTransform {
		public:
			FireWorkInstance(osg::Group *partent, DOUBLE x, DOUBLE y, DOUBLE z, osg::Vec4 fireWorkColor, DOUBLE startTime);
	};

}

#endif
