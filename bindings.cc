#include <emscripten/bind.h>
#include <string>

#include "dlx/dlx.h"
#include "iq_twist.h"

using namespace emscripten;


EMSCRIPTEN_BINDINGS(iq_twist) {
	class_<DancingLinks>("DancingLinks")
	 	.function("search", &DancingLinks::search)
		.function("solution_count", &DancingLinks::solution_count)
	 	;
	class_<IqTwist, base<DancingLinks>>("IqTwist")
		.constructor<std::string>()
		.function("get_solution", &IqTwist::get_solution)
		;
}
