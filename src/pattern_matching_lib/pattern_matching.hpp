#ifndef PATTERN_MATCHING_HPP
#define PATTERN_MATCHING_HPP

#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::vector<int>> match(const std::vector<std::string> &patterns,
                                                        const std::string &text);

#endif
