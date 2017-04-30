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
	// change = (int)sqrt(w);
	change = w/4;
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
			mutate(a);
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
	int in;

	for (int i = 0; i < change; i++) {
		in = rand()%w;
		if (rand()%100 > 10) {
			weights[index][in] += (ZERO_TO_ONE-0.5)*0.5;
		} else {
			weights[index][in] += (ZERO_TO_ONE-0.5);
		}
		// if (weights[index][in] > 1) weights[index][in] = 1;
		// else if (weights[index][in] < -1) weights[index][in] = -1;
	}
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
	double previous = min;
	double u;
	double time;
	double fit;
	int n = EA.population();
	bool first = true;
	vector<double> w;
	FILE *path;
	char str[] = "assests//path000.txt\0";	//13,14,15
	string num;

	fitness.clear();
	for (int i = 0; i < n; i++) {
		x = start.x;			y = start.y;
		theta = start.theta;	omega = start.omega;
		time = 0;
		fit = 0;
		w = EA.get_weights(i);
		first = true;
		distance = min;
		previous = min;

		if (data) {
			num = int2str(i+1,3);
		
			str[13] = num[0];
			str[14] = num[1];
			str[15] = num[2];

			path = fopen(str,"w+");
		}
		calc_beta();
		stray = beta-theta;
		if (stray < 0) {
			stray += 360;
		}
		if (stray > 180) {
			stray = 360 - stray;
		}
		//RUNNING SIMULATION
		do {
			update_input(distance,previous);
			NN.set_vector_input(input);
			NN.set_weights(w,ASSERT);
			NN.execute();
			u = NN.get_output(0);
			simulate(u);
			calc_beta();
			previous = distance;
			distance = dist(x,y,gx,gy);
			// if (min > distance) min = distance;
			time += TIME;
			if (data) {
				log(path);
				if (first) first = false;
			}
		} while (in_bounds() && !found_goal() && time < DURATION);
		
		// cout << "Outside loop" << endl;
		fit = distance+(time/2);
		if (!in_bounds()) fit += DURATION/2;
		if (see) {
			printf("%3d) ", i+1);
			if (!in_bounds())	{
				cout << "OUTSIDE ";
			} else if (distance <= 2.5) {
				cout << "FOUND   ";
			} else {
				cout << "INSIDE  ";
			}
			cout << fit << endl;
		}
		// cout << "add to fitness" << endl;
		fitness.push_back(fit);
		
		if (data) fclose(path);
	}
	EA.down_repop(fitness);
}

void Simulation::simulate(double u) {

	x += v*cos(theta*RADIANS)*TIME;
	y += v*sin(theta*RADIANS)*TIME;
	theta += omega*TIME;
	omega += (u-omega)*TIME/STICK;	//the "5.0" it "T"

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

void Simulation::update_input(double d,double prev) {
	input.clear();
	stray = beta-theta;

	if (stray < 0) {
		stray += 360;
	}
	if (stray > 180) {
		stray = 360 - stray;
	}

	// input.push_back(x);
	// input.push_back(y);
	// input.push_back(theta);
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

void Simulation::log(FILE *path) {
	fprintf(path,"%8f\t%8f\t%8f\t%8f\n", x,y,omega,stray);
}

bool Simulation::in_bounds() {
	if (x < 0 || x > SIZE || y < 0 || y > SIZE) {
		return false;
	}
	return true;
}

void Simulation::log_fit(FILE *fit) {
	int min = 0, max = 0;
	double avg = 0;

	for(int i = 0; i < fitness.size(); i++) {
		if (fitness[min] > fitness[i]) min = i;
		if (fitness[max] < fitness[i]) max = i;
		avg += fitness[i];
	}
	avg /= fitness.size();
	fprintf(fit,"%8f\t%8f\t%8f\n", fitness[min], avg, fitness[max]);
}

bool Simulation::found_goal() {
	if (dist(x,y,gx,gy) <= 2.5) return true;
	else return false;
}

void Simulation::log_best(char given[]) {
	int index = 0;
	string num;
	char str[] = "assests//path000.txt\0";
	char c = 0;
	FILE *original;
	FILE *fp = fopen(given,"w+");


	for (int i = 1; i < fitness.size(); i++) {
		if (fitness[index] > fitness[i]) index = i;
	}

	num = int2str(index+1,3);	
	str[13] = num[0];
	str[14] = num[1];
	str[15] = num[2];
	original = fopen(str,"r");

	while(!feof(original)) {
		fscanf(original, "%c", &c);
		fprintf(fp, "%c", c);
	}
	fclose(original);
	fclose(fp);
}

//===============================
//	Functions
//===============================

double dist(double x1, double y1, double x2, double y2) {
	return (double)sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

Boat randomize_boat(bool print,bool face_away) {
	Boat b;
	double theta = ZERO_TO_ONE*360;
	double angle = theta*RADIANS;
	double radius = SIZE*3/8;

	if (theta > 180) 	theta -= 180;
	else					theta += 180;

	b.x = radius*cos(angle)+(SIZE/2);
	b.y = radius*sin(angle)+(SIZE/2);
	if (face_away) {
		b.theta = angle+((ZERO_TO_ONE-0.5)*90);
		if (b.theta < 0) b.theta += 360;
		else if (b.theta >= 360) b.theta -= 360;
	} else {
		b.theta = ZERO_TO_ONE*360;
	}
	// b.theta = theta+(ZERO_TO_ONE*90)-45;
	// b.theta = 0;
	b.omega = 0;
	angle = (angle/RADIANS) - 180;
	if (angle < 0) angle += 360;
	// printf("\nAngle to the goal is %.1f. Theta is %.1f\n",angle,b.theta);
	if (print) {
		printf("\nDifference in angle is %.1f and theta is %.1f\n", angle-b.theta,b.theta);
		printf("x: %.1f\ty: %.1f\n", b.x, b.y);
	}

	return b;
}

string int2str(int num, int size) {
	string dummy, output;
	int val;

	for (int i = 0; i < size; i++) {
		val = num%10;
		dummy.push_back('0'+val);
		num = (num-val)/10;
	}

	for (int i = 0; i < size; i++) {
		output.push_back(dummy[dummy.size()-1-i]);
	}

	return output;
}

// void copy_file(FILE *in, FILE *out) {
// 	while (!feof(in)) {
// 		fscanf(in, "%c", &c);
// 		fprintf(out, "%c", c);
// 	}
// }