#include "DR__GameMode.h"
#include "DR_Drone.h"	
#include "DR_PlayerController.h"
ADR__GameMode::ADR__GameMode()
{
	DefaultPawnClass = ADR_Drone::StaticClass();
	PlayerControllerClass = ADR_PlayerController::StaticClass();
}