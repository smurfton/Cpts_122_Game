#ifndef CLOCK_H
#define CLOCK_H

class Clock {
public:
	Clock();
	~Clock();
	//getters
	clock_t getCurrentTime();
	clock_t getStartTime();
	int getCyclesPerSecond();

	//setters
	void setCyclesPerSecond(int cycle);

	//general functionality
	void start();
	void stop();
	int getCounts();

private:
	clock_t currentTime;
	clock_t startTime;
	int cyclesPerSecond;
};

#endif