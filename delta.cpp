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
	int count = 1;
	int spacing = 10;
	bool log_spacing = true;
	double max = dist(0,0,SIZE/2,SIZE/2);
	FILE *fit = fopen("assests//Learning.txt", "w+");
	char given[] = "path_over_time//step0.txt";	//21

	srand(time(NULL));

	cout << "What Population size do you want? ";
	cin >> pop;
	cout << "What is the maximum number of generations? ";
	cin >> n;
	cout << "What generation spacing do you want to log the best policy? ";
	cin >> spacing;
	
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
	b = randomize_boat(true);
	printf("Is this ok? (Y or N) ");
		cin >> c;
		while (c != 'Y' && c != 'N') {
			printf("Invalid option! Choose Y or N! ");
			cin >> c;
		}
		if (c == 'Y') 	use_boat = true;
		else				use_boat = false;
	} while (!use_boat);
	cout << endl;


	for (int i = 0; i < n; i++) {

		if ((i+1)%10 == 0) { 
			printf("Generation #%d\n", i+1);
		}
		// b = randomize_boat();
		if (i == n-1 || i == 0 || ((i+1)%spacing == 0 && log_spacing)) {
			cout << endl;
			world.run(b,true,true);
			given[20] = '0'+count;
			count++;
			world.log_best(given);
			cout << endl;
		} else {
			world.run(b,false,false);
		}
		world.log_fit(fit);
		if (((i+1)%spacing == 0 && log_spacing) && (i != n-1)) {
			cout << "Do you want to continue? (Y or N) ";
			cin >> c;
			while (c != 'Y' && c != 'N' && c != 'S') {
				printf("Invalid option! Choose Y or N! ");
				cin >> c;
			}
			if (c == 'N') break;
			else if (c == 'S') log_spacing = false;
			cout << endl;
		}
	}

	fclose(fit);
	return 0;
}
