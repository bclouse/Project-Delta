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
	x = size/4; 	y = size/4;
	theta = 90; 		omega = 0;
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

bool Simulation::simulate(double u) {
	bool inside = true;

	// vector<double> input = update_input();
	// NN.set_vector_input(input);
	// NN.execute();
	// u = NN.get_output(0);

	x += v*sin(theta*RADIANS)*TIME;
	y += v*cos(theta*RADIANS)*TIME;
	theta += omega*TIME;
	omega += (u-omega)*TIME/5.0;	//the "5.0" it "T"
	if (x < 0 || x > SIZE || y < 0 || y > SIZE) {
		inside = false;
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

	return inside;
}

vector<double> Simulation::update_input() {
	vector<double> input;

	input.push_back(x);
	input.push_back(y);

	return input;
}

void Simulation::log(bool first) {
	FILE *fp;

	if (first) {
		fp = fopen("path.txt","w+");
	} else {
		fp = fopen("path.txt","a");
	}
	fprintf(fp,"%8f\t%8f\n", x,y);
	fclose(fp);
}

//===============================
//	Functions
//===============================

