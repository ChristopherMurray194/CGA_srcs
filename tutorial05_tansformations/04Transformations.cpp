// 04Transformations
//
// This tutorial supports learning
// about assembling a scene in a scene graph
// using transformation cores

// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
bool rotate;	// Activate rotation handle

OSG::TransformRefPtr mercuryTransformCore, venusTransformCore, earthTransformCore, moonTransformCore;	// Planet transform cores
OSG::TransformRefPtr mAxisTransformCore, vAxisTransformCore, eAxisTransformCore, mnAxisTransformCore;	// Planet axis transform cores
OSG::Vec3f mercuryPos(20, 0, 0), venusPos(35, 0, 0), earthPos(50, 0, 0), moonPos(6, 0, 0);				// Planet default positions

OSG::NodeRefPtr createSolarSystem();
OSG::Matrix applyRotation(OSG::Vec3f, OSG::Quaternion);

int setupGLUT( int *argc, char *argv[] );

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
    
		//read the file which will be passed as an argument
		OSG::NodeRefPtr scene = OSG::Node::create();

		// Create Sun node
		OSG::NodeRefPtr root = createSolarSystem();

		// Scene > sunNode
		scene->addChild(root);
		
		//commit all changes to OpenSG 
        OSG::commitChanges();
    
        // create the SimpleSceneManager helper
        mgr = OSG::SimpleSceneManager::create();
    
        // tell the manager what to manage
        mgr->setWindow(gwin);
        mgr->setRoot  (root);
    
        // show the whole scene
        mgr->showAll();
    }
    
    // GLUT main loop
    glutMainLoop();

    return 0;
}

OSG::NodeRefPtr createSolarSystem(){
	//================== Setup Sun geo node ===============================
	OSG::NodeRefPtr sunNode = OSG::Node::create();
	OSG::GeometryRefPtr sphere = OSG::makeSphereGeo(3, 10);
	sunNode->setCore(sphere);
	//=====================================================================

	//==================== Setup Mercury nodes ============================
	OSG::NodeRefPtr mercuryNode = OSG::Node::create();
	OSG::NodeRefPtr mercuryTransformNode = OSG::Node::create();
	
	// Create axis transformation node and core
	OSG::NodeRefPtr mAxisTransformNode = OSG::Node::create();
	mAxisTransformCore = OSG::Transform::create();
	
	mercuryTransformCore = OSG::Transform::create();	// Create mercury transformation core

	OSG::GeometryRefPtr mercuryGeom = OSG::makeSphereGeo(3, 3);
	mercuryNode->setCore(mercuryGeom);
	// Create core of the transformNode
	OSG::Matrix mercuryMatrix;
	mercuryMatrix.setIdentity();
	mercuryMatrix.setTransform(mercuryPos);
	mercuryTransformCore->setMatrix(mercuryMatrix);
	mercuryTransformNode->setCore(mercuryTransformCore);

	{
		OSG::Matrix axisMatrix;
		axisMatrix.setIdentity();
		mAxisTransformCore->setMatrix(axisMatrix);
		mAxisTransformNode->setCore(mAxisTransformCore);
	}
	//=====================================================================

	//=================== Setup Venus nodes ===============================
	OSG::NodeRefPtr venusNode = OSG::Node::create();
	OSG::NodeRefPtr venusTransformNode = OSG::Node::create();

	OSG::NodeRefPtr vAxisTransformNode = OSG::Node::create();
	vAxisTransformCore = OSG::Transform::create();

	venusTransformCore = OSG::Transform::create();

	OSG::GeometryRefPtr venusGeom = OSG::makeSphereGeo(3, 4);
	venusNode->setCore(venusGeom);
	// Create core of the transformNode
	OSG::Matrix venusMatrix;
	venusMatrix.setIdentity();
	venusMatrix.setTransform(venusPos);
	venusTransformCore->setMatrix(venusMatrix);
	venusTransformNode->setCore(venusTransformCore);

	{
		OSG::Matrix axisMatrix;
		axisMatrix.setIdentity();
		vAxisTransformCore->setMatrix(axisMatrix);
		vAxisTransformNode->setCore(vAxisTransformCore);
	}
	//=====================================================================
	
	//=================== Setup Earth nodes ===============================
	OSG::NodeRefPtr earthNode = OSG::Node::create();
	OSG::NodeRefPtr earthTransformNode = OSG::Node::create();

	OSG::NodeRefPtr eAxisTransformNode = OSG::Node::create();
	eAxisTransformCore = OSG::Transform::create();

	earthTransformCore = OSG::Transform::create();

	OSG::GeometryRefPtr earthGeom = OSG::makeSphereGeo(3, 3);
	earthNode->setCore(earthGeom);
	// Create core of the transformNode
	OSG::Matrix earthMatrix;
	earthMatrix.setIdentity();
	earthMatrix.setTransform(earthPos);
	earthTransformCore->setMatrix(earthMatrix);
	earthTransformNode->setCore(earthTransformCore);

	{
		OSG::Matrix axisMatrix;
		axisMatrix.setIdentity();
		eAxisTransformCore->setMatrix(axisMatrix);
		eAxisTransformNode->setCore(eAxisTransformCore);
	}
	//====================================================================

	//==================== Setup Moon nodes ==============================
	OSG::NodeRefPtr moonNode = OSG::Node::create();
	OSG::NodeRefPtr moonTransformNode = OSG::Node::create();

	OSG::NodeRefPtr mnAxisTransformNode = OSG::Node::create();
	mnAxisTransformCore = OSG::Transform::create();

	moonTransformCore = OSG::Transform::create();

	OSG::GeometryRefPtr moonGeom = OSG::makeSphereGeo(3, 1);
	moonNode->setCore(moonGeom);
	// Create core of the transformNode
	OSG::Matrix moonMatrix;
	moonMatrix.setIdentity();
	// Transformation is relative to the Earth as the moon is its child
	moonMatrix.setTransform(moonPos);
	moonTransformCore->setMatrix(moonMatrix);
	moonTransformNode->setCore(moonTransformCore);

	{
		OSG::Matrix axisMatrix;
		axisMatrix.setIdentity();
		mnAxisTransformCore->setMatrix(axisMatrix);
		mnAxisTransformNode->setCore(mnAxisTransformCore);
	}
	//====================================================================

	// SunNode > transform nodes
	sunNode->addChild(mercuryTransformNode);
	sunNode->addChild(venusTransformNode);
	sunNode->addChild(earthTransformNode);
	earthNode->addChild(moonTransformNode);
	// transform nodes > geometry nodes
	mercuryTransformNode->addChild(mAxisTransformNode);
	mAxisTransformNode->addChild(mercuryNode);
	venusTransformNode->addChild(vAxisTransformNode);
	vAxisTransformNode->addChild(venusNode);
	earthTransformNode->addChild(eAxisTransformNode);
	eAxisTransformNode->addChild(earthNode);
	moonTransformNode->addChild(mnAxisTransformNode);
	mnAxisTransformNode->addChild(moonNode);

	return sunNode;
}

/*
* Function created to avoid duplicate code as the code to apply rotation for each planet is the same.
*
* Create the new transformed matrix
*/
OSG::Matrix applyRotation(OSG::Vec3f trans, OSG::Quaternion rot)
{
	OSG::Matrix translationMatrix, rotationMatrix;	// Create translation and rotation matrices
	translationMatrix.setIdentity();
	rotationMatrix.setIdentity();

	translationMatrix.setTransform(trans);			// Set the translation 
	rotationMatrix.setRotate(rot);					// Set the rotation
	rotationMatrix.mult(translationMatrix);			// rotation * translation - Translation matrix gets applied first
	
	return rotationMatrix;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
	if (rotate)
	{	
		OSG::Real32 time = glutGet(GLUT_ELAPSED_TIME);
		float valuetorotate = time / (1000);
		OSG::Quaternion rot = OSG::Quaternion(OSG::Vec3f(0, 1, 0), valuetorotate);

		// Setup Mecury matrix with rotation applied
		mercuryTransformCore->setMatrix(applyRotation(mercuryPos, rot));

		// Setup Venus matrix with rotation applied
		valuetorotate = time / (1000/0.7);	// Rotate at different speed
		rot = OSG::Quaternion(OSG::Vec3f(0, 1, 0), valuetorotate);
		venusTransformCore->setMatrix(applyRotation(venusPos, rot));

		// Setup Earth matrix with rotation applied
		valuetorotate = time / (1000/0.9);
		rot = OSG::Quaternion(OSG::Vec3f(0, 1, 0), valuetorotate);
		earthTransformCore->setMatrix(applyRotation(earthPos, rot));

		// Setup Moon matrix with rotation applied
		valuetorotate = time / (1000/0.5);
		rot = OSG::Quaternion(OSG::Vec3f(0, 1, 0), valuetorotate);
		moonTransformCore->setMatrix(applyRotation(moonPos, rot));

		// Setup rotation matrix for local axis rotation
		valuetorotate = time / (1000);
		rot = OSG::Quaternion(OSG::Vec3f(0, 1, 0), valuetorotate);
		mAxisTransformCore->setMatrix(applyRotation(OSG::Vec3f(0, 0, 0), rot));
		vAxisTransformCore->setMatrix(applyRotation(OSG::Vec3f(0, 0, 0), rot));
		eAxisTransformCore->setMatrix(applyRotation(OSG::Vec3f(0, 0, 0), rot));
		mnAxisTransformCore->setMatrix(applyRotation(OSG::Vec3f(0, 0, 0), rot));

	}
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
		case 'r':
		{
			if (!rotate)
			{
				rotate = true;
				// Call the redraw function whenever there's nothing else to do
				glutIdleFunc(display);
			}
			else
			{
				rotate = false;
				// Go back to normal whenever rotation is false
				glutIdleFunc(NULL);
			}
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
    
    int winid = glutCreateWindow("04 Transformations Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    return winid;
}