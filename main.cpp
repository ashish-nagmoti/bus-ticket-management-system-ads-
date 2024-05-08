#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Route {
    std::string destination;
    int routeNumber;
};

struct Ticket {
    std::string passengerName;
    int routeNumber;
    int seatNumber;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bus Ticket Management System");

    std::vector<Route> routes;
    std::vector<Ticket> tickets;

    sf::Font font;
    font.loadFromFile("./src/t.ttf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    sf::RectangleShape addRouteButton(sf::Vector2f(150, 50));
    addRouteButton.setFillColor(sf::Color::Green);
    addRouteButton.setPosition(50, 500);

    sf::Text addRouteText;
    addRouteText.setFont(font);
    addRouteText.setCharacterSize(24);
    addRouteText.setFillColor(sf::Color::Black);
    addRouteText.setString("Add Route");
    addRouteText.setPosition(60, 510);

    std::string newRouteName = "";
    sf::Text newRouteNameText;
    newRouteNameText.setFont(font);
    newRouteNameText.setCharacterSize(24);
    newRouteNameText.setFillColor(sf::Color::White);
    newRouteNameText.setPosition(50, 550);

    std::string newPassengerName = "";
    sf::Text newPassengerNameText;
    newPassengerNameText.setFont(font);
    newPassengerNameText.setCharacterSize(24);
    newPassengerNameText.setFillColor(sf::Color::White);
    newPassengerNameText.setPosition(400, 550);

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    window.setView(view);

    float viewPositionX = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                    viewPositionX += event.mouseWheelScroll.delta * 10;
                    view.setCenter(viewPositionX + 400, 300);
                    window.setView(view);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (addRouteButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // Add route functionality
                        if (!newRouteName.empty()) {
                            routes.push_back({newRouteName, static_cast<int>(routes.size()) + 1});
                            newRouteName = "";
                        }
                    } else {
                        // Check if any of the route register buttons are clicked
                        for (size_t i = 0; i < routes.size(); i++) {
                            sf::RectangleShape registerButton(sf::Vector2f(150, 50));
                            registerButton.setFillColor(sf::Color::Blue);
                            registerButton.setPosition(600, 50 + static_cast<float>(i * 100));

                            sf::Text registerText;
                            registerText.setFont(font);
                            registerText.setCharacterSize(24);
                            registerText.setFillColor(sf::Color::Black);
                            registerText.setString("Register");
                            registerText.setPosition(610, 60 + static_cast<float>(i * 100));

                            if (registerButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                // Prompt user for passenger name
                                sf::String inputString;
                                sf::Text inputText;
                                inputText.setFont(font);
                                inputText.setCharacterSize(24);
                                inputText.setFillColor(sf::Color::White);
                                inputText.setPosition(400, 550);

                                bool isEnteringText = true;
                                while (isEnteringText) {
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::TextEntered) {
                                            if (event.text.unicode < 128) {
                                                if (event.text.unicode == '\b' && !newPassengerName.empty()) {
                                                    newPassengerName.pop_back();
                                                } else if (event.text.unicode != '\b') {
                                                    newPassengerName += static_cast<char>(event.text.unicode);
                                                }
                                                inputText.setString(newPassengerName);
                                            }
                                        }
                                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                                            isEnteringText = false;
                                        }
                                    }

                                    window.clear();
                                    // Display other elements
                                    window.draw(inputText);
                                    window.display();
                                }

                                tickets.push_back({newPassengerName, routes[i].routeNumber, static_cast<int>(tickets.size()) + 1});
                                newPassengerName = "";
                            }
                        }
                    }
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == '\b' && !newRouteName.empty()) {
                        newRouteName.pop_back();
                    } else if (event.text.unicode != '\b') {
                        newRouteName += static_cast<char>(event.text.unicode);
                    }
                    newRouteNameText.setString("New Route Name: " + newRouteName);
                }
            }
        }

        window.clear();

        // Display route information
        int y = 50;
        for (const auto& route : routes) {
            text.setString("Route Number: " + std::to_string(route.routeNumber) + "\nDestination: " + route.destination);
            text.setPosition(50, y);
            window.draw(text);

            // Add register button for each route
            for (size_t i = 0; i < routes.size(); i++) {
                sf::RectangleShape registerButton(sf::Vector2f(150, 50));
                registerButton.setFillColor(sf::Color::Blue);
                registerButton.setPosition(600, y);

                sf::Text registerText;
                registerText.setFont(font);
                registerText.setCharacterSize(24);
                registerText.setFillColor(sf::Color::Black);
                registerText.setString("Register");
                registerText.setPosition(610, y + 10);

                window.draw(registerButton);
                window.draw(registerText);

                // Display registered passengers for the current route
                int passengerX = 760;
                for (const auto& ticket : tickets) {
                    if (ticket.routeNumber == route.routeNumber) {
                        text.setString("Passenger: " + ticket.passengerName);
                        text.setPosition(passengerX, y);
                        window.draw(text);
                        y += 30; // Adjust the vertical spacing between passengers
                    }
                }
            }

            y += 100;
        }

       
        window.draw(addRouteButton);
        window.draw(addRouteText);
        window.draw(newRouteNameText);

        window.display();
    }

    return 0;
}