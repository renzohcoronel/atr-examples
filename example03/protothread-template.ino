/* Aplicaciones en tiempo real - FRLP
  Ejemplo: 03
  Circuito simulado: https://www.tinkercad.com/things/ffl84zS9Lln-threads/editel?sharecode=2TBQMTCOTI6vwOd5Vc-v3kFIP0pLY9kaTZ8JEdZDrDg
*/

#include <pt.h>
// Crear tus protothread(s)
static struct protoThread pt1; // ref:. https://www.programiz.com/c-programming/c-structures

// Funcion que ejecutara el thread.
static int protothreadFunction(struct protoThread *pt)
{
  
  // Arrancar el protothread
  PT_BEGIN(pt);
  /* Codigo que se ejecutara en el thread */
  // Detener el tread
  PT_END(pt);
}

void setup() {
  // Se debe iniciar el thread
  PT_INIT(&pt1);
}

void loop() {
  // Ejecutar la funcion del thread
  protothreadFunction(&pt1); // Siempre se pasa el parametro por referencia
}