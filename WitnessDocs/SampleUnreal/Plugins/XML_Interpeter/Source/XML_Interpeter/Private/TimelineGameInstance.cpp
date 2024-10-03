#include "TimelineGameInstance.h"

//Constructor
UTimelineGameInstance::UTimelineGameInstance()
{
	TimeLineObjectsMap = TMap<FString, AWitnessObject*>();
}
//Destructor
UTimelineGameInstance::~UTimelineGameInstance()
{
	
}
