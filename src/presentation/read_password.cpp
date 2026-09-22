// presentation/read_password.cpp
#include "presentation/read_password.hpp"

#include <iostream>
#include <print>
#include <termios.h>
#include <unistd.h>

namespace pm::presentation {

std::string read_master_password() {
    std::print("Master password: ");

    auto&& oldt = termios{};
    tcgetattr(STDIN_FILENO, &oldt);

    auto&& newt = termios{oldt};
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    auto&& password = std::string{};
    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::print("\n");

    return password;
}

}  // namespace pm::presentation
