class Player : public Object {
public:
	Player();
	~Player();


	void updatePosition();
	void updateOrientation();
private:
	float velocity, angularVelocity;

};