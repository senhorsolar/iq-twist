#ifndef PIECES_H
#define PIECES_H

#include <iostream>
#include <complex>
#include <sstream>
#include <vector>

namespace IqTwist
{

typedef std::complex<bool> Element;
typedef std::vector<std::vector<Element>> Piece;
    
const Element P(1, 1); // Peg opening
const Element X(1, 0); // Regular element
const Element O(0, 0); // Empty element

const std::vector<Piece> pieces =
{
 
 // Red pieces
 {{X, P, O},
  {O, X, X}},
 
 {{P, X, P},
  {O, O, X}},
 
 // Blue pieces
 {{X, X, X},
  {O, P, P}},
 
 {{X, X, P, X}},
 
 // Yellow pieces
 {{P, O, O},
  {P, X, X},
  {O, P, O}},
 
 {{P, X, X}},
 
 // Green pieces
 {{P, P},
  {O, X}},
 
 {{P, X, X},
  {O, P, O}}
};

inline
Piece rotate_piece(const Piece& p) {
    size_t nrows = p.size();
    size_t ncols = p[0].size();

    Piece rp; // rotated piece
    
    for (int j = ncols-1; j >= 0; --j) {
	std::vector<Element> row;
	for (size_t i = 0; i < nrows; ++i) {
	    row.push_back(p[i][j]);
	}
	rp.push_back(row);
    }
    return rp;
}

inline
Piece mirror_piece(const Piece& p) {
    size_t nrows = p.size();
    size_t ncols = p[0].size();
    
    Piece mp; // mirrored piece
		
    for (size_t i = 0; i < nrows; i++) {
	std::vector<Element> row; 
	for (int j = ncols-1; j >= 0; j--) {
	    row.push_back(p[i][j]);
	}
	mp.push_back(row);
    }    
    return mp;
}

// Returns string representation of piece that ignores peg openings
inline
std::string piece_to_str(const Piece& p) {
    std::ostringstream oss;
		
    size_t nrows = p.size();
    size_t ncols = p[0].size();
		
    for (size_t i = 0; i < nrows; i++) {
	for (size_t j = 0; j < ncols; j++) {
	    oss << p[i][j].real();
	    oss << ((j < ncols-1) ? "," : "\n");
	}
    }    
    return oss.str();
}

// Merges imaginary elements
inline
void merge_imag(Piece& p1, const Piece& p2) {
    // Assumes pieces are of the same dimension
    size_t nrows = p1.size();
    size_t ncols = p1[0].size();
    
    for (size_t i = 0; i < nrows; i++) {
	for (size_t j = 0; j < ncols; j++) {
	    p1[i][j].imag(p1[i][j].imag() | p2[i][j].imag());
	}
    }
}

inline
std::vector<Piece> gen_unique_configurations(const Piece& p) {
    
    std::vector<Piece> rotations;
    
    rotations.push_back(p);
    for (size_t i = 0; i < 3; i++)
	rotations.push_back(rotate_piece(rotations.back()));
    
    rotations.push_back(mirror_piece(p));
    for (int i = 0; i < 3; i++)
	rotations.push_back(rotate_piece(rotations.back()));
		
    std::map<std::string, Piece> uc; // unique configurations

    
    for (auto& p : rotations) {
	std::string key = piece_to_str(p);
			
	if (uc.count(key)) {
	    merge_imag(uc[key], p);
	} else {
	    uc[key] = p;
	}
    }
		
    std::vector<Piece> unique_pieces;
    for (auto& [key, val] : uc) {
	unique_pieces.push_back(val);
    }

    return unique_pieces;
}

const std::vector<std::vector<Piece>>
unique_configurations = []()
{
    std::vector<std::vector<Piece>> uc;
    std::transform(pieces.begin(), pieces.end(),
		   std::back_inserter(uc),
		   [](const Piece& p) {
		       return gen_unique_configurations(p);});
    return uc;
}();
    
} // namespace

#endif // PIECES_H
