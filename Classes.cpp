#include "Classes.h"

//===============================
//	Evolution Class
//===============================

void Evolution::init (int wt, int pop) {
	w = wt;
	p = pop;
	change = (int)sqrt(w);
	vector<double> dummy;

	if (p%2 != 0) p++;

	for (int i = 0; i < p; i++) {
		dummy.clear();
		for (int j = 0; j < w; j++) {
			dummy.push_back(ZERO_TO_ONE*2-1);
			// if (i == 0) printf("%.4f\t", dummy[j]);
		}
		// if (i == 0) cout << endl;
		weights.push_back(dummy);
	}
	change = (int)sqrt(w);
}

void Evolution::down_repop(vector<double> fitness) {
	vector<int> choices;
	int a,b;
	int first, second;

	assert(fitness.size() == p);

	for (int i = 0; i < fitness.size(); i++) {
		choices.push_back(i);
	}
	while (choices.size() > 0) {
		first = rand()%choices.size();
		do {
			second = rand()%choices.size();
		} while (first == second);
		a = choices[first];
		b = choices[second];

		if (fitness[a] <= fitness[b]) {
			weights[b] = weights[a];
			mutate(b);
		} else if (fitness[b] < fitness[a]) {
			weights[a] = weights[b];
			mutate(b);
		}
		if (first > second) {
			// cout << "a > b\t" << first << ", " << second << endl << endl;
			choices.erase(choices.begin()+first);
			choices.erase(choices.begin()+second);
		} else {
			// cout << "b > a\t" << second << ", " << first << endl << endl;
			choices.erase(choices.begin()+second);
			choices.erase(choices.begin()+first);
		}
		// cout << "end" << endl;
	}
}

void Evolution::mutate(int index) {
	// for (int i = 0; i < weights[index].size(); i++) {
	// 	printf("%.4f\t",weights[index][i]);
	// }
	// cout << "\n\n";

	for (int i = 0; i < change; i++) {
		if (rand()%100 > 5) {
			weights[index][rand()%w] += (ZERO_TO_ONE-0.5)*0.2;
		} else {
			weights[index][rand()%w] += (ZERO_TO_ONE-0.5)*0.5;
		}
	}

	// for (int i = 0; i < weights[index].size(); i++) {
	// 	printf("%.4f\t",weights[index][i]);
	// }
	// cout << "\n==================================\n";
}

vector<double> Evolution::get_weights(int n) {
	return weights[n];
}

int Evolution::population() {
	return p;
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

void Simulation::run(Boat start, bool see, bool data) {
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
			update_input(distance);
			NN.set_vector_input(input);
			NN.set_weights(w,true);
			NN.execute();
			u = NN.get_output(0);
			simulate(u);
			calc_beta();
			distance = dist(x,y,gx,gy);
			if (min > distance) min = distance;
			time += TIME;
			if (data && i == 0) {
				log(first);
				if (first) first = false;
			}
		} while (in_bounds() && distance > 2.5 && time < 150);
		fit = min+time;
		if (!in_bounds()) fit += 150;
		if (see) {
			if (!in_bounds())	{
				cout << "OUTSIDE ";
			} else if (distance <= 2.5) {
				cout << "FOUND   ";
			} else {
				cout << "INSIDE  ";
			}
			cout << fit << endl;
		}
		fitness.push_back(fit);
	}
	EA.down_repop(fitness);
}

void Simulation::simulate(double u) {

	x += v*cos(theta*RADIANS)*TIME;
	y += v*sin(theta*RADIANS)*TIME;
	theta += omega*TIME;
	omega += (u-omega)*TIME/5.0;	//the "5.0" it "T"

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

void Simulation::update_input(double d) {
	input.clear();
	double stray = beta-theta;

	if (stray < 0) {
		stray += 360;
	}
	if (stray > 180) {
		stray -= 360;
	}

	// input.push_back(x);
	// input.push_back(y);
	input.push_back(d);
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

Boat randomize_boat() {
	Boat b;
	double angle = ZERO_TO_ONE*360*RADIANS;
	double radius = SIZE*3/8;

	b.x = radius*cos(angle)+(SIZE/2);
	b.y = radius*sin(angle)+(SIZE/2);
	b.theta = ZERO_TO_ONE*360;
	// b.theta = 0;
	b.omega = 0;

	return b;
}