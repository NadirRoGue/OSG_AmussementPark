#include <osg/ArgumentParser>

#include <iostream>

#include "world.h"
#include "util.h"

using namespace amussementpark;

int main(int argc, char **argv) {

	osg::ArgumentParser arguments(&argc,argv);

	return World::getInstance().run();
}
