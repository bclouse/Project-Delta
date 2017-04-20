//
//	Bryant Clouse
//	NN finds goal in a boat
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <random>
#include <iomanip>
#include <cmath>
#include "Classes.cpp"

using namespace std;

int main() {
	bool first = true;
	double u = 0;
	int wt;
	int pop = 6;
	int n = 1;
	srand(time(NULL));
	
	neural_network NN;
	NN.setup(4,5,1);
	NN.set_in_min_max(0.0,(double)SIZE);
	NN.set_in_min_max(0.0,(double)SIZE);
	NN.set_in_min_max(0.0,360.0);
	NN.set_in_min_max(0.0,360.0);
	NN.set_out_min_max(-15.0,15.0);
	wt = NN.get_number_of_weights();
	// cout << wt << "\n\n";

	Evolution EA;	
	EA.init(wt,pop);

	Simulation world(NN,EA);

	Boat b;
	b.x = SIZE/4;	b.y = b.x;
	b.theta = 0;	b.omega = 0;

	for (int i = 0; i < n; i++) {
		world.run(b);
	}

	return 0;
}
