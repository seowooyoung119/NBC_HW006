#include "HW_Drone_GameMode.h"
#include "HW_Drone.h"
#include "HW_Drone_Controller.h"

AHW_Drone_GameMode::AHW_Drone_GameMode()
{
	DefaultPawnClass = AHW_Drone::StaticClass();
	PlayerControllerClass = AHW_Drone_Controller::StaticClass();


}
