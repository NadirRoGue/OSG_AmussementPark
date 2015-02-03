#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/Object>
#include <osg/Geode>

#include <cmath>
#include <dirent.h>
#include <cstring>
#include <sys/types.h>
#include <iostream>

#include "util.h"
#include "animationcallbacks.h"
#include "userinputhandlers.h"

using namespace std;
using namespace osg;
using namespace amussementpark;

//##########################################################################################

Axis::Axis(DOUBLE xLen, DOUBLE yLen, DOUBLE zLen) {

	ref_ptr<Geometry> xGeo = new Geometry;

	DOUBLE x = xLen / 2.0;
	addAxis(xGeo, Vec3(-x, 0.0, 0.0), Vec3(x, 0.0, 0.0), 1.0, 0.0, 0.0);

	ref_ptr<Geometry> yGeo = new Geometry;

	DOUBLE y = yLen / 2.0;
	addAxis(yGeo, Vec3(0.0, -y, 0.0), Vec3(0.0, y, 0.0), 0.0, 1.0, 0.0);

	ref_ptr<Geometry> zGeo = new Geometry;

	DOUBLE z = zLen / 2.0;
	addAxis(zGeo, Vec3(0.0, 0.0, -z), Vec3(0.0, 0.0, z), 0.0, 0.0, 1.0);

	addDrawable(xGeo);
	addDrawable(yGeo);
	addDrawable(zGeo);
}

void Axis::addAxis(Geometry *geo, Vec3 min, Vec3 max, DOUBLE r, DOUBLE g, DOUBLE b) {
	ref_ptr<Vec3Array> points = new Vec3Array;
	points->push_back(min);
	points->push_back(max);

	ref_ptr<Vec4Array> color = new Vec4Array;
	color->push_back(Vec4(r,g,b,1.0));

	geo->setVertexArray(points);
	geo->setColorArray(color);
	geo->setColorBinding(osg::Geometry::BIND_OVERALL);

	geo->addPrimitiveSet(new osg::DrawArrays(GL_LINES,0,2)); 
}

//##########################################################################################

TiledSurface::TiledSurface(DOUBLE width, DOUBLE len, DOUBLE height, DOUBLE tileWidth, DOUBLE tileLen, BOOLEAN fixAliasing) {
	if(tileLen == -1.0)
		tileLen = tileWidth;

	INT columns = (INT)(width / tileWidth);
	INT rows = (INT)(len / tileLen);

	DOUBLE xAlyasingFix = 0.0;
	DOUBLE yAlyasingFix = 0.0;
	DOUBLE xAlyasingIncr = 0.0;
	DOUBLE yAlyasingIncr = 0.0;

	if(fixAliasing) {
		// Calculamos el solapamiento
		xAlyasingFix = tileWidth * 0.01; // 2% del ancho de la ficha
		yAlyasingFix = tileLen * 0.01;

		// Recalcularmos las fichas para arreglar las pérdidas
		INT xLossFix = (INT)((xAlyasingFix * columns) / tileWidth);
		columns += xLossFix;

		INT yLossFix = (INT)((yAlyasingFix * rows) / tileLen);
		rows += yLossFix;
	}

	ref_ptr<Vec4Array> color = new Vec4Array;
	color->push_back( Vec4(1.0f,1.0f,1.0f,1.0f) );

	ref_ptr<Vec3Array> normal = new Vec3Array;
	normal->push_back( Vec3(0.0f, 0.0f, 1.0f) );

	osg::ref_ptr<osg::Vec2Array> textCoords = new osg::Vec2Array;
	textCoords->push_back( osg::Vec2(0.0f, 0.0f) );
	textCoords->push_back( osg::Vec2(0.0f, 1.0f) );
	textCoords->push_back( osg::Vec2(1.0f, 1.0f) );
	textCoords->push_back( osg::Vec2(1.0f, 0.0f) );

	DOUBLE xBottomLeft = 0.0;
	DOUBLE yBottomLeft = 0.0;

	for(INT i = 0; i < columns; i++) {
			
		xBottomLeft = (width / 2.0) - (DOUBLE)((i * tileWidth) - xAlyasingIncr) - tileWidth;
		
		for(INT j = 0; j < rows; j++) {
			
			yBottomLeft = -(len / 2.0) + (DOUBLE)(( (j + 1) * tileLen ) - yAlyasingIncr);

			xAlyasingIncr += xAlyasingFix;
			yAlyasingIncr += yAlyasingFix;
	
			ref_ptr<Geometry> tileGeom = new Geometry;

			ref_ptr<Vec3Array> coords = new Vec3Array;
			coords->push_back( Vec3(xBottomLeft, yBottomLeft, height) );
			coords->push_back( Vec3(xBottomLeft + tileWidth, yBottomLeft, height));	
			coords->push_back( Vec3(xBottomLeft + tileWidth, yBottomLeft - tileLen, height) );
			coords->push_back( Vec3(xBottomLeft, yBottomLeft - tileLen, height) );

			tileGeom->setVertexArray(coords);
			tileGeom->setNormalArray(normal);
			tileGeom->setNormalBinding(Geometry::BIND_OVERALL);
			tileGeom->setTexCoordArray(0, textCoords);
			tileGeom->setColorArray(color);
			tileGeom->setColorBinding(Geometry::BIND_OVERALL);	
			tileGeom->addPrimitiveSet(new DrawArrays(osg::PrimitiveSet::QUADS,0,4));

			addDrawable(tileGeom);
		}
	}
}

//##########################################################################################

HUDDebugText::HUDDebugText(INT width, INT height, osgViewer::Viewer *v) {

	hudProj = new Projection;

	hudProj->setMatrix(Matrix::ortho2D(0,width,0,height));


	MatrixTransform* HUDModelViewMatrix = new MatrixTransform;
       	HUDModelViewMatrix->setMatrix(Matrix::identity());
	HUDModelViewMatrix->setReferenceFrame(Transform::ABSOLUTE_RF);


	// FPS & Pos debugger
	ref_ptr<osgText::Text> fps = new osgText::Text;
	ref_ptr<osgText::Text> viewerPos = new osgText::Text;
	setUpText(fps, string("Frames per second (FPS): "), 10, 35, 0);
	setUpText(viewerPos, string("Viewer x = ; y = ; z = ;"), 10, 10, 0);

	fps->setUpdateCallback(new HUDDebuggerCallBack);
	viewerPos->setUpdateCallback(new HUDDebuggerPosCallBack(v));
	Geode *hudGeode = new Geode;
	v->addEventHandler(new DebuggerKeyboardHandler(hudGeode));
	StateSet* HUDStateSet = new StateSet();
	HUDStateSet->setMode(GL_DEPTH_TEST, StateAttribute::OFF);
        HUDStateSet->setRenderingHint(StateSet::TRANSPARENT_BIN);
	HUDStateSet->setRenderBinDetails( 11, "RenderBin");
	hudGeode->setStateSet(HUDStateSet);

	hudGeode->addDrawable(fps);
	hudGeode->addDrawable(viewerPos);
	
	HUDModelViewMatrix->addChild(hudGeode);	

	// Help menu
	Geode *helpGeode = new Geode;
	helpGeode->setStateSet(HUDStateSet);

	ref_ptr<osgText::Text> menu = new osgText::Text;
	setUpText(menu, string("F1: This menu; F3: Real-time debug info"), 10, height - 20, 0);
	height -= 25;
	ref_ptr<osgText::Text> menu2 = new osgText::Text;
	setUpText(menu2, string("Carrousel- q=faster, Ctrl+q=slower, click=stops"), 10, height - 20, 0);
	height -= 25;
	ref_ptr<osgText::Text> menu3 = new osgText::Text;
	setUpText(menu3, string("BumperCars Build- e=faster, Ctrl+e=slower, click=stops all cars"), 10, height - 20, 0);
	height -= 25;
	ref_ptr<osgText::Text> menu4 = new osgText::Text;
	setUpText(menu4, string("Single BumperCar- click=stops car"), 10, height - 20 , 0);
	height -= 25;
	ref_ptr<osgText::Text> menu5 = new osgText::Text;
	setUpText(menu5, string("Threadmil- w=faster, Ctrl+w=slower, click=stops"), 10, height - 20, 0);

	helpGeode->addDrawable(menu);
	helpGeode->addDrawable(menu2);
	helpGeode->addDrawable(menu3);
	helpGeode->addDrawable(menu4);
	helpGeode->addDrawable(menu5);

	v->addEventHandler(new HelpMenuKeyboardHandler(helpGeode));

	HUDModelViewMatrix->addChild(helpGeode);

	hudProj->addChild(HUDModelViewMatrix);
}

void HUDDebugText::setUpText(osgText::Text *draw, std::string text, DOUBLE x, DOUBLE y, DOUBLE z) {
	draw->setCharacterSize(25);
	draw->setFont("fonts/Trajan Pro Bold.ttf");
       	draw->setText(text);
       	draw->setAxisAlignment(osgText::Text::SCREEN);
       	draw->setPosition( Vec3(x,y,z) );       	
	draw->setColor( Vec4(0, 0, 0, 1) );
	draw->setDataVariance(Object::DYNAMIC) ;
}

Projection * HUDDebugText::getProjection() {
	return hudProj;
}

//##########################################################################################

void NormalCalculator::auxSaveVectorProduct(VectorMap &m, DWORD32 key, DWORD32 face) {
	VectorMap::iterator it = m.find(key);
    	if(it == m.end()) {
        	vector<DWORD32> normals;
        	normals.push_back(face);
        	m[key] = normals;
    	} else {
    		vector<DWORD32> v = it->second;
        	v.push_back(face);
        	m[key] = v;
    	}
}

Vec3Array * NormalCalculator::calcNormals(const Vec3Array &vertices, const DrawElementsUInt &faces) {

	Vec3Array *normals = new Vec3Array;
	VectorMap m;
	Vec3 v1, v2, v3;
	DWORD32 f1, f2, f3;
	vector<Vec3> faceNormal;

	for(DWORD i = 0; i < faces.size(); i = i+3) {
		f1 = faces[i];
		f2 = faces[i+1];
		f3 = faces[i+2];

		v1 = vertices[f1]; v2 = vertices[f2]; v3 = vertices[f3];

		Vec3 BtoA (v2.x() - v1.x(), v2.y() - v1.y(), v2.z() - v1.z());
		Vec3 CtoB (v2.x() - v3.x(), v2.y() - v3.y(), v2.z() - v3.z());

		Vec3 product = BtoA ^ CtoB;
		product.normalize();

		faceNormal.push_back(product);
		auxSaveVectorProduct(m,f1,i);
		auxSaveVectorProduct(m,f2,i);
		auxSaveVectorProduct(m,f3,i);
	}

	for(DWORD i = 0; i < vertices.size(); i++) {
		vector<DWORD32> vlist = m.find(i)->second;
		FLOAT x = 0, y = 0, z = 0;
		DWORD j = 0;
		while(j < vlist.size()) {
			Vec3 temp = faceNormal[vlist[j]];
		    	x += temp.x();
		    	y += temp.y();
		    	z += temp.z();
		    	j++;
		}

		Vec3 final(x,y,z);
		final.normalize();
		normals->push_back(final);
	}

	return normals;
}

//##########################################################################################

MeshTable *MeshTable::INSTANCE = NULL;

MeshTable::MeshTable() {
}

MeshTable::~MeshTable() {
	meshTable.clear();
}

void MeshTable::initialize() {
	cout << "Loading meshes..." << endl;
	DIR *directory = opendir("meshes");
	if(directory != NULL) {
		struct dirent *d = NULL;
		while(d = readdir(directory)) {
			INT len = strlen(d->d_name);
			if(len > 4 && (strcmp(d->d_name + len - 4, ".ply") == 0
				|| strcmp(d->d_name + len - 4, ".3DS") == 0)) {
				addMesh( string(d->d_name), string("meshes/") + string(d->d_name) );
				cout << "Loaded " << d->d_name << endl;
			}
		}
	}
}

MeshTable & MeshTable::getInstance() {
	if(INSTANCE == NULL)
		INSTANCE = new MeshTable();
	return *INSTANCE;
}

void MeshTable::addMesh(string identifier, string fileName) {
	ref_ptr<Geode> mesh = (Geode*)osgDB::readNodeFile(fileName);
	if(mesh) {
		Geometry* geom = new Geometry(*(mesh->getDrawable(0)->asGeometry()));
		meshTable[identifier] = geom;
	}
}

Geometry * MeshTable::getMesh(string fileName) {
	map<string, Geometry*>::iterator it = meshTable.find(fileName);
	if(it != meshTable.end()) {
		return it->second;
	}

	return NULL;
}

