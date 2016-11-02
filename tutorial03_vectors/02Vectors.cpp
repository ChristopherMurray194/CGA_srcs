// 02Vectors 
//
// This tutorial will 
// show how to use vectors in OpenSG
// and visualise them in an interactive scene viewer.


#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGGeoProperties.h>
#include <iostream>

// The SimpleSceneManager to manage simple applications
OSG::SimpleSceneManagerRefPtr mgr;
// forward declarations 
int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr drawPointInScene(OSG::Pnt3f point,OSG::Color3f color);
OSG::NodeRefPtr drawVectorInScene(OSG::Vec3f vector, OSG::Color3f color);
void displayVectorValues(OSG::Vec3f vector);

int main(int argc, char **argv)
{
	
    OSG::osgInit(argc,argv);

    int winid = setupGLUT(&argc, argv);

    {
        OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();
    
        OSG::NodeRefPtr scene = OSG::Node::create();
		scene->setCore(OSG::Group::create());

		//EXAMPLE CREATING A POINT
		OSG::Pnt3f p1(0,0,0);
		scene->addChild(drawPointInScene(p1,OSG::Color3f(0,0,1)));

		//==== First set of three points =====
		OSG::Pnt3f p2(1, 0, 0);
		OSG::Pnt3f p3(0, 1, 0);
		OSG::Pnt3f p4(0, 0, 1);
		scene->addChild(drawPointInScene(p2, OSG::Color3f(0, 1, 0)));
		scene->addChild(drawPointInScene(p3, OSG::Color3f(0, 1, 0)));
		scene->addChild(drawPointInScene(p4, OSG::Color3f(0, 1, 0)));
		//=====================================

		//===== Second set of three points ====
		OSG::Pnt3f p5(-1, 0, 0);
		OSG::Pnt3f p6(0, -1, 0);
		OSG::Pnt3f p7(0, 0, -1);
		scene->addChild(drawPointInScene(p5, OSG::Color3f(1, 0, 0)));
		scene->addChild(drawPointInScene(p6, OSG::Color3f(1, 0, 0)));
		scene->addChild(drawPointInScene(p7, OSG::Color3f(1, 0, 0)));
		//======================================

		//EXAMPLE CREATING A VECTOR
		OSG::Vec3f v1(p2 - p1);
		//scene->addChild(drawVectorInScene(v1,OSG::Color3f(1,1,1)));

		//===== Creating the vectors ========
		OSG::Vec3f v2(p3 - p1);
		//scene->addChild(drawVectorInScene(v2, OSG::Color3f(1, 1, 1)));

		OSG::Pnt3f p1v3(1, 1, 1);
		OSG::Pnt3f p2v3(5, 5, 5);
		OSG::Vec3f v3(p2v3 - p1v3);
		scene->addChild(drawVectorInScene(v3, OSG::Color3f(1, 1, 1)));

		OSG::Pnt3f p1v4(-1, 1, 1);
		OSG::Pnt3f p2v4(5, 5, 5);
		OSG::Vec3f v4(p2v4 - p1v4);
		scene->addChild(drawVectorInScene(v4, OSG::Color3f(1, 1, 1)));

		//displayVectorValues(v3);	// Display v3 values to console
		//displayVectorValues(v4);	// Display v4 values to console
		//===================================

		//======== Vector addition ==========
		OSG::Vec3f v1v2 = v1 + v2;
		//scene->addChild(drawVectorInScene(v1v2, OSG::Color3f(1, 1, 1)));
		//displayVectorValues(v1v2);
		//===================================

		//======= Vector multiply by scalar ======
		OSG::Vec3f v1Mult = v1 * 5;
		//scene->addChild(drawVectorInScene(v1Mult, OSG::Color3f(1, 0, 0)));
		//displayVectorValues(v1Mult);

		OSG::Vec3f v2Mult = v2 * 0.2;
		//scene->addChild(drawVectorInScene(v2Mult, OSG::Color3f(0, 1, 0)));
		//displayVectorValues(v2Mult);
		//========================================

		//====== Vector subtraction =========
		OSG::Vec3f v1v4 = v1 - v4;
		OSG::Vec3f v2v3 = v2 - v3;

		//scene->addChild(drawVectorInScene(v1v4, OSG::Color3f(1, 0, 0)));
		//scene->addChild(drawVectorInScene(v2v3, OSG::Color3f(0, 1, 0)));

		//displayVectorValues(v1v4);
		//displayVectorValues(v2v3);
		//===================================

		//========= Vector magnitude========
		float vec3Mag = std::sqrt(
									(v3.x() * v3.x()) + 
									(v3.y() * v3.y()) + 
									(v3.z() * v3.z())
									);
		std::cout << "|v3| : " << vec3Mag << std::endl;
		//==================================

		//======= Vector normailisation ========
		float vec4Mag = std::sqrt(
								(v4.x() * v4.x()) + 
								(v4.y() * v4.y()) + 
								(v4.z() * v4.z())
								);
		OSG::Vec3f Nv4 = v4 / vec4Mag;
		//scene->addChild(drawVectorInScene(Nv4, OSG::Color3f(0, 0, 1)));
		std::cout << "v4 Normal: " << Nv4.x() + Nv4.y() + Nv4.z() << std::endl;
		//======================================

		//======= Vector Dot Product ===========
		// Normalize the vectors
		// RESULT > 0 angle is less than 90 degrees
		// RESULT = 0 angle is exactly 90 degrees
		// RESULT < 0 angle is greater than 90 degrees
		float v1v2Dot = ((v1.x() * v2.x()) + 
						(v1.y() * v2.y()) + 
						(v1.z() * v2.z())
						);
		float v1v3Dot = ((v1.x() * v3.x()) + 
						(v1.y() * v3.y()) + 
						(v1.z() * v3.z())
						);

		float v2v1Dot = ((v2.x() * v1.x()) + 
						(v2.y() * v1.y()) + 
						(v2.z() * v1.z())
						);
		std::cout << "v1 . v2 : " << v1v2Dot << std::endl;
		std::cout << "v1 . v3 : " << v1v3Dot << std::endl;
		std::cout << "v2 . v1 : " << v2v1Dot << std::endl;

		// Symmetry
		std::cout << "Symmetry" << std::endl;
		std::cout << v1v2Dot << std::endl << v2v1Dot << std::endl;
		std::cout << std::endl;

		// Linearity
		float linearity = (v1 + v2).dot(v3);
		float linearity2 = v1.dot(v3) + v2.dot(v3);
		std::cout << "Linearity:" << std::endl;
		std::cout << linearity << std::endl << linearity2 << std::endl;
		std::cout << std::endl;

		// Homogeneity
		float homogeneity = (v1 * 3).dot(v2);
		float homogeneity2 = 3 * (v1.dot(v2));
		std::cout << "Homogeneity" << std::endl;
		std::cout << homogeneity << std::endl << homogeneity2 << std::endl;
		std::cout << std::endl;

		// |b| squared
		float v1Mag = std::sqrt(
									(v1.x() * v1.x()) + 
									(v1.y() * v1.y()) + 
									(v1.z() * v1.z())
									);
		float v1Sqr = std::sqrt(v1Mag);
		float v1Dot = v1.dot(v1);
		std::cout << "|b|sqr" << std::endl;
		std::cout << v1Sqr << std::endl << v1Dot << std::endl;
		std::cout << std::endl;
		//======================================

		//========== Cross Product =============
		OSG::Vec3f v3v4Cross = ((((v3.y() * v4.z()) - (v4.y() * v3.z())) * OSG::Vec3f(1, 0, 0)) -
								(((v3.x() * v4.z()) - (v4.x() * v3.z())) * OSG::Vec3f(0, 1, 0)) +
								(((v3.x() * v4.y()) - (v4.x() * v3.y())) * OSG::Vec3f(0, 0, 1))
								);
		scene->addChild(drawVectorInScene(v3v4Cross, OSG::Color3f(0, 1, 0)));
		//======================================

		OSG::commitChanges();
    
        mgr = OSG::SimpleSceneManager::create();
    
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
        //mgr->showAll();
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

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("02Vectors Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    return winid;
}
OSG::NodeRefPtr drawPointInScene(OSG::Pnt3f point,OSG::Color3f color){

	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_POINTS);
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(1);
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	pnts->addValue(point);
	OSG::GeoVec3fPropertyRefPtr colors = OSG::GeoVec3fProperty::create();
	colors->addValue(color);
	
	geo->setTypes    (type);
	geo->setLengths  (lens);
	geo->setPositions(pnts);
	geo->setColors	 (colors);
	
    
    // assign a material to the geometry to make it visible. 
    geo->setMaterial(OSG::getDefaultUnlitMaterial());   
        
	geonode->setCore(geo);
	return geonode;
}
OSG::Pnt3f getPointAlongLine(OSG::Pnt3f pA, OSG::Pnt3f pB, OSG::Real32 distance){
			
			OSG::Vec3f vA(pA);
			OSG::Vec3f vAB(pB-pA);
			OSG::Vec3f vC = vA + distance * vAB;
			OSG::Pnt3f newPoint(vC.x(), vC.y(), vC.z());

			return newPoint;

}
OSG::NodeRefPtr drawVectorInScene(OSG::Vec3f vector,OSG::Color3f color){

	OSG::NodeRefPtr geogroup = OSG::Node::create();
	geogroup->setCore(OSG::Group::create());

	//the line
	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_LINES);
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(2);
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	
	
	OSG::Pnt3f fpoint = OSG::Pnt3f(vector.x(),vector.y(),vector.z());
	OSG::Pnt3f ipoint = fpoint-fpoint;
	pnts->addValue(ipoint);
	pnts->addValue(fpoint);

	OSG::GeoVec3fPropertyRefPtr colors = OSG::GeoVec3fProperty::create();
	colors->addValue(color);
	
	geo->setTypes    (type);
	geo->setLengths  (lens);
	geo->setPositions(pnts);
	geo->setColors	 (colors);
	
    
    // assign a material to the geometry to make it visible. 
    geo->setMaterial(OSG::getDefaultUnlitMaterial());   
	geonode->setCore(geo);
	geogroup->addChild(geonode);


	//the line
	OSG::NodeRefPtr geonode1 = OSG::Node::create();
	OSG::GeometryRefPtr geo1 = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type1 = OSG::GeoUInt8Property::create();
	type1->addValue(GL_LINE);
	OSG::GeoUInt32PropertyRefPtr lens1 = OSG::GeoUInt32Property::create();
	lens1->addValue(4);
	OSG::GeoPnt3fPropertyRefPtr  pnts1 = OSG::GeoPnt3fProperty::create();
	//calculate an arrow
	//get a point along the vector
	OSG::Pnt3f pointalongline = getPointAlongLine(ipoint, fpoint,0.8);
	//get the distance to the second point of the vector
	OSG::Real32 dist= fpoint.dist(pointalongline);
	//now look for the points for the arrows
	float angle = 20;
	OSG::Real32 npx = fpoint.x()+(pointalongline.x()-fpoint.x())*cos(OSG::osgDegree2Rad(angle))-(pointalongline.y()-fpoint.y())*sin(OSG::osgDegree2Rad(angle));
	OSG::Real32 npy = fpoint.y()+(pointalongline.x()-fpoint.x())*sin(OSG::osgDegree2Rad(angle))+(pointalongline.y()-fpoint.y())*cos(OSG::osgDegree2Rad(angle));
	pnts1->addValue(OSG::Pnt3f(npx,npy,pointalongline.z()));
	pnts1->addValue(fpoint);
	
	OSG::Real32 npx1 = fpoint.x()+(pointalongline.x()-fpoint.x())*cos(OSG::osgDegree2Rad(-angle))-(pointalongline.y()-fpoint.y())*sin(OSG::osgDegree2Rad(-angle));
	OSG::Real32 npy1 = fpoint.y()+(pointalongline.x()-fpoint.x())*sin(OSG::osgDegree2Rad(-angle))+(pointalongline.y()-fpoint.y())*cos(OSG::osgDegree2Rad(-angle));
	pnts1->addValue(fpoint);
	pnts1->addValue(OSG::Pnt3f(npx1,npy1,pointalongline.z()));

	OSG::GeoVec3fPropertyRefPtr colors1 = OSG::GeoVec3fProperty::create();
	colors1->addValue(color);
	
	geo1->setTypes    (type1);
	geo1->setLengths  (lens1);
	geo1->setPositions(pnts1);
	geo1->setColors	 (colors1);
	
    
    // assign a material to the geometry to make it visible. 
    geo1->setMaterial(OSG::getDefaultUnlitMaterial());   
	geonode1->setCore(geo1);
	geogroup->addChild(geonode1);

	return geogroup;
}
void displayVectorValues(OSG::Vec3f vector){
		std::cout << "Vector values (" << vector.x() << ", " << vector.y() << ", " << vector.z() << ") " << std::endl;
}
