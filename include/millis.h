#ifndef MILLIS_H
#define MILLIS_H

#include <stdint.h>

typedef uint32_t millis_t;

void millis_init(void);
millis_t millis_get(void);

#define millis() millis_get()

#endif
