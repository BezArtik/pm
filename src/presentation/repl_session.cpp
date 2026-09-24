module;

#include <istream>
#include <ostream>
#include <string>

module pm.presentation.repl_session;

namespace pm::presentation {

void repl_session::run(std::istream& in, std::ostream& out) {
    auto&& line = std::string{};

    while (true) {
        out << "pm> " << std::flush;

        if (!std::getline(in, line)) { break; }

        if (!view_.parse_line(line)) { break; }
    }
}

}  // namespace pm::presentation
