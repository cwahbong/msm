#include "msm/land.h"

#include <vector>

namespace msm {

namespace {

struct Cell {
    bool mine;
    bool flag;
    bool dig;
    Size neighbors_mine_count;
};

class SquareLand: public Land {
public:
    SquareLand(Size row, Size col);
    virtual ~SquareLand();

    Result<Void> SetNeighbor(const Location & location1, const Location & location2) override;

    Result<Void> SetMine(const Location & location, bool isMine) override;
    Result<bool> HasMine(const Location & location) const override;

    Result<Void> SetFlag(const Location & location, bool flag) override;
    Result<bool> HasFlag(const Location & location) const override;

    Result<Void> SetDig(const Location & location, bool digged) override;
    Result<bool> IsDigged(const Location & location) const override;

    Result<Size> GetNeighborsMineCount(const Location & location) const override;

    Result<std::unique_ptr<LocationViewer>> GetAllLocations() const override;
    Result<std::unique_ptr<LocationViewer>> GetNeighbors(const Location & location) const override;

private:
    Result<Void> ValidateLocation(const Location & location) const;
    void TryAddNeighborCount(const Location & location);

    Size _row;
    Size _col;
    std::vector<std::vector<Cell>> _cell;
};

SquareLand::SquareLand(Size row, Size col):
    Land(),
    _row(row),
    _col(col),
    _cell(row, std::vector<Cell>(col, Cell{.mine = false, .flag = false, .dig = false, .neighbors_mine_count = 0}))
{}

SquareLand::~SquareLand() = default;

Result<Void>
SquareLand::SetNeighbor(const Location &, const Location &)
{
    return Error::NOT_SUPPORTED;
}

Result<Void>
SquareLand::SetMine(const Location & location, bool is_mine)
{
    VALUE_OR_RETURN(ValidateLocation(location));
    _cell[location.y][location.x].mine = is_mine;
    for (int z = 0; z < 2; ++z) {
        TryAddNeighborCount(Location(location.x - 1 + z, location.y - 1)); // top
        TryAddNeighborCount(Location(location.x + 1, location.y - 1 + z)); // right
        TryAddNeighborCount(Location(location.x + 1 - z, location.y + 1)); // bottom
        TryAddNeighborCount(Location(location.x - 1, location.y + 1 - z)); // left
    }
    return VOID;
}

Result<bool>
SquareLand::HasMine(const Location & location) const
{
    VALUE_OR_RETURN(ValidateLocation(location));
    return _cell[location.y][location.x].mine;
}

Result<Void>
SquareLand::SetFlag(const Location & location, bool flag)
{
    VALUE_OR_RETURN(ValidateLocation(location));
    _cell[location.y][location.x].flag = flag;
    return VOID;
}

Result<bool>
SquareLand::HasFlag(const Location & location) const
{
    VALUE_OR_RETURN(ValidateLocation(location));
    return _cell[location.y][location.x].flag;
}

Result<Void>
SquareLand::SetDig(const Location & location, bool digged)
{
    VALUE_OR_RETURN(ValidateLocation(location));
    _cell[location.y][location.x].dig = digged;
    return VOID;
}

Result<bool>
SquareLand::IsDigged(const Location & location) const
{
    VALUE_OR_RETURN(ValidateLocation(location));
    return _cell[location.y][location.x].dig;
}

Result<Size>
SquareLand::GetNeighborsMineCount(const Location & location) const
{
    VALUE_OR_RETURN(ValidateLocation(location));
    return _cell[location.y][location.x].neighbors_mine_count;
}

Result<std::unique_ptr<LocationViewer>>
SquareLand::GetAllLocations() const
{
    // TODO
}

Result<std::unique_ptr<LocationViewer>>
SquareLand::GetNeighbors(const Location & location) const
{
    // TODO
}

Result<Void>
SquareLand::ValidateLocation(const Location & location) const
{
    if (location.x < 0 || static_cast<Size>(location.x) >= _col ||
        location.y < 0 || static_cast<Size>(location.y) >= _row) {
        return Error::BAD_PARAMETER;
    }
    return VOID;
}

void
SquareLand::TryAddNeighborCount(const Location & location)
{
    if (ValidateLocation(location).IsError()) {
        return;
    }
    _cell[location.y][location.x].neighbors_mine_count += 1;
}

} // namespace

Land::~Land() = default;

namespace land {

std::unique_ptr<Land>
NewSquare(Size row, Size col)
{
    return std::make_unique<SquareLand>(row, col);
}

} // namespace land

} // namespace msm
