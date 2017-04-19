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
	int iter = 0;
	int state = 0;
	//neural_network NN;
	Simulation world (1,SIZE);

	state = world.simulate(u);
	while ((state == 0) && (iter < 1000)) {
		// if (i < 100) {
			// u += 0.1;	
		// } else {
		// 	u -= 0.1;
		// }
		world.calc_beta();
		world.log(first);
		if (first) first = false;
		iter++;
		state = world.simulate(u);
		// cout << state << '\t';
	}
	if (state == 1) cout << "\nOut of bounds\n";
	else if (state == 2) cout << "\nFound goal\n";

	return 0;
}
