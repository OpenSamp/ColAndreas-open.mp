// Open.mp component bridge for ColAndreas.
//
// Mirrors the streamer plugin's approach: a single IComponent subclass handles the
// open.mp lifecycle (onLoad/onInit/free) and wires up Pawn native registration for
// every AMX the server loads. The physics core in DynamicWorld / ColObject / Natives
// is untouched and continues to operate on AMX cells, so the legacy colandreas.inc
// API surface remains source-compatible.

#ifndef OPENMP_COMPONENT_H
#define OPENMP_COMPONENT_H

struct ICore;
struct IPawnComponent;

namespace ColAndreasRuntime
{
	ICore *core();
	IPawnComponent *pawn();
}

// Routed to open.mp's core logger by the component. Matches the legacy
// `typedef void(*logprintf_t)(char*, ...);` signature so ColAndreas core sources
// keep compiling unchanged.
extern "C" void ca_logprintf(char *format, ...);

#endif
