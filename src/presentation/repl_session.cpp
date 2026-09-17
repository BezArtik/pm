#include "presentation/repl_session.hpp"

#include <iostream>
#include <string>

namespace pm::presentation {

void repl_session::run(std::istream& in, std::ostream& out) {
    std::string line;

    while (true) {
        out << "pm> " << std::flush;

        if (!std::getline(in, line)) { break; }

        if (!view_.parse_line(line)) { break; }
    }
}

}  // namespace pm::presentation
