#include "DrawableGrid.hpp"

#include <boost/program_options.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

auto parseCommandLine(int const argc, char const *argv[]) -> po::variables_map
{
    po::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "Show this information")
        ("grid-width", po::value<unsigned>()->default_value(40),
            "Width dimension of the grid")
        ("grid-height", po::value<unsigned>()->default_value(75),
            "Height dimension of the grid")
        ("window-width", po::value<unsigned>()->default_value(900),
            "Width of the window")
        ("window-height", po::value<unsigned>()->default_value(480),
            "Height of the window")
    ;

    /*po::options_description hidden("Opciones ocultas");
    hidden.add_options()
        ("input-file", po::value<std::string>(), "Archivos de entrada")
    ;*/

    po::options_description cmdline_options;
        cmdline_options.add(generic)/*.add(hidden)*/;

    /*po::options_description visible("Opciones permitidas");
        visible.add(generic);

    po::positional_options_description pd;
    pd.add("input-file", -1);
    */

    po::variables_map ret;
    po::store(po::command_line_parser(argc, argv).
                options(cmdline_options)/*.positional(pd)*/.run(), ret);

    if (ret.count("help") > 0) {
        std::clog << generic << "\n";
        return 0;
    }

    return ret;
}

auto increaseSpeed(sf::Time &period) -> void
{
    if (period.asMilliseconds() > 1)
    {
        period -= sf::milliseconds(1);
        spdlog::info("Period increased to {}ms", period.asMilliseconds());
    }
}

auto decreaseSpeed(sf::Time &period) -> void
{
    if (period.asMilliseconds() < 500)
    {
        period += sf::milliseconds(1);
        spdlog::info("Period decreased to {}ms", period.asMilliseconds());
    }
}

auto main(int const argc, char const *argv[]) -> int try
{
    auto vm = parseCommandLine(argc, argv);

    sf::RenderWindow window
    (
        sf::VideoMode(
            vm["window-width"].as<unsigned>(),
            vm["window-height"].as<unsigned>()
        ),
        "Game of Life (not running)"
    );

    gol::DrawableGrid grid
    {
        vm["grid-width"].as<unsigned>(),
        vm["grid-height"].as<unsigned>()
    };

    bool running = false;
    sf::Clock timer;
    sf::Time period = sf::milliseconds(70);

    while (window.isOpen())
    {
        sf::Event event{};

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Up:
                            increaseSpeed(period);
                            break;
                        case sf::Keyboard::Down:
                            decreaseSpeed(period);
                            break;
                        case sf::Keyboard::Right:
                            if (!running)
                                grid.update();
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::C:
                            grid.clear();
                            running = false;
                            window.setTitle("Game of Life (not running)");
                            break;
                        case sf::Keyboard::Q:
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                        case sf::Keyboard::S:
                            running = !running;

                            if (running)
                                window.setTitle("Game of Life (running)");
                            else
                                window.setTitle("Game of Life (not running)");

                            break;
                        default:
                            break;
                    }

                    break;
                case sf::Event::MouseButtonReleased:
                    if (0 <= event.mouseButton.x &&
                        0 <= event.mouseButton.y &&
                        event.mouseButton.x < static_cast<int>(window.getSize().x) &&
                        event.mouseButton.y < static_cast<int>(window.getSize().y) &&
                        event.mouseButton.button == sf::Mouse::Left)
                    {
                        grid.flipCell
                        (
                            (event.mouseButton.y * grid.getRows()) / window.getSize().y,
                            (event.mouseButton.x * grid.getCols()) / window.getSize().x
                        );
                    }
                    break;
                default:
                    break;
            }
        }

        if (running && timer.getElapsedTime() > period)
        {
            grid.update();
            timer.restart();
        }

        grid.setPointedCell
        (
            (sf::Mouse::getPosition(window).y * grid.getRows()) / window.getSize().y,
            (sf::Mouse::getPosition(window).x * grid.getCols()) / window.getSize().x
        );

        window.clear();
        window.draw(grid);
        window.display();
    }
}
// Error handling for command line parsing
catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
}
catch(...) {
    std::cerr << "Exception of unknown type!\n";
    return 1;
}
