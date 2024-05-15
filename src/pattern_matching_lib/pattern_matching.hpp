#ifndef PATTERN_MATCHING_HPP
#define PATTERN_MATCHING_HPP

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace pattern_matching {

std::unordered_map<std::string, std::vector<size_t>>
aho_corasick(const std::vector<std::string> &patterns, const std::string &text);

} // namespace pattern_matching

#endif
