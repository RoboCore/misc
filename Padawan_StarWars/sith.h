#ifndef SITH_H
#define SITH_H

#include <Arduino.h>

// ------------------------

#define reject  false
#define accept  true

// ------------------------
// Prototypes *****

void finishwhatVaderStarted(void);

// ------------------------
// Classes *****

class DarkSide {
  public:
    void invitation(boolean b);
};

extern DarkSide KyloRen;

// ------------------------

#endif // SITH_H


