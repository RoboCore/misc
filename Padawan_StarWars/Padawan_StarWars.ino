#include "jedi.h"
#include "sith.h"

void setup()
{
  forceStatus(idle);
  forceMode(awaken);
}

void loop()
{  
  forceSide = userRead();

  if (force == true) {
    if (forceSide != dark) {
      F = ma;
    } else {
      KyloRen.invitation(accept);
      finishwhatVaderStarted();
    }
  }
}



