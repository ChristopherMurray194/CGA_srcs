// 06DataStructures
//
// This tutorial will 
// show you how to create your own data structure
// to store custom information on the geometry


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <OpenSG/OSGFaceIterator.h>
#include <math.h>
#include "MyVertex.h"

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
std::vector<MyVertex*> vertices;

int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr createCube();
void calculateVertexList(OSG::NodeRefPtr node);
OSG::NodeRefPtr createDual();

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
    
		OSG::NodeRefPtr scene = OSG::Node::create();
		scene->setCore(OSG::Group::create());

		//create a cube
		OSG::NodeRefPtr cube = createCube();
		scene->addChild(cube);

		// Populate my data structure
		calculateVertexList(cube);

		OSG::NodeRefPtr dual = createDual();
		scene->addChild(dual);
  
		//commit all changes to OpenSG 
        OSG::commitChanges();
    
        // create the SimpleSceneManager helper
        mgr = OSG::SimpleSceneManager::create();
    
        // tell the manager what to manage
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
    
        // show the whole scene
        mgr->showAll();
    }
    
    // GLUT main loop
    glutMainLoop();

    return 0;
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
        case 'e':
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
    
    int winid = glutCreateWindow("06DataStructures Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

	//we will create a menu to show different views of the geometry
	createMenu();

    return winid;
}

OSG::NodeRefPtr createCube(){
	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_QUADS);
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(24);


	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
    //MODIFY HERE with positions of your geometry

	//		  4     5
	//		  ------
	//		/ |    /|
	//	0	--|----1|
	//		| |7--|-|	
	//		|/	  | /6
	//	3	------2

	pnts->addValue(OSG::Pnt3f(-0.5, 0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, -0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, -0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, 0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(0.5, -0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, -0.5, -0.5));

	// The normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
    norms->push_back(OSG::Vec3f(0, 0, 1));
    norms->push_back(OSG::Vec3f(1, 0, 0));
    norms->push_back(OSG::Vec3f(0, 0, -1));
    norms->push_back(OSG::Vec3f(-1, 0, 0));
	norms->push_back(OSG::Vec3f(0, 1, 0));
    norms->push_back(OSG::Vec3f(0, -1, 0));

	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
    OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
    colors->addValue(OSG::Color3f(0,0,1));
    colors->addValue(OSG::Color3f(0,0,1));
    colors->addValue(OSG::Color3f(0,0,1));
	colors->addValue(OSG::Color3f(0,0,1));
    colors->addValue(OSG::Color3f(0,0,1));
    colors->addValue(OSG::Color3f(0,0,1));

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1: front 
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(2);
	indices->addValue(1);


	//face 2: right 
	indices->addValue(5);
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(6);


	//face 3: back
	indices->addValue(4);
	indices->addValue(5);
	indices->addValue(6);
	indices->addValue(7);
	
	//face 4: left
	indices->addValue(0);
	indices->addValue(4);
	indices->addValue(7);
	indices->addValue(3);

	//face 5: top
	indices->addValue(4);
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(5);
	
	//face 6: bottom
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(7);
	indices->addValue(6);


	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1: front 
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	

	//face 2: right 
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	

	//face 3: back
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);


	//face 4: left
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);

	//face 5: top
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);

	//face 6: bottom
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);

   // Put it all together into a Geometry NodeCore.
	geo->setTypes    (type);
	geo->setLengths  (lens);
	
	geo->setProperty (pnts,   OSG::Geometry::PositionsIndex);
	geo->setIndex    (indices,   OSG::Geometry::PositionsIndex);
        
	geo->setProperty (norms,  OSG::Geometry::NormalsIndex  );
	geo->setIndex    (indicesnormpos,   OSG::Geometry::NormalsIndex  );
        
	geo->setProperty (colors, OSG::Geometry::ColorsIndex   );
	geo->setIndex   (indicesnormpos,   OSG::Geometry::ColorsIndex   );
   
        
	geonode->setCore(geo);
        
	return geonode;

}

void calculateVertexList(OSG::NodeRefPtr node)
{
	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(node->getCore());

	OSG::FaceIterator faceIterator;
	float vx, vy, vz;

	// Assign all the position values to our vector of vertices.
	OSG::GeoVectorProperty *pos = geocore->getPositions();
	for (OSG::UInt32 i = 0; i < pos->size(); i++)
	{
		OSG::Pnt3f p;
		pos->getValue(p, i);
		
		vertices.push_back(new MyVertex(p));	// Add the vertex to the vertex list

		// Iterate through all faces
		for (faceIterator = geocore->beginFaces();
			faceIterator != geocore->endFaces();
			++faceIterator)
		{
			// Calculate the face's centre
			vx = (1.0 / 4.0) * (faceIterator.getPosition(0).x() +
				faceIterator.getPosition(1).x() +
				faceIterator.getPosition(2).x() +
				faceIterator.getPosition(3).x()
				);
			vy = (1.0 / 4.0) * (faceIterator.getPosition(0).y() +
				faceIterator.getPosition(1).y() +
				faceIterator.getPosition(2).y() +
				faceIterator.getPosition(3).y()
				);
			vz = (1.0 / 4.0) * (faceIterator.getPosition(0).z() +
				faceIterator.getPosition(1).z() +
				faceIterator.getPosition(2).z() +
				faceIterator.getPosition(3).z()
				);

			OSG::Pnt3f centre = OSG::Pnt3f(vx, vy, vz);

			// Check if the current vertex is a vertex shared by the current face
			if (faceIterator.getPosition(0) == vertices.at(i)->getVertex() ||
				faceIterator.getPosition(1) == vertices.at(i)->getVertex() ||
				faceIterator.getPosition(2) == vertices.at(i)->getVertex() ||
				faceIterator.getPosition(3) == vertices.at(i)->getVertex())
			{
				vertices.at(i)->addFace(new MyFace(centre));	// Add face to the current vertex
			}
		}
	}
}

OSG::NodeRefPtr createDual()
{
	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_LINE_LOOP);
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(24);

	OSG::GeoPnt3fPropertyRefPtr pnts = OSG::GeoPnt3fProperty::create();

	std::vector<MyVertex*>::iterator myVertexIterator;

	// Iterate through vertices
	for (myVertexIterator = vertices.begin();
		myVertexIterator != vertices.end();
		myVertexIterator++)
	{
		MyVertex * vertex = *myVertexIterator;
		// Loop through number of faces held by each vertex (3 for each vertex)
		for (int i = 0; i < vertex->getFacesLength(); i++)
			pnts->addValue(vertex->getFaces().at(i)->getFaceCentre());	// Get the vertices of the dual for each face of the cube
	}

	// Put it all together into a Geometry NodeCore.
	geo->setTypes(type);
	geo->setLengths(lens);
	geo->setPositions(pnts);
	geonode->setCore(geo);

	return geonode;
}