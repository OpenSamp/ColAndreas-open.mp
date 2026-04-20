#include "ColAndreas.h"
#include "Natives.h"
#include "natives_table.h"

AMX_NATIVE_INFO gColAndreasNatives[] =
{
	// Standard Natives
	{ "CA_Init", ColAndreasNatives::CA_Init },
	{ "CA_RayCastLine", ColAndreasNatives::CA_RayCastLine },
	{ "CA_RayCastLineExtraID", ColAndreasNatives::CA_RayCastLineExtraID },
	{ "CA_RayCastLineID", ColAndreasNatives::CA_RayCastLineID },
	{ "CA_RayCastLineAngle", ColAndreasNatives::CA_RayCastLineAngle },
	{ "CA_RayCastMultiLine", ColAndreasNatives::CA_RayCastMultiLine },
	{ "CA_RayCastReflectionVector", ColAndreasNatives::CA_RayCastReflectionVector },
	{ "CA_RayCastLineNormal", ColAndreasNatives::CA_RayCastLineNormal },
	{ "CA_ContactTest", ColAndreasNatives::CA_ContactTest },
	{ "CA_LoadFromDff", ColAndreasNatives::CA_LoadFromDff },
	{ "CA_CreateObject", ColAndreasNatives::CA_CreateObject },
	{ "CA_DestroyObject", ColAndreasNatives::CA_DestroyObject },
	{ "CA_IsValidObject", ColAndreasNatives::CA_IsValidObject },
	{ "CA_EulerToQuat", ColAndreasNatives::CA_EulerToQuat },
	{ "CA_QuatToEuler", ColAndreasNatives::CA_QuatToEuler },
	{ "CA_RemoveBuilding", ColAndreasNatives::CA_RemoveBuilding },
	{ "CA_RestoreBuilding", ColAndreasNatives::CA_RestoreBuilding },
	{ "CA_SetObjectPos", ColAndreasNatives::CA_SetObjectPos },
	{ "CA_SetObjectRot", ColAndreasNatives::CA_SetObjectRot },
	{ "CA_GetModelBoundingSphere", ColAndreasNatives::CA_GetModelBoundingSphere },
	{ "CA_GetModelBoundingBox", ColAndreasNatives::CA_GetModelBoundingBox },
	{ "CA_SetObjectExtraID", ColAndreasNatives::CA_SetObjectExtraID },
	{ "CA_GetObjectExtraID", ColAndreasNatives::CA_GetObjectExtraID },

	// Extended Natives
	{ "CA_RayCastLineEx", ColAndreasNatives::CA_RayCastLineEx },
	{ "CA_RayCastLineAngleEx", ColAndreasNatives::CA_RayCastLineAngleEx },
	{ nullptr, nullptr }
};

namespace ColAndreasUtility
{
	int checkInterfaceAndRegisterNatives(AMX *amx, AMX_NATIVE_INFO *amxNativeList)
	{
		// AMX_ERR_NOTFOUND is fine — it just means a script doesn't use every CA_* native.
		return amx_Register(amx, amxNativeList, -1);
	}
}
