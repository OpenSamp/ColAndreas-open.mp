// Open.mp component entry point for the ColAndreas port.
//
// Replaces the old SA-MP Load/Unload/AmxLoad/AmxUnload/Supports exports. All the
// physics state (collisionWorld, colInit, colDataLoaded, logprintf, pAMXFunctions)
// lives in ColAndreas.cpp as before; this file just drives the lifecycle and routes
// AMX native registration through the IPawnComponent event dispatcher.

#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <sdk.hpp>
#include <Server/Components/Pawn/pawn.hpp>

#include "ColAndreas.h"
#include "DynamicWorld.h"
#include "natives_table.h"
#include "openmp_component.h"

extern void *pAMXFunctions;
extern bool colInit;
extern bool colDataLoaded;
extern logprintf_t logprintf;
extern ColAndreasWorld *collisionWorld;

namespace
{
	ICore *g_core = nullptr;
	IPawnComponent *g_pawn = nullptr;

	// UID picked fresh for this port. Arbitrary but stable.
	constexpr UID kColAndreasComponentUID = UID(0x436f6c416e647273ULL); // "ColAndrs"

	class ColAndreasComponent final
		: public IComponent
		, public PawnEventHandler
	{
	public:
		PROVIDE_UID(kColAndreasComponentUID)

		StringView componentName() const override
		{
			return "ColAndreas";
		}

		SemanticVersion componentVersion() const override
		{
			return SemanticVersion(1, 6, 0, 0);
		}

		void onLoad(ICore *c) override
		{
			g_core = c;
			logprintf = &ca_logprintf;

			ca_logprintf((char *)"*********************");
			ca_logprintf((char *)"** ColAndreas      **");
			ca_logprintf((char *)"** open.mp port    **");
			ca_logprintf((char *)"*********************");

			collisionWorld = new ColAndreasWorld();

			if (collisionWorld->loadCollisionData())
			{
				ca_logprintf((char *)"ColAndreas: loaded collision data.");
				colDataLoaded = true;
			}
			else
			{
				ca_logprintf((char *)"ColAndreas: no collision data found (scriptfiles/colandreas/ColAndreas.cadb).");
			}

			ca_logprintf((char *)"*********************");
			ca_logprintf((char *)"  ColAndreas Loaded");
			ca_logprintf((char *)"   " CA_VERSION);
			ca_logprintf((char *)"*********************");
		}

		void onInit(IComponentList *list) override
		{
			g_pawn = list->queryComponent<IPawnComponent>();
			if (g_pawn)
			{
				pAMXFunctions = const_cast<void *>(static_cast<const void *>(g_pawn->getAmxFunctions().data()));
				g_pawn->getEventDispatcher().addEventHandler(this);
			}
			else if (g_core)
			{
				g_core->logLn(LogLevel::Error, "ColAndreas: Pawn component not available; CA_* natives will not be registered");
			}
		}

		void onReady() override { }

		void onFree(IComponent * /*component*/) override { }

		void free() override
		{
			if (g_pawn)
			{
				g_pawn->getEventDispatcher().removeEventHandler(this);
				g_pawn = nullptr;
			}

			// NOTE: the original plugin leaked collisionWorld on purpose (Unload commented
			// out the delete) because Bullet's teardown was crashy. Preserve that behaviour.

			pAMXFunctions = nullptr;
			g_core = nullptr;
		}

		void reset() override { }

		// --- PawnEventHandler ---------------------------------------------------------------
		void onAmxLoad(IPawnScript &script) override
		{
			if (AMX *amx = script.GetAMX())
			{
				ColAndreasUtility::checkInterfaceAndRegisterNatives(amx, gColAndreasNatives);
			}
		}

		void onAmxUnload(IPawnScript & /*script*/) override { }
	};

	ColAndreasComponent g_colandreasComponent;
}

namespace ColAndreasRuntime
{
	ICore *core() { return g_core; }
	IPawnComponent *pawn() { return g_pawn; }
}

extern "C" void ca_logprintf(char *format, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	buffer[sizeof(buffer) - 1] = '\0';
	va_end(args);

	if (g_core)
	{
		g_core->printLn("%s", buffer);
	}
	else
	{
		// Pre-onLoad fallback: component not wired up yet, go straight to stdout so the
		// message isn't lost entirely.
		std::fprintf(stdout, "%s\n", buffer);
	}
}

COMPONENT_ENTRY_POINT()
{
	return &g_colandreasComponent;
}
