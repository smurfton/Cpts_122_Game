#include "Small_Scene_Render.h"

void addVectors(vector<Vertex> &v1, vector<Vertex> &v2) {
	//add everything to v1.
	for (int i = 0; i < v2.size(); i++) {
		(v1).push_back(v2[i]);
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

Transformation calculateTransform(Position desiredLocation, Position desiredOrientation, float scale) {
	Transformation transform = {{0,0,0}, {1,0,0}, 0};
	Position currentOrientation = {1,0,0};
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
	currentOrientation.x = x1;
	currentOrientation.y = y1;
	currentOrientation.z = z1;
	desiredOrientation.x = x2;
	desiredOrientation.y = y2;
	desiredOrientation.z = z2;

	//first calculate the normal vector by crossing the desired orientation and the current orientation
	transform.normalVector.x = y1*z2-z1*y2;
	transform.normalVector.y = z1*x2-x1*z2;
	//normal has been calculated

	//calculate the degree between the desired orientation and the current orientation
	float magnitudeCross = sqrt(transform.normalVector.x*transform.normalVector.x + transform.normalVector.y*transform.normalVector.y + transform.normalVector.z*transform.normalVector.z);
	float magnitudeCurr = sqrt(x1*x1 + y1*y1 + z1*z1);
	float magnitudeDesired = sqrt(x2*x2 + y2*y2 + z2*z2);
	transform.rotation = asin(magnitudeCross/(magnitudeCurr*magnitudeDesired));
	transform.location = desiredLocation;
	transform.scale = scale;

	return transform;
}

void translate(vector<Vertex> &v1, vector<Vertex> &v2, Position newLocation) {

	//translate the points in the vector by this much
	translate(v1, v2, 'x', newLocation.x);
	translate(v1, v2, 'y', newLocation.y);
	translate(v1, v2, 'z', newLocation.z);
}

void transformVector(Transformation transform, vector<Vertex> &v1, vector<Vertex> &v2) {
	//first we must copy the vectors
	copyVector(v1, v2);
	//first we must rotate the vertices
	//rotate(v1, v2, transform.normalVector, transform.rotation);

	//now we should scale
	scale(v1, v2, transform.scale);

	//next we must translate the vector
	translate(v1, v2, transform.location);

}

void copyVector (vector<Vertex> &v1, vector<Vertex> &v2) {
	//empty v2 first
	while (v2.size() > 0) {
		v2.pop_back();
	}
	//copy information from v1
	for (int i = 0; i < v1.size(); i++) {
		v2.push_back(v1.at(i));
	}
}

void calculateNormal(vector<Vertex> &v1) {
	Vertex p1, p2, p3;
	Vector normal;
	float length = 0;
//loop through each triangle
	for (int i = 0; i < (v1.size()/3); i++) {
		//get the three points of the triangle
		p1 = v1.at(3*i);
		p2 = v1.at(3*i+1);
		p3 = v1.at(3*i+2);

		//calculate the normal for this surface
		normal.x = (p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y);
		normal.y = -((p2.x-p1.x)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.x-p1.x));
		normal.z = (p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x);

		//calculate the normal length
		length = calculateMagnitude(normal);
		normal.x /= length;
		normal.y /= length;
		normal.z /= length;

		//assign the normal to each of the points
		v1.at(3*i).normal.x = normal.x;
		v1.at(3*i).normal.y = normal.y;
		v1.at(3*i).normal.z = normal.z;
		
		v1.at(3*i+1).normal.x = normal.x;
		v1.at(3*i+1).normal.y = normal.y;
		v1.at(3*i+1).normal.z = normal.z;
		
		v1.at(3*i+2).normal.x = normal.x;
		v1.at(3*i+2).normal.y = normal.y;
		v1.at(3*i+2).normal.z = normal.z;
	}
}

float calculateMagnitude(Vector v1) {
	return sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
}

Vector scaleVector(Vector v1, float length) {
	Vector newVector;
	float lengthV1 = 0;
	//calculate the unit vector of v1
	lengthV1 = calculateMagnitude(v1);

	newVector.x = v1.x/(lengthV1*lengthV1);
	newVector.y = v1.y/(lengthV1*lengthV1);
	newVector.z = v1.z/(lengthV1*lengthV1);

	//multiply these values by the length desired
	newVector.x *= length*length;
	newVector.y *= length*length;
	newVector.z *= length*length;

	return newVector;
}