// iq_tiwst.h

#pragma once

#ifndef IQ_TWIST_H
#define IQ_TWIST_H

#include <complex>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "dlx/dlx.h"
#include "pieces.h"

namespace IqTwist {

// Coordinate type
typedef std::pair<size_t, size_t> Coord;
// Peg type
typedef std::pair<char, Coord> Peg;
    
class IqTwistSolver
{
public:	

    // ctor
    IqTwistSolver(std::string cmd);

    size_t Solve();
    void Reset(std::string cmd);
	
    const std::vector<std::string>& GetColumnNames() { return m_column_names;}

    const std::vector<std::string>& GetSolutions() { return m_solutions;}
    
    std::string GetSolution(size_t idx) {
	std::string res;
	if (idx < m_solutions.size())
	    res = m_solutions[idx];
	return res;
    }

    size_t SolutionCount() { return m_solutions.size();}
    
private:

    // Converts command line args to peg placements
    std::vector<Peg> ParseCmd(std::string cmd);
    
    std::vector<std::string> GenColumnNames();
    Dlx::BinaryMatrix GenBinaryMatrix(); 
    	
    static const std::map<char, Coord> color_to_idxs;
    static const std::vector<std::vector<bool> > iq_twist_board;

    // Member vars
    std::vector<Peg> m_pegs;
    std::vector<std::string> m_column_names;
    std::vector<std::string> m_solutions;
    Dlx::DancingLinks m_dlx;
    char m_board[33];
};

} // namespace
    
#endif // IQ_TWIST_H
