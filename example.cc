#include <iostream>
#include <string>
#include "iq_twist.h"

using namespace std;
using namespace IqTwist;

int main() {
    
    // Peg placements
    // - colors: r (1x), g (2x), b (2x), y (2x)
    // - places: x: 0..3, y: 0..7
    string cmd = "-r32 -g21 -b05 -y13";
    cmd = "";
    auto iqtwist = IqTwistSolver(cmd);
    
    // find solution
    int nsolutions = iqtwist.Solve();

    std::cout << "nsolutions " << nsolutions << '\n';
    
    for (int isolution = 0; isolution < nsolutions; isolution++) {
    	string solution = iqtwist.GetSolution(isolution);
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
