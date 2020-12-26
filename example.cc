#include <iostream>
#include <string>
#include "iq_twist.h"

using namespace std;

int main(int argc, char **argv) {

	// colors: r (1x), g (2x), b (2x), y (2x)
	// places: x: 0..3, y: 0..7
	string cmd = "-r32 -g21 -b05 -y13";
	IqTwist iqtwist = IqTwist(cmd);

	// find solution
	int nsolutions = iqtwist.search();

	for (int isolution = 0; isolution < nsolutions; isolution++) {
		string solution = iqtwist.get_solution(isolution);
		cout << "solution: " << isolution+1 << '\n';
		for (int i = 0; i < 4; i++) {
			cout << '\t';
			for (int j = 0; j < 8; j++) {
				cout << solution[8*i + j];
			}
			cout << '\n';
		}
	}

	return 0;
}
