// iq_tiwst.h
#pragma once

#include <complex>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
//#include <wordexp.h>

#include "dlx/dlx.h"

using namespace std;


class IqTwist : public DancingLinks {
public:	

	typedef vector<vector<complex<int> > > piece;
	typedef pair<int, int> coord;
	typedef pair<char, coord> peg;
	
	IqTwist(string cmd) : IqTwist(parse_cmd(cmd)) {}
	IqTwist(vector<peg> pegs) : DancingLinks(iq_twist_dlx_matrix(pegs),
						 iq_twist_column_names(pegs))
		{
		}

	vector<peg> parse_cmd(string cmd) {

		vector<peg> pegs;
		
		set<coord> peg_placements;
		map<char, int> possible_placements = {
					{'r', 1},
					{'b', 2},
					{'y', 2},
					{'g', 2}
		};

		// getopt expects the argv structure
		cmd = cmd.insert(0, "dummy_cmd ");

		/*
		  A little hack to get argc and argv from a string, 
		  since wordexp doesn't work with webassembly
		 */
		//wordexp_t p;
		//wordexp(cmd.c_str(), &p, WRDE_NOCMD);
		vector<string> v;
		istringstream iss(cmd);
		for (string s; iss >> s; )
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
				
				coord po = {coord_1, coord_2};
				
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

	static inline
	piece rotate_piece(piece p) {
		int nrows = p.size();
		int ncols = p[0].size();
		
		piece rp; // rotated piece
		
		for (int j = ncols-1; j >= 0; j--) {
			vector<complex<int> > row;
			for (int i = 0; i < nrows; i++) {
				row.push_back(p[i][j]);
			}
			rp.push_back(row);
		}
		
		return rp;
	}

	static inline
	piece mirror_piece(piece p) {
		int nrows = p.size();
		int ncols = p[0].size();
		
		piece mp; // mirrored piece
		
		for (int i = 0; i < nrows; i++) {
			vector<complex<int> > row; 
			for (int j = ncols-1; j >= 0; j--) {
				row.push_back(p[i][j]);
			}
			mp.push_back(row);
		}
		
		return mp;
	}

	static inline
	string piece_to_str(piece p) {
		ostringstream oss;
		
		int nrows = p.size();
		int ncols = p[0].size();
		
		for (int i = 0; i < nrows; i++) {
			for (int j = 0; j < ncols; j++) {
				oss << p[i][j].real();
				oss << ((j < ncols-1) ? "," : "\n");
			}
		}
		
		return oss.str();
	}

	void merge_imag(piece& p1, piece p2) {
		// Assumes pieces are of the same dimension
		int nrows = p1.size();
		int ncols = p1[0].size();
		
		for (int i = 0; i < nrows; i++) {
			for (int j = 0; j < ncols; j++) {
				p1[i][j].imag(p1[i][j].imag() | p2[i][j].imag());
			}
		}
	}
	
	vector<piece> unique_configurations(piece p) {
		
		vector<piece> rotations;
		
		rotations.push_back(p);
		for (int i = 0; i < 3; i++)
			rotations.push_back(rotate_piece(rotations.back()));
		
		rotations.push_back(mirror_piece(p));
		for (int i = 0; i < 3; i++)
			rotations.push_back(rotate_piece(rotations.back()));
		
		map<string, piece> uc; // unique configurations
		
		for (piece it : rotations) {
			string key = piece_to_str(it);
			
			if (uc.count(key)) {
				merge_imag(uc[key], it);
			} else {
				uc[key] = it;
			}
		}
		
		vector<piece> unique_pieces;
		for (auto& kv : uc) {
			unique_pieces.push_back(kv.second);
		}
		
		return unique_pieces;
	}

	static inline
	int real_sum(piece p) {
		int sum = 0;
		
		for (int i = 0; i < p.size(); i++)
			for (int j = 0; j < p[0].size(); j++)
				sum += p[i][j].real();
		
		return sum;
	}
	
	vector<vector<complex<int> > > init_complex_row() {
		vector<vector<complex<int> > > row2d;
		
		for (int i = 0; i < iq_twist_board.size(); i++) {
			vector<complex<int> > row(iq_twist_board[0].size(), 0);
			row2d.push_back(row);
		}
		
		return row2d;
	}

	vector<string> iq_twist_column_names(vector<peg> pegs) {
		vector<string> column_names = {"r1", "r2", "b1", "b2", "y1", "y2", "g1", "g2"};
		
		for (int i = 0; i < iq_twist_board.size(); i++) {
			for (int j = 0; j < iq_twist_board[0].size(); j++) {
				string s = to_string(i) + to_string(j);
				column_names.push_back(s);
			}
		}
		
		for (peg p : pegs) {
			auto [r_coord, c_coord] = p.second;
			
			string s = "p" + to_string(r_coord) + to_string(c_coord);
			column_names.push_back(s);
		}
		
		return column_names;
	}

	vector<vector<int> > iq_twist_dlx_matrix(vector<peg> pegs) {
		
		int r = iq_twist_board.size();
		int c = iq_twist_board[0].size();
		
		vector<vector<int> > mat;

		int debug_idx = 0;
		for (int piece_idx = 0; piece_idx < pieces.size(); piece_idx++) {
			piece p = pieces[piece_idx];
			for (piece uc : unique_configurations(p)) {
				
				int pr = uc.size();
				int pc = uc[0].size();

				for (int i = 0; i <= (r-pr); i++) {
					for (int j = 0; j <= (c-pc); j++) {
						
						vector<vector<complex<int> > > row2d = init_complex_row();
						
						for (int ii = 0; ii < pr; ii++)
							for (int jj = 0; jj < pc; jj++)
								row2d[i+ii][j+jj] = uc[ii][jj];

						vector<int> row(pieces.size());
						row[piece_idx] = 1;
						
						for (vector<complex<int> > row1d : row2d)
							for (complex<int> val : row1d)
								row.push_back(val.real());
						
						for (peg p_ : pegs) {
							int can_place_peg = 0;
							
							coord piece_idxs = color_to_idxs.at(p_.first);
							
							if ((piece_idxs.first == piece_idx) or (piece_idxs.second == piece_idx)) {
								int r_coord = p_.second.first;
								int c_coord = p_.second.second;
								if (row2d[r_coord][c_coord].imag() == 1)
									can_place_peg = 1;
							}
							
							row.push_back(can_place_peg);
							
							
						}
						
						mat.push_back(row);
						debug_idx++;
					}
				}
			}
		}
		
		return mat;
	}

	static inline
	int char_to_int(char c) {
		return c - '0';
	}

	void add_coord(vector<coord> &coords, string s) {
		pair<int, int> p{char_to_int(s[0]), char_to_int(s[1])};
		coords.push_back(p);
	}

	char assign_color(string s) {
		int d = char_to_int(s[1]) - 1;
		
		char color = s[0];
		if (d)
			color = toupper(color);
		
		return color;
	}

	
	string get_solution(size_t idx) {
		string res;
		if (idx < solutions.size())
			res = solutions[idx];
		return res;
	}
	
private:

	vector<string> solutions;
	
	void display_solution(int q) {
		char board[33];
		for (int k = 0; k < q; k++) {
			vector<coord> coords;
			char color;

			Node* j_node = solution[k];

			string s = j_node->column->name;

			if (isdigit(s[0]))
				add_coord(coords, s);
			else if (s.size() == 2)
				color = assign_color(s);

			for (Node* i_node = j_node->right; i_node != j_node; i_node = i_node->right) {
				string s = i_node->column->name;

				if (isdigit(s[0]))
					add_coord(coords, s);
				else if (s.size() == 2)
					color = assign_color(s);
			}

			for (auto& [r, c] : coords) {
				board[r*8 + c] = color;
			}
		}
		
		board[32] = '\0';
		solutions.push_back(string(board));
		// for (int i = 0; i < 32; i++)
		//  	cout << solutions.back()[i];
		// cout << '\n';
	}
	
        static const vector<piece> pieces;
	static const map<char, pair<int, int> > color_to_idxs;
	static const vector<vector<int> > iq_twist_board;
};

const vector<vector<int> > IqTwist::iq_twist_board = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

const map<char, IqTwist::coord> IqTwist::color_to_idxs = {
	{'r', {0, 1}},
	{'b', {2, 3}},
	{'y', {4, 5}},
	{'g', {6, 7}}
};


const complex<int> P(1, 1); // Peg opening
const complex<int> X(1, 0); // Regular element
const complex<int> O(0, 0); // Empty element

const vector<IqTwist::piece> IqTwist::pieces = {
	
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
