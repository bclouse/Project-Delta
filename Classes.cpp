#include "Classes.h"

//===============================
//	Evolution Class
//===============================

void Evolution::init (int wt, int pop) {
	w = wt;
	p = pop;
	vector<double> dummy;

	for (int i = 0; i < p; i++) {
		dummy.clear();
		for (int j = 0; j < w; j++) {
			dummy.push_back(ZERO_TO_ONE*2-1);
		}
		weights.push_back(dummy);
	}
}

void Evolution::evaluate() {

}


//===============================
//	Simulation Class
//===============================

Simulation::Simulation(int pop, int size) {
	Evolution dummy;
	dummy.init(1,pop);
	EA = dummy;
	gx = size/2; 	gy = size/2;
	x = size/4; 	y = size/4*3;
	theta = 0; 		omega = 0;
	v = 3;
}

// Simulation::Simulation(neural_network nn, int pop, int size) {
// 	NN = nn;
// 	int w = NN.get_number_of_weights();
// 	Evolution dummy;
// 	dummy.init(w,pop);
// 	EA = dummy;
// 	gx = size/2; 	gy = size/2;
// 	x = size/4; 	y = size/4;
// 	theta = 0; 		omega = 0;
// 	v = 3;
// }

int Simulation::simulate(double u) {
	int state = 0;
	double distance = 0;

	// vector<double> input = update_input();
	// NN.set_vector_input(input);
	// NN.execute();
	// u = NN.get_output(0);

	x += v*cos(theta*RADIANS)*TIME;
	y += v*sin(theta*RADIANS)*TIME;
	theta += omega*TIME;
	// omega += (u-omega)*TIME/5.0;	//the "5.0" it "T"

	distance = sqrt(pow(x-gx,2)+pow(y-gy,2));

	if (x < 0 || x > SIZE || y < 0 || y > SIZE) {
		state = 1;
	} else if (distance <= 2.5) {
		state = 2;
	}
	while (theta < 0 || theta >= 360) {
		if (theta < 0) {
			theta += 360;
		} else if (theta >= 360) {
			theta -= 360;
		}
	}
	if (omega > 15) {
		omega = 15;
	} else if (omega < -15) {
		omega = -15;
	}

	return state;
}

vector<double> Simulation::update_input() {
	vector<double> input;

	input.push_back(x);
	input.push_back(y);

	return input;
}

void Simulation::calc_beta() {
	beta = atan((y-gy)/(x-gx))/RADIANS;
	// printf("%6f\t", beta);
	if (x > gx) {
		beta += 180;
	} else if (x < gx && y > gx) {
		beta += 360;
	}
	// printf("%6f\n", beta);
}

void Simulation::log(bool first) {
	FILE *fp;
	double stray = beta-theta;

	if (stray < 0) {
		stray += 360;
	}
	if (first) {
		fp = fopen("path.txt","w+");
	} else {
		fp = fopen("path.txt","a");
	}
	fprintf(fp,"%8f\t%8f\t%8f\n", x,y,stray);
	fclose(fp);
}

//===============================
//	Functions
//===============================

