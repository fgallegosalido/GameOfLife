#ifndef GRID_HPP
#define GRID_HPP

#include <functional>
#include <iostream>
#include <vector>

namespace gol
{

class Grid
{
public:
    using container_type    = std::vector<int>;
    using value_type        = typename container_type::value_type;
    using size_type         = typename container_type::size_type;
    using difference_type   = typename container_type::difference_type;
    using reference         = typename container_type::reference;
    using const_reference   = typename container_type::const_reference;

    /*
    enum class Type
    {
        Plane,
        VerticalCilinder,
        HorizontalCilinder,
        VerticalMoebiusStrip,
        HorizontalMoebiusStrip,
        Torus,
        VerticalKleinBottle,
        HorizontalKleinBottle
    };
    */

    /*
    // Still lifes
    static Grid const Block;
    static Grid const BeeHive;
    static Grid const Loaf;
    static Grid const Boat;
    static Grid const Tub;

    // Oscillators
    static Grid const Blinker;
    static Grid const Toad;
    static Grid const Beacon;
    static Grid const Pulsar;
    static Grid const PentaDecathlon;

    // Spaceships
    static Grid const Glider;
    static Grid const LightWeightSpaceship;
    static Grid const MiddleWeightSpaceship;
    static Grid const HeavyWeightSpaceship;

    // Guns
    static Grid const GosperGliderGun;
    static Grid const SimkinGliderGun;
    */

    Grid()                                     = default;
    Grid(Grid const &other)                    = default;
    Grid(Grid &&other)                         = default;
    auto operator=(Grid const &other) -> Grid& = default;
    auto operator=(Grid &&other)      -> Grid& = default;
    ~Grid()                                    = default;

    // Initialize the grid with the dimensions (all cells are dead)
    Grid(size_type rows, size_type cols);
    // Initialize from initializer_list, to allow a nice syntax
    //Grid(std::initializer_list<std::initializer_list<value_type>> grid);
    // Read grid from file
    explicit Grid(char const *filename);

    //[[nodiscard]] auto getGridType() const -> Type;
    //auto setGridType(Type type) -> void;

    [[nodiscard]] auto getRows() const -> size_type;
    [[nodiscard]] auto getCols() const -> size_type;

    [[nodiscard]] auto get(difference_type i, difference_type j)       -> reference;
    [[nodiscard]] auto get(difference_type i, difference_type j) const -> const_reference;

    [[nodiscard]] auto operator()(difference_type i, difference_type j)       -> reference;
    [[nodiscard]] auto operator()(difference_type i, difference_type j) const -> const_reference;

    [[nodiscard]] auto isAlive(difference_type i, difference_type j) const -> bool;
    [[nodiscard]] auto isDead (difference_type i, difference_type j) const -> bool;

    auto kill  (difference_type i, difference_type j) -> void;
    auto revive(difference_type i, difference_type j) -> void;

    auto flipCell(difference_type i, difference_type j) -> void;
    auto clear() -> void;

    // Method to get the next state of the grid without modifying the original
    [[nodiscard]] auto next() const -> Grid;
    // Method to update the grid to the next state
    auto update() -> Grid&;

    // Embed another grid starting at position (i, j)
    //
    // If the grid to embed is bigger than this grid, the behaviour is undefined
    auto embedGrid(Grid const &grid, difference_type i, difference_type j) -> Grid&;

    // Function to read a grid from a stream (stdin, file, etc).
    // The first line is two integers (dimensions of the matrix) followed
    // by n lines of m zeros and ones. For example:
    //
    // 5 5
    // 0 0 0 0 0
    // 0 0 1 0 0
    // 0 0 0 1 0
    // 0 1 1 1 0
    // 0 0 0 0 0
    template <typename Istream>
    friend auto operator>>(Istream &in, Grid &grid) -> Istream&
    {
        in >> grid.m_rows >> grid.m_cols;
        grid.m_data.resize(grid.m_rows * grid.m_cols);

        for (Grid::size_type i=0; i<grid.m_rows; ++i)
            for (Grid::size_type j=0; j<grid.m_cols; ++j)
                in >> grid(i, j);

        return in;
    }

    template <typename Ostream>
    friend auto operator<< (Ostream &out, Grid const &grid) -> Ostream&
    {
        for (Grid::size_type i=0; i<grid.m_rows; ++i)
        {
            for (Grid::size_type j=0; j<grid.m_cols; ++j)
            {
                if (grid(i, j) == 0)
                    out << " \x1b[38;2;255;0;0m\u25FC\x1b[0m";
                else
                    out << " \x1b[38;2;255;255;255m\u25FC\x1b[0m";
            }
            out << '\n';
        }

        return out;
    }

private:
    // Some auxiliar functions not needed in the public interface

    // If an index is out of bounds, just map it to an in-bound index
    [[nodiscard]] static auto mapIndex(difference_type a, size_type n) -> size_type;
    // This function counts alive neighbours of a given cell
    [[nodiscard]] auto countAliveNeighbours(size_type i, size_type j) const -> size_type;
    // This function updates a cell from the given grid based in this object's grid
    auto updateCell(Grid &updated, size_type i, size_type j) const -> void;

    // Data section
    //Type m_type = Torus;
    size_type m_rows = 3;
    size_type m_cols = 3;
    container_type m_data = container_type(9, 0);
};

}   // namespace gol

#endif  // GRID_HPP
