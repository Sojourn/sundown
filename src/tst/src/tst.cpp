#include <iostream>
#include <thread>
#include <vector>
#include <tuple>
#include <array>
#include <string>
#include <stdexcept>

#include <cassert>
#include <cstdint>
#include <cstdio>

#include "util/util.h"
#include "sys/sys.h"

using namespace Sundown;

int main(int argc, char **argv)
{
    Option<std::string> o("What is up ma niggas hahahahahdfdslfj lasdjf lakjflkasjdf;l jasd;lkfj as;dlkfj a;sldj");
    std::cout << o.value() << std::endl;


    return 0;
}

