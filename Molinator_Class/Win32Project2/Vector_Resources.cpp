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

void scale(vector<Vertex> &v1, vector<Vertex> &v2, Vector factor) {
	for (int i = 0; i < v1.size(); i++) {
		v2.at(i).x *= factor.x;
		v2.at(i).y *= factor.y;
		v2.at(i).z *= factor.z;
	}
}

void rotate(vector<Vertex> &v1, vector<Vertex> &v2, Vector rotation) {

	float magnitude = 0, currentAngle = 0, newAngle = 0;
	//loop through each vertice
	for (int i = 0; i < v1.size(); i++) {

		//rotate about the x axis - x remains constant
		/*//calculate the magnitude of the unit circle vector
		magnitude = sqrt(v2.at(i).y*v2.at(i).y+v2.at(i).z*v2.at(i).z);
		currentAngle = 0;

		//calculate our current angle
		//if quad 4/1
		if (v2.at(i).z > 0) {
			currentAngle = asin(v2.at(i).y/magnitude);
		} else if (v2.at(i).z == 0) { //if on boundary
			if (v2.at(i).y > 0) {
				currentAngle = D3DXToRadian(90);
			} else {
				currentAngle = D3DXToRadian(270);
			}
		} else {
		//else on quad 2/3
			currentAngle = acos(v2.at(i).y/magnitude)+ D3DXToRadian(90);
		}

		float newAngle = currentAngle + D3DXToRadian(rotation.x);

		//recalculate the vertice position
		v2.at(i).y = magnitude*sin(newAngle);
		v2.at(i).z = magnitude*cos(newAngle);
		v2.at(i).x = v2.at(i).x;*/
		if (rotation.x != 0) {
			//calculate the magnitude
			magnitude = sqrt(v2.at(i).y*v2.at(i).y+v2.at(i).z*v2.at(i).z);

			//calculate the current angle
			if (v2.at(i).z > 0) { //quad 4/1
				currentAngle = asin(v2.at(i).y/magnitude);
			} else if (v2.at(i).z == 0) { //boundary
				if (v1.at(i).y > 0) {
					currentAngle = D3DXToRadian(90);
				} else {
					currentAngle = D3DXToRadian(270);
				}
			} else { //quad 2/3
				currentAngle = acos(v2.at(i).y/magnitude)+ D3DXToRadian(90);
			}

			//calculate new angle
			newAngle = currentAngle + D3DXToRadian(rotation.x);

			//calculate the vertice positions
			v2.at(i).x = v2.at(i).x;
			v2.at(i).y = magnitude * sin(newAngle);
			v2.at(i).z = magnitude * cos(newAngle);
		}

		//rotate about the y axis - y remains constant
		if (rotation.y != 0) {
			//calculate the magnitude
			magnitude = sqrt(v2.at(i).x*v2.at(i).x+v2.at(i).z*v2.at(i).z);

			//calculate the current angle
			if (v2.at(i).x > 0) { //quad 4/1
				currentAngle = asin(v2.at(i).z/magnitude);
			} else if (v2.at(i).x == 0) { //boundary
				if (v2.at(i).z > 0) {
					currentAngle = D3DXToRadian(90);
				} else {
					currentAngle = D3DXToRadian(270);
				}
			} else { //quad 2/3
				currentAngle = acos(v2.at(i).z/magnitude)+ D3DXToRadian(90);
			}

			//calculate the new angle
			newAngle = currentAngle + D3DXToRadian(rotation.y);

			//calculate vertice positions
			v2.at(i).y = v2.at(i).y;
			v2.at(i).x = magnitude * cos(newAngle);
			v2.at(i).z = magnitude * sin(newAngle);
		}
		//rotate about the z axis - z remains constant
			if (rotation.z != 0) {
			//calculate magnitude
			magnitude = sqrt(v2.at(i).x*v2.at(i).x+v2.at(i).y*v2.at(i).y);

			//calculate current angle
			if (v2.at(i).x > 0) { //quad 4/1
				currentAngle = asin(v2.at(i).y/magnitude);
			} else if (v2.at(i).x == 0) { //boundary
				if (v2.at(i).y > 0) {
					currentAngle = D3DXToRadian(90);
				} else {
					currentAngle = D3DXToRadian(270);
				}
			} else { //quad 2/3
				currentAngle = acos(v2.at(i).y/magnitude)+ D3DXToRadian(90);
			}

			//calculate new angle
			newAngle = currentAngle + D3DXToRadian(rotation.z);

			//calculate vertice positions
			v2.at(i).z = v2.at(i).z;
			v2.at(i).x = magnitude * cos(newAngle);
			v2.at(i).y = magnitude * sin(newAngle);
		}
		//vertices should now be transformed
	}


}

/*Transformation calculateTransform(Position desiredLocation, Position desiredOrientation, float scale) {
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
}*/

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
	scale(v1, v2, transform.scaling);

	//rotate the object
	rotate(v1, v2, transform.rotation);

	//next we must translate the vector
	translate(v1, v2, transform.translation);

}

Cube transformHitBox(Cube box, Transformation transform) {

	Cube box2;
	//load the box into a vector
	vector<Vertex> temp, temp2;
	for (int i = 0; i < 8; i++) {
		temp.push_back(box.vertices[i]);
	}
	copyVector(temp, temp2);

	//scale the hitbox
	scale(temp, temp2, transform.scaling);

	//rotate the hitbox
	rotate(temp, temp2, transform.rotation);

	//translate the hitbox
	translate(temp, temp2, transform.translation);

	//extract the information from the temp2 vector back into the box object
	for (int i = 0; i < 8; i++) {
		box2.vertices[i] = temp2.at(i);
	}
	//box is transformed
	return box2;
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
		/*length = calculateMagnitude(normal);
		normal.x /= length;
		normal.y /= (length);
		normal.z /= (length);*/

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

//returns true if the objects collide
bool checkVerticesCube(Cube c1, Cube c2) {
	bool collision = false, collX = false, collY = false, collZ = false;
	//pick a vertice to serve as the origin of the new coordinate system
	Vertex v1 = c1.vertices[0]; //origin
	Vertex v3 = c1.vertices[4]; //unit X point
	Vertex v4 = c1.vertices[1]; //unit Z point
	Vertex v2 = c1.vertices[2]; //unit Y point
	vector<Vector> collision2Sides;

	Vertex point;

	//now select unit vector vertices.
	//they are stored in a form such that the first 4 make up one face, and the second four make up the second face
	//generate new unit vectors associated with a coordinate space about cube2
	Vector uX, uY, uZ;
	Vector components;

	uX.x = v3.x - v1.x;
	uX.y = v3.y - v1.y;
	uX.z = v3.z - v1.z;

	uY.x = v2.x - v1.x;
	uY.y = v2.y - v1.y;
	uY.z = v2.z - v1.z;

	uZ.x = v4.x - v1.x;
	uZ.y = v4.y - v1.y;
	uZ.z = v4.z - v1.z;

	//we have now generated our unit vectors associated with our cube.

	//we should now check the point to see if it is a linear combination of the unit vectors
	//check each vertice of cube 1 to see if it is within cube 2
	for (int i = 0; i < 8; i++) {
		point = c2.vertices[i];

		//calculate our vector in relation to the cube
		point.x -= v1.x;
		point.y -= v1.y;
		point.z -= v1.z;

		//calculate the components
		components.x = (point.x*uX.x + point.y*uX.y + point.z*uX.z)/ (uX.x*uX.x + uX.y*uX.y + uX.z*uX.z);
		components.y = (point.x*uY.x + point.y*uY.y + point.z*uY.z)/ (uY.x*uY.x + uY.y*uY.y + uY.z*uY.z);
		components.z = (point.x*uZ.x + point.y*uZ.y + point.z*uZ.z)/ (uZ.x*uZ.x + uZ.y*uZ.y + uZ.z*uZ.z);

		if (components.x > 0 && components.x < 1 ) {
			collX = true;
		} else {
			collX = false;
		}

		if (components.y < 1 && components.y > 0) { //if the componenets are all greater than 0 or less than 1, collision
			collY = true;
		} else {
			collY = false;
		}

		if (components.z > 0 && components.z < 1) {
			collZ = true;
		} else {
			collZ = false;
		}

		//if the point is atleast 2 collision axes, we need to analyze further
		if ((collX + collY + collZ) >= 2) {

			//assign the side of two collisions into our Vector construction
			collision2Sides.push_back(components);

			//if all three are true, there is a collision
			if ((collX + collY + collZ) == 3) {
				collision = true;
			}
		}
	}

	int axis = 0, axis2 = 0;

	//check for possible collision of 2 axes
	if (collision == false && collision2Sides.size() != 0) {
		//check if vertices are drawn on opposite sides of the cubes

		//check each point

		//axis code: 1 = x-axis, 2 = y-axis, 3 = z-axis, negative is the negative side
		for (int i = 0; i < collision2Sides.size(); i++) {

			//find the component of vector at i that isn't in the cube
			if (collision2Sides.at(i).x <= 0) {
				axis = -1;
			} else if(collision2Sides.at(i).x >= 1) {
				axis = 1;
			} else if(collision2Sides.at(i).y >= 1) {
				axis = 2;
			} else if(collision2Sides.at(i).y <= 0) {
				axis = -2;
			} else if(collision2Sides.at(i).z >= 1) {
				axis = 3;
			} else if(collision2Sides.at(i).z <= 0) {
				axis = -3;
			}

			//check each other point
			for (int j = 0; j < collision2Sides.size(); j++) {

				//find the component of vector at j that isnt in the cube
				if (collision2Sides.at(j).x <= 0) {
					axis2 = -1;
				} else if(collision2Sides.at(j).x >= 1) {
					axis2 = 1;
				} else if(collision2Sides.at(j).y >= 1) {
					axis2 = 2;
				} else if(collision2Sides.at(j).y <= 0) {
					axis2 = -2;
				} else if(collision2Sides.at(j).z >= 1) {
					axis2 = 3;
				} else if(collision2Sides.at(j).z <= 0) {
					axis2 = -3;
				}

				//if they are opposite sides, we have collision
				if (axis == -1*axis2) {
					collision = true;
				}
			}
		}
	}

	return collision;
}
