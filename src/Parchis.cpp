#include "Parchis.h"

const vector<int> Parchis::safe_boxes = {4, 13, 17, 21, 30, 34, 38, 47, 51, 55, 64, 68};

const vector<color> Parchis::game_colors = vector<color>{yellow, blue, red, green};

const map<color, int> Parchis::final_boxes = {
    {yellow, final_yellow_box},
    {blue, final_blue_box},
    {red, final_red_box},
    {green, final_green_box}
};

const map<color, int> Parchis::init_boxes = {
    {yellow, init_yellow_box},
    {blue, init_blue_box},
    {red, init_red_box},
    {green, init_green_box}
};

Parchis::Parchis()
{
    this->board = Board();
    this->dice = Dice();

    this->last_dice = -1;

    this->current_player = 0;
    this->current_color = yellow;
    GUIPlayer *p1 = new GUIPlayer("Player 1"), *p2 = new GUIPlayer("Player 2");
    players.push_back(p1);
    players.push_back(p2);
    // last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

Parchis::Parchis(const BoardConfig & b){
    this->board = Board(b);
    this->dice = Dice();

    this->last_dice = -1;

    this->current_player = 0;
    this->current_color = yellow;
    //GUIPlayer *p1 = new GUIPlayer("Player 1"), *p2 = new GUIPlayer("Player 2");
    GUIPlayer *p1 = new GUIPlayer("Player 1");
    AIPlayer *p2 = new AIPlayer("Player 2");
    players.push_back(p1);
    players.push_back(p2);
    // last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

Parchis::Parchis(const Board & b, const Dice & d){
    this->board = board;
    this->dice = dice;

    this->last_dice = -1;
    this->current_player = 0;
    this->current_color = yellow;
    GUIPlayer p1("Player 1"), p2("Player 2");
    players.push_back(&p1);
    players.push_back(&p2);
    //players = {&p1, &p2};
    // last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

Parchis::Parchis(const BoardConfig &b, const Dice &d)
{
    this->board = Board(b);
    this->dice = dice;

    this->last_dice = -1;
    this->current_player = 0;
    this->current_color = yellow;
    GUIPlayer p1("Player 1"), p2("Player 2");
    players.push_back(&p1);
    players.push_back(&p2);
    // players = {&p1, &p2};
    //  last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

Parchis::Parchis(const Board & b, const Dice & d, Player & p1, Player & p2){
    this->board = board;
    this->dice = dice;

    this->last_dice = -1;
    this->current_player = 0;
    this->current_color = yellow;
    players.push_back(&p1);
    players.push_back(&p2);// = {&p1, &p2};
    // last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

Parchis::Parchis(const BoardConfig &b, const Dice &d, Player &p1, Player &p2)
{
    this->board = Board(b);
    this->dice = dice;

    this->last_dice = -1;
    this->current_player = 0;
    this->current_color = yellow;
    players.push_back(&p1);
    players.push_back(&p2); // = {&p1, &p2};
    // last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

Parchis::Parchis(const BoardConfig &b, Player &p1, Player &p2){
    this->board = Board(b);
    this->dice = Dice();

    this->last_dice = -1;
    this->current_player = 0;
    this->current_color = yellow;
    players.push_back(&p1);
    players.push_back(&p2); // = {&p1, &p2};
    // last_moves vacío por defecto.

    this->illegal_move_player = -1;
    this->goal_move = false;
    this->eating_move = false;
    this->goal_bounce = false;

    this->turn = 1;
}

bool Parchis::operator==(const Parchis & parchis) const{
    return this->board == parchis.board && this->turn == parchis.turn;
}

const Dice & Parchis::getDice() const{
    return this->dice;
}

const Board & Parchis::getBoard() const{
    return this->board;
}

const color Parchis::isWall(const Box & b) const{
    if(b.type == home || b.type == goal) return none;

    const vector<pair <color, int>> occupation = boxState(b);
    if (occupation.size() == 2 && occupation.at(0).first == occupation.at(1).first){
        return  occupation.at(0).first;
    }else{
        return none;
    }
}

const vector<color> Parchis::anyWall(const Box & b1, const Box & b2) const{
    Box final_box;
    if (b2.type == final_queue || b2.type == goal){
        switch (b2.col){
            case blue:
                final_box = Box(final_blue_box, normal, none);
            break;
            case red:
                final_box = Box(final_red_box, normal, none);
            break;
            case green:
                final_box = Box(final_green_box, normal, none);
            break;
            case yellow:
                final_box = Box(final_yellow_box, normal, none);
            break;
        }
    }else{
        final_box = b2;
    }
    vector<color> walls;
    bool reached_final_box = false;
    if (b1.type == normal){
        for (int i = b1.num+1; !reached_final_box; i = i%68 + 1){
            reached_final_box = (final_box.num == i);
            color c = isWall(Box(i, normal, none));
            if(c != none){
                walls.push_back(c);
            }
        }
    }
    return walls;
}


bool Parchis::isLegalMove(color player, const Box & box, int dice_number) const{
    if(gameOver())
        return false;
    // Controlar si intento mover una ficha que no es del color del jugador actual.
    if(player != current_color)
        return false;
    // Controlar si intento mover un número que no está disponible en mis dados.
    if(!dice.isAvailable(player, dice_number))
        return false;
    // Controlar si intento contar un número distinto de 20 cuando me he comido una ficha.
    if(isEatingMove() && dice_number != 20)
        return false;
    // Controlar si intento contar un número distinto de 10 cuando he llevado una ficha a la meta.
    if(isGoalMove() && dice_number != 10)
        return false;
    // Control de movimientos
    Box final_box = computeMove(player, box, dice_number);
    // Controlar si barreras, si está en la casa el movimiento solo sería legal si dice_number == 5, ...
    if (box.type == home && dice_number != 5){
        return false;
    //Controlar que ya estés en la meta
    }else if(box.type == goal){
        return false;
    // Comprobar que no haya ya dos fichas en la casilla (a menos que la casilla de destino sea home, meta o la misma que la de partida).
    }
    else if (final_box.type != goal && final_box.type != home && !(final_box == box) && boxState(final_box).size() == 2)
    {
        return false;
    //Controlar los muros
    }
    else{
        // Comprobar que en el camino del movimiento no hay barreras
        vector<color> walls = anyWall(box, final_box);
        for (int i = 0; i < walls.size(); i++){
            //cout << walls.at(i) << endl;
            if (walls.at(i) != player){
                return false;
            }
        }
        // Comprobar que si he sacado un 6 y tengo barreras de mi color he elegido una de las fichas en la barrera.
        if(dice_number == 6){
            bool hay_walls = false;
            for(int i = 0; i < board.getPieces(player).size() && !hay_walls; i++){
                //cout << "HOLAA" << endl;
                //cout << player << " " << isWall(board.getPiece(player, i)) << endl;
                hay_walls = (isWall(board.getPiece(player, i)) == player);
            }

            if(hay_walls && isWall(box) != player){
                return false;
            }
        }
    } // TODO: falta (al menos) un caso: que salga un 6, haya alguna barrera de ese color y se elija una ficha fuera de la barrera.
    return true;
}

bool Parchis::canSkipTurn(color player, int dice_number) const{
    return dice.isAvailable(player, dice_number) and getAvailablePieces(player, dice_number).size() == 0;
}

const vector<int> Parchis::getAvailablePieces(color player, int dice_number) const{
    vector<Box> player_pieces = board.getPieces(player);
    vector<int> available_pieces;

    for(int i = 0; i < player_pieces.size(); i++){
        if(isLegalMove(player, player_pieces[i], dice_number)){
            available_pieces.push_back(i);
        }
    }

    return available_pieces;

}

const Box Parchis::computeMove(color player, const Box & piece_box, int dice_number, bool * goal_bounce) const{
    Box final_box;

    if(goal_bounce != NULL) *goal_bounce = false;

    //Si sale un 6, comprobar si se da condición para avanzar 7 o 6
    if(dice_number == 6){
        bool pieces_out = true;
        for (int i = 0; i < board.getPieces(player).size() && pieces_out; i++){
            if (board.getPieces(player).at(i).type == home){
                pieces_out = false;
            }
        }

        if (pieces_out){
            dice_number = 7;
        }
    }

    //Si sale de la casilla de home
    if (piece_box.type == home){
        switch (player){
            case red:
                final_box = Box(init_red_box, normal, none);
                break;
            case blue:
                final_box = Box(init_blue_box, normal, none);
                break;
            case yellow:
                final_box = Box(init_yellow_box, normal, none);
                break;
            case green:
                final_box = Box(init_green_box, normal, none);
                break;
        }
    }        
    //Condiciones para empezar a avanzar por pasillo de meta
    else if(piece_box.num <= final_boxes.at(player) && piece_box.num + dice_number > final_boxes.at(player) && piece_box.type == normal){
        //final_box = Box(final_boxes[player], normal, none);
        int count = piece_box.num + dice_number - final_boxes.at(player);
        if (count <= 7){
            final_box = Box(count, final_queue, player);
        }else if (count == 8){
            final_box = Box(0, goal, player);
        }else{
            int diff = 16 - (count);
            if(diff > 0){
                final_box = Box(diff, final_queue, player);
            }else{
                final_box = Box(final_boxes.at(player) + diff, normal, none);
            }
            if(goal_bounce != NULL) *goal_bounce = true;
        }

    }
    
    /*
    else if(piece_box.num <= 17 && piece_box.num + dice_number > 17 && player == blue && piece_box.type == normal){
        final_box = Box(piece_box.num + dice_number-17, final_queue, blue);

    }else if(piece_box.num <= 34 && piece_box.num + dice_number > 34 && player == red && piece_box.type == normal){
        final_box = Box(piece_box.num + dice_number-34, final_queue, red);

    }else if(piece_box.num <= 51 && piece_box.num + dice_number > 51 && player == green && piece_box.type == normal){
        final_box = Box(piece_box.num + dice_number-51, final_queue, green);

    }else if(piece_box.num <= 68 && piece_box.num + dice_number > 68 && player == yellow && piece_box.type == normal){
        final_box = Box(piece_box.num + dice_number-68, final_queue, yellow);
    }
    */
    //Si ya está en pasillo de meta
    else if(piece_box.type == final_queue){
        if (piece_box.num + dice_number <= 7){
            final_box = Box(piece_box.num + dice_number, final_queue, player);
        }else if (piece_box.num + dice_number == 8){
            final_box = Box(0, goal, player);
        }else{
            int diff = 16 - (piece_box.num + dice_number);
            if(diff > 0){
                final_box = Box(diff, final_queue, player);
            }else{
                final_box = Box(final_boxes.at(player) + diff, normal, none);
            }
            if(goal_bounce != NULL) *goal_bounce = true;
        }
    }
    //Por defecto
    else{
        final_box = Box(1 + (piece_box.num+ dice_number - 1) % 68, box_type::normal, color::none);
    }

    return final_box;
}

void Parchis::movePiece(color player, int piece, int dice_number){
    if(!gameOver()){
        // Si elijo pasar turno compruebo que efectivamente puedo hacerlo. 
        // Si sí, pongo el turno en el siguiente jugador. Si no, el jugador ha hecho un movimiento ilegal.
        if(piece == SKIP_TURN){
            if(canSkipTurn(player, dice_number)){
                eating_move = false;
                goal_move = false;
                
                this->last_dice = dice_number;
                this->last_moves.clear();
                this->dice.removeNumber(player, dice_number);
                this->nextTurn();
                cout << "TURN SKIPPED" << endl;

                turn++;
                last_action = tuple<color, int, int>(player, piece, dice_number);
            }else{
                turn++;
                illegal_move_player = current_player;
                cout << "ILLEGALLY TRIED TO SKIP TURN" << endl;
            }
            return;
        }
        // Switch por colores
        Box piece_box = board.getPiece(player, piece);
        if(isLegalMove(player, piece_box, dice_number)){
            goal_bounce = false;
            Box final_box = computeMove(player, piece_box, dice_number, &goal_bounce);
            
            /* Gestión de las "comidas"*/
            eating_move = false;
            goal_move = false;

            //Comprobar si hay una ficha de otro color en la casilla destino
            vector<pair <color, int>> box_states = boxState(final_box);

            if (!box_states.empty() && box_states[0].first != player){
                //Comprobar que la casilla no es segura
                vector<int>::const_iterator ci; 
                if (final_box.type == normal && count(safe_boxes.begin(), safe_boxes.end(), final_box.num) == 0){
                    //Movemos la ficha
                    eating_move = true;
                    
                }   
            }


            board.movePiece(player, piece, final_box);

            this->last_dice = dice_number;
            this->last_moves.clear();

            if(!goal_bounce)
                this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, final_box));
            else{
                this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, piece_box, Box(0, goal, player)));
                this->last_moves.push_back(tuple<color, int, Box, Box>(player, piece, Box(0, goal, player), final_box));
            }

            // Controlar si se come alguna ficha. En ese caso se actualiza también la ficha comida.
            // La ficha comida se añadiría también al vector last_moves.
            if(eating_move){
                Box origen_comida = board.getPiece(box_states[0].first, box_states[0].second);
                board.movePiece(box_states[0].first, box_states[0].second, Box(0, home, box_states[0].first));
                this->last_moves.push_back(tuple<color, int, Box, Box>(box_states[0].first, box_states[0].second, origen_comida, Box(0, home, box_states[0].first)));
            }

            // Controlar si la ficha ha llegado a la meta. En ese caso el jugador se cuenta 10 con otra ficha (salvo que sea la última)
            if(final_box.type == goal && !gameOver()){
                goal_move = true;
            }


            this->dice.removeNumber(player, dice_number);

            if(eating_move){
                // Añadir al dado de player el valor 20
                dice.forceNumber(player, 20);
            }
            if(goal_move){
                dice.forceNumber(player, 10);
            }

            nextTurn();
            turn++;
            last_action = tuple<color, int, int>(player, piece, dice_number);
        }
        else{
            illegal_move_player = current_player;
        }

        
    }
}

const vector<tuple<color, int, Box, Box>> & Parchis::getLastMoves() const{
    return this->last_moves;
}

int Parchis::getLastDice(){
    return this->last_dice;
}

const vector<pair <color, int>> Parchis::boxState(const Box & box) const{

    vector<pair <color, int>> occupation;

    for (int c = color::blue; c < color::none; c++){
        color col = (color)c;
        for (int i = 0; i < board.getPieces(col).size(); i++){
            if (board.getPiece(col, i) == box ){
                occupation.push_back(pair<color, int>(col, i));
            }
        }
    }

    return occupation;
}


void Parchis::nextTurn(){
    //cout << "--------- CAMBIO DE TURNO ---------" << endl;
    if (last_dice != 6 && !eating_move && !goal_move){
        this->current_player = (current_player+1)%2;
        switch(this->current_color){
            case yellow:
                this->current_color = blue;
                break;
            case blue:
                this->current_color = red;
                break;
            case red:
                this->current_color = green;
                break;
            case green:
                this->current_color = yellow;
                break;
        }
    }
}

void Parchis::gameLoop(){
    // Incializar el juego para los jugadores por primera vez
    for (int i = 0; i < players.size(); i++)
    {
        players.at(i)->perceive(*this);
    }

    // Bucle principal del juego.
    while (!gameOver()){
        // :)
        gameStep();
    }

    // Gestionar final de la partida.
    if (gameOver())
    {
        cout << "++++++++++++++++++++++++" << endl;
        cout << "La partida ha terminado" << endl;
        int winner = getWinner();
        color winner_color = getColorWinner();

        cout << "Ha ganado el jugador " << winner << " (" << str(winner_color) << ")" << endl;
        if (illegalMove())
        {
            cout << "El jugador " << (winner == 1 ? 0 : 1) << " ha hecho un movimiento ilegal" << endl;
        }
        cout << "++++++++++++++++++++++++" << endl;
    }
}

bool Parchis::gameStep(){
    //int id_piece;
    //color c_piece;
    //int dice;



    //cout << current_player << endl;
    //cout << players.size() << endl;
    //cout << players.at(current_player) << endl;
    //for(int i = 0; i < players.size(); i++){
    //    players.at(i)->perceive(*this);
    //}

    cout << "----------------" << endl;
    cout << "Turno: " << turn << endl;
    cout << "Jugador actual: " << this->current_player << endl;
    cout << "Color actual: " << str(this->current_color) << endl;
    cout << "----------------" << endl;

    // El jugador actual hace su movimiento.
    bool move = players.at(current_player)->move();

    // Se notifica el movimiento a todos los jugadores.
    for (int i = 0; i < players.size(); i++)
    {
        players.at(i)->perceive(*this);
    }

    // Se espera a que todos los jugadores estén listos para el siguiente turno.
    waitForPlayers();

    return true;

    //cout << "HOLAAAA" << endl;
    //cout << move << endl;

    /*
    if(move){
        // movePiece(c_piece, id_piece, dice);
        //Actualizar interfaz
        
        // cout << "------- Parchis -------" << endl;
        cout << "Jugador actual: " << this->current_player << endl;
        cout << "Color actual: " << str(this->current_color) << endl;        
    }

    for (int i = 0; i < players.size(); i++){
        players.at(i)->perceive(*this);
    }

    return move;
    */
}

void Parchis::waitForPlayers() const{
    // Sleep 10 milliseconds while there is a player that is not ready for next turn.
    vector<bool> ready_players(players.size(), false);
    bool ready = false;
    while (!ready){
        ready = true;
        for (int i = 0; i < players.size(); i++){
            if(!ready_players[i]){
                ready_players[i] = players.at(i)->readyForNextTurn();
                ready = ready && ready_players[i];
            }
        }
        if(!ready) sleep(milliseconds(10));
    }
}

bool Parchis::gameOver() const{
    return getWinner() != -1;
}

int Parchis::getWinner() const{
    if(illegal_move_player != -1){
        return (illegal_move_player == 0)?1:0;
    }
    color col = getColorWinner();
    switch(col){
        case yellow:
        case red:
            return 0;
        case blue:
        case green:
            return 1;

        default: 
            return -1;

        break;
    }
}

color Parchis::getColorWinner() const{
    // Recorro todos los colores. En principio da igual el orden, solo debería encontrarse como mucho un ganador.
    for(int i = 0; i < Parchis::game_colors.size(); i++){
        color col = Parchis::game_colors.at(i);
        Box goal(0, box_type::goal, col);

        if(boxState(goal).size() == board.getPieces(col).size()){
            return col;
        }
    }
    return none;
}

bool Parchis::illegalMove() const{
    return this->illegal_move_player != -1;
}

int Parchis::piecesAtGoal(color col) const{
    Box goal(0, box_type::goal, col);
    return boxState(goal).size();
}


Parchis Parchis::generateNextMove(color & c_piece,  int & id_piece, int & dice) const{
    c_piece = this->getCurrentColor();
    bool change_dice = false;
    vector<int> current_dices;
    vector<int> current_pieces;

    if (dice == -1){
        dice = this->getAvailableDices(c_piece).at(0);
    }

    do{
        //Compruebo si quedan movimientos legales con dice
        current_pieces = this->getAvailablePieces(c_piece, dice);
        if (current_pieces.size() > 0){
            if(id_piece == -1){
                id_piece = current_pieces.at(0);
            }
            else{
                //Siguiente pieza a id_piece
                for(int i = 0; i < current_pieces.size(); i++){
                    if(current_pieces.at(i) == id_piece){
                        if (i == current_pieces.size() - 1){
                            //Cambio de dado
                            //change_dice = true;
                            if(this->canSkipTurn(c_piece, dice) && id_piece != SKIP_TURN){
                                id_piece = SKIP_TURN;
                            }
                            else{
                                change_dice = true;
                            }
                        }
                        else{
                            id_piece = current_pieces.at(i+1);
                            change_dice = false;
                        }
                        break;
                    }
                }
            }
        }else if(this->canSkipTurn(c_piece, dice) && id_piece != SKIP_TURN){
            id_piece = SKIP_TURN;
        }
        else{
            //Siguiente dado
            change_dice = true;
        }

        if(change_dice){
            current_dices = this->getAvailableDices(c_piece);
            for(int j = 0; j < current_dices.size(); j++){
                if(current_dices.at(j) == dice){
                    if(j == current_dices.size() -1){
                        return *this;
                    }else{
                        dice = current_dices.at(j+1);
                        break;
                    }
                }
            }
        }
    }while(change_dice);

    Parchis next_move(*this);
    //cout << str(c_piece) << " " << id_piece << " " << dice << endl;
    next_move.movePiece(c_piece, id_piece, dice);
    return next_move;

}

bool Parchis::isSafeBox(const Box & box) const{
    return (box.type == normal && find(safe_boxes.begin(), safe_boxes.end(), box.num) != safe_boxes.end());
}

bool Parchis::isSafePiece(const color & player, const int & piece) const{
    return isSafeBox(this->board.getPiece(player, piece));
}

vector<color> Parchis::getPlayerColors(int player) const{
    if (player == 0){
        return {yellow, red};
    }
    else{
        return {blue, green};
    }
}

int Parchis::distanceToGoal(color player, const Box & box) const{
    switch(box.type){
        case normal:
            return abs(box.num - final_boxes.at(player)) + 8;
        case goal:
            return 0;
        case final_queue:
            return 8 - box.num;
        case home:
            return 1 + 65 + 8;                    
        default:
            return -1;
    }
}

int Parchis::distanceToGoal(color player, int id_piece) const{
    return distanceToGoal(player, this->board.getPiece(player, id_piece));
}
