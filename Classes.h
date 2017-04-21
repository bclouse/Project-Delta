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
#define SIZE 300

#ifndef CLASSES_H
#define CLASSES_H

//===============================
//	Boat Struct
//===============================

struct Boat {
	double x, y;
	double theta, omega;
};

//===============================
//	EA Class
//===============================

class Evolution {
private:
	vector<vector<double>> weights;	//[-1,1]
	// vector<double> fitness;
	int w;			//Number of weights
	int p;			//Number of policies
	int change; 	//how many weights to change per mutation
public:
	void init(int,int);
	void down_repop(vector<double>);	//A combination of downselecting and repopulating, will also call the mutate function
	void mutate(int);
	vector<double> get_weights(int);
	int population();
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
	vector<double> fitness;
	vector<double> input;
	neural_network NN;
	Evolution EA;
public:
	// Simulation (int,int);
	Simulation (neural_network,Evolution);
	void run(Boat,bool,bool);
	void simulate(double);
	void update_input(double);
	void log(bool); 
	void calc_beta();
	bool in_bounds();
};

//===============================
//	Functions
//===============================

double dist(double,double,double,double);
Boat randomize_boat();

#endif