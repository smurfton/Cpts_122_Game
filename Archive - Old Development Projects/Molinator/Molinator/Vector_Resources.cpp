#include "Molinator.h"

void combineVectors(vector<Vertex> &v1, vector<Vertex> &v2) {

	//add everything to v1.
	for (int i = 0; i < v2.size(); i++) {
		(v1).push_back(v2[i]);
	}
	//free up v2
	for (int i = v2.size(); i > 0; i--) {
		v2.pop_back();
	}
}

void translate(vector<Vertex> &v1, vector<Vertex> &v2, char dir, float amount) {
	for (int i = 0; i < v1.size(); i++) {
		switch(dir) {
		case 'x':
			v2.at(i).x += amount;
			break;
		case 'y':
			v2.at(i).y += amount;
			break;
		case 'z':
			v2.at(i).z += amount;
			break;
		}
	}
}

void scale(vector<Vertex> &v1, vector<Vertex> &v2, float factor) {
	for (int i = 0; i < v1.size(); i++) {
		v2.at(i).x *= factor;
		v2.at(i).y *= factor;
		v2.at(i).z *= factor;
	}
}

//current does not work!
void rotate(vector<Vertex> &v1, vector<Vertex> &v2, Position axis, float radians) {
	//we must rotate points in the vector about the amount of degrees specified
	float transform[9] = {
	axis.x*axis.x*(1-cos(radians)) + cos(radians), axis.x*axis.y*(1-cos(radians))-axis.z*sin(radians), axis.z*axis.x*(1-cos(radians)) + axis.y*sin(radians),
	axis.x*axis.y*(1-cos(radians))+axis.z*sin(radians), axis.y*axis.y*(1-cos(radians)) + cos(radians), axis.z*axis.y*(1-cos(radians)) - axis.x*sin(radians),
	axis.x*axis.z*(1-cos(radians))-axis.y*sin(radians), axis.y*axis.z*(1-cos(radians))+axis.x*sin(radians), axis.z*axis.z*(1-cos(radians)) + cos(radians)
	};

	//now multiple the transform with the coordinates of each point in the vector
	for (int i = 0; i < v1.size(); i++) {
		v2.at(i).x = transform[0] * v1.at(i).x + transform[1] * v1.at(i).y + transform[2] * v1.at(i).z;
		v2.at(i).y = transform[3] * v1.at(i).x + transform[4] * v1.at(i).y + transform[5] * v1.at(i).z;
		v2.at(i).z = transform[6] * v1.at(i).x + transform[7] * v1.at(i).y + transform[8] * v1.at(i).z;
	}
}

void calculateTransform(Position currentOrientation, Position desiredOrientation, Position *normal, float *radians) {
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
	currentOrientation.x = x1;
	currentOrientation.y = y1;
	currentOrientation.z = z1;
	desiredOrientation.x = x2;
	desiredOrientation.y = y2;
	desiredOrientation.z = z2;

	//first calculate the normal vector by crossing the desired orientation and the current orientation
	normal->x = y1*z2-z1*y2;
	normal->y = z1*x2-x1*z2;
	normal->z = x1*y2-y1*x2;
	//normal has been calculated

	//calculate the degree between the desired orientation and the current orientation
	float magnitudeCross = sqrt(normal->x*normal->x + normal->y*normal->y + normal->z*normal->z);
	float magnitudeCurr = sqrt(x1*x1 + y1*y1 + z1*z1);
	float magnitudeDesired = sqrt(x2*x2 + y2*y2 + z2*z2);
	*radians = asin(magnitudeCross/(magnitudeCurr*magnitudeDesired));
}

void translate(vector<Vertex> &v1, vector<Vertex> &v2, Position currentLocation, Position newLocation) {
	//calculate the distances we need to move
	float moveX = 0, moveY = 0, moveZ = 0;
	moveX = newLocation.x - currentLocation.x;
	moveY = newLocation.y - currentLocation.y;
	moveZ = newLocation.z - currentLocation.z;

	//translate the points in the vector by this much
	translate(v1, v2, 'x', moveX);
	translate(v1, v2, 'y', moveX);
	translate(v1, v2, 'z', moveX);
}