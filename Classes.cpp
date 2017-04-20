#include "Classes.h"

//===============================
//	Evolution Class
//===============================

void Evolution::init (int wt, int pop) {
	w = wt;
	p = pop;
	vector<double> dummy;

	if (p%2 != 0) p++;

	for (int i = 0; i < p; i++) {
		dummy.clear();
		for (int j = 0; j < w; j++) {
			dummy.push_back(ZERO_TO_ONE*2-1);
		}
		weights.push_back(dummy);
	}
	change = (int)sqrt(w);
}

void Evolution::down_select() {

}

void Evolution::repopulate() {

}

void Evolution::mutate(int index) {

}

void Evolution::give_fitness(vector<double> input) {
	fitness = input;
}

vector<double> Evolution::get_weights(int n) {
	return weights[n];
}

int Evolution::population() {
	return (int) weights.size();
}

//===============================
//	Simulation Class
//===============================

Simulation::Simulation(neural_network nn, Evolution ea) {
	EA = ea;
	NN = nn;
	gx = SIZE/2; 	gy = SIZE/2;
	x = SIZE/4; 	y = SIZE/4*3;
	theta = 0; 		omega = 0;
	v = 3;

}

void Simulation::run(Boat start) {
	double min = dist(x,y,gx,gy);
	double distance = min;
	double u;
	double time;
	double fit;
	int n = EA.population();
	bool first = true;
	vector<double> w;

	fitness.clear();
	for (int i = 0; i < n; i++) {
		x = start.x;			y = start.y;
		theta = start.theta;	omega = start.omega;
		time = 0;
		fit = 0;
		w = EA.get_weights(i);
		do {
			update_input();
			NN.set_vector_input(input);
			NN.set_weights(w,true);
			NN.execute();
			u = NN.get_output(0);
			simulate(u);
			calc_beta();
			distance = dist(x,y,gx,gy);
			if (min > distance) min = distance;
			time += TIME;
			if (i == 0) {
				log(first);
				if (first) first = false;
			}
		} while (in_bounds() && distance > 2.5 && time < 100);
		fit = min+time;
		if (!in_bounds())	{
			fit += 100;
			//cout << "OUTSIDE ";
		} //else if (distance <= 2.5) {
		// 	cout << "FOUND   ";
		// } else {
		// 	cout << "INSIDE  ";
		// }
		cout << fit << endl;
		fitness.push_back(fit);
	}
	// assert(n == fitness.size());
	EA.give_fitness(fitness);
	EA.down_select();
	EA.repopulate();
}

void Simulation::simulate(double u) {

	x += v*cos(theta*RADIANS)*TIME;
	y += v*sin(theta*RADIANS)*TIME;
	theta += omega*TIME;
	omega += (u-omega)*TIME/5.0;	//the "5.0" it "T"

	// distance = sqrt(pow(x-gx,2)+pow(y-gy,2));

	// if (x < 0 || x > SIZE || y < 0 || y > SIZE) {
	// 	state = 1;
	// } else if (distance <= 2.5) {
	// 	state = 2;
	// }
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
}

// int Simulation::simulate(double u) {
// 	int state = 0;
// 	double distance = 0;

// 	update_input();
// 	NN.set_input_vector(input);
// 	NN.execute();
// 	u = NN.get_output(0);

// 	x += v*cos(theta*RADIANS)*TIME;
// 	y += v*sin(theta*RADIANS)*TIME;
// 	theta += omega*TIME;
// 	// omega += (u-omega)*TIME/5.0;	//the "5.0" it "T"

// 	distance = sqrt(pow(x-gx,2)+pow(y-gy,2));

// 	if (x < 0 || x > SIZE || y < 0 || y > SIZE) {
// 		state = 1;
// 	} else if (distance <= 2.5) {
// 		state = 2;
// 	}
// 	while (theta < 0 || theta >= 360) {
// 		if (theta < 0) {
// 			theta += 360;
// 		} else if (theta >= 360) {
// 			theta -= 360;
// 		}
// 	}
// 	if (omega > 15) {
// 		omega = 15;
// 	} else if (omega < -15) {
// 		omega = -15;
// 	}

// 	return state;
// }

void Simulation::update_input() {
	input.clear();
	double stray = beta-theta;

	if (stray < 0) {
		stray += 360;
	}

	input.push_back(x);
	input.push_back(y);
	input.push_back(stray);
	input.push_back(omega);
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

bool Simulation::in_bounds() {
	if (x < 0 || x > SIZE || y < 0 || y > SIZE) {
		return false;
	}
	return true;
}

//===============================
//	Functions
//===============================

double dist(double x1, double y1, double x2, double y2) {
	return (double)sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}