// iq_twist.cpp

#include <cstring>
#include <set>
#include "iq_twist.h"

namespace IqTwist {

IqTwistSolver::IqTwistSolver(std::string cmd) :
    m_pegs(ParseCmd(cmd)),
    m_column_names(GenColumnNames()),
    m_dlx(GenBinaryMatrix())
{
}

size_t
IqTwistSolver::Solve()
{
    m_dlx.Search();
    m_solutions.reserve(m_dlx.SolutionCount());

    auto char_to_int = [](char c) -> int
    {
	return c - '0';
    };
	
    // ex. "r1" -> 'r', "r2" -> 'R'
    auto assign_color = [&](std::string s) -> char
    {
	int d = char_to_int(s[1]) - 1;
	
	char color = s[0];
	if (d)
	    color = toupper(color);
	
	return color;
    };

    for (const auto& dlx_solution : m_dlx.GetSolutions()) {
	// Get rows whose elements cover each column

	std::vector<std::vector<std::string>> decoded_solution
	    = m_dlx.DecodeSolution(m_column_names, dlx_solution);
	
	for (const auto& row : decoded_solution) {
	    // Each row has an individual piece (color) covering some
	    // coordinates
	    std::vector<Coord> coords;
	    char color;

	    for (const auto& s : row) {
		if (isdigit(s[0])) {
		    coords.emplace_back(char_to_int(s[0]),
					char_to_int(s[1]));
		}
		else if (s.size() == 2) {
		    color = assign_color(s);
		}
		
	    }

	    for (const auto& [r, c] : coords) {
		m_board[r*8 + c] = color;
	    }
	}
	m_board[32] = '\0';
	m_solutions.push_back(std::string(m_board));
    }
    return m_solutions.size();
}

void
IqTwistSolver::Reset(std::string cmd)
{
    m_pegs = ParseCmd(cmd);
    m_column_names = GenColumnNames();
    m_dlx = Dlx::DancingLinks(GenBinaryMatrix());
    m_solutions.clear();
}
    
std::vector<Peg>
IqTwistSolver::ParseCmd(std::string cmd) {

    std::vector<Peg> pegs;
		
    std::set<Coord> peg_placements;
    std::map<char, int> possible_placements = {
					       {'r', 1},
					       {'b', 2},
					       {'y', 2},
					       {'g', 2}
    };

    auto char_to_int = [](char c) -> int
    {
	return c - '0';
    };
    
    // getopt expects the argv structure
    cmd = cmd.insert(0, "dummy_cmd ");
    
    /*
      A little hack to get argc and argv from a string
    */
    std::vector<std::string> v;
    std::istringstream iss(cmd);
    for (std::string s; iss >> s; )
	v.push_back(s);
    
    std::vector<char*> tmp;
    for (int i = 0; i < v.size(); i++) {
	tmp.push_back(&v.at(i)[0]);
    }
    
    char** argv = &tmp[0];
    
    char c;
    while ((c = getopt(v.size(), argv, "r:b:y:g:")) != -1) {
	
	switch(c) {
	case 'r':
	case 'b':
	case 'y':
	case 'g':
	    // TODO - come up with error msg
	    assert(strlen(optarg) == 2);
	    
	    int coord_1 = char_to_int(optarg[0]);
	    int coord_2 = char_to_int(optarg[1]);
	    
	    // TODO - come up with error msg
	    assert((coord_1 >= 0) and (coord_2 >= 0));
	    assert((coord_1 < 4) and (coord_2 < 8));
	    
	    Coord po = {coord_1, coord_2};
	    
	    // TODO - come up with error msg
	    assert(peg_placements.count(po) == 0);
	    peg_placements.insert(po);
	    
	    assert(possible_placements[c]--);
	    
	    pegs.push_back({c, po});
	    break;
	}
    }
    
    // Reset getopt for future calls to it
    optind = 0;
    return pegs;
}

std::vector<std::string>
IqTwistSolver::GenColumnNames() {
    // Initialize column names with each individual piece
    std::vector<std::string> column_names = {"r1", "r2", "b1", "b2", "y1", "y2", "g1", "g2"};

    // Now add "${ij}" coordinates
    for (size_t i = 0; i < iq_twist_board.size(); i++) {
	for (size_t j = 0; j < iq_twist_board[0].size(); j++) {
	    std::string s = std::to_string(i) + std::to_string(j);
	    column_names.push_back(s);
	}
    }

    // Finally, add pegs as "p${ij}"
    for (const auto& p : m_pegs) {
	auto& [r_coord, c_coord] = p.second;
	
	std::string s = "p" + std::to_string(r_coord) + std::to_string(c_coord);
	column_names.push_back(s);
    }
    return column_names;
}

Dlx::BinaryMatrix
IqTwistSolver::GenBinaryMatrix()
{
    size_t r = iq_twist_board.size();
    size_t c = iq_twist_board[0].size();
    
    Dlx::BinaryMatrix bin_mat;

    // Dimension of each row in bin_mat
    size_t row_size = pieces.size() + (r * c) + m_pegs.size();
    
    for (size_t piece_idx = 0; piece_idx < pieces.size(); ++piece_idx) {

	for (const auto& uc : unique_configurations[piece_idx]) {
	    
	    size_t pr = uc.size();
	    size_t pc = uc[0].size();
	    
	    for (size_t i = 0; i <= (r-pr); ++i) {
		for (size_t j = 0; j <= (c-pc); ++j) {
		    		    
		    std::vector<bool> row(row_size);
		    size_t col_idx = 0;
		    
		    row[piece_idx] = true;
		    
		    col_idx += pieces.size();

		    for (size_t ii = 0; ii < pr; ++ii) {
			for (size_t jj = 0; jj < pc; ++jj) {
			    row[col_idx + (i+ii)*c + (j+jj)] = uc[ii][jj].real();
			}
		    }

		    col_idx += (r*c);

		    for (const auto& [color, coord] : m_pegs) {
			const auto& [piece_idx_1, piece_idx_2] = color_to_idxs.at(color);
			
			if ((piece_idx_1 == piece_idx) or (piece_idx_2 == piece_idx)) {
			    
			    auto [ii, jj] = coord;
			    if (ii >= i and (ii-i) < pr and jj >= j and (jj-j) < pc) {
				if (uc[ii-i][jj-j].imag()) {
				    row[col_idx] = true;
				}
			    }
			}

			++col_idx;
		    }
		    
		    bin_mat.push_back(std::move(row));
		}
	    }
	}
    }
    
    return bin_mat;
}

const std::vector<std::vector<bool>> IqTwistSolver::iq_twist_board =
{
 {1, 1, 1, 1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1, 1, 1, 1}
};
    
const std::map<char, Coord> IqTwistSolver::color_to_idxs = {
	{'r', {0, 1}},
	{'b', {2, 3}},
	{'y', {4, 5}},
	{'g', {6, 7}}
};

} // namespace
    
