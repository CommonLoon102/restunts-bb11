// Invoked only by the host package builder in a disposable stock TempleOS guest.
#include "DISC/PORT/LEGACY.HH"

#define TEMPLE_PACKAGE_IMAGE "C:/GAME.ISO.C"
#define TEMPLE_PACKAGE_SOURCE "C:/PORT/DISC"
#define TEMPLE_PACKAGE_EXCEPTION 'GameISO'

legacy_void RestuntsBuildCD()
{
  legacy_s64 blocks=RedSeaISO(TEMPLE_PACKAGE_IMAGE,TEMPLE_PACKAGE_SOURCE);
  if(blocks<=0||!FileFind(TEMPLE_PACKAGE_IMAGE)) {
    throw(TEMPLE_PACKAGE_EXCEPTION);
  }
  Print("Restunts RedSea CD created: %d blocks.\n",blocks);
}

RestuntsBuildCD();
