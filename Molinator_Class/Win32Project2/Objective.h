#ifndef OBJECTIVE_H
#define OBJECTIVE_H
class Objective : public Obstacle {
public:
	Objective() {};
	Objective(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color, Vector traverse);
	Objective (const Objective &copy);
};

#endif