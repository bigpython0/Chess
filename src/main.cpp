    #include <SFML/Graphics.hpp>
    #include <iostream> 
    #include <string>
    #include <optional> 
    #include <list>
    #include <SFML/System.hpp>
    #include <SFML/Window.hpp>

    class Board;

    class Piece {
        public:
        enum class Color {White, Black};

        protected: 
            Color color;
            sf::Texture texture;
            sf::Sprite sprite;

        public:
            Piece(Color c) 
            : color(c),  sprite(texture){}
            virtual ~Piece() = default;

            virtual bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board) {
                return true;
            }

            Piece::Color getColor() {
                return this->color;
            }

            void scaleToFit(float tileSize) {
                float scaleX = tileSize / sprite.getGlobalBounds().size.x;
                float scaleY = tileSize / sprite.getGlobalBounds().size.y;
                sprite.setScale({scaleX, scaleY});
            }

            void draw(sf::RenderWindow& window) {
                window.draw(sprite);
            }

            void updateSpritePosition(float posX, float posY) {
                sprite.setPosition({posX, posY});
            }
    };

    class Pawn : public Piece {
        private: 
            bool isFirstMove = true;

        public:
            explicit Pawn(Color c)
            : Piece(c) {
            if(c == Color::White) {
                if(texture.loadFromFile("src/images/pieces/white-pawn.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load white pawn texture" << std::endl;
                        }
            } else {
                if(texture.loadFromFile("src/images/pieces/black-pawn.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black pawn texture" << std::endl;
                        }
                }
            }
            bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board &board) override;
    };

    class Knight : public Piece {
        public:
            explicit Knight(Color c)
            : Piece(c) {
            if(c == Color::White) {
                if(texture.loadFromFile("src/images/pieces/white-knight.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load white pawn texture" << std::endl;
                        }
            } else {
                if(texture.loadFromFile("src/images/pieces/black-knight.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black pawn texture" << std::endl;
                        }
            }
        }

        bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board &board) override;
    };

    class Bishop : public Piece {
        public:
            explicit Bishop(Color c)
            : Piece(c) {
            if(c == Color::White) {
                if(texture.loadFromFile("src/images/pieces/white-bishop.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load white pawn texture" << std::endl;
                        }
            } else {
                if(texture.loadFromFile("src/images/pieces/black-bishop.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black pawn texture" << std::endl;
                        }
            }
        }

        bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board &board) override;
    };

    class Queen : public Piece {
        public:
            explicit Queen(Color c)
            : Piece(c) {
            if(c == Color::White) {
                if(texture.loadFromFile("src/images/pieces/white-queen.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load white pawn texture" << std::endl;
                        }
            } else {
                if(texture.loadFromFile("src/images/pieces/black-queen.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black pawn texture" << std::endl;
                        }
            }
        }

        bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board &board) override;
    };

    class King : public Piece {
        public:
            explicit King(Color c)
            : Piece(c) {
            if(c == Color::White) {
                if(texture.loadFromFile("src/images/pieces/white-king.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load white pawn texture" << std::endl;
                        }
            } else {
                if(texture.loadFromFile("src/images/pieces/black-king.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black pawn texture" << std::endl;
                        }
            }
        }

        bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board &board) override;
    };

    class Rook : public Piece {
        public:
            explicit Rook(Color c) 
                : Piece(c)
                {
                    if(c == Color::White) {
                        if(texture.loadFromFile("src/images/pieces/white-rook.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black rook texture" << std::endl;
                        }
                    } else {
                        if(texture.loadFromFile("src/images/pieces/black-rook.png")) {
                            sprite.setTexture(texture, true);
                        } else {
                            std::cout << "couldnt load black rook texture" << std::endl;
                        }
                    }
                } 

            bool isValidMove(sf::Vector2i from, sf::Vector2i to, Board &board) override;
    };

        class Board {
            private:
                Piece* board[8][8] = { nullptr }; 
                float offsetX;
                float offsetY;
                float tileSize;
            
            public:
                
                Board(float x, float y, float tile)
                    : offsetX(x), offsetY(y), tileSize(tile) {
                        //WHITE ____________________
                        for(int x = 0; x<8; x++) {
                            setPiece({x,1}, new Pawn(Piece::Color::White));
                        }
                        setPiece({1,0}, new Knight(Piece::Color::White));
                        setPiece({6,0}, new Knight(Piece::Color::White));

                        setPiece({2,0}, new Bishop(Piece::Color::White));
                        setPiece({5,0}, new Bishop(Piece::Color::White));

                        setPiece({0,0}, new Rook(Piece::Color::White));
                        setPiece({7,0}, new Rook(Piece::Color::White));

                        setPiece({3,0}, new Queen(Piece::Color::White));
                        setPiece({4,0}, new King(Piece::Color::White));



                        //BLACK____________________
                        for(int x = 0; x<8; x++) {
                            setPiece({x,6}, new Pawn(Piece::Color::Black));
                        }
                        setPiece({1,7}, new Knight(Piece::Color::Black));
                        setPiece({6,7}, new Knight(Piece::Color::Black));

                        setPiece({2,7}, new Bishop(Piece::Color::Black));
                        setPiece({5,7}, new Bishop(Piece::Color::Black));

                        setPiece({0,7}, new Rook(Piece::Color::Black));
                        setPiece({7,7}, new Rook(Piece::Color::Black));

                        setPiece({3,7}, new Queen(Piece::Color::Black));
                        setPiece({4,7}, new King(Piece::Color::Black));

                        
                    }                    

                    void printBoard() {
                        std::cout << "  0 1 2 3 4 5 6 7" << std::endl; // Spalten-Indizes
                        for (int y = 0; y < 8; y++) {
                            std::cout << y << " "; // Zeilen-Index
                            for (int x = 0; x < 8; x++) {
                                if (board[y][x] == nullptr) {
                                    std::cout << ". "; // Leeres Feld
                                } else {
                                    // Hier könntest du später p->getSymbol() nutzen
                                    std::cout << "P "; // 'P' für Piece (irgendeine Figur)
                                }
                            }
                            std::cout << std::endl;
                        }
                        std::cout << "---------------" << std::endl;
                    }
                                        
                    void setPiece(sf::Vector2i pos, Piece* piece) {
                        if(pos.x >=0 && pos.y>=0 && pos.x<8 && pos.y <8){
                            if(board[pos.y][pos.x] != nullptr){
                            delete board[pos.y][pos.x];
                        }
                            board[pos.y][pos.x] = piece;
                            if(piece) {
                                piece->scaleToFit(tileSize);
                                int drawY = 7-pos.y;
                                piece->updateSpritePosition(offsetX + pos.x * tileSize, 
                                offsetY + drawY*tileSize);
                            }
                        }
                        
                    }

                void movePiece(sf::Vector2i from, sf::Vector2i to) {
                    if(board[from.y][from.x]->isValidMove(from,to, *this)){

                        board[to.y][to.x] = board[from.y][from.x];
                        board[from.y][from.x] = nullptr;

                        float newPosX = offsetX + static_cast<float>(to.x) * tileSize;
                        float newPosY = offsetY + static_cast<float>(7-to.y) * tileSize;

                        board[to.y][to.x]->updateSpritePosition(newPosX, newPosY); 
                    }
                }

                Piece* getPieceFromMouse(sf::Vector2i pos) { //input is raw mouse pos, not converted yet
                    sf::Vector2i convertedPos = mouseToGrid(pos);
                    return board[convertedPos.y][convertedPos.x];
                }

                Piece* getPieceFromGrid(sf::Vector2i pos) { //input is board pos, already converted
                    return board[pos.y][pos.x];
                }

                sf::Vector2i mouseToGrid(sf::Vector2i mP) {
                    int x = (mP.x - offsetX) / tileSize;
                    int y = (mP.y - offsetY) / tileSize;

                    if(x<0) x=0; else if (x>7) x=7;
                    if(y<0) y=0; else if (y>7) y=7;

                    y = 7-y;

                    return sf::Vector2i(x,y);
                }

                bool isPathBlocked(sf::Vector2i from, sf::Vector2i to) { //PATH BLOCKED = TRUE
                    int xDiff = to.x - from.x;
                    int yDiff = to.y - from.y;

                    int stepX = (xDiff == 0) ? 0 : (xDiff > 0 ? 1 : -1);
                    int stepY = (yDiff == 0) ? 0 : (yDiff > 0 ? 1 : -1);

                    int currX = from.x + stepX;
                    int currY = from.y + stepY;

                    while(currX != to.x || currY != to.y) {
                        if(board[currY][currX] != nullptr) {
                            return true;
                        }

                        currX += stepX;
                        currY += stepY;
                    }
                    return false;
                }
        };


        bool Pawn::isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board){  
                int direction = (color == Piece::Color::White) ? 1 : -1;
                int yDiff = to.y - from.y;
                int xDiff = from.x-to.x;
                
                //normal move / first move
                if (xDiff == 0) {
                    if(isFirstMove) {
                        if(board.getPieceFromGrid(to) == nullptr && !(board.isPathBlocked(from, to))){
                            if(yDiff==2*direction || yDiff == 1 * direction) {
                                isFirstMove = false;
                                return true; 
                            }   
                        }
                    } else if (yDiff == (1*direction) && board.getPieceFromGrid(to) == nullptr){  
                        return true;
                    }
                    //attack
                } 
                else if (std::abs(xDiff) == 1 && yDiff == direction) {
                    Piece* target = board.getPieceFromGrid(to);
                    // Es MUSS ein Gegner dort stehen
                    if (target != nullptr && target->getColor() != this->color) {
                        isFirstMove = false;
                        return true;
                    }
                }
                
                 
                
                return false;
            };

        bool Knight::isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board){
            
        }

        bool Bishop::isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board){
            int xDiff = to.x - from.x;
            int yDiff = to.y - from.y;

            if(!(std::abs(xDiff) == std::abs(yDiff))) {
                return false;
            }
            
            Piece* targetPiece = board.getPieceFromGrid(to);
            if( !board.isPathBlocked(from, to)) {
                return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
            }
            return false;
        }
        bool Queen::isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board){
            int xDiff = to.x - from.x;
            int yDiff = to.y - from.y;

            if(((xDiff == 0 && std::abs(yDiff) > 0 )|| (yDiff == 0 && std::abs(xDiff) > 0))) {
                    if( !board.isPathBlocked(from, to)) {
                        Piece* targetPiece = board.getPieceFromGrid(to);
                        return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
                    }
            }

            if(!(std::abs(xDiff) == std::abs(yDiff))) {
                return false;
            }
            
            Piece* targetPiece = board.getPieceFromGrid(to);
            if( !board.isPathBlocked(from, to)) {
                return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
            }
            return false;
        }

        bool King::isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board){
                return true;
        }

        bool Rook::isValidMove(sf::Vector2i from, sf::Vector2i to, Board& board){
            int xDiff = to.x - from.x;
            int yDiff = to.y - from.y;

            if(((xDiff == 0 && std::abs(yDiff) > 0 )|| (yDiff == 0 && std::abs(xDiff) > 0))) {
                    if( !board.isPathBlocked(from, to)) {
                        Piece* targetPiece = board.getPieceFromGrid(to);
                        return (targetPiece != nullptr) ? targetPiece->getColor() != color : true;
                    }
            }
            return false;

        }

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
        if (!texture.loadFromFile("src/images/chessboard.png")) {
            std::cout << "couldnt load image" << std::endl;
            return -1;
        }

        //_____________SPRITE creation (chessboaed?)_____________
        sf::Sprite boardSprite(texture);
        sf::FloatRect bounds = boardSprite.getLocalBounds(); 
        sf::FloatRect globalBounds = boardSprite.getGlobalBounds(); 
        float rightBound = bounds.position.x + bounds.size.x;
        float factor = static_cast<float>(screenHeight) / bounds.size.y;
        boardSprite.setScale({factor, factor});
        float posX = (static_cast<float>(screenWidth) - boardSprite.getGlobalBounds().size.x) / 2.f;
        boardSprite.setPosition({posX, 0.f});

        //cursor text
        sf::Font font;
        if (!font.openFromFile("src/fonts/arial.ttf")) {
        std::cout << "couldnt find font" << std::endl;
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

        Board board(posX, 0, boardSprite.getGlobalBounds().size.x/8.f);

        bool isPieceSelected = false;
        sf::Vector2i selectedGridPos;

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
                        //HIGHLIGHT START
                        if(showHighlightRec) {
                            showHighlightRec = false;
                            highlightRecPositions.clear();
                            //std::cout << highlightRecPositions.front().x <<std::endl;
                        }
                        //HIGHLIGHT END

                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                        // Wir brauchen hier eine neue Funktion im Board (siehe Punkt 3)
                        sf::Vector2i clickedGridPos = board.mouseToGrid(mousePos);

                        if (!isPieceSelected) {
                            if (board.getPieceFromGrid(clickedGridPos) != nullptr) {
                                isPieceSelected = true;
                                selectedGridPos = clickedGridPos;
                            }
                        } else {
                            if (selectedGridPos != clickedGridPos) {
                                board.movePiece(selectedGridPos, clickedGridPos);
                                board.printBoard();
                            }
                            isPieceSelected = false; 
                        }

                    }
            
                }
        }

            
            //Cursor text / position
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

                // set mouse Boundaries
                //mouseWorld.x - posX;
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

            window.draw(boardSprite);

            if(showHighlightRec) {
                if(!highlightRecPositions.empty()) {
                    for(const sf::Vector2i mousePos : highlightRecPositions) {
                        
                        highlightRec.setFillColor(sf::Color(255, 0, 0, 128));

                        highlightRec.setPosition({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
                        window.draw(highlightRec);
                    }
                }
            }

            // Alle Figuren auf dem Brett zeichnen
            for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 8; x++) {
                    Piece* piece = board.getPieceFromGrid({x,y});
                    if(piece != nullptr) {
                        piece->draw(window);
                    }
                }
            }

            window.draw(cursorText);
            window.display(); //show window
            }
        return 0;    
}