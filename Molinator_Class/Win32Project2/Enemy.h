#ifndef ENEMY_H
#define ENEMY_H
class Enemy : public Obstacle {
public:
	Enemy() {};
	Enemy(int ID, char vertexFileName[MAX_FILE_LENGTH], LPDIRECT3DDEVICE9 *device, _D3DCOLORVALUE color, Vector traverse);
	Enemy (const Enemy &copy);
};
#endif