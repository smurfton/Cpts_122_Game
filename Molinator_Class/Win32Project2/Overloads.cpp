#include "Small_Scene_Render.h"


Vector operator+(Vector &lhs, Vector &rhs) {
	Vector add;
	add.x = lhs.x + rhs.x;
	add.y = lhs.y + rhs.y;
	add.z = lhs.z + rhs.z;
	return add;
}
Vector operator-(Vector &lhs, Vector &rhs) {
	Vector add;
	add.x = lhs.x - rhs.x;
	add.y = lhs.y - rhs.y;
	add.z = lhs.z - rhs.z;
	return add;
}
bool operator==(Vector &lhs, Vector &rhs) {
	bool equal = false;
	if (lhs.x == rhs.x) {
		if (lhs.y == rhs.y) {
			if (lhs.z == rhs.z) {
				equal = true;
			}
		}
	}

	return equal;
}