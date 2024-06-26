# Practica Inteligencia Artificial 3

## Búsqueda con Adversario (Juegos) - El Parchis - MinMax, podaAlfaBeta

### Objetivos
- Juego del parchís con técnicas de búsqueda con adversario.
- Selección de dados entre un conjunto disponible, sin azar.
- Implementación de estrategias Minimax y poda alpha-beta.
- Desarrollo de MinMax antes que la poda alpha-beta.
- Utilización de la poda alpha-beta para la entrega
- Heuristica del ejercicio


### Antes de empezar
* Instalar primero libreria libsfml-dev
* make
* para ejecutar desde la carpeta de la Practica ./bin/Parchis
* Para usar los ninjas usar la vpn de la universidad UGR

`heuristica:` conjunto de técnicas o métodos para resolver un problema
            (RAE)En algunas ciencias, manera de buscar la solución de un problema mediante métodos no rigurosos, como por tanteo, reglas empíricas

### Datos

- El orden de juego es Amarillo → Azul → Rojo → Verde ||  Amarillo + ROjo vs Azul y verde
- DADO no es aleatorio, vas eligiendo un dado de valor1-6 lo descartas, cuando hayas usado todos se repone
- Si se saca un 6 se vuelve a tirar con el mismo color
- Cuando se saca un 6 y las 4 fichas de ese color están fuera de la casa, se avanza 7 casillas en
lugar de 6

```bash
./bin/Parchis <opciones_normales> | <opciones_servidor> |
<emparejamiento_aleatorio> | <sala_privada> (--no-gui)
- <opciones_normales> = --p1 <opciones_p1> --p2 <opciones_p2>
(--ip direccion_ip=localhost) (--port port=8888)
- <opciones_p1> = [GUI|AI|Remote|Ninja] (id=0) (name=J1)
- <opciones_p2> = [GUI|AI|Remote|Ninja] (id=0) (name=J2)
- <opciones_servidor> = --server [GUI|AI] (id=0) (name=Server) (--port=8888)
- <emparejamiento_aleatorio> = --random [GUI|AI] (id=0) (name=J1)
- <sala_privada> = --private <nombre_sala> [GUI|AI] (id=0) (name=J1)
```





- estoy diciendo que quiero una partida en la que el jugador 1 es mi heurística (AI) asociada al id número 0, y juego contra el jugador 2, que es el Ninja 1 (Ninja). Además, no quiero que se abra la interfaz gráfica, la partida se jugará solo por terminal. Si no especifico el --no-gui la interfaz sí se abrirá.

```bash
./bin/Parchis --p1 AI 0 Juanlu --p2 Ninja 1 Nuria --no-gui
```


Para la practica, trabajamos con AIPlayer.h y AIPlayer.cpp


### Funciones a destacar

- box - casilla
- board
- Dice
- Parchis


- ¿Quien ha ganado?
  - gameOver()
  - getWinner()
  - getColorWinner()

- ¿Numero de fichas en la meta?
  - piecesAtGoal(color player)

- Funciones de distancia a la meta
  - DistanceToGoal(color player, const Bx & box)
  - DistanceToGoal(color player, int id_piece)


- Funciones de generacion del siguiente movimiento

  - dado un estado del juego, a partir de los parámetros de color, id de ficha y dado que se pase por referencia,  asociados a un determinado movimiento en el tablero determinara el siguiente hijo que se expandirá en el arbol de búsqueda
  - Los parametros se actualizaran de forma que se correspondan con el movimiento necesario para generar el nuevo hijo desarrollado.
  - Incialmente, para generar el primer hijo de una ramificacion, se debe pasar los parametros inicialiozados a -1

- Funcion que genera el siguiente movimiento sigiendo un orden ascendente de los dados
  - Parchis generateNestMove(color &c_piece, int & id_iece, int & dice) const;
  - Parchis generateNextMoveDescending(color &c_piece, int & id_iece, int & dice) const;

Los resultados de la busqueda se almacenan en los valores pasados por referencia

- Casilla y fichas Seguras
  - isSafeBox ( box)
  - isSafePIece (player, piece) devuelve si una determinada ficha de un determinado esta en una casilla segura o no

- Funciones para Comprobar barreras 
  - isWall
  - anyWall

- Acceso a las prolongaciones de Dice y BOard 
  - Para acceder a una referencia constatne al tablero y a los dados para hacer consultas

  - getDice()
  - getBoard()
  - getAvaliablePieces(player, dice_number)
  - getAvaliableDices()

- Funciones de consulta

  - isLegalMove // tiene en cuenta las barreras
  - canSKipTurn // si puedo pasar turno con el dado seleccionado, no tiene fichas para mover
  - iseatingmove
  - isgoalmove
  - goalBOunce
  - getcurrentColor()


### Clase AIPlayer

representa a los jugadores, es como comportamientoJUgador tiene su think

perceive y move so se pueden modificar

Invocacion MINIMAX o la PODA ALFA-BETA se debe hacer dentro del Think()

metodo think
  - variable c_piece color que le toca
  - current dices - vector almacena los dados disponibles para el move - lo cogo de primera de forma aleatoria, hay que cambiarlo
  - vector id fichas que puedes mover

  - Una vez elegido el dad miramos las fichas
  - obtenemos el vector de  fichas que se pueden mover para el dado elegido
  - si tengo fichas para el dado elegido muevo una al azar si no skipeo

  - 1º assignar valores a c_piece, id_piece y dice -> con eso determinamos el siguioente movimiento y se ejecutar

  - Descomentar la parte algoritmo y comentar lo aleatorio

  - Primera tarea del estudiante es familiarizarse con el codigo y hacer una implementacion de uno de los dos algoritmos de búsqueda
  - Primero se ofrece una heuristica para que se pueda utilizar ValoracionTest 


  - ValoracionTest se encarga de valorar de forma positica las fichas de los colores de mi jugador que estén en casillas seguras o en la meta, a la vez que se penaliza por las fichas del oponente que estén en la misma situacion( simpre que puedan) a casillas en las  que esten a salvo de los movimientos rivales


### ValoracionTest 
basicamente

  - Diferencia color de mi juugador y del oponente ( 2 jugadores creo)
  - hace una puntuacion y devuelvo la resta entre mi jugador y la puntuacion del oponente
  - se valora positivamente si la casilla es segura o meta(home) con +1 o +5 recorriendo todas las posibilidades
  - ValoracionTest debe existir, no la podemos cambiar creo

  - Tengo que definir una heuristica  propia (distinta de ValoracionTRest) que permita hacer  un 
  - jugador automatico juege lo mejor posible contra otros compañeros 
  - Se puede diseñar funcioes con las mismas entradas y salidas que ValoracionTerst, pero distinto nombre_sala
  - intancia id -> establedce tanto por línea de comandos como interfaz grafica antes de iniciar la partida

### COSAS A TENER EN cuenta
  - Necesario implementar una busqueda en profundidad partiendo del estado actual del juego - para cado nodo se hace necesario obtener todos los posibles  tableros a los que se podria pasargenerateNextMove y generateNextMoveDescendign  implementan esta funcionalidad A veces un valor mayor del dado supone una mayor ventaja para el juegador, es posible que explorando primero los valores altors se poden más nodos al aplicar la poda alfa-bera con una heuristica apropiada

  - Tmb cabe a destacar que en el juego propuesto un turno se corresponde con un solo movimiento , independientemente de que ese moviemto sea repetido tras sacar un 6 o de contar 10 o 20 tras llegar a la meta o comer

- `IMPORTANTE LEERLO ESTA PAARTE YA QUE ES IMPORTANTE PARA LOS NODOS` 
    - podemos saber si somos un nodo MAX o MIn, ya que conocemos el jugador que llama a la heuristica y las funciones como getCurrentPLayerId
    - Un nodo deberia ser MAX cuando jugador que mueve es el que llamo al algoritmo de busqueda

### CALIFICACION

- Memoria de la practica 0-3
- Uso de Minmax -> 0 y uso Poda ALfa-Beta 1

Eficacia del algoritmo 0 a 6 basado en competir a los ninjas , 1 punto por victoria 2 particas por cada ninja, que son 3

Objetivo: desarrolar un porgrama modificando AIPlayer que implemente MINIMAX O ALFA-BETA 
.zip

### MEMORIA DE LA PRACTICA 

1. Analisis del problema
2. Descripcion de la solucion planteada
    - detallamos las heuristicas empleadas y ventajas e inconvenientes de cada una de ellas
    detallar que heuristaca es mejor (y por tanto la que se quiere usar como solucion) aquella con el id = 0


### MINIMAX ALFABETA TEORIA

- Ahora dos o más agentes intervienen simultaneamentte 
- cierta diferencia de matrizz entre entornos multiagentes y sistemas multiagentes
    - 2 situaciones cooperaticas
    - competitivas teoria de juego

- Juego -> el resultado obtenido es el resultado de todos los jugadores Interdependencia Estrategica
  - Informacion Perfecta
  - NO hay azar 

- Juego Suma Nula vs No NUla  suma nula -> lo que gana un jugador lo pierde de manera equivalente el otro
Parchis es un juego de suma nula, bipersonal y de informacion perfecta, como el de los palillos

- Arbol de exploracion de juegos
  - representacion explicita de todas las formas de jugatr
      - ramas O -> ramas mias 
      - ramas Y -> ramas de mi contringante

```bash
            0 
        Y        Y 
    O      O O       O 
```

###  NOTACION MINMAX 
Se supone que el arbol de juego se puede desarrollar completamente 
- MAX 1º jugador
- MIN al segundo 

existiran los nodos MAX y MIN dependiendo de quie juege en ese momento 
   - Los nodos terminales  se pueden etiquerar como V vistoria E empate y D Derrota DESDE PUNTO VISTA 
 eJMEPLO ARBOL TIC TAC TOE

0 1 -1 PARA CADA NODO HOJA SE ETIQUETA CON ESOS VALORES  
oBJETIVO ES ENCONTRAR UN CONJUNTO DE MOVIMIENTOS ACCESIBLE QUE DE COMO GANADOR A MAX

UNA Estrategia ganadora para MAX es un subarbol en el que todos los nodos terminales son ganadores 
  - Tic tac toe es dificil dibujar el arbol entero, aun mas dificil para el parchis 

Como no podemos dibujar completamente el arbol -> nuevo objetivo encontrar una buena jugada inmediata 
 - Se necesita una funcion heuristica que permita guar al proceso

`Regla MINIMAX` 
  - Se va a desarrollar el arbol del juego hasta cierta profundidad, y ese arbol de juego tendra una nueva forntera de busqueda con totods los arboles 
  - El  valor V(j) de un nodo J de la forntera de busqueda es igual al de su evaluacion heuristica, evaluacion estatica 
  - Si J es nodo MAX , el valor V(J) es igual al maximo de los valores de sus nodos sucesores 
  - si es MIN , es igual al miimo 

  - Para determinar el valor MINIMAX de un nodo J se hace lo siguiente:

```bash
        J nodo terminal devuelve V(J) = f(J)

        para k = 1,2 ... b hacer: // RECORRE EL ARBOL 

            Genero JK , el kesimo sucesor de J 
            Calculo V (JK )
            k = 1 hacer AV(J) <- v(J1) ya ue es el primero 
                en otro caso cuando  k >=2
                    hacer AV(J ) <- max {AV(j), V(HK )} si MAX 
                    Si no hacer lo mismo para MIN 
            
            devolver V(J) = AV(J)
```

### PODA ALFA BETA
- Igual a MiniMax con menos esfuerzo 

- recurrimos a la poda , se usa cuando el arbol MiniMax es muy grande y se debe explorar hasta cierta profundidad , Tengo que tener una heuristica para saber que caminos realmente valen la pena explorar y cuales no , los no son los que tendremos que explorar 

- la Poda esta guiada por: tengo alfa A y beta B , describen los limites sobre los valores que aparecen a loo largo del camino 

    - alfa es la mejor opcion, el valor más alto que se ha encontrado hasta el momento encuanlquier punto del camino para MAX 
    - beta lo mismo pero la mas baja para MIN 

- busqueda alfa beta actualiza dichos valores segun se va recorriendo el arbol y termina cuando se encuaentra un nodo peor que el valor actual de alfa y beta 
  - Inicializado a alfa -INF e beta INF respectivamente 

  - desde nodo max se ira actualizando el valor de alfa segun la exploracion , comparando el valor obtenido por el arco con el valor actual de alfa 
      - beta es equivalente 

- beta almacena el peor valor para MAX encontrado por MIN hasta el momento, solo interesara que la busqueda prosiga si MIN puediera reducir beta
- cuando alfa crece tanto que supera a beta o cuando o b disminuye tanto que es menor que alfa se hace una poda y no seguir explorando dicho subarbol devolviendo la busqueda al nodo padre 

```bash
    f es mi heuristica
    Para calcular el valor V(J, alfa, beta) hacer lo siguiente:

       1. SI J es un nodo terminal devuelve V(j) = f(j)
        otro caso sea J1, J2, J3 ... los sucesores de J k -> 1 si J es un nodo MAX ir al paso 2 si en minimo ir al paso 3

        2 para max 
            hacer alfa = max(alfa, V(Jk, alfa, beta))
                si alfa >= beta devolver beta si no continuar 
                si k = b devolver alfa, si no k = k+1 y volver al paso 2 

        3 para min 
            hacer beta <- min (beta, V (JK, alfa, beta))
            si beta <= alfa devolver alfa, si no continuamos 
            si k = b devolver beta,. si no k = k+1 y pasamos al siguiente nodo J b es la cota
```


### TUTORIAL
Por defecto un comportamiento aleatorio tanto el valor del dado como la ficha  a moverid 0 jugador 1 
id 1 jugador 2

CurrentColor
- getAvaliable Dices 
- getAvaliablePieces

Diseñar comportamientos más inteligentes
- Tenemos varios agentes  -> id, se puede usar para implementar distintos comportamientos 

Aleatorio pero con cabeza
- elegir aleatoriamente solo entre los dados para los que puede mover fichas para cada dado veo el vector de fichas, si es mayor que 0 push back 
  - si el vector = 0 skip si no cojo un dado aleatorio de mi nuevo vector

Eligiendo que quiero mover 
  - los movimietnos siguen siendo aleatorio -> ahora vamso a intentar elegir que ficha queremos mover de forma inteligente 
  - lo que haremos sera mover siempre la ficha que tengamos mas adelantada, con el objetivo de llevarla cuanto antes a la meta . dado sigue siendo aleatorio
    - distanceBoxtoBox medir distancia entre dos casillas 
    - distance toGoal  para medir distancia a la meta 

Buscando entre los hijos -> generateNextMove 
  - Es necesario implementar uno de los algoritmos de busqueda pedidos 

Parchis dispone de generateNextMOve y generateNextMOveDescending para ello 
  - estos metodos son como un iterador  de forma que la primera vez que se llamen se pueda 
  - acceder al primer hijo del nodo que se este desarrollando 
  - las fichas se recorreran siempre en orden segun su id (0 a 3)

que es lo que vamos a hacer -> mejorar el comportamiento del agente anterior 
  - si detectamos que, para el siguiente turno, alguno de los movimientos que hagamos nos lleva a comernos una ficha 
  - a colocar una de nuestras fichas en la meta o ganar la partida nos quedamos con dicho movimientocaso contrario comportamiento ficha mas adelantada 

En parchis tenemos metodos de consulta casi cualquier cosa 
  - isEatingMOve() - ult movimiento se ha comido alguna ficha 
  - isGoalMove() - ult mov ha entrado a la meta 
  - gameOver - partido terminado 
  - getWInner - ganador partida 

#### Estrategia 
con generateNextMOve recorremos todos los hijos del tablero actual y cuando nos encontremso con un hijo que cumpla alguna de las condiciones inidcada nos quedaremos con el correspondiente movimiento 

generateNextMove 
  - argumentos - c_piece, id_piece, dice 
  - Como va a actuar como un iterador esos datos son los del ult movimiento, por lo que se actualizaran cuando  finalice la funcion 
  - los inicializop a none -1 -1 , entonces la funcion me devuelve el primer hijo, y las var se actualizaran para sel el mov que nos conlleva el primer hijo 
  - Cuando no haya hijos por recorrer la funcion devuelve el nodo padre 
  - con == podemos saber cuando hemos terminado 

  - actual es el nodo padre y queremos recorrer todos los hijos de actual 

  - Parchis hijo = actual.generateNextMove(c_piece, id_piece, dice)

  - recorre como en anchura 
      - hijo == actual termianr recorrer todos los hijos 
    

LO QUE TENEMOS QUE HACER SIMPLEMENTE ES 
  - recorrer todos los hijos 
  - Comprobar si algun hijo se cumple alguna condicion indicada 
  - si se cumple me quedo con dicha accion 
  - Si no muevo con el comportamiento anterior. 
    
Este ultimo agente si que se come una ficha sin pensarselo ,, se puede comer las de su  otro color ( puede perjudicar, o no )Esto es un ejemplo de generateNextMove pero no se usa ninguno de los algoritmos de busqueda que se pide 

Una de las tareas es la implementacion de uno de los algoritmos de busqueda e integrar la funcion  de recorrer los hijos de forma adecuada dentoro de los algorimos de busqueda, con esto se explica como emplezar a recorrer los hijos, de como iterar y como parar.

### Y AHORA QUE 

hemos victoria  
  - Usar clase AIPlayer diseño aleatorio o ligeramente racional 
  - Parchis tiene herramientas utiles para info de la partida 

Ahora es necesario implementar o bien Minimax o Alfa-BEeta 
  - EL COMPORTAMIENTO RACIONAL DEL AGENTE DEBERA SER CONSECUENCIA DE ALGUNO DE ESTOS ALGORITMOS 
  - Un comportamiento sin busqueda como los que se realizan en el tutorial es invalido 

EL algoritmo de busqueda, cuando llegue al limite de profuncidad estabnlecido, establece una valoracion para el nodo al que haya llegado 
  - Esa valoracion tendra que medir lo prometedor que es ese tablero de juego, ese camino conseguido (coste) y ahi habra que tener en cuenta las funciones clase Parchis 

Durante la practica podemos implementar distintas heuristicas y enfrentarlas ente ellas , parecido al tutorial 

software inicial hay una parte comentada que puede ser un punto de partida ver 5.4 guia 

metodos publicos de Parchis Board Dice y attributes es importante para la heuristica SECCION 5 IMPORTANTISIMA
distancias, barreras, casillas seguras, turnos, posiciones en el tablero, rebotes, etc

  - IMPORTANTE: para que una función de consulta pueda usarse en el método think debe llevar el calificador const al final.

### CONSIDERACIONES FINALES 

- valor mayor del dado supone una mayor ventaja para el jugador que lo usa  generateNextMove 

- un turno corresponde con un  unico movimiento de ficha 
- SACANDO UN 6 BAJAMOS A UNA NUEVA RAMA
- nodo max o min geturrentPLayerID nos indica a que jugador le toca mover en cada turno 
- Recordemos que un nodo debería ser MÁX cuando el jugador que mueve es el que llamó al algoritmo de búsqueda

- La ramificación del árbol de búsqueda va a variar de forma significativa según la cantidad de dados

Importante leer antes la documentacion