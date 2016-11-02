#include <MyFace.h>

MyFace::MyFace(OSG::Pnt3f centre)
{
	faceCentre = centre;
};

/*
* Accessor method for faceCentre.
* Get the centre point/position of the face.
*/
OSG::Pnt3f MyFace::getFaceCentre()
{
	return faceCentre;
}

/*
* Mutator method for faceCentre.
* Change the value of faceCentre.
*/
void MyFace::setFaceCentre(OSG::Pnt3f centre)
{
	faceCentre = centre;
}

MyFace::~MyFace()
{

};