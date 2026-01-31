#include "main.h"
#include "types.h"
#include "natives.h"

#pragma comment(lib, "ScriptHookV.lib")

#define DRIFT_KEY_CODE VK_LSHIFT

static inline bool IsKeyDown(DWORD dwKeyCode) {
	return (bool) (GetAsyncKeyState(dwKeyCode) & 0x8000);
}

void ScriptMain(void) {
	bool bKeyIsDown = false;
	while (true) {
		bKeyIsDown = IsKeyDown(DRIFT_KEY_CODE);

		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle playerVehicle;
		
		if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) {
			goto _NEXT;
		}

		if (!PLAYER::IS_PLAYER_CONTROL_ON(player)) {
			goto _NEXT;
		}
		
		if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE)) {
			goto _NEXT;
		}

		playerVehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (!ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
			goto _NEXT;
		}

		VEHICLE::SET_VEHICLE_REDUCE_GRIP(playerVehicle, bKeyIsDown);
		
	_NEXT:
		WAIT(0);
	}
}

BOOL APIENTRY DllMain(
	HMODULE hInstance, 
	DWORD dwReason, 
	LPVOID lpReserved
) {
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			scriptRegister(hInstance, ScriptMain);
			break;
		case DLL_PROCESS_DETACH:
			scriptUnregister(hInstance);
			break;
		default:
			break;
	}
	return TRUE;
}