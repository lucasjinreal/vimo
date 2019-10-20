// ************************************
// Copyrights by Jin Fagang
// 1/19/19-19-12
// xplo_linux
// jinfagang19@gmail.com
//
// Explosion Lab
// ************************************

//
// Created by jintain on 1/19/19.
//

#include <iostream>
#include <vector>

#include "xinputsimulator/xinputsimulator.h"
#ifdef __linux__
//sleep
#include <unistd.h>
#elif __APPLE__
//sleep
#include <unistd.h>
#elif _WIN32
//sleep
#include <windows.h>
#endif

using namespace std;


void waitABit()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#ifdef __linux__
	sleep(1);
#elif __APPLE__
	sleep(1);
#elif _WIN32
	Sleep(1000);
    
#endif
    
}


int main() {

  cout << "Hello World!" << endl;
    XInputSimulator &sim = XInputSimulator::getInstance();

    waitABit();
    // sim.mouseMoveTo(500,400);
    // waitABit();
    sim.mouseMoveRelative(16, -10);
    waitABit();
    sim.mouseDown(XIS::LEFT_MOUSE_BUTTON);
    sim.mouseMoveRelative(0, 300);
    sim.mouseUp(XIS::LEFT_MOUSE_BUTTON);
    waitABit();

}