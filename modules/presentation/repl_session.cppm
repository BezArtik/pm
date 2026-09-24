module;

#include <iosfwd>

export module pm.presentation.repl_session;

import pm.presentation.console_view;

export namespace pm::presentation {

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
