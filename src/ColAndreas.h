#ifndef _INCL_COLANDREAS
#define _INCL_COLANDREAS

#pragma warning (disable:4005 700 996)

#include <cstdarg>
#include <cstdint>
#include <sstream>
#include <vector>

// Pawn AMX headers (vendored under lib/pawn). The component builds amxplugin.cpp into
// the shared library, so the usual amx_Register / amx_GetAddr / etc. calls resolve
// through the IPawnComponent function table set at component init time.
#include <amx/amx.h>

// Matches the legacy SA-MP logprintf signature. ColAndreas core sources keep calling
// it with string literals; the component binds the pointer to open.mp's core logger
// before any user of logprintf runs.
typedef void (*logprintf_t)(char *format, ...);

extern bool colInit;
extern bool colDataLoaded;
extern logprintf_t logprintf;

#endif
