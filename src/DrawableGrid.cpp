#include "DrawableGrid.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include <utility>

namespace gol
{

[[nodiscard]] auto DrawableGrid::getPointedCell() const -> sf::Vector2<difference_type>
{
    return m_pointed_cell;
}

auto DrawableGrid::setPointedCell(difference_type i, difference_type j) -> void
{
    m_pointed_cell.x = i;
    m_pointed_cell.y = j;
}

/*
sf::RectangleShape const white_box
{[]{
    sf::RectangleShape ret;
    square.setFillColor(sf::Color::White);
    square.setOutlineColor(sf::Color{200, 200, 200});
    square.setOutlineThickness(1);

    return ret;
}()};

sf::RectangleShape const black_box
{[]{
    sf::RectangleShape ret;
    square.setFillColor(sf::Color::Black);
    square.setOutlineColor(sf::Color{200, 200, 200});
    square.setOutlineThickness(1);

    return ret;
}()};

sf::RectangleShape const red_square
{[]{
    sf::RectangleShape ret;
    square.setFillColor(sf::Color::Transparent);
    square.setOutlineColor(sf::Color::Red);
    square.setOutlineThickness(1);

    return ret;
}()};
*/

[[nodiscard]] auto DrawableGrid::getCellSize(sf::RenderTarget &target) const -> sf::Vector2f
{
    return sf::Vector2f
    {
        static_cast<float>(target.getView().getSize().x / this->getCols()),
        static_cast<float>(target.getView().getSize().y / this->getRows())
    };
}

[[nodiscard]] auto DrawableGrid::isPointedCellValid() const -> bool
{
    return
        0 <= m_pointed_cell.x &&
        0 <= m_pointed_cell.y &&
        m_pointed_cell.x < static_cast<difference_type>(this->getRows()) &&
        m_pointed_cell.y < static_cast<difference_type>(this->getCols());
}

auto DrawableGrid::draw(sf::RenderTarget &target, sf::RenderStates states) const -> void
{
    static sf::RectangleShape square{this->getCellSize(target)};
    square.setOutlineColor(sf::Color{200, 200, 200});
    square.setOutlineThickness(1);

    for (size_type i=0; i<this->getRows(); ++i)
    {
        for (size_type j=0; j<this->getCols(); ++j)
        {
            square.setPosition(j * square.getSize().x, i * square.getSize().y);

            if (this->get(i, j) == 0)
                square.setFillColor(sf::Color::White);
            else
                square.setFillColor(sf::Color::Black);

            target.draw(square, states);
        }
    }

    if (this->isPointedCellValid())
    {
        square.setPosition(m_pointed_cell.y * square.getSize().x, m_pointed_cell.x * square.getSize().y);
        square.setFillColor(sf::Color::Transparent);
        square.setOutlineColor(sf::Color::Red);

        target.draw(square, states);
    }
}

}   // namespace gol
