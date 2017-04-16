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
	double u = -2.5;
	int iter;
	Simulation world (1,SIZE);

	while (world.simulate(u) && iter < 1000) {
		// if (i < 100) {
			// u += 0.1;	
		// } else {
		// 	u -= 0.1;
		// }
		world.log(first);
		if (first) first = false;
		iter++;
	}

	return 0;
}
