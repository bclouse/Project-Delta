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
	int wt;
	int pop = 20;
	int n = 50;
	srand(time(NULL));
	double max = dist(0,0,SIZE/2,SIZE/2);

	cout << "What Population size do you want? ";
	cin >> pop;
	cout << "How many iterations? ";
	cin >> n;
	
	neural_network NN;
	NN.setup(3,5,1);
	// NN.set_in_min_max(0.0,(double)SIZE);
	// NN.set_in_min_max(0.0,(double)SIZE);
	NN.set_in_min_max(0,max);
	NN.set_in_min_max(-180.0,180.0);
	NN.set_in_min_max(-15.0,15.0);
	NN.set_out_min_max(-15.0,15.0);
	wt = NN.get_number_of_weights();
	// cout << wt << "\n\n";

	Evolution EA;	
	EA.init(wt,pop);

	Simulation world(NN,EA);

	Boat b;
	b = randomize_boat();
	// b.x = SIZE/4;	b.y = SIZE/4;
	// b.theta = 0;	b.omega = 0;

	for (int i = 0; i < n; i++) {
		if ((i+1)%10 == 0) { 
			printf("Iteration #%d\n", i+1);
		}
		// b = randomize_boat();
		if (i == n-1) {
			cout << endl;
			world.run(b,true,true);
		// } else if (i == 0) {
		// 	world.run(b,true,false);
		// 	cout << endl;
		} else {
			world.run(b,false,false);
		}
	}

	return 0;
}
