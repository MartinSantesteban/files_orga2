/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"

#include "i386.h"
#include "kassert.h"

typedef struct sched_entry_t {
  int8_t task_id;
  int16_t selector;
  bool alive;
} sched_entry_t;

static sched_entry_t sched_tasks[MAX_TASKS] = {0};
static int8_t last_task = -1;
int8_t current_task = IDLE_TASK_ID;

/**
 * @brief Agrega una tarea al primer slot libre
 * 
 * @param selector el selector de la GDT de la tarea
 * @param task_id el identificador de la tarea
 */
void sched_add_task(uint16_t selector, int8_t task_id) {
  kassert(selector != 0, "No se puede agregar el selector nulo");
  kassert(task_id >= 0 && task_id < MAX_TASKS, "Invalid task_id");
  
  // Se busca el primer slot libre para agregar la tarea
  for (size_t i = 0; i < MAX_TASKS; i++) {
    if (sched_tasks[i].task_id == -1) {
      sched_tasks[i].task_id = task_id;
      sched_tasks[i].selector = selector;
      return;
    }
  }
  kassert(false, "No task slots available");
}

/**
 * @brief Deshabilita una tarea en el scheduler
 * 
 * @param task_id la tarea a deshabilitar
 */

void sched_disable_task(int8_t task_id) {
  kassert(task_id >= 0 && task_id < MAX_TASKS, "Invalid task_id");
  
  for (size_t i = 0; i < MAX_TASKS; i++) {
    if (sched_tasks[i].task_id == task_id) {
      sched_tasks[i].alive = false;
      return;
    }
  }
  kassert(false, "Task Not Found!");
}

/**
 * @brief Habilita un tarea en el scheduler
 * 
 * @param task_id la tarea a habilitar
 */

void sched_enable_task(int8_t task_id) {
  kassert(task_id >= 0 && task_id < MAX_TASKS, "Invalid task_id");
  
  for (size_t i = 0; i < MAX_TASKS; i++) {
    if (sched_tasks[i].task_id == task_id) {
      sched_tasks[i].alive = true;
      return;
    }
  }
  kassert(false, "Task Not Found!");
}

/**
 * @brief Inicializa el scheduler
 * 
 */
void sched_init(void) {
  for (size_t i = 0; i < MAX_TASKS; i++) {
    sched_tasks[i].task_id = -1;
  }
}

uint8_t sched_current_task(){
	return sched_tasks[current_task].task_id;
}

/**
 * @brief Obtiene la siguiente tarea disponible con una política round-robin. Si no 
 * existen tareas disponibles, se salta a la tarea Idle.
 * 
 * @return uint16_t el selector de segmento de la tarea a saltar
 */

uint16_t sched_next_task(void) {                //Porque last & curr?

  int8_t i = (last_task + 1) % MAX_TASKS;       //tomo la siguiente en orden, interpretando las teareas como un buffer circular.
  
  do{                                           
    if (sched_tasks[i].alive) {                 //la siguiente tarea viva
      current_task = sched_tasks[i].task_id;    //tomo su id
      last_task = current_task;                 //y la anoto como la ultima
      return sched_tasks[i].selector;           //retorno el selector
    }
    i = (i + 1) % MAX_TASKS;                    //itero
  } while(i != last_task);                      //mientras no haya dado la vuelta
  

  if (!sched_tasks[last_task].alive) {          //si la tarea en la que estaba murió, y no había otra tarea viva
    // No alive tasks. Jump to idle.
    current_task = IDLE_TASK_ID;                //salto a la tarea idle
    return GDT_IDX_TASK_IDLE << 3;
  }
  // Only one task alive for this player.

  kassert(sched_tasks[last_task].alive, "Switching to dead task!");
  current_task = sched_tasks[last_task].task_id;
  return sched_tasks[last_task].selector;       //si la ultima tarea que estaba ejecutando (no idle) era la unica viva, me mantengo en ella.
}