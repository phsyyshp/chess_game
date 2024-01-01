#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
std::vector<uint64_t> line_to_nums_vec(const std::string &line);
std::vector<uint64_t> read_magic_numbers_to_vec(std::string piece);
std::vector<std::vector<uint64_t>> read_look_up_tables(std::string piece);
int get_linear_position(const uint64_t &position);
std::vector<int> position_to_row_col(const uint64_t &position);
int rook_relevant_bits(const uint64_t &position);
int bishop_relevant_bits(const uint64_t &position);
uint64_t generate_magic_index(const uint64_t &bitboard,
                              const uint64_t &magic_number, int shiftBits);

uint64_t
get_attack_mask(const u_int64_t &position, const u_int64_t &bitboard,
                const std::vector<uint64_t> &magic_numbers,
                const std::vector<std::vector<uint64_t>> &look_up_tables,
                std::string piece_type);