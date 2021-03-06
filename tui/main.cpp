#include <iostream>
#include <string>
#include <memory>

#include <msm/agent.h>
#include <msm/land.h>
#include <msm/land_viewer.h>
#include <msm/location_range.h>
#include <msm/miner.h>
#include <msm/observer.h>
#include <msm/runner.h>


namespace {

enum class ExitStatus: int {
    OK = 0,
    CMD_NOT_FOUND,
    BAD_IO,
    MSM_ERROR,
};

class SquareTui: public msm::Observer, public msm::Agent {
public:
    SquareTui(std::istream &, std::ostream &);

    void OnDig(const msm::Location &, msm::Size, bool) noexcept override;
    void OnEnd(bool) noexcept override;

    msm::Action GenAction(const msm::LandViewer &) noexcept override;

private:
    std::string EmptyCellString() const;
    std::string LandCellString(const msm::Result<msm::CellView> & cellView) const;
    void PrintLand(const msm::LandViewer & landViewer) const;

    std::istream & _is;
    std::ostream & _os;
};

SquareTui::SquareTui(std::istream & is, std::ostream & os):
    _is(is),
    _os(os)
{}

void
SquareTui::OnDig(const msm::Location & location, msm::Size n, bool hasMine) noexcept
{
    if (hasMine) {
        _os << "Location (" << location.x << ", " << location.y << ") has mine, bomb!" << std::endl;
    } else {
        _os << "Dig location (" << location.x << ", " << location.y << "), which is a number [" << n << "]" << std::endl;
    }
}

void
SquareTui::OnEnd(bool win) noexcept
{
    _os << (win ? "Win" : "Lose") << std::endl;
}

msm::Action
SquareTui::GenAction(const msm::LandViewer &) noexcept
{
    while (true) {
        std::string line;
        if (!getline(_is, line)) {
            return msm::Action {
                .type = msm::ActionType::END,
                .location = msm::Location(),
            };
        }
        // TODO parse line
    }
}

std::string
SquareTui::EmptyCellString() const
{
    return " ";
}

std::string
SquareTui::LandCellString(const msm::Result<msm::CellView> & cellViewResult) const
{
    if (cellViewResult.IsError()) {
        return "!";
    }
    const msm::CellView cellView = cellViewResult.GetValue();
    if (cellView.has_mine) {
        return "*";
    }
    if (cellView.digged) {
        if (cellView.neighbors_mine_count == 0) {
            return " ";
        } else {
            return std::string(1, '0' + cellView.neighbors_mine_count % 10);
        }
    }
    if (cellView.flagged) {
        return "F";
    }
    return "O";
}

void
SquareTui::PrintLand(const msm::LandViewer & landViewer) const
{
    auto result = landViewer.GetAllLocations();
    if (result.IsError()) {
        // LOG
        return;
    }
    const auto locations = result.GetValue();
    msm::Location prev(-1, -1);
    for (const auto & location: *locations) {
        if (prev.y < location.y) {
            prev.x = 0;
            do {
                prev.y += 1;
                _os << std::endl;
            } while (prev.y < location.y);
        }
        while (prev.x + 1 < location.x) {
            prev.x += 1;
            _os << EmptyCellString();
        }
        _os << LandCellString(landViewer.View(location));
    }
}

ExitStatus
square()
{
    auto land = msm::land::NewSquare(10, 12);
    auto tui = std::make_shared<SquareTui>(std::cin, std::cout);
    auto runner = msm::runner::New(std::move(land), tui, tui);
    const auto result = runner->Run();
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
