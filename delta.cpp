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
	bool use_boat;
	char c;
	int wt;
	int pop = 20;
	int n = 50;
	double max = dist(0,0,SIZE/2,SIZE/2);
	FILE *fit = fopen("Learning.txt", "w+");

	srand(time(NULL));

	cout << "What Population size do you want? ";
	cin >> pop;
	cout << "How many iterations? ";
	cin >> n;
	
	neural_network NN;
	NN.setup(3,5,1);
	// NN.set_in_min_max(0.0,SIZE);		//x
	// NN.set_in_min_max(0.0,SIZE);		//y
	// NN.set_in_min_max(0,360.0);		//theta
	NN.set_in_min_max(0,max);			//distance
	NN.set_in_min_max(0.0,180.0);		//stray
	NN.set_in_min_max(-15.0,15.0);	//omega
	NN.set_out_min_max(-15.0,15.0);	//OUTPUT
	wt = NN.get_number_of_weights();
	// cout << wt << "\n\n";

	Evolution EA;	
	EA.init(wt,pop);

	Simulation world(NN,EA);

	Boat b;
	do {
	b = randomize_boat();
	printf("\nBoat is located at %.1f, %.1f at angle %.1f\nIs this ok? (Y or N) ", b.x, b.y, b.theta);
		cin >> c;
		while (c != 'Y' && c != 'N') {
			printf("Invalid option! Choose Y or N! ");
			cin >> c;
		}
		if (c == 'Y') 	use_boat = true;
		else				use_boat = false;
	} while (!use_boat);
	for (int i = 0; i < n; i++) {

		if ((i+1)%10 == 0) { 
			printf("Generation #%d\n", i+1);
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
		world.log_fit(fit);
	}

	fclose(fit);
	return 0;
}
