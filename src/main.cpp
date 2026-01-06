    #include <SFML/Graphics.hpp>
    #include <iostream> 
    #include <string>
    #include <optional> 
    #include <list>
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
        std::list<sf::Vector2i> highlightRecPositions; //SHOULD BE MORE (64 tiles on board)  

        //EVENTS
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                //RIGHT click
                if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseClick->button == sf::Mouse::Button::Right) {
                        lastMouseClick = sf::Mouse::getPosition(window);
                        bool currentMouseClickInList = false;
                        sf::Vector2i snappedPos;
                        snappedPos.x = static_cast<float>(((lastMouseClick.x)/100) * 100);
                        snappedPos.y = static_cast<float>(((lastMouseClick.y)/100) * 100);

                        bool alreadyExists = false;
                        auto hBegin = highlightRecPositions.begin();
                        auto hEnd = highlightRecPositions.end();
                        for( auto it = hBegin; it != hEnd; ++it) {
                            if(*it == snappedPos) {
                                highlightRecPositions.erase(it);
                                alreadyExists = true;
                                break;
                            }
                        }
                        if(!alreadyExists) {
                                highlightRecPositions.push_back(snappedPos);    
                        }
                        showHighlightRec = !(highlightRecPositions.empty());
                    }

                    //LEFT click
                    if (mouseClick->button == sf::Mouse::Button::Left) {
                        if(showHighlightRec) {
                            showHighlightRec = false;
                            highlightRecPositions.clear();
                            //std::cout << highlightRecPositions.front().x <<std::endl;
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

            bool mouseInBounds = !(mY >= screenHeight || mY <= 0 || mouseXIndex < 0 || mouseXIndex > 7);

            if(!mouseInBounds) {
                cursorText.setString("Mouse out of bounds");
            } else {
                cursorText.setString(mouseXField + std::to_string((int)mouseYIndex));
            }

            highlightRec.setSize({101,101});       

            //RENDER
            window.clear(sf::Color(45, 45, 45));

            window.draw(sprite);
            window.draw(cursorText);

            if(showHighlightRec) {
                if(!highlightRecPositions.empty()) {
                    for(const sf::Vector2i mousePos : highlightRecPositions) {
                        
                        highlightRec.setFillColor(sf::Color(255, 0, 0, 128));

                        highlightRec.setPosition({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
                        window.draw(highlightRec);
                    }
                }
            }
            window.display(); //show window
            }
        return 0;    
}