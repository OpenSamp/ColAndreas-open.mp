#ifndef NATIVES_TABLE_H
#define NATIVES_TABLE_H

#include <amx/amx.h>

// Master table of every Pawn native ColAndreas exposes. Definition in
// natives_table.cpp. Terminated by a { nullptr, nullptr } sentinel.
extern AMX_NATIVE_INFO gColAndreasNatives[];

namespace ColAndreasUtility
{
	int checkInterfaceAndRegisterNatives(AMX *amx, AMX_NATIVE_INFO *amxNativeList);
}

#endif
