#ifndef PATTERN_MATCHING_HPP
#define PATTERN_MATCHING_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace pattern_matching {

std::vector<std::vector<size_t>> aho_corasick(const std::string &alphabet, const std::string &text,
                                              const std::vector<std::string> &patterns);

} // namespace pattern_matching

#endif
