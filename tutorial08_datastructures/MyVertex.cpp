#include <MyVertex.h>

MyVertex::MyVertex(OSG::Pnt3f thevertex)
{
	vertex = thevertex;
};

/*
* Accessor method for vertex.
* Return the vertex's value.
*/
OSG::Pnt3f MyVertex::getVertex()
{
	return vertex;
}

/*
* Mutator method for vertex.
* Change the vertex value.
*/
void MyVertex::setVertex(OSG::Pnt3f thevertex)
{
	vertex = thevertex;
}

/*
* Add a MyFace object to the faces list.
* Add a face that this vertex shares.
*/
void MyVertex::addFace(MyFace* face)
{
	faces.push_back(face);
}

/*
* Accessor method for the faces list of MyFace objects.
* Return the list of faces this vertex shares.
*/
std::vector<MyFace*> MyVertex::getFaces()
{
	return faces;
}

/*
* Return the total number of faces this vertex shares.
*/
int MyVertex::getFacesLength()
{
	return faces.size();
}

MyVertex::~MyVertex()
{
	delete &faces;
	faces.clear();
};