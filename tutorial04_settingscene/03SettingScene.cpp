
// 03SettingScene 
//
// This tutorial will 
// set up a simple interactive scene 
// 

// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>

// GLUT Menu items
enum MENU_ITEMS
{
	WIREFRAME,
	SOLID,
	VERTEX,
};

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
OSG::NodeRefPtr spherenode;
OSG::NodeRefPtr file;	// Read the file which will be passed as an argument
OSG::BoxVolume vol;

void createMenu();
int setupGLUT(int *argc, char *argv[]);

int main(int argc, char **argv)
{

	// Initialise GLUT
	int winid = setupGLUT(&argc, argv);

	// Initialise OpenSG
	OSG::osgInit(argc, argv);

	// Create a OSGGLUT window
	OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
	gwin->setGlutId(winid);
	gwin->init();

	// Create the SimpleSceneManager helper
	mgr = OSG::SimpleSceneManager::create();

	// Tell the manager what to manage
	mgr->setWindow(gwin);
	
	/*
	OSG::GeometryRefPtr spheregeo = OSG::makeSphereGeo(2, 1);
	spherenode = OSG::Node::create();
	OSG::GeometryRefPtr sphereGeo02 = OSG::makeSphereGeo(2, 5);
	OSG::NodeRefPtr sphereNode02 = OSG::Node::create();

	spherenode->setCore(spheregeo);
	sphereNode02->setCore(sphereGeo02);
	spherenode->addChild(sphereNode02);
	*/

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			std::string option = argv[i];
			if (option == "-f")
				file = OSG::SceneFileHandler::the()->read(argv[i + 1]);
		}
	}

	OSG::commitChanges();

	mgr->setRoot(file);

	// Show the whole scene
	mgr->showAll();

	mgr->setHighlight(file); // Visualise the bounding box

	// GLUT main loop
	glutMainLoop();

	return 0;
}

// GLUT callback functions
//

// Redraw the window
void display(void)
{

	// Get the volume of the node and all its children
	file->getWorldVolume(vol);

	// Get minimum and maximum values
	OSG::Pnt3f min, max;
	vol.getBounds(min, max);
	std::cout << min << " " << max << std::endl;

	mgr->redraw();
}

// React to size changes
void reshape(int w, int h)
{
	mgr->resize(w, h);
	glutPostRedisplay();
}

// React to mouse button presses
void mouse(int button, int state, int x, int y)
{
	if (!state)
		mgr->mouseButtonPress(button, x, y);

	glutPostRedisplay();
}

// React to mouse motions with pressed buttons
void motion(int x, int y)
{
	mgr->mouseMove(x, y);
	glutPostRedisplay();
}

// React to keys
void keyboard(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 'e':
		{
			// Clean up global variables
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

void menu(int item)
{
	switch (item)
	{
		case VERTEX:
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		break;
		case WIREFRAME:
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		break;
		case SOLID:
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		break;
		default:
		{ }
		break;
	}

	glutPostRedisplay();

	return;
}

void createMenu()
{
	glutCreateMenu(menu);
	// Add menu items
	glutAddMenuEntry("Show vertices", VERTEX);
	glutAddMenuEntry("Show wireframe", WIREFRAME);
	glutAddMenuEntry("Show solid", SOLID);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return;
}

// Setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	int winid = glutCreateWindow("03SettingScene Tutorial");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	createMenu();

	return winid;
}
