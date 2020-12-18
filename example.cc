#include <iostream>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <wordexp.h>

#include "src/iq_twist.h"

using namespace std;

int main(int argc, char **argv) {

	string cmd = "poo -r12";
	IqTwist iqtwist = IqTwist(cmd);
	iqtwist.search();

	return 0;
}
