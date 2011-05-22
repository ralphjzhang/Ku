#include "memory.hpp"
#include "singleton.hpp"
#include <string>
#include <type_traits>

using ku::su::singleton;

class bachelor : public singleton<bachelor>
{
};

int main()
{
    using namespace ku::su;

    auto sp = construct<std::string>(1, 'e');
    auto sptr = make_unique<std::string>(1, 'e');

    bachelor::get();
}
