#include <string>
#include <cmath>

#include "abstracttypes.h"

using namespace std;
using namespace osg;
using namespace amussementpark;

//#######################################################################

ManipulableObject::ManipulableObject() {
}

//#######################################################################

MovableObject::MovableObject() {
}

//#######################################################################

ICollidableObject::ICollidableObject():
	heading(heading) {
}

ICollidableObject::~ICollidableObject() {
}

void ICollidableObject::addToKnownList(ICollidableObject *obj) {
	knownList.push_back(obj);
}

//#######################################################################

CircleCollidableObject::CircleCollidableObject() {
}

BOOLEAN CircleCollidableObject::collides() {

	DOUBLE curX = (cos((M_PI * (heading - 90.0)) / 180.0) * radio) + x;
	DOUBLE curY = (sin((M_PI * (heading - 90.0)) / 180.0) * radio) + y;

	for(DWORD32 i = 0; i < knownList.size(); i++) {
		if(knownList[i]->isBeingCollided(curX, curY))
			return true;
	}

	return false;
}

BOOLEAN CircleCollidableObject::isBeingCollided(DOUBLE tx, DOUBLE ty) {

	DOUBLE deltaX = tx - x;
	DOUBLE deltaY = ty - y;
	
	return (sqrt( (deltaX*deltaX) + (deltaY*deltaY) ) < radio);
}

//#######################################################################

SquareCollidableObject::SquareCollidableObject() {
}

BOOLEAN SquareCollidableObject::collides() {
	
	DOUBLE xHeading = (cos( (M_PI * heading) / 180.0) * widthParam) + x;
	DOUBLE yHeading = (sin( (M_PI * heading) / 180.0) * widthParam) + y;

	DOUBLE urx = xHeading + ( radio * cos( (M_PI * (90.0 - heading)) / 180.0));
	DOUBLE ury = yHeading + ( radio * sin( (M_PI * (90.0 - heading)) / 180.0));

	DOUBLE ulx = xHeading + ( radio * cos( (M_PI * (90.0 + heading) / 180.0)) );
	DOUBLE uly = yHeading + ( radio * sin( (M_PI * (90.0 + heading) / 180.0)) );

	for(DWORD32 i = 0; i < knownList.size(); i++) {
		ICollidableObject *obj = knownList[i];
		if(obj->isBeingCollided(urx, ury) || obj->isBeingCollided(ulx, uly))
			return true;
	}

	return false;
}

BOOLEAN SquareCollidableObject::isBeingCollided(DOUBLE tX, DOUBLE tY) {
	DOUBLE minX = x - (radio) - 0.3;
	DOUBLE maxX = x + (radio) + 0.3;
	DOUBLE minY = y - (widthParam / 2.0) - 0.3;
	DOUBLE maxY = y + (widthParam / 2.0) + 0.3;

	return (tX >= minX && tX <= maxX && tY >= minY && tY <= maxY);
}

//#######################################################################

StaticCameraAttachment::StaticCameraAttachment(DOUBLE xF, DOUBLE yF, DOUBLE zF, DOUBLE hF):
	xFix(xF),yFix(yF),zFix(zF),headingFix(hF) {
}

Vec3 StaticCameraAttachment::calcCenter() {
	DOUBLE x = (cos((M_PI * (getHeading() + headingFix)) / 180.0) * 20.0) + (getX() + xFix);
	DOUBLE y = (sin((M_PI * (getHeading() + headingFix)) / 180.0) * 20.0) + (getY() + yFix);

	return Vec3(x, y, getZ() + zFix);
}

Vec3 StaticCameraAttachment::calcEye() {
	DOUBLE x = (cos((M_PI * (getHeading() + headingFix)) / 180.0) * xFix) + getX();
	DOUBLE y = (sin((M_PI * (getHeading() + headingFix)) / 180.0) * yFix) + getY();
	return Vec3(x, y, getZ() + zFix);
}

Vec3 StaticCameraAttachment::calcUp() {
	return Vec3(0,0,1);
}
