#include "Grid.hpp"

#include <algorithm>
#include <fstream>

namespace gol
{

// Initialize the grid with the dimensions (all cells are dead)
Grid::Grid(Grid::size_type rows, Grid::size_type cols)
    : m_rows{rows}, m_cols{cols}, m_data(rows*cols, 0)
{

}

/*Grid::Grid(std::initializer_list<std::initializer_list<Grid::value_type>> grid)
{

}
*/

// Read grid from file
Grid::Grid(char const *filename)
{
    std::ifstream file{filename};
    file >> (*this);
}

[[nodiscard]] auto Grid::getRows() const -> size_type
{
    return m_rows;
}
[[nodiscard]] auto Grid::getCols() const -> size_type
{
    return m_cols;
}

[[nodiscard]] auto Grid::get(Grid::difference_type i, Grid::difference_type j)       -> reference
{
    return m_data[mapIndex(i, m_rows)*m_cols + mapIndex(j, m_cols)];
}
[[nodiscard]] auto Grid::get(Grid::difference_type i, Grid::difference_type j) const -> const_reference
{
    return m_data[mapIndex(i, m_rows)*m_cols + mapIndex(j, m_cols)];
}

[[nodiscard]] auto Grid::operator()(Grid::difference_type i, Grid::difference_type j)       -> reference
{
    return this->get(i, j);
}
[[nodiscard]] auto Grid::operator()(Grid::difference_type i, Grid::difference_type j) const -> const_reference
{
    return this->get(i, j);
}

[[nodiscard]] auto Grid::isAlive(Grid::difference_type i, Grid::difference_type j) const -> bool
{
    return this->get(i, j) == 1;
}
[[nodiscard]] auto Grid::isDead (Grid::difference_type i, Grid::difference_type j) const -> bool
{
    return this->get(i, j) == 0;
}

auto Grid::kill  (Grid::difference_type i, Grid::difference_type j) -> void
{
    this->get(i, j) = 0;
}
auto Grid::revive(Grid::difference_type i, Grid::difference_type j) -> void
{
    this->get(i, j) = 1;
}

auto Grid::flipCell(Grid::difference_type i, Grid::difference_type j) -> void
{
    this->get(i, j) = (this->isAlive(i, j)) ? 0 : 1;
}

auto Grid::clear() -> void
{
    std::fill(m_data.begin(), m_data.end(), 0);
}
// Method to get the next state of the grid without modifying the original
[[nodiscard]] auto Grid::next() const -> Grid
{
    Grid updated{*this};

    for (size_type i=0; i<m_rows; ++i)
        for (size_type j=0; j<m_cols; ++j)
            this->updateCell(updated, i, j);

    return updated;
}

// Method to update the grid to the next state
auto Grid::update() -> Grid&
{
    *this = this->next();
    return *this;
}

// Embed another grid starting at position (i, j)
//
// If the grid to embed is bigger than this grid, the behaviour is undefined
auto Grid::embedGrid(Grid const &grid, Grid::difference_type i, Grid::difference_type j) -> Grid&
{
    for (size_type i_=0; i_<grid.getRows(); ++i_)
        for (size_type j_=0; j_<grid.getCols(); ++j_)
            this->get(i+i_, j+j_) = grid(i_, j_);

    return *this;
}

// If an index is out of bounds, just map it to an in-bound index
[[nodiscard]] auto Grid::mapIndex(Grid::difference_type a, Grid::size_type n) -> size_type
{
    return (a < 0)
        ? n - static_cast<size_type>(-a) % n
        : static_cast<size_type>(a) % n;
}

// This function counts alive neighbours of a given cell
[[nodiscard]] auto Grid::countAliveNeighbours(size_type i, size_type j) const -> size_type
{
    size_type count = 0;

    /* More general approach, but less readable and maybe slower
     *
    for (difference_type i_=-1; i_<=1; ++i_)
        for (difference_type j_=-1; j_<=1; ++j_)
            if ((i_!=0 || j_!=0) && this->isAlive(i+i_, j+j_))
                ++count;
    */

    if (this->isAlive(i-1, j)  )  // Top square
        ++count;
    if (this->isAlive(i-1, j+1))  // Top-Right square
        ++count;
    if (this->isAlive(i, j+1)  )  // Right square
        ++count;
    if (this->isAlive(i+1, j+1))  // Bottom-Right square
        ++count;
    if (this->isAlive(i+1, j)  )  // Bottom square
        ++count;
    if (this->isAlive(i+1, j-1))  // Bottom-Left square
        ++count;
    if (this->isAlive(i, j-1)  )  // Left square
        ++count;
    if (this->isAlive(i-1, j-1))  // Top-Left square
        ++count;

    return count;
}

// This function updates a cell from the given grid based in this object's grid
auto Grid::updateCell(Grid &updated, size_type i, size_type j) const -> void
{
    auto alive_neighbours = this->countAliveNeighbours(i, j);

    if (this->isAlive(i, j) && (alive_neighbours < 2 || alive_neighbours > 3))
        updated(i, j) = 0;
    else if (this->isDead(i, j) && alive_neighbours == 3)
        updated(i, j) = 1;
}

}   // namespace gol
