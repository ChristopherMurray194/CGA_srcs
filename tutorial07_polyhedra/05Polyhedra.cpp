// 05Polyhedra
//
// This tutorial will 
// show you how to generate geometry
// based on another polyhedron


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>
#include <OpenSG/OSGTriangleIterator.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;

int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr createCube();
OSG::NodeRefPtr createTetrahedron(float s);
OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo);

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

		//create a tetrahedron
		OSG::NodeRefPtr tetra = createTetrahedron(1);
		scene->addChild(tetra);

		OSG::NodeRefPtr dual = createDual(tetra);
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
    
    int winid = glutCreateWindow("05Polyhedra Tutorial");
    
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
	//create a node to hold the geometry
	OSG::NodeRefPtr geonode = OSG::Node::create();

	//create a geometry
	OSG::GeometryRefPtr geo = OSG::Geometry::create();

	
	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
    //MODIFY HERE 
	type->addValue(GL_TRIANGLES);


	//The primitive lengths.
    //These define the number of vertices to be passed to OpenGL for each primitive. 
    //Thus there have to be at least as many entries as in the types property.
	//in the case of the cube we are using 12 triangles which each have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
    //MODIFY HERE 
	lens->addValue(36);


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
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(1);
	//face 1 - triangle 2
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(2);


	//face 2: right 
	//face 2 - triangle 1
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(6);
	//face 3 - triangle 2
	indices->addValue(1);
	indices->addValue(6);
	indices->addValue(5);


	//face 3: back
	//face 3 - triangle 1
	indices->addValue(5);
	indices->addValue(6);
	indices->addValue(7);
	//face 3 - triangle 2
	indices->addValue(5);
	indices->addValue(7);
	indices->addValue(4);


	//face 4: left
	//face 4 - triangle 1
	indices->addValue(4);
	indices->addValue(7);
	indices->addValue(3);
	//face 4 - triangle 2
	indices->addValue(4);
	indices->addValue(3);
	indices->addValue(0);

	//face 5: top
	//face 5 - triangle 1
	indices->addValue(4);
	indices->addValue(1);
	indices->addValue(5);
	//face 5 - triangle 2
	indices->addValue(4);
	indices->addValue(0);
	indices->addValue(1);

	//face 6: bottom
	//face 6 - triangle 1
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(7);
	//face 6 - triangle 2
	indices->addValue(2);
	indices->addValue(7);
	indices->addValue(6);


	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1: front 
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	//face 1 - triangle 2
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);


	//face 2: right 
	//face 2 - triangle 1
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	//face 3 - triangle 2
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);


	//face 3: back
	//face 3 - triangle 1
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	//face 3 - triangle 2
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);


	//face 4: left
	//face 4 - triangle 1
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	//face 4 - triangle 2
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);

	//face 5: top
	//face 5 - triangle 1
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	//face 5 - triangle 2
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);

	//face 6: bottom
	//face 6 - triangle 1
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	//face 6 - triangle 2
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

OSG::NodeRefPtr createTetrahedron(float s){
	OSG::NodeRefPtr geoNode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	//MODIFY HERE 
	type->addValue(GL_TRIANGLES);


	//The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each primitive. 
	//Thus there have to be at least as many entries as in the types property.
	//in the case of the tetrahedron we are using 4 triangles which each have 3 vertices (4 X 3 = 12) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(12);

	float h = std::sqrt(3.0 * s) / 2.0;
	float H = std::sqrt(6.0 * s) / 3.0;

	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();

	OSG::Pnt3f p0 = OSG::Pnt3f(0.0, 0.0, 0.0);
	OSG::Pnt3f p1 = OSG::Pnt3f(-s / 2, 0.0, h);
	OSG::Pnt3f p2 = OSG::Pnt3f(s / 2, 0.0, h);
	OSG::Pnt3f p3 = OSG::Pnt3f(0.0, H, ((2.0 / 3.0) * h));

	// Add vertices
	pnts->addValue(p0);
	pnts->addValue(p1);
	pnts->addValue(p2);
	pnts->addValue(p3);

	// Calculate vectors
	OSG::Vec3f v02 = p2 - p0;
	OSG::Vec3f v21 = p1 - p2;
	OSG::Vec3f v10 = p0 - p1;
	OSG::Vec3f v23 = p3 - p1;
	OSG::Vec3f v30 = p0 - p3;
	OSG::Vec3f v13 = p3 - p1;
	OSG::Vec3f v32 = p2 - p3;
	OSG::Vec3f v03 = p3 - p0;
	OSG::Vec3f v31 = p1 - p3;

	// The face normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(v02.cross(v21)));	// Face 1
	norms->push_back(OSG::Vec3f(v02.cross(v30)));	// Face 2
	norms->push_back(OSG::Vec3f(v13.cross(v21)));	// Face 3
	norms->push_back(OSG::Vec3f(v03.cross(v10)));	// Face 4

	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(2);
	//face 2 - triangle 2
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(3);
	//face 3 - triangle 3
	indices->addValue(2);
	indices->addValue(1);
	indices->addValue(3);
	//face 4 - triangle 4
	indices->addValue(1);
	indices->addValue(0);
	indices->addValue(3);


	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1: bottom
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	//face 2 - triangle 2
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	//face 3 - triangle 3
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	//face 4 - triangle 4
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);

	// Put it all together into a Geometry NodeCore.
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

OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo)
{
	OSG::NodeRefPtr geoNode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	
	OSG::TriangleIterator triangleIterator;

	std::vector<OSG::Vec3f> dualVerts;	// Vector to store dual vertices
	float vx, vy, vz;					// Variables to store a vertex's calculated x y and z value

	// Extract the geometry core using dynamic casting
	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(nodegeo->getCore());
	for (triangleIterator = geocore->beginTriangles();
		triangleIterator != geocore->endTriangles();
		++triangleIterator)
	{

		// calculate the center of the triangle face. The center's x y and z position is the corresponding vertex positions of the dual
		vx = (1.0 / 3.0) * (triangleIterator.getPosition(0).x() + 
							triangleIterator.getPosition(1).x() + 
							triangleIterator.getPosition(2).x()
							);
		
		vy = (1.0 / 3.0) * (triangleIterator.getPosition(0).y() +
							triangleIterator.getPosition(1).y() +
							triangleIterator.getPosition(2).y()
							);

		vz = (1.0 / 3.0) * (triangleIterator.getPosition(0).z() +
							triangleIterator.getPosition(1).z() +
							triangleIterator.getPosition(2).z()
							);

		dualVerts.push_back(OSG::Vec3f(vx, vy, vz));
	}

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	//MODIFY HERE 
	type->addValue(GL_TRIANGLES);


	//The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each primitive. 
	//Thus there have to be at least as many entries as in the types property.
	//in the case of the tetrahedron we are using 4 triangles which each have 3 vertices (4 X 3 = 12) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(12);

	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();

	std::vector<OSG::Vec3f>::iterator it;
	for (it = dualVerts.begin();
		it != dualVerts.end();
		++it)
	{
		pnts->addValue(*it); // Add vertices
	}

	// Create vectors
	OSG::Vec3f v02 = dualVerts[2] - dualVerts[0];
	OSG::Vec3f v21 = dualVerts[1] - dualVerts[2];
	OSG::Vec3f v10 = dualVerts[0] - dualVerts[1];
	OSG::Vec3f v23 = dualVerts[3] - dualVerts[2];
	OSG::Vec3f v30 = dualVerts[0] - dualVerts[3];
	OSG::Vec3f v13 = dualVerts[3] - dualVerts[1];
	OSG::Vec3f v32 = dualVerts[2] - dualVerts[3];
	OSG::Vec3f v03 = dualVerts[3] - dualVerts[0];
	OSG::Vec3f v31 = dualVerts[1] - dualVerts[3];

	// The face normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(v02.cross(v21)));	// Face 1
	norms->push_back(OSG::Vec3f(v02.cross(v30)));	// Face 2
	norms->push_back(OSG::Vec3f(v13.cross(v21)));	// Face 3
	norms->push_back(OSG::Vec3f(v03.cross(v10)));	// Face 4

	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
	colors->addValue(OSG::Color3f(0, 1, 0));
	colors->addValue(OSG::Color3f(0, 1, 0));
	colors->addValue(OSG::Color3f(0, 1, 0));
	colors->addValue(OSG::Color3f(0, 1, 0));


	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(2);
	//face 2 - triangle 2
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(3);
	//face 3 - triangle 3
	indices->addValue(2);
	indices->addValue(1);
	indices->addValue(3);
	//face 4 - triangle 4
	indices->addValue(1);
	indices->addValue(0);
	indices->addValue(3);

	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1: bottom
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	//face 2 - triangle 2
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	//face 3 - triangle 3
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	//face 4 - triangle 4
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);


	// Put it all together into a Geometry NodeCore.
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