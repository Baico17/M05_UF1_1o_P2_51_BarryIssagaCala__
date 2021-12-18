#include "TimeManager.h"

TimeManager& TimeManager::getInstance()
{
	static TimeManager timeManager;
	return timeManager;
}

void TimeManager::NextFrame()
{
	frameCount++;
	auto elapsed = std::chrono::high_resolution_clock::now() - start;  // auto sirve para no poner todo de la variable esta start
	deltaTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() /(float) 1000;  // transforma un numero a milisegundos y los cuenta
	time += deltaTime;
	start = std::chrono::high_resolution_clock::now();
}