    #include <SFML/Graphics.hpp>
    #include <iostream> 
    #include <string>
    #include <optional> 
    #include <list>
    #include <SFML/System.hpp>
    #include <SFML/Window.hpp>
    #include <map>

    sf::Vector2f castToFloat(sf::Vector2i vector) {
        float newX = static_cast<float>(vector.x);
        float newY = static_cast<float>(vector.y);
        return {newX, newY};
    }

    class Board;

    class Piece {
        public:
        enum class Color {White, Black};
        enum class Type {Pawn, Bishop, Knight, Rook, Queen, King};
        sf::Vector2f visualStartPos;
        sf::Vector2f visualTargetPos;
        bool isAnimating = false;
        int currentFrame;

        protected: 
            Type type;
            Color color;
            sf::Texture texture;
            sf::Sprite sprite;

        public:
            Piece(Color c, Type t) 
            : color(c), type(t),  sprite(texture){}
            virtual ~Piece() = default;

            void update() {
                if(!isAnimating) return;
                int totalFrames = 8;
                
                if(currentFrame <= totalFrames) {
                    sf::Vector2f newPos;
                    float progress = static_cast<float>(currentFrame) / totalFrames;
                    newPos = visualStartPos + (visualTargetPos - visualStartPos) * progress;
                    sprite.setPosition(newPos);
                    currentFrame++;
                } else {
                    sprite.setPosition(visualTargetPos);
                    isAnimating = false;
                    return;
                }
            }

            void startAnimation(sf::Vector2f start, sf::Vector2f target) {
                visualStartPos = start;
                visualTargetPos = target;
                currentFrame = 0;
                isAnimating = true;

            }

            Piece::Type getType() {
                return type;
            }

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

            void updateAnimation(sf::Vector2i to) {
                float posX = static_cast<float>(to.x);
                float posY = static_cast<float>(to.y);
                sprite.setPosition({posX, posY});
            }

            void setSpritePosition(float posX, float posY) {
                sprite.setPosition({posX, posY});
            }
    };

    class Pawn : public Piece {
        private: 
            bool isFirstMove = true;

        public:
            
            explicit Pawn(Color c)
            : Piece(c, Type::Pawn) { //muss noch für alle anderen piece klassen type implementieren 
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
            : Piece(c, Type::Knight) {
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
            : Piece(c, Type::Bishop) {
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
            : Piece(c, Type::Queen) {
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
            bool isChecked;

            explicit King(Color c)
            : Piece(c, Type::King) {
                isChecked = false;
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
                : Piece(c, Type::Rook)
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
            sf::Vector2i whiteKingPos;
            sf::Vector2i blackKingPos;
            float offsetX;
            float offsetY;
            float tileSize;
            bool rightToCastle = true;
            Piece::Color currentTurn = Piece::Color::White;
        
        public:
            
            Board(float x, float y, float tile)
                : offsetX(x), offsetY(y), tileSize(tile) {
                    //WHITE ____________________
                    for(int x = 0; x<8; x++) {
                        setPiece({x,1}, new Pawn(Piece::Color::White));
                    }
                    //alle Pieces
                    setPiece({1,0}, new Knight(Piece::Color::White));
                    setPiece({6,0}, new Knight(Piece::Color::White));

                    setPiece({2,0}, new Bishop(Piece::Color::White));
                    setPiece({5,0}, new Bishop(Piece::Color::White));

                    setPiece({0,0}, new Rook(Piece::Color::White));
                    setPiece({7,0}, new Rook(Piece::Color::White));

                    setPiece({3,0}, new Queen(Piece::Color::White));
                    setPiece({4,0}, new King(Piece::Color::White));
                    whiteKingPos = {4,0};




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
                    blackKingPos = {4,7};
                }
                
                Piece::Color getCurrentTurn() {
                    return currentTurn;
                }

                void switchTurn() {
                    if(currentTurn == Piece::Color::White) {
                        currentTurn = Piece::Color::Black;
                    } else {
                        currentTurn = Piece::Color::White;
                    }
                }               

                void printBoard() { //how its actually STORED, NOT how its RENDERED
                    std::cout << "  0 1 2 3 4 5 6 7" << std::endl; // Spalten-Indizes
                    static std::map<Piece::Type, char> symbols = {
                        {Piece::Type::King, 'K'}, {Piece::Type::Queen, 'Q'}, {Piece::Type::Knight, 'N'},
                        {Piece::Type::Bishop, 'B'}, {Piece::Type::Rook, 'R'}, {Piece::Type::Pawn, 'P'}
                    };

                    for (int y = 0; y < 8; y++) {
                        std::cout << y << " "; // Zeilen-Index
                        for (int x = 0; x < 8; x++) {
                            if (board[y][x] == nullptr) {
                                std::cout << ". "; // Leeres Feld
                            } else {
                                // Hier könntest du später p->getSymbol() nutzen
                                //char pieceSymbol = board[][];
                                std::cout << symbols[board[y][x]->getType()] << " "; // 'P' für Piece (irgendeine Figur)
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
                            piece->setSpritePosition(offsetX + pos.x * tileSize, 
                            offsetY + drawY*tileSize);
                        }
                    }
                }

                King* getKing() { //gives the King who's move it would be --> currentTurn King
                    if(currentTurn == Piece::Color::White) {
                        return dynamic_cast<King*>(board[whiteKingPos.y][whiteKingPos.x]);
                    } else {
                        return dynamic_cast<King*>(board[blackKingPos.y][blackKingPos.x]);
                    }
                }
                
                void movePiece(sf::Vector2i from, sf::Vector2i to) {
                    Piece* currentPiece = board[from.y][from.x];

                    if(currentPiece == nullptr || currentPiece->getColor() != currentTurn) return;

                    if(currentPiece->isValidMove(from,to, *this)){
                        Piece* capturedPiece = board[to.y][to.x]; 
                        
                        sf::Vector2i oldKingPos = (currentTurn == Piece::Color::White) ? whiteKingPos : blackKingPos;

                        board[to.y][to.x] = currentPiece;
                        board[from.y][from.x] = nullptr;

                        if(currentPiece->getType() == Piece::Type::King) {
                            (currentTurn == Piece::Color::White) ? (whiteKingPos = to) : (blackKingPos = to);
                        }

                        if(isKingChecked()) {
                            //move invalid
                            board[from.y][from.x] = currentPiece;
                            board[to.y][to.x] = capturedPiece;

                            if(currentPiece->getType() == Piece::Type::King) {
                                (currentTurn == Piece::Color::White) ? (whiteKingPos = oldKingPos) : (blackKingPos = oldKingPos); 
                            }
                            return;
                        }

                        float newPosX = offsetX + static_cast<float>(to.x) * tileSize;
                        float newPosY = offsetY + static_cast<float>(7-to.y) * tileSize;

                        board[to.y][to.x]->startAnimation(gridToPosition(castToFloat((from))), gridToPosition(castToFloat(to)));

                        if(capturedPiece != nullptr) {
                            //std::cout << "gonna delete";
                            delete capturedPiece; //free up space from dead piece
                          //  std::cout << "even deleted memory";
                        } 
                        //std::cout << "gonna switch turns";

                        
                        switchTurn();
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

                sf::Vector2f gridToPosition(sf::Vector2f gridPos) {
                    sf::Vector2f position;

                    position.x = offsetX + (gridPos.x * tileSize);
                    position.y = offsetY + ((7.f - gridPos.y) * tileSize);

                    return position;
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

                bool isSquareInCheck(sf::Vector2i square) {
                //Rook / Queen steps
                    //X AXIS
                    for(int direction = -1; direction <=2; direction += 2) {
                        for(int i = square.x + direction; 0<=i && i<= 7; i+=(direction)*(1)) { 
                            Piece* foundPiece = board[square.y][i];
                            if(foundPiece != nullptr) {
                                if(foundPiece->getType() == Piece::Type::Queen || foundPiece->getType() == Piece::Type::Rook) {
                                    if(foundPiece->getColor() != currentTurn) {
                                        return true;
                                    }
                                }
                                break;
                            }
                        }
                    }

                    //Y AXIS
                    for(int direction = -1; direction <=2; direction += 2) {
                        for(int i = square.y + direction;0<=i && i<= 7; i+=(direction)*(1)) { 
                            Piece* foundPiece = board[i][square.x];
                            if(foundPiece != nullptr) {
                                if(foundPiece->getType() == Piece::Type::Queen || foundPiece->getType() == Piece::Type::Rook) {
                                    if(foundPiece->getColor() != currentTurn) {
                                        return true;
                                    }
                                }
                                break;
                            }
                        }
                    }

                //bishop / queen // DIAGONAL
                for(int dirX : {-1, 1}) {
                    for(int dirY : {-1,1}) {
                        for(int dist = 1; dist <8 ; dist++) {
                            int tx = square.x + (dirX * dist);
                            int ty = square.y + (dirY*dist);

                            if(tx < 0 || tx > 7 || ty < 0 || ty > 7) break;

                            Piece* foundPiece = board[ty][tx];
                            if(foundPiece != nullptr) {
                                if(foundPiece->getType() == Piece::Type::Queen || foundPiece->getType() == Piece::Type::Bishop) {
                                    if(foundPiece->getColor() != currentTurn) {
                                        return true;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                //knight 
                sf::Vector2i knightMoves[] = {
                    {2,1}, {2,-1}, {-2,1}, {-2,-1},
                    {1,2}, {1,-2}, {-1,2}, {-1,-2}
                };

                for(const auto& move : knightMoves) {
                    int x = square.x + move.x;
                    int y = square.y + move.y; 
                    if( x < 0 || x > 7 || y < 0 || y > 7) continue;
                    Piece* foundPiece = board[y][x];
                    if(foundPiece != nullptr && foundPiece->getType() == Piece::Type::Knight && foundPiece->getColor() != currentTurn) {
                        return true;
                    }
                }

                //PAWN
                int direction = currentTurn == Piece::Color::White ? 1 : -1;
                for(int i : {-1, 1}) {
                    int targetY = square.y + (direction * 1);
                    int targetX = square.x + i;
                    if(targetY < 0 || targetY > 7 || targetX < 0 || targetX > 7) continue;
                    Piece* potentialPawn = board[targetY][targetX];
                    if(potentialPawn != nullptr && potentialPawn->getType() == Piece::Type::Pawn
                        && potentialPawn->getColor()!=currentTurn) {
                        return true;
                    }
                }

                return false;
                }
                
                bool isKingChecked(){
                    Piece* currentKing = getKing();
                    sf::Vector2i KingPos;
                    if(currentKing->getColor() == Piece::Color::White) {
                        KingPos = whiteKingPos;
                    } else {
                        KingPos = blackKingPos;
                    }
                    
                    if(isSquareInCheck(KingPos)) {
                        return true;
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
        int xDiff = to.x - from.x;
        int yDiff = to.y - from.y;
        
        //safety from nullptr exception
        if(to.x < 0 || to.x > 7 || from.y < 0 || from.y > 7) {
            return false;
        }

        Piece* piece = board.getPieceFromGrid(to);
        if( piece != nullptr && piece->getColor() == this->color) {
            return false;
        }

        if((std::abs(xDiff) == 1 && std::abs(yDiff) == 2 ) || std::abs(xDiff) == 2 && std::abs(yDiff) == 1 ) {
            return true;
        }

        return false;

        
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
            int xDiff = to.x - from.x;
            xDiff = std::abs(xDiff);
            int yDiff = to.y - from.y;
            yDiff = std::abs(yDiff);

            Piece* piece = board.getPieceFromGrid(to);
                if( piece != nullptr && piece->getColor() == this->color) {
                    return false;
                }

            if(( xDiff == 1 || yDiff == 1 || xDiff + yDiff == 2 )&& xDiff <2 && yDiff < 2) {
                return true;
            }
            return false;
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

        //create chess board
        float tileSize = static_cast<float>(screenHeight / 8.f);
        float boardWidth = tileSize * 8.f;
        float posX = static_cast<float>(screenWidth - boardWidth) / 2.f;

        std::vector<sf::RectangleShape> boardSquares;
        sf::Color lightColor(240, 238, 210);
        sf::Color darkColor(119, 150, 86);

        for(int y = 0; y < 8; y++) {
            for(int x = 0; x<8; x++) {
                sf::RectangleShape square({tileSize, tileSize});

                square.setPosition({posX + x * tileSize, y * tileSize});

                if((x+y) % 2 == 0) {
                    square.setFillColor(lightColor);
                } else {
                    square.setFillColor(darkColor);
                }
                boardSquares.push_back(square);
            }
        }

        //cursor text
        sf::Font font;
        if (!font.openFromFile("src/fonts/arial.ttf")) {
        std::cout << "couldnt find font" << std::endl;
        return -1;
        }
        std::string cursorField = "";


        sf::Text cursorText(font);
        cursorText.setString("White's Turn");
        cursorText.setCharacterSize(40);
        cursorText.setFillColor(sf::Color::White);
        cursorText.setStyle(sf::Text::Bold);
        cursorText.setPosition({5.f,5.f});

        //Highlight Rectangle
        sf::RectangleShape highlightRec;
        highlightRec.setFillColor(sf::Color(255, 0, 0, 0)); //last value is alpha 0-255

        //MOVE PIECE FROM HIGHLIGHT
        sf::RectangleShape originSquareHighlight;
        originSquareHighlight.setFillColor(sf::Color(50,75,200, 200));
        originSquareHighlight.setSize({tileSize, tileSize});

        //MOVE PIECE TO HIGHLIGHT
        sf::RectangleShape targetSquareHighlight;
        targetSquareHighlight.setFillColor(sf::Color(50,75,200, 150));
        targetSquareHighlight.setSize({tileSize,tileSize});
        
        sf::Vector2i lastMouseClick = {0,0};
        bool showHighlightRec = false;
        std::list<sf::Vector2i> highlightRecPositions; 

        Board board(posX, 0, tileSize);

        bool isPieceSelected = false;
        sf::Vector2i selectedGridPos;

        //possible Moves
        std::vector<sf::CircleShape> possibleMoves;


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
                        snappedPos.x = static_cast<float>(((lastMouseClick.x)/tileSize) * tileSize);
                        snappedPos.y = static_cast<float>(((lastMouseClick.y)/tileSize) * tileSize);

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
                    
                        sf::Vector2i clickedGridPos = board.mouseToGrid(mousePos);

                        if (!isPieceSelected ) {
                            possibleMoves.clear();
                            Piece* clickedPiece = board.getPieceFromGrid(clickedGridPos);
                            if (clickedPiece != nullptr && clickedPiece->getColor() == board.getCurrentTurn()) {
                                isPieceSelected = true;
                                selectedGridPos = clickedGridPos;

                                for(int y = 0; y<8; y++) {
                                    for(int x = 0; x<8;x++) {
                                        if(clickedPiece->isValidMove(selectedGridPos, {x,y}, board)) {
                                            sf::CircleShape circle;
                                            float circleRadius = tileSize / 6.f;
                                            float centerOffset = (tileSize / 2) - circleRadius;
                                            circle.setPosition({posX + (x * tileSize) + centerOffset, (7-y) * tileSize + centerOffset});
                                            circle.setFillColor(sf::Color(30,30,30,100));
                                            circle.setRadius(circleRadius);

                                            possibleMoves.push_back(circle);
                                        } 
                                    }
                                }

                                lastMouseClick = sf::Mouse::getPosition(window);
                                sf::Vector2f snappedPos;
                                snappedPos.x = static_cast<float>(((lastMouseClick.x)/100) * 100);
                                snappedPos.y = static_cast<float>(((lastMouseClick.y)/100) * 100); //should be tilesize but doesnt align

                                originSquareHighlight.setPosition({snappedPos.x, snappedPos.y});
                                // originSquareHighlight.setFillColor()
                            }
                        } else {
                            Piece* targetPiece = board.getPieceFromGrid(clickedGridPos);
                            if (targetPiece != nullptr && targetPiece->getColor() == board.getCurrentTurn()) {
                                selectedGridPos = clickedGridPos;
                                sf::Vector2f snappedPos;
                                snappedPos.x = static_cast<float>(((selectedGridPos.x)/tileSize) * tileSize);
                                snappedPos.y = static_cast<float>(((selectedGridPos.y)/tileSize) * tileSize);

                                originSquareHighlight.setPosition({snappedPos.x+1, snappedPos.y+1});                                
                            } else {
                                board.movePiece(selectedGridPos, clickedGridPos); //movePiece then checks for valid move
                                sf::Vector2f floatGridPos;
                                floatGridPos.x = static_cast<float>(clickedGridPos.x);
                                floatGridPos.y = static_cast<float>(clickedGridPos.y);

                                sf::Vector2f targetPosition = board.gridToPosition(floatGridPos);

                                targetSquareHighlight.setPosition(targetPosition);
                                //std::cout << targetPosition.y << " ist y und " << targetPosition.x << "ist x" << std::endl; 
                                board.printBoard();
                            }
                            possibleMoves.clear();
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
                } else if(mouseWorld.x> posX + boardWidth) { //idk why 18 but it works
                    mouseWorld.x = posX + boardWidth;
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

            int mouseYIndex = 8-(int)mY / tileSize ;
            int mouseXIndex = (int)mX / tileSize;
            char mouseXField = 'A' + mouseXIndex;

            bool mouseInBounds = !(mY >= screenHeight || mY <= 0 || mouseXIndex < 0 || mouseXIndex > 7);

            /*
            if(!mouseInBounds) {
                cursorText.setString("Mouse out of bounds");
            } else {
                cursorText.setString(mouseXField + std::to_string((int)mouseYIndex));
            }*/

            cursorText.setString((board.getCurrentTurn() == Piece::Color::White ) ? "White's \n Turn" : "Black's \n turn");

            highlightRec.setSize({tileSize, tileSize});    

            //*RENDER_______________________________________________________________________________________
            if(board.isKingChecked()) {
                window.clear(sf::Color(150,20,20));
            } else {
                if(board.getCurrentTurn() == Piece::Color::White) {
                    window.clear(sf::Color(100, 100, 100));
                } else {
                    window.clear(sf::Color(30, 30, 30));
                }
            }
            
            
            for (const auto& square : boardSquares){
                window.draw(square);
            }

            for(const auto& circle : possibleMoves) {
                window.draw(circle);
            }

            if(showHighlightRec) {
                if(!highlightRecPositions.empty()) {
                    for(const sf::Vector2i mousePos : highlightRecPositions) {
                        
                        highlightRec.setFillColor(sf::Color(255, 0, 0, 128));

                        highlightRec.setPosition({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
                        window.draw(highlightRec);
                    }
                }
            }

            window.draw(originSquareHighlight);
            window.draw(targetSquareHighlight);

            // Alle Figuren auf dem Brett zeichnen
            for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 8; x++) {
                    Piece* piece = board.getPieceFromGrid({x,y});
                    if(piece) {
                        piece->update();
                        piece->draw(window);
                    }
                }
            }

            window.draw(cursorText);
            window.display(); //show window
            }
        return 0;    
}