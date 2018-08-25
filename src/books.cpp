#include "books.hpp"

int Books::add(const std::string& title)
{
	std::lock_guard<std::mutex> guard(m);
    auto next = map.size() + 1;
    map[next] = title;
	return next;
}

std::string Books::title(int id) const
{
	return map.at(id);
}

