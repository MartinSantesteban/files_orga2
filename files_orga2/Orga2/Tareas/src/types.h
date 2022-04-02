/* ** por compatibilidad se omiten tildes **
================================================================================
 TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de tipos comunes en el kernel.
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

typedef uint32_t vaddr_t; // direccion virtual.
typedef uint32_t paddr_t; // direccion fisica.

#endif //  __TYPES_H__




/*

typedef struct lru_entry_t {
    uint16_t tss_sel;
    paddr_t  direccion_fisica;
    uint8_t  entry_index;
}lru_entry_t;


typedef struct lru_table_t {
    lru_entry_t estructura[100];
    uint8_t  actual_index;
}lru_table_t;

static lru_table_t lru_table = {0};

void init_lru_table(){
  for(int i = 0; i < 100; i++){
    lru_table[i].entry_index = -1 ;
  }

}

*/