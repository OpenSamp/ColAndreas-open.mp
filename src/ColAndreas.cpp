//*********************************************************************************************************//
// ColAndreas by [uL]Pottus & [uL]Chris42O & [uL]Slice ****************************************************//
// Open.mp port: global state only; lifecycle + native registration live in openmp_component.cpp.          //
//*********************************************************************************************************//

#include <btBulletDynamicsCommon.h>

#include "ColAndreas.h"
#include "DynamicWorld.h"

bool colInit = false;
bool colDataLoaded = false;

logprintf_t logprintf = nullptr;

ColAndreasWorld *collisionWorld = nullptr;
