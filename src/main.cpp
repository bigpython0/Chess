    #include <SFML/Graphics.hpp>
    #include <iostream> 
    #include <string>
    #include <optional> 
    #include "SFML\System.hpp"
    #include "SFML\Window.hpp"


    int main() {
        //init game

        unsigned int screenWidth = 1200;
        unsigned int screenHeight = 800;
        sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "Chess - SFML 3");
        window.setFramerateLimit(60);

        //calculate sprite size
        float chessboardPngSize = 1028.f;
        float spriteFactor = (static_cast<float>(screenHeight)/chessboardPngSize)-0.2f;

        //insert chessboard
        sf::Texture texture;
        if (!texture.loadFromFile("images/chessboard.png")) {
            std::cout << "couldnt load image";
            return -1;
        }

        //_____________SPRITE creation
        sf::Sprite sprite(texture);
        sf::FloatRect bounds = sprite.getLocalBounds(); 
        sf::FloatRect globalBounds = sprite.getGlobalBounds(); 
        float rightBound = bounds.position.x + bounds.size.x;
        float factor = static_cast<float>(screenHeight) / bounds.size.y;
        sprite.setScale({factor, factor});
        float posX = (static_cast<float>(screenWidth) - sprite.getGlobalBounds().size.x) / 2.f;
        sprite.setPosition({posX, 0.f});

        //cursor text
        sf::Font font;
        if (!font.openFromFile("C:/Coding_Projects/c++/Chess/fonts/arial.ttf")) {
        std::cout << "couldnt find font";
        return -1;
        }
        std::string cursorField = "";


        sf::Text cursorText(font);
        cursorText.setString(cursorField);
        cursorText.setCharacterSize(40);
        cursorText.setFillColor(sf::Color::White);
        cursorText.setStyle(sf::Text::Bold);
        cursorText.setPosition({5.f,5.f});

        //Highlight Rectangle
        sf::RectangleShape highlightRec;
        highlightRec.setFillColor(sf::Color(255, 0, 0, 0)); //last value is alpha 0-255
        
        sf::Vector2i lastMouseClick = {0,0};
        bool showHighlightRec = false;

        //EVENTS
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseClick->button == sf::Mouse::Button::Right) {
                        if(showHighlightRec) {
                            showHighlightRec = false;
                        } else {
                            lastMouseClick = sf::Mouse::getPosition(window);
                            showHighlightRec = true;
                        }
                    }
                
                }
            }
            

            //Cursor text / position
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

                // set mouse Boundaries
                mouseWorld.x - posX;
                if(mouseWorld.x<posX) {
                    mouseWorld.x = 0;
                } else if(mouseWorld.x>rightBound-posX-18) { //idk why 18 but it works
                    mouseWorld.x = rightBound-posX-18;
                }

                if(mouseWorld.y<0) {
                    mouseWorld.y = 0;
                } else if(mouseWorld.y>screenHeight) {
                    mouseWorld.y = screenHeight;
                }
            
            /*    cursorText.setString("X: " + std::to_string((int)mouseWorld.x) + 
                                "Y: " + std::to_string((int)mouseWorld.y));
                                */
                                
            //convert mouse cords to field position
            int mX = mouseWorld.x; //just to make it simpler
            int mY = mouseWorld.y;

            int mouseYIndex = 8-(int)mY / 100 ;
            int mouseXIndex = (int)mX / 100 - 2;
            char mouseXField = 'A' + mouseXIndex;

            if(mY >= screenHeight || mY <= 0 || mouseXIndex < 0 || mouseXIndex > 7) {
                cursorText.setString("Mouse out of bounds");
            } else {
                cursorText.setString(mouseXField + std::to_string((int)mouseYIndex));
            }

            if(mY >= screenHeight || mY <= 0 || mouseXIndex < 0 || mouseXIndex > 7) {
                highlightRec.setPosition({2000.f, 2000.f}); // outside of window
            } else {
                float snapX = static_cast<float>(((lastMouseClick.x)/100) * 100);
                float snapY = static_cast<float>(((lastMouseClick.y)/100) * 100);
                highlightRec.setPosition({snapX, snapY});

                if(showHighlightRec) {
                    highlightRec.setFillColor(sf::Color(255, 0, 0, 128));
                } else {
                    highlightRec.setFillColor(sf::Color(255, 0, 0, 0));
                }
            }
            highlightRec.setSize({101,101});       

            //RENDER
            window.clear(sf::Color(45, 45, 45));

            window.draw(sprite);
            window.draw(cursorText);
            window.draw(highlightRec);

            highlightRec.setFillColor(sf::Color::Blue);
            highlightRec.setPosition({200,200});
            window.draw(highlightRec);



            window.display(); //show window
        }
        return 0;
}