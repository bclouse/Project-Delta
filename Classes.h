#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <random>
#include <iomanip>
#include <cmath>
// #include "LY_NN.h"

using namespace std;

#define ZERO_TO_ONE (float)rand()/RAND_MAX
#define RADIANS (double)3.1415926535897/180

#ifndef CLASSES_H
#define CLASSES_H

//===============================
//	Boat Struct
//===============================

struct Boat {
	float x;
	float y;
	float theta;
	float omega;
};

//===============================
//	EA Class
//===============================

class EA {
private:
public:
};

//===============================
//	Simulation Class
//===============================

class Simulation {
private:
public:
};

//===============================
//	Functions
//===============================

#endif