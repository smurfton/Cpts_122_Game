#include "Small_Scene_Render.h"

Clock::Clock() {
	currentTime = 0;
	startTime = 0;
	cyclesPerSecond = 60;
}
Clock::~Clock() {

}
//getters
clock_t Clock::getCurrentTime() {
	return currentTime;
}
clock_t Clock::getStartTime() {
	return startTime;
}
int Clock::getCyclesPerSecond() {
	return cyclesPerSecond;
}
void Clock::setCyclesPerSecond(int cycle) {
	cyclesPerSecond = cycle;
}

//general functionality
void Clock::start() {
	startTime = clock();
}
void Clock::stop() {
	currentTime = clock();
}
int Clock::getCounts() {
	currentTime = clock();
	//only measure the seconds difference
	return (currentTime-startTime)*CLOCKS_PER_SEC*cyclesPerSecond;
}