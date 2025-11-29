<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Timoteo Argibay - 113678 - targibay@fi.uba.ar

- Para compilar:

```bash
make compilar_todo
```

- Para ejecutar:

```bash
make jugar
```

- Para ejecutar con valgrind:
```bash
make valgrind_juego
```
---
##  Funcionamiento

El programa funciona principalmente con un switch grande que maneja el input del usuario luego de mostrarle las opciones del menu,
decidi separar cada escenario posible en funciones mas chicas para que no quede muy grande el loop, se tiene la posibilidad de ir derecho al juego, donde usando el tiempo de semilla, siempre vamos a obtener 
una mezcla distinta de las cartas, pero tambien podemos ingresar una semilla manualmente que nos asegura (siempre q la pokedex sea igual) que las cartas esten en el mismo orden siempre, esto resulta especialmente util para 
probar la funcionalidad del mismo

<div>
<img src="img/diagrama_cartas.jpeg">
</div>

---

## Respuestas a las preguntas teóricas
1) Principalmente se uso el TDA lista para este TP ya que es el que mejor costo-beneficio proporciona, dandonos la libertade movernos por los elementos (en este caso pokemons) sin requerir un trozo continuo
gigante de memoria como pasaria si usaramos tp1_t directamente

2) El tda menu resulto un tda particularmente simple en mi situacion, debido a que la pantalla la manejamos nosotros como usuarios, el tda se encarga de guardar el titulo y las opciones dentro de una lista simplemente 
enlazada, algo que nos facilita bastante la impresion de los elementos, posee las opciones basicas de crear. agregar, mostrar y destruir sin perdidas de memoria posibles.

3) Una estructura auxiliar que use para este trabajo fue CARTA_T, se trata de un TDA que contiene en si un puntero al pokemon que representa y su propio estado (si se la elimino y o si se encuentra revelada), esto resulta fundamental para el juego ya que si no era muy dificil llevar cuenta de las cartas del juego y en que estado estaban sin alterar al struct pokemon original, si bien no tienen su propio archivo, este TDA tambien cuenta con funciones internas de crear, agregar y destruir pero a diferencia de otros, cuenta con la particularidad de que solo se libera a si mismo y no al pokemon al que contiene como sucederia normalmente, esto se debe a que, al 
ser el juego un memotest, hay al menos 2 cartas q apuntan a un pokemon, si las liberara al mismo tiempo que la carta, nos encontrariamos con problemas de doble free, asi que gracias a que el pokemon se sigue encontrando en su lista original, se lo libera por ahi y no a traves de la carta.

4) Las dificultades que encontre con el TP2 fueron muchas, al mismo nivel que con el TP1 o peor, pero me resulto un aprendizaje muy grande, creo que el principal reto fue el manejo de la terminal con escapes ansi, creando una pantalla alternativa y haciendo mi primera falsa interfaz grafica con las cartas, despues la logica del juego tambien resulto complicada o mas bien larga, no tanto por su dificultad en si si no que, al necesitar poner casi 
todo lo trabajado hasta ahora en practica en un solo trabajo, llevo mucho mas tiempo del esperado, sumado a, naturalmente, los errores de hacerlo por primera vez, que si lo tuviera q hacer de vuelta ya no ocurririan.


