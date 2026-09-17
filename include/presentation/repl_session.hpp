// presentation/repl_session.hpp
#pragma once

#include "presentation/console_view.hpp"

#include <iosfwd>

namespace pm::presentation {

class repl_session {
public:
    repl_session(console_view& view) : view_{view} {}

    ~repl_session() = default;
    repl_session(const repl_session&) = delete;
    repl_session& operator=(const repl_session&) = delete;
    repl_session(repl_session&&) = delete;
    repl_session& operator=(repl_session&&) = delete;

    void run(std::istream& in, std::ostream& out);

private:
    console_view& view_;
};

}  // namespace pm::presentation
