#ifndef DRAWABLE_GRID_HPP
#define DRAWABLE_GRID_HPP

#include "Grid.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

namespace gol
{

class DrawableGrid : public sf::Drawable, public Grid
{
public:
    using Grid::Grid;

    [[nodiscard]] auto getPointedCell() const -> sf::Vector2<difference_type>;
    auto setPointedCell(difference_type i, difference_type j) -> void;

private:
    /*
    static sf::RectangleShape const white_box;
    static sf::RectangleShape const black_box;
    static sf::RectangleShape const red_square;
    */

    [[nodiscard]] auto getCellSize(sf::RenderTarget& target) const -> sf::Vector2f;
    [[nodiscard]] auto isPointedCellValid() const -> bool;

    auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void override;

    sf::Vector2<difference_type> m_pointed_cell{-1, -1};
};

}

#endif  // DRAWABLE_GRID_HPP
