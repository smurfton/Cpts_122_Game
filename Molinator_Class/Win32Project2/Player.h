#ifndef PLAYER_H
#define PLAYER_H
class Player : public Object {
public:
	Player();
private:
	Clock verticalClock;
	float verticalVelocity;

};


#endif