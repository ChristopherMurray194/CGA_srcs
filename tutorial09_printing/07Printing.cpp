// 07Printing
//
// This tutorial will 
// allow you to implement an algorithm
// for 3D printing


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGTriangleIterator.h>

#include <math.h>


// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
std::vector<OSG::Pnt3f> vertices;

int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr file;
OSG::Vec3f calculateVec(OSG::Vec3f * vec, OSG::Vec3f v1, OSG::Vec3f v2);
OSG::NodeRefPtr implementAlg(OSG::NodeRefPtr geom, float s, float spikiness);
OSG::NodeRefPtr scene;

// GLUT Menu items
enum MENU_ITEMS
{
        WIREFRAME,
        SOLID,
		VERTEX,
	    FLAT,
		SMOOTH,
};

int main(int argc, char **argv)
{
	
    // initialise OpenSG
    OSG::osgInit(argc,argv);

    // initialise GLUT
    int winid = setupGLUT(&argc, argv);

    {
        // create a OSGGLUT window
        OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();
    
	    scene = OSG::Node::create();
		scene->setCore(OSG::Group::create());

		//read the file which will be passed as an argument
		if(argc > 1)
		{
			for (int i=1; i<argc ; i++){
				std::string option = argv[i];
				if (option == "-f")
					file = OSG::SceneFileHandler::the()->read(argv[i+1]);
			}
		}else{
			file = OSG::makeBox(1,1,1,1,1,1);
		}
		scene->addChild(file);

		OSG::NodeRefPtr extrudedGeom = implementAlg(file, 0.0, 2.0);
		scene->addChild(extrudedGeom);

		//commit all changes to OpenSG 
        OSG::commitChanges();
    
        // create the SimpleSceneManager helper
        mgr = OSG::SimpleSceneManager::create();
    
        // tell the manager what to manage
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
    
        // show the whole scene
        mgr->showAll();
		//mgr->setHighlight(geometry);

    }
    
    // GLUT main loop
    glutMainLoop();


    return 0;
}

OSG::Vec3f calculateCross(OSG::Vec3f * vec, OSG::Vec3f v1, OSG::Vec3f v2)
{
	*vec = ((((v1.y() * v2.z()) - (v2.y() * v1.z())) * OSG::Vec3f(1, 0, 0)) -
		((v1.x() * v2.z()) - (v2.x() * v2.z())) * OSG::Vec3f(0, 1, 0) +
		((v1.x() * v2.y()) - (v2.x() * v1.y())) * OSG::Vec3f(0, 0, 1)
		);

	return *vec;
}

OSG::NodeRefPtr implementAlg(OSG::NodeRefPtr geom, float s, float spikiness)
{
	OSG::NodeRefPtr geoNode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();

	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(geom->getCore());

	OSG::TriangleIterator triangleIterator;

	int totalTriangles = 0;
	for (triangleIterator = geocore->beginTriangles();
		triangleIterator != geocore->endTriangles();
		++triangleIterator)
	{
		totalTriangles++; // Need total number of triangles to calculate lens

		OSG::Vec3f v1, v2, crossVec;
		OSG::Pnt3f newVert;
		// Calculate vertex 0 extruded vertex
		v1 = triangleIterator.getPosition(1) - triangleIterator.getPosition(0);	// Get vector v0 to v1
		v2 = triangleIterator.getPosition(2) - triangleIterator.getPosition(0);	// Get vector v0 to v2
		crossVec = calculateCross(&crossVec, v1, v2);							// Calculate cross product of vector
		crossVec *= s;															// Scale the vector
		newVert = triangleIterator.getPosition(0) + crossVec;					// Obtain new point/vertex
		vertices.push_back(newVert);

		// Calcualte vertex 1 extruded vertex
		v1 = triangleIterator.getPosition(2) - triangleIterator.getPosition(1);	// Get vector v1 to v2
		v2 = triangleIterator.getPosition(0) - triangleIterator.getPosition(1);	// Get vector v1 to v0
		crossVec = calculateCross(&crossVec, v1, v2);
		crossVec *= s;
		newVert = triangleIterator.getPosition(1) + crossVec;
		vertices.push_back(newVert);

		// Calculate vertex 2 extruded vertex
		v1 = triangleIterator.getPosition(0) - triangleIterator.getPosition(2);	// Get vector v1 to v2
		v2 = triangleIterator.getPosition(1) - triangleIterator.getPosition(2);	// Get vector v1 to v0
		crossVec = calculateCross(&crossVec, v1, v2);
		crossVec *= s;
		newVert = triangleIterator.getPosition(2) + crossVec;
		vertices.push_back(newVert);

		// Calculate face centre extruded vertex
		OSG::Pnt3f faceCentre;
		float x, y, z;
		x = (1.0 / 3.0) * (triangleIterator.getPosition(0).x() + 
							triangleIterator.getPosition(1).x() + 
							triangleIterator.getPosition(2).x()
							);
		y = (1.0 / 3.0) * (triangleIterator.getPosition(0).y() + 
							triangleIterator.getPosition(1).y() + 
							triangleIterator.getPosition(2).y()
							);
		z = (1.0 / 3.0) * (triangleIterator.getPosition(0).z() + 
							triangleIterator.getPosition(1).z() + 
							triangleIterator.getPosition(2).z()
							);
		faceCentre = OSG::Pnt3f(x, y, z);
		v1 = triangleIterator.getPosition(0) - faceCentre;
		v2 = triangleIterator.getPosition(1) - faceCentre;
		crossVec = calculateCross(&crossVec, v1, v2);
		crossVec *= (s + spikiness);
		newVert = faceCentre + crossVec;
		vertices.push_back(newVert);

	}
	std::cout << totalTriangles << std::endl;	// Print total number of triangles of passed geometry
	std::cout << vertices.size() << std::endl;	// Print total number of new vertices

	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_TRIANGLES);

	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	// There are n triangles (n = totalTriangles) each with 3 vertices. And each triangle is split into 3 separate triangles
	lens->addValue((totalTriangles * 3) * 3);

	OSG::GeoPnt3fPropertyRefPtr pnts = OSG::GeoPnt3fProperty::create();

	std::vector<OSG::Pnt3f>::iterator it;
	for (it = vertices.begin();
		it != vertices.end();
		++it)
	{
		pnts->push_back(*it);
	}
	std::cout << pnts->size() << std::endl;	// Print number of pnts to ensure it matches the vertices list

	vertices.clear(); // Empty vertices, no longer needed

	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	for (int i = 0; i < pnts->size(); i += 4)
	{
		// Face 1
		indices->addValue(i);
		indices->addValue(i + 2);
		indices->addValue(i + 3);
		// Face 2
		indices->addValue(i + 2);
		indices->addValue(i + 1);
		indices->addValue(i + 3);
		// Face 3
		indices->addValue(i + 1);
		indices->addValue(i);
		indices->addValue(i + 3);
	}

	// Calculate vectors
	// Get two vectors for each of the three faces that make up one triangle of the original geometry
	std::vector<OSG::Vec3f> vectors;
	for (int i = 0; i < pnts->size(); i += 4)
	{
		OSG::Vec3f v01, v03, v23, v21, v02;
		v01 = pnts->getValue(i + 1) - pnts->getValue(i);
		v03 = pnts->getValue(i + 3) - pnts->getValue(i);
		v23 = pnts->getValue(i + 3) - pnts->getValue(i + 2);
		v21 = pnts->getValue(i + 1) - pnts->getValue(i + 2);
		v02 = pnts->getValue(i + 2) - pnts->getValue(i);

		vectors.push_back(v01);
		vectors.push_back(v03);
		vectors.push_back(v23);
		vectors.push_back(v21);
		vectors.push_back(v02);
	}

	// Calculate normals
	OSG::GeoVec3fPropertyRefPtr norms = OSG::GeoVec3fProperty::create();
	for (int i = 0; i < vectors.size(); i+=5)
	{
		norms->push_back(OSG::Vec3f(vectors.at(i).cross(vectors.at(i + 1))));		// v01 X v03
		norms->push_back(OSG::Vec3f(vectors.at(i + 2).cross(vectors.at(i + 3))));	// v23 X v21
		norms->push_back(OSG::Vec3f(vectors.at(i + 2).cross(vectors.at(i + 4))));	// v23 cross v02
	}

	vectors.clear(); // Empty vectors, no longer needed

	// Add color values
	int numberFaces = totalTriangles * 3;
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
	for (int i = 0; i < numberFaces; i++)
	{
		colors->push_back(OSG::Color3f(1, 0.6, 0)); // To change color of entire mesh edit this value.
	}

	// Add normal index positions
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	for (int i = 0; i < numberFaces; i++)
	{
		indicesnormpos->addValue(i);
		indicesnormpos->addValue(i);
		indicesnormpos->addValue(i);
	}

	// Put it all together into a Geometry nodeCore
	geo->setTypes(type);
	geo->setLengths(lens);

	geo->setProperty(pnts, OSG::Geometry::PositionsIndex);
	geo->setIndex(indices, OSG::Geometry::PositionsIndex);

	geo->setProperty(norms, OSG::Geometry::NormalsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::NormalsIndex);

	geo->setProperty(colors, OSG::Geometry::ColorsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::ColorsIndex);

	geoNode->setCore(geo);

	return geoNode;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
	mgr->redraw();
}

// react to size changes
void reshape(int w, int h)
{
    mgr->resize(w, h);
    glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
    if (!state)
      mgr->mouseButtonPress(button, x, y);
	        
    glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 'mm':
        {
            // clean up global variables
            mgr = NULL;
        
            OSG::osgExit();
            exit(0);
        }
        break;
        case 's':
        {
            mgr->setStatistics(!mgr->getStatistics());
        }
        break;

    }
}

// GLUT menu handling function definition
void menu(int item)
{
        switch (item)
        {
        case VERTEX:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_POINT );
					}
					break;
		case WIREFRAME:{
						   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					   }
					   break;
        case SOLID:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
					}
					   break;

        case FLAT:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
						   glShadeModel( GL_FLAT );
					}
					   break;
        case SMOOTH:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
						   glShadeModel( GL_SMOOTH );
					}
					   break;
        default:
                {            }
                break;
        }

        glutPostRedisplay();

        return;
}
void createMenu(){
	glutCreateMenu(menu);
	// Add menu items
    glutAddMenuEntry("Show vertices", VERTEX);
    glutAddMenuEntry("Show wireframe", WIREFRAME);
    glutAddMenuEntry("Show solid", SOLID);
	glutAddMenuEntry("Shading flat", FLAT);
	glutAddMenuEntry("Shading smooth", SMOOTH);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return;
}
// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("08Printing Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

	//we will create a menu to show different views of the geometry
	createMenu();

    return winid;
}
