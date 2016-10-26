#include <iostream>
#include <string>
#include <memory>

#include <msm/land.h>
#include <msm/miner.h>
#include <msm/observer.h>


namespace {

enum class ExitStatus: int {
    OK = 0,
    CMD_NOT_FOUND,
    BAD_IO,
    MSM_ERROR,
};

class TuiObserver: public msm::Observer {
public:
    void OnDig(const msm::Location &, msm::Size, bool) noexcept override;
    void OnEnd(bool) noexcept override;
};

void
TuiObserver::OnDig(const msm::Location & location, msm::Size n, bool hasMine) noexcept
{
    // TODO
}

void
TuiObserver::OnEnd(bool win) noexcept
{
}

ExitStatus
square()
{
    std::unique_ptr<msm::Land> land = msm::land::NewSquare(5, 2);
    // TODO init a new miner
    msm::Result<msm::Void> result(msm::Error::OK);
    if (result.IsError()) {
        std::cerr << "Msm internal error: " << static_cast<int>(result.GetError()) << '\n';
        return ExitStatus::MSM_ERROR;
    }
    return ExitStatus::OK;
}

ExitStatus
help(ExitStatus defaultExitStatus)
{
    std::cout << R"(Usage: msm_tui [OP]
Text ui for msm. If no OP is specified then it will go into menu mode.

Operation:
    square                  square mine mode
    help                    show help message
)";
    return defaultExitStatus;
}

ExitStatus
run(const std::string & cmd)
{
    if (cmd == "square") {
        return square();
    } else if (cmd == "help") {
        return help(ExitStatus::OK);
    }
    return ExitStatus::CMD_NOT_FOUND;
}

ExitStatus
menu()
{
    while (true) {
        std::string line;
        if (!getline(std::cin, line)) {
            return ExitStatus::BAD_IO;
        }
        run(line);
    }
    return ExitStatus::OK;
}

ExitStatus
main_impl(int argc, char ** argv)
{
    if (argc > 1) {
        const ExitStatus runExitStatus = run(argv[1]);
        if (runExitStatus == ExitStatus::CMD_NOT_FOUND) {
            return help(ExitStatus::CMD_NOT_FOUND);
        } else {
            return runExitStatus;
        }
    }
    return menu();
}

} // namespace

int
main(int argc, char ** argv)
{
    return static_cast<int>(main_impl(argc, argv));
}
