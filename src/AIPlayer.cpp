# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;
    
    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    

    /*
    switch(id){
        case 0:
            thinkAleatorio(c_piece,id_piece,dice);
            break;
        case 1:
            thinkAleatorioMasInteligente(c_piece,id_piece,dice);
            break;
        case 2:
           thinkFichaMasAdelantada(c_piece,id_piece,dice);
            break;
        case 3:
            thinkMejorOpcion(c_piece,id_piece,dice);
            break;
    }
    
    */

    
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta

    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    //valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    
    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            //valor = MinMax(*actual, jugador, 0 ,PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, ValoracionTest );
            //thinkAleatorio(c_piece,id_piece,dice);
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            //thinkMejorOpcion(c_piece,id_piece,dice);
            cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
            break;
        case 1:
            //thinkAleatorio(c_piece,id_piece,dice);
            thinkMejorOpcion(c_piece,id_piece,dice);
            //valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
        case 2:
           // valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
    
    
    //OPCION MINIMAX  accion -> c_piece dice id_puece, 
    //valor = MiniMax(actual, jugador, 0 Profunciodad minimax, accion, heuristica )
    //cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    //Opcion POda ALfaBeta
    //valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    MIS VALORACIONES ADEMAS DE VALORACIONTEST
*/


double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    MINIMAX Y PODA ALFA BETA
*/

double AIPlayer::MinMax (const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double (*heuristic)(const Parchis &, int)) const{

    /*
        Pasos que voy a realizar para realizar MINMAX

        Desarrolo el arbol del juego hasta cierta profuncidad,
        NOdo J tiene un valor ValoracionTest(J)
        nodo MAX en J -> maximo de los valores de sus nodos sucesores
        nodo MIN en J -> minimo de los valores de sus nodos sucesores 

        para k = 1,2, ... b hacer 

            genero hijo Jk
            ValoracionTest(JK)
            si k = 1 -> AV(J) <- v(J1) ya que es el primero
            para el resto comparo 
                hacer max{AV(J), V(Hk)} si MAX
                si no HACER LO MISMO PARA min

        devolver V(J) = AV(J)

        Poda alfa beta sera lo mismo pero podando segun alfa y beta

        const double masinf = 9999999999.0, menosinf = -9999999999.0;
        const double gana = masinf - 1, pierde = menosinf + 1;
        const int num_pieces = 4;
        const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax 6
        const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta 8

        color &c_piece, int &id_piece, int &dice -> si defino eso es mi ACCION
    */

    // COmpruebo si es nodo final
    if (profundidad  == profundidad_max || actual.gameOver()){
        return ValoracionTest(actual,jugador);
        cout << "FIN" << endl;
    }
        

    // datos 
    color last_c_piece = none; // color ultima ficha movida
    int last_id_piece = -1;// id de la ultima ficha que se movio
    int last_dice = -1; // el dado que se uso en el ultimo movimiento
    double valor, aux;
    
    
    if (actual.getCurrentPlayerId() == jugador)
        valor = menosinf;
    else 
        valor = masinf;
   
    // genero hijo
    Parchis hijo = actual.generateNextMove(last_c_piece,last_id_piece, last_dice);

    while (!(hijo == actual)){
        
        // genero aux
         aux = MinMax(hijo,jugador,profundidad+2,profundidad_max,last_c_piece, last_id_piece, last_dice, ValoracionTest);
        //aux = MinMax(hijo,jugador,profundidad+2,profundidad_max,c_piece, id_piece, dice, ValoracionTest); funcionan ambos a primera instancia

        cout << "Genero Hijo" << endl;
        cout << profundidad << endl;
        cout << profundidad_max <<endl;
        cout << valor << endl;

        if (actual.getCurrentPlayerId() == jugador){ // MAX

            //cout << "Hola" << endl;
            if (aux > valor){
                valor = aux;
                if (profundidad == 0){
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                }
                
            }

        } else { // MIN
           //valor = masinf;
           // cout << "Adios" << endl;

            if (aux < valor){
                valor = aux;

                
                if (profundidad == 0){
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                }
                
            }

        }

        // genero siguiente hijo
        hijo = actual.generateNextMove(last_c_piece,last_id_piece, last_dice);

    }
    

    

    return valor; 
}

 double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const{
    
    // COmpruebo si es nodo final
    if (profundidad  == profundidad_max || actual.gameOver()){
        return ValoracionTest(actual,jugador);
        
    }
        

    // datos 
    color last_c_piece = none; // color ultima ficha movida
    int last_id_piece = -1;// id de la ultima ficha que se movio
    int last_dice = -1; // el dado que se uso en el ultimo movimiento
    double aux; // valor pasa a ser alpha y beta
    
    /*
    if (actual.getCurrentPlayerId() == jugador)
        valor = menosinf;
    else 
        valor = masinf;
    */

    // genero hijo
    Parchis hijo = actual.generateNextMove(last_c_piece,last_id_piece, last_dice);

    while (!(hijo == actual)){
        
        // genero aux
        //aux = MinMax(hijo,jugador,profundidad+2,profundidad_max,last_c_piece, last_id_piece, last_dice, ValoracionTest);
        aux = Poda_AlfaBeta(hijo, jugador, profundidad+2, profundidad_max,last_c_piece, last_id_piece, last_dice, alpha, beta, ValoracionTest );

        cout << "Genero Hijo" << endl;
        cout << profundidad << endl;
        cout << profundidad_max <<endl;
        //cout << valor << endl;
        
        if (actual.getCurrentPlayerId() == jugador){ // MAX

            //cout << "Hola" << endl;
            if (aux > alpha ){
                alpha = aux;

                if (profundidad == 0){
                    c_piece = last_c_piece;
                    id_piece = last_id_piece;
                    dice = last_dice;
                }
                
            }

            if (alpha >= beta)
                return beta;

        } else { // MIN
           //valor = masinf;
           // cout << "Adios" << endl;

            if (aux < beta){
                beta = aux;

                
                if (profundidad == 0){
                   c_piece = last_c_piece;
                   id_piece = last_id_piece;
                   dice = last_dice;
                }
                
            }

             if (alpha >= beta)
                return alpha;

        }
        
        // genero siguiente hijo
        // ¿POR QUE LO CASTEO MAL?
        if (alpha < beta)
         hijo = actual.generateNextMove(last_c_piece,last_id_piece, last_dice);
        
    }

    if (actual.getCurrentPlayerId() == jugador)
        return alpha;
    else 
        return beta;

 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    TUTORIAL
*/



void AIPlayer::thinkAleatorio (color & c_piece,  int & id_piece, int & dice) const{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente 
    //como lo que se muestran al final de la función.
    
    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores, 
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(c_piece, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if(current_pieces.size() > 0){
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
    else{
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
    }
}

void AIPlayer::thinkAleatorioMasInteligente (color & c_piece,  int & id_piece, int & dice) const{

    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);

    // ahora en ve de elegir un dado al azar, miro primero cúales tienen fichas que se puedan mover
    vector<int> current_dices_que_pueden_mover_fichas;
    for (int i=0; i < current_dices.size(); i++){

        // se obtiene el vector de fichas que se piede mover para el dado elegido 
        current_pieces = actual->getAvailablePieces(c_piece,current_dices[i]);

        // Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas

        if (current_pieces.size() > 0){
            current_dices_que_pueden_mover_fichas.push_back(current_dices[i]);
        }
    }

    // Si no tengo ningñun dado que pueda mover fichas, paso turno con un dado al azar
    if (current_dices_que_pueden_mover_fichas.size() == 0){
        dice = current_dices[rand() % current_dices.size()];

        id_piece = SKIP_TURN;
    } else {
        dice = current_dices_que_pueden_mover_fichas[rand() % current_dices_que_pueden_mover_fichas.size()];

        // se obtiene el vector de fichas que se puede mover para el dado elegido
        current_pieces = actual->getAvailablePieces(c_piece, dice);

        // Muevo una ficha al azar entre las que se pueden mover.
        id_piece = current_pieces[rand() % current_pieces.size()];
    }


}

void AIPlayer::thinkFichaMasAdelantada (color & c_piece,  int & id_piece, int & dice) const{

        // eligo el dado haciendo lo mismo que el jugador anterior
        thinkAleatorioMasInteligente(c_piece,id_piece,dice);

        /*
            Tras llamar a esta funcionb ya tengo en dice el numero dado que quiero usar .
            Ahora voy a seleccionar la ficha más adelantada , la mas cercana a la meta 
        */

       vector<int> current_pieces = actual->getAvailablePieces(c_piece,dice);

       int id_ficha_mas_adelantada = -1;
       int min_distancia_meta = 9999;

       for (int i = 0; i < current_pieces.size(); ++i){

           //distanceToGoal(color, id) devuelve la distancia a la meta de la ficha id del color indicado
           int distancia_meta = actual->distanceToGoal(c_piece, current_pieces[i]);

           if (distancia_meta < min_distancia_meta ){
               min_distancia_meta = distancia_meta;
               id_ficha_mas_adelantada = current_pieces[i];
           }
       }

       // caso en el que no haya encontrado nunguna ficha paso de turno.
       if (id_ficha_mas_adelantada == -1){
           id_piece = SKIP_TURN;
       } else {
           id_piece = id_ficha_mas_adelantada;
       }


}

void AIPlayer::thinkMejorOpcion (color & c_piece,  int & id_piece, int & dice) const{

    // vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.

    // generateNextMOve va iterandp sobre cada hijo. Le paso la accion del ultimo movimiento sobe
    // el sque he iterado y me devolvera el siguiente . Incicialmente, cuando aun no he hecho nungun movimiento lo inicializo asi 

    color last_c_piece = none; // color ultima ficha movida
    int last_id_piece = -1;// id de la ultima ficha que se movio
    int last_dice = -1; // el dado que se uso en el ultimo movimiento

    // cuando ya he recorrido todos los hijos, la funcion devuelve el estado actual, de esa forma puedo saber
    // cuando he de parar de iterar 

    Parchis siguiente_hijo = actual->generateNextMove(last_c_piece,last_id_piece, last_dice);

    bool me_quedo_con_esta_accion = false;

    while (!(siguiente_hijo == *actual) && !me_quedo_con_esta_accion ){
        if (siguiente_hijo.isEatingMove() or siguiente_hijo.isGoalMove() or (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador)){
            // si con este movimiento me como ficha, o llego a la meta o con este movimiento gano la partida 

            // me quedo con la accion actual (se almaceno en last_c_piece, last_id_piece, last_dice al llamar a generateNextMove)
            me_quedo_con_esta_accion = true;
        } else {
            // si no genero el siguiente hijo
            siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);
        }
    }

    // si me quedo con la accion que me interesa guardo las variables 
    if (me_quedo_con_esta_accion){
        c_piece = last_c_piece;
        id_piece = last_id_piece;
        dice = last_dice;
    } else {
        // si no muevo la ficha más adelantada como antes 
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }


}