#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <random>
#include <iomanip>
#include <cmath>
#include "LY_NN.h"

using namespace std;

#define ZERO_TO_ONE ((double)rand()/RAND_MAX)
#define RADIANS ((double)3.1415926535897/180)
#define TIME 0.2
#define SIZE 200

#ifndef CLASSES_H
#define CLASSES_H

//===============================
//	EA Class
//===============================

class Evolution {
private:
	vector<vector<double>> weights;	//[-1,1]
	vector<double> fitness;
	int w;	//Number of weights
	int p;	//Number of policies
public:
	void init(int,int);
	void evaluate();

};

//===============================
//	Simulation Class
//===============================

class Simulation {
private:
	double gx, gy;
	double x, y;
	double theta, omega;
	double beta;			//pitch that the boat needs to be at to go directly to the goal
	double v;
	Evolution EA;
	//neural_network NN;
public:
	Simulation (int,int);
	// Simulation (neural_network,int,int);
	int simulate(double);
	vector<double> update_input();
	void log(bool); 
	void calc_beta();
};

// class Simulation {
// private:
// 	double velocity;

// public:
// 	Simulation (double);
// 	void run(Boat,)
// };

//===============================
//	Functions
//===============================

#endif