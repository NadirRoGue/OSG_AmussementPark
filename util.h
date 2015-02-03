#ifndef UTIL_H_
#define UTIL_H_

#include <map>
#include <vector>
#include <string>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osg/Projection>
#include <osgViewer/Viewer>

#include "macro.h"

namespace amussementpark {

	class Axis: public osg::Geode {
		private:
			void addAxis(osg::Geometry *geo, osg::Vec3 min, osg::Vec3 max, DOUBLE r, DOUBLE g, DOUBLE b);
		public:
			Axis(DOUBLE xLen, DOUBLE yLen, DOUBLE zLen);
	};

	class TiledSurface: public osg::Geode {
		public:
			TiledSurface(DOUBLE width, DOUBLE len, DOUBLE height, DOUBLE tileWidth, DOUBLE tileLen, BOOLEAN fixAliasing);
	};

	class HUDDebugText {
		private:
			osg::Projection *hudProj;

			void setUpText(osgText::Text *draw, std::string text, DOUBLE x, DOUBLE y, DOUBLE z);
		public:
			HUDDebugText(INT width, INT height, osgViewer::Viewer *v);
			void setFPS(INT fps);
			void setViewerPos(DOUBLE x, DOUBLE y, DOUBLE z);
			osg::Projection * getProjection();
	};

	typedef std::map<DWORD32, std::vector<DWORD32> > VectorMap;

	class NormalCalculator {
		private:
			NormalCalculator();
			static void auxSaveVectorProduct(VectorMap &m, DWORD32 key, DWORD32 face);
		public:
			static osg::Vec3Array * calcNormals(const osg::Vec3Array &vertices, const osg::DrawElementsUInt &faces);
	};

	class MeshTable {
		private:
			static MeshTable *INSTANCE;
		
			std::map<std::string, osg::Geometry*> meshTable;
		protected:
			MeshTable();
		public:
			~MeshTable();
			void initialize();
			static MeshTable & getInstance();
			void addMesh(std::string identifier, std::string filename);
			osg::Geometry * getMesh(std::string filename);
	};
}

#endif
