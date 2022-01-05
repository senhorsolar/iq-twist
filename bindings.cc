#include <emscripten/bind.h>
#include <string>

#include "dlx/dlx.h"
#include "iq_twist.h"

using namespace emscripten;


EMSCRIPTEN_BINDINGS(iq_twist) {
	// class_<DancingLinks>("DancingLinks")
	//     .function("search", &Dlx::DancingLinks::search)
	//     .function("solution_count", &Dlx::DancingLinks::solution_count)
	//  	;
    class_<IqTwist::IqTwistSolver>("IqTwist")
	.constructor<std::string>()
	.function("get_solution", &IqTwist::IqTwistSolver::GetSolution)
	;
}
