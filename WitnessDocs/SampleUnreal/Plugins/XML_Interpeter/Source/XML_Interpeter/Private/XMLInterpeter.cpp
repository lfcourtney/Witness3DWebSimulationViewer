#include "XMLInterpeter.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/TextBuffer.h"

void AUxmlInterpeter::SimulateNavigation() const
{
	
	
	UE_LOG(LogTemp, Display, TEXT("Simulating!!"));
	const float Deltatime = GetWorld()->DeltaTimeSeconds;
	const int Length = GameInstancePtr->NavigationTasks.Num();
	
	for(int i = 0; i < Length; i++)
	{
		FNavigationTask CurrentTask = GameInstancePtr->NavigationTasks[i];

		if(CurrentTask.NavigationProcess >= 1 || !CurrentTask.Object)
		{
			if(CurrentTask.Object)
			{
				CurrentTask.Object->SetActorLocation(CurrentTask.End);
				CurrentTask.Object->SetActorRotation(CurrentTask.EndRot);
			}
			GameInstancePtr->NavigationTasks.RemoveAt(i);
			break;
		}
		
		if(CurrentTask.NavigationProcess == 0)
		{
			CurrentTask.Object->SetActorLocation(CurrentTask.Start);
		}
		
		CurrentTask.NavigationProcess = CurrentTask.NavigationProcess + ((CurrentTask.NavigationSpeed * Deltatime) / 10);
		const FVector CalculatedPosition = FMath::LerpStable(CurrentTask.Start, CurrentTask.End, CurrentTask.NavigationProcess);
		if(!CurrentTask.StartRot.Equals(CurrentTask.EndRot))
		{
			FRotator TargetRotation;
			const float RotDifference = GetRotationDifference(CurrentTask.StartRot,CurrentTask.EndRot);
			if(RotDifference <= 5 || RotDifference >= 330)
			{
				CurrentTask.Object->SetActorRotation(CurrentTask.EndRot);
			}
			else
			{
				TargetRotation = FMath::RInterpTo(CurrentTask.Object->GetActorRotation(),CurrentTask.EndRot,
															 Deltatime,10);
			}
			
			
			CurrentTask.Object->SetActorRotation(TargetRotation);
		}
		CurrentTask.Object->SetActorLocation(CalculatedPosition);

		GameInstancePtr->NavigationTasks[i] = CurrentTask;
	}
}

float AUxmlInterpeter::GetRotationDifference(const FRotator A, const FRotator B)
{
	const float RollDifference = A.Roll - B.Roll;
	const float PitchDifference = A.Pitch - B.Pitch;
	const float YawDifference = A.Yaw - B.Yaw;
	const float TotalDifference = RollDifference + PitchDifference + YawDifference;
	return FMath::Abs(TotalDifference);
}


void AUxmlInterpeter::Init(FString Path)
{
	GameInstancePtr = Cast<UTimelineGameInstance>(WorldContext->GetWorld()->GetGameInstance());
	if(GameInstancePtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Game Instance!"));
	}
	LoadXMLFile(Path);
	CreateTimeStamps(Actions);
	GetWorld()->GetTimerManager().SetTimer(SimulateTimerHandle, this, &AUxmlInterpeter::SimulateNavigation, 0.005, true);
}

bool AUxmlInterpeter::LoadXMLFile(FString path)
{
	if(path == "") { path = FPaths::ProjectContentDir() + "/XML/WitnessRun.xml"; }
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/WitnessRun.xml"; }
	
	XmlFile = new FXmlFile();
	if(XmlFile->LoadFile(path) == false) { UE_LOG(LogTemp,Error, TEXT("Failed to load file"));  return false; }
	if(XmlFile->IsValid() == false) { return false; }
	bool bIsInBuild = true;

	GetActions(XmlFile->GetRootNode()); // get all nodes and create actions
	for (int i = 0; i < Actions.Num(); i++)
	{
		UAction* CurrentAction = Actions[i];
		if(bIsInBuild == true)
		{
			UEndAction* EndCheck = static_cast<UEndAction*>(CurrentAction);
			if(EndCheck)
			{
				if(EndCheck->Type == EStartType::Build)
				{
					bIsInBuild = false;
					Initialize.Add(CurrentAction);
				}
			}
			else{ Initialize.Add(CurrentAction); }
		}
		else
		{
			if(Actions[i]->Time < Actions[i-1]->Time)
				Actions[i]->Time = Actions[i-1]->Time;
			
			Update.Add(CurrentAction);
		}
	}
	return true;
}

void AUxmlInterpeter::CreateTimeStamps(TArray<UAction*> _Actions)
{
	UTimeFrame* StartFrame = NewObject<UTimeFrame>();
	StartFrame->Time = 0;
			
	TimeFrames.Add(0, StartFrame);
	for (auto Action : _Actions)
	{
		if(TimeFrames.Contains(Action->Time) == false)
		{
			UTimeFrame* NewFrame = NewObject<UTimeFrame>();
			NewFrame->Time = Action->Time;
			TimeFrames.Add(Action->Time, NewFrame);
		}
		TimeFrames[Action->Time]->Actions.Add(Action);
	}
}

void AUxmlInterpeter::GetActions(FXmlNode* CurrentNode)
{
	TArray<FXmlNode*> Nodes = CurrentNode->GetChildrenNodes();

	for (auto Node : Nodes)
	{
		UAction* Action = nullptr; //define action
		FString CurrentTag = Node->GetTag();
		
		
		if(CurrentTag == "start")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			UStartAction* StartAction = NewObject<UStartAction>();
			
			FString Convert = Node->GetAttribute("time");
			Convert = Convert.Replace(TEXT("."),TEXT(""));
			StartAction->Time = FCString::Atoi(*Convert);
			
			FString StartType = Node->GetAttribute("type");
			if(StartType == "session") { StartAction->Type = EStartType::Session; }
			else if(StartType == "build") { StartAction->Type = EStartType::Build; }
			else if(StartType == "run") { StartAction->Type = EStartType::Run; }
			else { UE_LOG(LogTemp, Error, TEXT("Could not Parse StartType: %s"), *StartType); }

			Action = StartAction;
		}
		else if(CurrentTag == "version")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			//Set Version Number
			ModelVersion = FCString::Atof(*Node->GetAttribute("number"));
			//UE_LOG(LogTemp, Warning, TEXT("ModelVersion: %f"), ModelVersion);
		}
		else if(CurrentTag == "new")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			//Set Model Name
			ModelName = Node->GetAttribute("model");
			//UE_LOG(LogTemp, Display, TEXT("ModelName: %s"), *ModelName);
		}
		else if(CurrentTag == "load")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			ULoadAction* LoadAction = NewObject<ULoadAction>();
			
			LoadAction->ModelName = Node->GetAttribute("geometry");
			Action = LoadAction;
		}
		else if(CurrentTag == "create")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			UCreateAction* CreateAction = NewObject<UCreateAction>();

			TArray<FXmlAttribute> NodeAttributes = Node->GetAttributes();
			
			for (auto SubNode : Node->GetChildrenNodes())
			{
				FString subTag = SubNode->GetTag();
				if(subTag == "queueInfo")
				{
					TArray<FXmlAttribute> SubNodeAttributes = SubNode->GetAttributes();
					UQueueInfo* QueueInfo = NewObject<UQueueInfo>();
					//UE_LOG(LogTemp, Error, TEXT("%s"), *subTag);
					for (auto SubNodeAttribute : SubNodeAttributes)
					{
						if(SubNodeAttribute.GetTag() == "queueParent")
						{ QueueInfo->QueueParent = SubNodeAttribute.GetValue(); }
					}

					for (auto QueueNode : SubNode->GetChildrenNodes())
					{
						FString QueueTag = QueueNode->GetTag();
						if(QueueTag == "behaviour")
						{
							for (auto QueueAttribute : QueueNode->GetAttributes())
							{
								if(QueueAttribute.GetTag() == "partPositioning")
								{
									QueueInfo->PartRotation = FRotator();
									if(QueueAttribute.GetValue() == "partOver") { QueueInfo->PartPositioning = EPartPositioning::PartOver;}
									else if(QueueAttribute.GetValue() == "partUnder") { QueueInfo->PartPositioning = EPartPositioning::PartUnder;}
									else { UE_LOG(LogTemp, Error, TEXT("Failed to parse Partposition: %s"), *QueueAttribute.GetValue()); }
								}
								else if(QueueAttribute.GetTag() == "partRoll") { QueueInfo->PartRotation.Roll = FCString::Atof(*QueueAttribute.GetValue()); }
								else if(QueueAttribute.GetTag() == "partPitch") { QueueInfo->PartRotation.Pitch = FCString::Atof(*QueueAttribute.GetValue()); }
								else if(QueueAttribute.GetTag() == "partYaw") { QueueInfo->PartRotation.Yaw = FCString::Atof(*QueueAttribute.GetValue()); }
								else { UE_LOG(LogTemp, Error, TEXT("Failed to parse QueueAttribute: %s"), *QueueAttribute.GetTag()); } 
							}
						}
						else if(QueueTag == "position")
						{
							for (auto QueueAttribute : QueueNode->GetAttributes())
							{
								if(QueueAttribute.GetTag() == "x") { QueueInfo->Position.X = FCString::Atof(*QueueAttribute.GetValue()); }
								else if(QueueAttribute.GetTag() == "y") { QueueInfo->Position.Y = FCString::Atof(*QueueAttribute.GetValue()); }
								else if(QueueAttribute.GetTag() == "z") { QueueInfo->Position.Z = FCString::Atof(*QueueAttribute.GetValue()); }
								else { UE_LOG(LogTemp, Error, TEXT("Failed to parse QueueAttribute: %s"), *QueueAttribute.GetTag()); } 
							}
						}
						else if(QueueTag == "direction")
						{
							for (auto QueueAttribute : QueueNode->GetAttributes())
							{
								if(QueueAttribute.GetTag() == "dx") { QueueInfo->Direction.X = FCString::Atof(*QueueAttribute.GetValue()); }
								else if(QueueAttribute.GetTag() == "dy") { QueueInfo->Direction.Y = FCString::Atof(*QueueAttribute.GetValue()); }
								else if(QueueAttribute.GetTag() == "dz") { QueueInfo->Direction.Z = FCString::Atof(*QueueAttribute.GetValue()); }
								else { UE_LOG(LogTemp, Error, TEXT("Failed to parse QueueAttribute: %s"), *QueueAttribute.GetTag()); } 
							}
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Failed to parse QueueTag: %s"), *QueueTag);
						}
					}
					CreateAction->CreateData.Add(QueueInfo);
					//UE_LOG(LogTemp, Display, TEXT("QueueInfo: %s"), *QueueInfo); //TODO Print string
				}
				else if(subTag == "path")
				{
					//UE_LOG(LogTemp, Warning, TEXT("%s"), *subTag);
					UPath* Path = NewObject<UPath>();

					CreateAction->ObjectType = ObjectType::Path;
					
					TArray<FXmlAttribute> SubNodeAttributes = SubNode->GetAttributes();
					for (auto SubNodeAttribute : SubNodeAttributes)
					{
						//UE_LOG(LogTemp, Warning, TEXT("SubNode Value of path %s: %s"),*SubNodeAttribute.GetTag() ,*SubNodeAttribute.GetValue());

						if(SubNodeAttribute.GetTag() == "startX") { Path->Startlocation.X = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "startY") { Path->Startlocation.Y = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "startZ") { Path->Startlocation.Z = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "width") { Path->width = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else { UE_LOG(LogTemp, Error, TEXT("Failed to parse QueueAttribute: %s"), *SubNodeAttribute.GetTag()); } 
					}
					TArray<FXmlNode*> PathNodes = SubNode->GetChildrenNodes();
					for (auto PathNode : PathNodes)
					{
						UPathData* CurrentData = nullptr;
						if(PathNode->GetTag() == "line")
						{
							ULine* Line = NewObject<ULine>();
							TArray<FXmlAttribute> LineAttributes = PathNode->GetAttributes();
							for (auto LineAttribute : LineAttributes)
							{
								//UE_LOG(LogTemp, Warning, TEXT("LineAttribute Value of Line %s: %s"),*LineAttribute.GetTag() ,*LineAttribute.GetValue());
								if(LineAttribute.GetTag() == "startX") { Line->Start.X = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "startY") { Line->Start.Y = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "startZ") { Line->Start.Z = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "endX") { Line->End.X = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "endY") { Line->End.Y = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "endZ") { Line->End.Z = FCString::Atof(*LineAttribute.GetValue()); }
								else { UE_LOG(LogTemp, Error, TEXT("Failed to parse LineAttribute: %s"), *LineAttribute.GetTag()); }
							}
							CurrentData = Line;
						}
						else if(PathNode->GetTag() == "arc")
						{
							UArc* Arc = NewObject<UArc>();
							TArray<FXmlAttribute> ArcAttributes = PathNode->GetAttributes();
							for (auto ArcAttribute : ArcAttributes)
							{
								//UE_LOG(LogTemp, Warning, TEXT("ArcAttribute Value of Arc %s: %s"),*ArcAttribute.GetTag() ,*ArcAttribute.GetValue());
								
								if(ArcAttribute.GetTag() == "startX") { Arc->Start.X = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "startY") { Arc->Start.Y = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "startZ") { Arc->Start.Z = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "endX") { Arc->End.X = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "endY") { Arc->End.Y = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "endZ") { Arc->End.Z = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "centreX") { Arc->Centre.X = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "centreY") { Arc->Centre.Y = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "centreZ") { Arc->Centre.Z = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "angle") { Arc->Angle = FCString::Atof(*ArcAttribute.GetValue()); }
								else if(ArcAttribute.GetTag() == "sweepDirection") { Arc->bIsClockwise = (ArcAttribute.GetValue() == "clockwise") ? true: false; }
								else { UE_LOG(LogTemp, Error, TEXT("Failed to parse ArcAttribute: %s"), *ArcAttribute.GetTag()); }
							}
							CurrentData = Arc;
						}
						
						if(Path != nullptr)
						{
							Path->PathData.Add(CurrentData);
						}
						
					}
					CreateAction->CreateData.Add(Path);
					//UE_LOG(LogTemp, Display, TEXT("Path: %s"), *Path); //TODO Print string
				}
				else if(subTag == "extrude")
				{
					//UE_LOG(LogTemp, Warning, TEXT("%s"), *subTag);
					UExtrude* Extrude = NewObject<UExtrude>();

					CreateAction->ObjectType = ObjectType::Extrude;
					
					TArray<FXmlAttribute> SubNodeAttributes = SubNode->GetAttributes();
					for (auto SubNodeAttribute : SubNodeAttributes)
					{
						//UE_LOG(LogTemp, Warning, TEXT("SubNode Value of extrude %s: %s"),*SubNodeAttribute.GetTag() ,*SubNodeAttribute.GetValue());
						if(SubNodeAttribute.GetTag() == "width") { Extrude->Width = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "height") { Extrude->Height = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "texture") { Extrude->Texture = SubNodeAttribute.GetValue(); }
						else { UE_LOG(LogTemp, Error, TEXT("Failed to parse ExtrudeAttribute: %s"), *SubNodeAttribute.GetTag()); } 
					}
					TArray<FXmlNode*> ExtrudeNodes = SubNode->GetChildrenNodes();
					for (auto ExtrudeNode : ExtrudeNodes)
					{
						if(ExtrudeNode->GetTag() == "line")
						{
							ULine* Line = NewObject<ULine>();
							TArray<FXmlAttribute> LineAttributes = ExtrudeNode->GetAttributes();
							for (auto LineAttribute : LineAttributes)
							{
								//UE_LOG(LogTemp, Warning, TEXT("Extrude Value of Line %s: %s"),*LineAttribute.GetTag() ,*LineAttribute.GetValue());
								if(LineAttribute.GetTag() == "startX") { Line->Start.X = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "startY") { Line->Start.Y = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "startZ") { Line->Start.Z = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "endX") { Line->End.X = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "endY") { Line->End.Y = FCString::Atof(*LineAttribute.GetValue()); }
								else if(LineAttribute.GetTag() == "endZ") { Line->End.Z = FCString::Atof(*LineAttribute.GetValue()); }
								else { UE_LOG(LogTemp, Error, TEXT("Failed to parse LineAttribute: %s"), *LineAttribute.GetTag()); }
							}
							Extrude->Line = Line;
						}
					}
					CreateAction->CreateData.Add(Extrude);
					//UE_LOG(LogTemp, Display, TEXT("Extrude: \n Width: %f, Height: %f, Line: from %s to %s"), Extrude->Width, Extrude->Height, *Extrude->Line->Start.ToString(), *Extrude->Line->End.ToString());
				}
				else if(subTag == "surface")
				{
					//UE_LOG(LogTemp, Warning, TEXT("%s"), *subTag);
					USurface* Surface = NewObject<USurface>();

					CreateAction->ObjectType = ObjectType::Surface;
					
					TArray<FXmlAttribute> SubNodeAttributes = SubNode->GetAttributes();
					for (auto SubNodeAttribute : SubNodeAttributes)
					{
						//UE_LOG(LogTemp, Warning, TEXT("SubNode Value of extrude %s: %s"),*SubNodeAttribute.GetTag() ,*SubNodeAttribute.GetValue());
						
						if(SubNodeAttribute.GetTag() == "x1") { Surface->StartLoc.X = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "y1") { Surface->StartLoc.Z = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "z1") { Surface->StartLoc.Y = FCString::Atof(*SubNodeAttribute.GetValue()); }

						else if(SubNodeAttribute.GetTag() == "x2") { Surface->EndLoc.X = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "y2") { Surface->EndLoc.Z = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "z2") { Surface->EndLoc.Y = FCString::Atof(*SubNodeAttribute.GetValue()); }

						else if(SubNodeAttribute.GetTag() == "normalX") { Surface->Normal.X = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "normalY") { Surface->Normal.Z = FCString::Atof(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "normalZ") { Surface->Normal.Y = FCString::Atof(*SubNodeAttribute.GetValue()); }

						else if(SubNodeAttribute.GetTag() == "color1") { Surface->color1 = FColor::FromHex(*SubNodeAttribute.GetValue()); }
						else if(SubNodeAttribute.GetTag() == "color2") { Surface->color2 = FColor::FromHex(*SubNodeAttribute.GetValue()); }

						else if(SubNodeAttribute.GetTag() == "singleSided") { Surface->bIsSingleSided = (SubNodeAttribute.GetValue() == "True") ? true: false; }
						
						else { UE_LOG(LogTemp, Error, TEXT("Failed to parse SurfaceAttribute: %s"), *SubNodeAttribute.GetTag()); } 
					}
					CreateAction->CreateData.Add(Surface);
					//UE_LOG(LogTemp, Display, TEXT("Extrude: \n Width: %f, Height: %f, Line: from %s to %s"), Extrude->Width, Extrude->Height, *Extrude->Line->Start.ToString(), *Extrude->Line->End.ToString());
				} else if(subTag == "text")
				{
					
					UE_LOG(LogTemp,Warning,TEXT("Has Text"));
					
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Could not parse SubTag: %s"), *SubNode->GetTag());
				}
			}

			for (auto NodeAttribute : NodeAttributes)
			{
				FString Attribute = NodeAttribute.GetTag();
				//UE_LOG(LogTemp, Display, TEXT("%s: %s"), *NodeAttribute.GetTag(), *NodeAttribute.GetValue());
				if(Attribute == "time")
				{
					FString Convert = NodeAttribute.GetValue().Replace(TEXT("."),TEXT(""));
					CreateAction->Time = FCString::Atoi(*Convert);
				}
				else if(Attribute == "geometry")
				{
					CreateAction->Geometry = NodeAttribute.GetValue();
				}
				else if(Attribute == "instanceName") { CreateAction->InstanceName = NodeAttribute.GetValue(); }
				else { UE_LOG(LogTemp, Error, TEXT("Could not parse: %s"), *Attribute); }
			}

			//UE_LOG(LogTemp, Warning, TEXT("Action Name: %s | %S"), *CreateAction->Geometry, *CreateAction->InstanceName);
			//UE_LOG(LogTemp, Warning, TEXT("Create Time: %f"), CreateAction->Time);
			
			Action = CreateAction;
		}
		else if(CurrentTag == "update")
		{
			UUpdateAction* UpdateAction = NewObject<UUpdateAction>();
			
			FString Convert = Node->GetAttribute("time").Replace(TEXT("."),TEXT(""));
			UpdateAction->Time = FCString::Atoi(*Convert);
			
			UpdateAction->InstanceName = Node->GetAttribute("instanceName");
			UpdateAction->bIsVisible = Node->GetAttribute("visible") == "true" ? true : false;

			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s on %s."), *CurrentTag, *UpdateAction->InstanceName);

			for (auto SubNode : Node->GetChildrenNodes())
			{
				if(SubNode->GetTag() == "translate")
				{
					UTranslate* Translate = NewObject<UTranslate>();

					Translate->Translation.X = FCString::Atof(*SubNode->GetAttribute("x"));
					Translate->Translation.Y = FCString::Atof(*SubNode->GetAttribute("y"));
					Translate->Translation.Z = FCString::Atof(*SubNode->GetAttribute("z"));

					//UE_LOG(LogTemp, Warning, TEXT("Translation: %s"), *Translate->Translation.ToString());
					
					UpdateAction->UpdateData.Add(Translate);
				}
				else if(SubNode->GetTag() == "rotate")
				{
					URotate* Rotate = NewObject<URotate>();

					Rotate->RotationChange.X = FCString::Atof(*SubNode->GetAttribute("x"));
					Rotate->RotationChange.Y = FCString::Atof(*SubNode->GetAttribute("y"));
					Rotate->RotationChange.Z = FCString::Atof(*SubNode->GetAttribute("z"));

					//UE_LOG(LogTemp, Warning, TEXT("RotationChange: %s"), *Rotate->RotationChange.ToString());
					
					UpdateAction->UpdateData.Add(Rotate);
				}
				else if(SubNode->GetTag() == "scale")
				{
					UScale* Scale = NewObject<UScale>();

					Scale->ScaleChange.X = FCString::Atof(*SubNode->GetAttribute("x"));
					Scale->ScaleChange.Y = FCString::Atof(*SubNode->GetAttribute("y"));
					Scale->ScaleChange.Z = FCString::Atof(*SubNode->GetAttribute("z"));

					//UE_LOG(LogTemp, Warning, TEXT("ScaleChange: %s"), *Scale->ScaleChange.ToString());
					
					UpdateAction->UpdateData.Add(Scale);
				}
				else if(SubNode->GetTag() == "partPosition")
				{
					UPartPosition* PartPosition = NewObject<UPartPosition>();

					PartPosition->InstanceName = SubNode->GetAttribute("instanceName");
					PartPosition->Position = FCString::Atof(*SubNode->GetAttribute("position"));
					PartPosition->bIsReverse = Node->GetAttribute("reverse") == "true" ? true : false;

					//UE_LOG(LogTemp, Warning, TEXT("PartPosition for part: %s. position: %f."), *PartPosition->InstanceName, PartPosition->Position);
					
					UpdateAction->UpdateData.Add(PartPosition);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Could not parse UpdateTag: %s"), *SubNode->GetTag());
				}
			}

			Action = UpdateAction;
		}
		else if(CurrentTag == "delete")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			UDeleteAction* DeleteAction = NewObject<UDeleteAction>();

			FString Convert = Node->GetAttribute("time").Replace(TEXT("."),TEXT(""));
			DeleteAction->Time = FCString::Atoi(*Convert);
			
			DeleteAction->InstanceName = Node->GetAttribute("instanceName");

			Action = DeleteAction;
		}
		else if(CurrentTag == "end")
		{
			//UE_LOG(LogTemp, Error, TEXT("Succesfully Called %s"), *CurrentTag);
			UEndAction* EndAction = NewObject<UEndAction>();

			FString Convert = Node->GetAttribute("time").Replace(TEXT("."),TEXT(""));
			EndAction->Time = FCString::Atoi(*Convert);
			
			FString EndType = Node->GetAttribute("type");
			if(EndType == "session") { EndAction->Type = EStartType::Session; }
			else if(EndType == "build") { EndAction->Type = EStartType::Build; }
			else if(EndType == "run") { EndAction->Type = EStartType::Run; }
			else { UE_LOG(LogTemp, Error, TEXT("Could not Parse EndType: %s"), *EndType); }

			Action = EndAction;
		}
		
		if(Action == nullptr) { UE_LOG(LogTemp, Error, TEXT("Failed to get current Action: %s"), *CurrentTag); }
		else
		{
			//Action->Time += 1000000;
			Action->GameInstance = GameInstancePtr;
			Actions.Add(Action);
			//UE_LOG(LogTemp, Error, TEXT("ActionTime: %d"), Action->Time);
		}
	}

}

int AUxmlInterpeter::GetIndexFromTime(int64 time)
{
	for (int i = 0; i < Update.Num(); i++)
	{
		if(time == Update[i]->Time)
		{
			return i;
		}
	}
	return 0;
}

void AUxmlInterpeter::StartTimeline()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Timeline"));
	
	TArray<UTimeFrame*> Frames;
	TimeFrames.GenerateValueArray(Frames);

	if(Frames[0] == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Frames are faulty"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Got Frames"));
	
	UTimeFrame* StartTimeFrame = Frames[0];
	TArray<UAction*> CurrentActions = StartTimeFrame->Actions;

	if(CurrentActions.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No Actions"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Got Actions"));
	
	for (int  i = 0; i < CurrentActions.Num(); i++)
	{
		UCreateAction* CreateAction = Cast<UCreateAction>(CurrentActions[i]);
		UUpdateAction* UpdateAction = Cast<UUpdateAction>(CurrentActions[i]);
		if(CreateAction != nullptr)
		{
			AActor* Object = SpawnObject(CreateAction->ObjectType, CreateAction->Transform);
			if(Object != nullptr)
			{
				CreateAction->WitnessObject = Cast<AWitnessObject>(Object);
				
				GameInstancePtr->TimeLineObjectsMap.Add(CreateAction->InstanceName, CreateAction->WitnessObject);
				//ObjectMap.Add(CreateAction->InstanceName, CreateAction->WitnessObject);
				
				/*for (auto Key : GetKeys())
				{
					UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Key);
				}*/
				
				if(CreateAction->WitnessObject != nullptr)
				{
					//UE_LOG(LogTemp, Error, TEXT("Trying to execute using object"));
					AMachineObject* MachineObject = Cast<AMachineObject>(CreateAction->WitnessObject);
					if(MachineObject != nullptr)
					{
						//UE_LOG(LogTemp, Error, TEXT("Geometry Value: %s"), *CreateAction->Geometry);
						MachineObject->GeometryName = CreateAction->Geometry;
						FString MeshPath = GetStaticMeshPath(GeometryPathToName(MachineObject->GeometryName));
						MachineObject->ContentPath = MeshPath;
					}
					CreateAction->Execute();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to cast object"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn object"));
			}
			
		}
		else if(UpdateAction != nullptr)
		{
			/*for (auto Key : GetKeys())
			{
				UE_LOG(LogTemp, Warning, TEXT("Key Before Update: %s"), *Key);
			}*/
			AWitnessObject* UpdateObject = GetWitnessObjectFromName(UpdateAction->InstanceName);
			if(UpdateObject == nullptr) { UE_LOG(LogTemp, Error, TEXT("Actor pointer is not whole before execute")); }
			UpdateAction->Execute(UpdateObject);
		}
		else
		{
			CurrentActions[i]->Execute();
		}
	}
				
	/*for (auto Key : GetKeys())
	{
		UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Key);
	}*/
}

void AUxmlInterpeter::UpdateTimeline()
{
	TArray<int> Keys;
	TimeFrames.GenerateKeyArray(Keys);
	
	CurrentTimeIndex++;
	
	if(CurrentTimeIndex >= Keys.Num())
	{
	    return;
	}

	CurrentTime = Keys[CurrentTimeIndex];

	UE_LOG(LogTemp, Warning, TEXT("Update Timeline at Time: %d"), CurrentTime);

	TArray<UTimeFrame*> Frames;
	TimeFrames.GenerateValueArray(Frames);
	
	UTimeFrame* CurrentTimeFrame = Frames[CurrentTimeIndex];

	for (auto Action : CurrentTimeFrame->Actions)
	{
		UCreateAction* CreateAction = Cast<UCreateAction>(Action);
		UUpdateAction* UpdateAction = Cast<UUpdateAction>(Action);
		UDeleteAction* DeleteAction = Cast<UDeleteAction>(Action);
		if(CreateAction != nullptr)
		{
			AActor* Object = SpawnObject(CreateAction->ObjectType, CreateAction->Transform);
			if(Object != nullptr)
			{
				CreateAction->WitnessObject = Cast<AWitnessObject>(Object);
				GameInstancePtr->TimeLineObjectsMap.Add(CreateAction->InstanceName, CreateAction->WitnessObject);
				//ObjectMap.Add(CreateAction->InstanceName, CreateAction->WitnessObject);
				
				/*for (auto Key : GetKeys())
				{
					UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Key);
				}*/
				
				if(CreateAction->WitnessObject != nullptr)
				{
					//UE_LOG(LogTemp, Error, TEXT("Trying to execute using object"));
					AMachineObject* MachineObject = Cast<AMachineObject>(CreateAction->WitnessObject);
					if(MachineObject != nullptr)
					{
						//UE_LOG(LogTemp, Error, TEXT("Geometry Value: %s"), *CreateAction->Geometry);
						MachineObject->GeometryName = CreateAction->Geometry;
						FString MeshPath = GetStaticMeshPath(GeometryPathToName(MachineObject->GeometryName));
						MachineObject->ContentPath = MeshPath;
					}
					CreateAction->Execute();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to cast object"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn object"));
			}
			
		}
		else if(UpdateAction != nullptr)
		{
			/*for (auto Key : GetKeys())
			{
				UE_LOG(LogTemp, Error, TEXT("Key Before Update: %s"), *Key);
			}*/
			AWitnessObject* UpdateObject = GetWitnessObjectFromName(UpdateAction->InstanceName);
			if(UpdateObject == nullptr) { UE_LOG(LogTemp, Error, TEXT("Actor pointer is not whole before execute")); }
			UpdateAction->Execute(UpdateObject);
		}
		else
		{
			Action->Execute();
		}
	}
}

void AUxmlInterpeter::GoToTime(int Time)
{
	//Undo All
	TArray<int> Keys;
	TimeFrames.GenerateKeyArray(Keys);
	
	for(int i = 0; i < Keys.Num(); i++)
	{
		if(Keys[i] == Time)
		{
			CurrentTimeIndex = i;
		}
	}

	CurrentTime = Keys[CurrentTimeIndex];

	for(int i = 0; i <= CurrentTimeIndex; i++)
	{
		TArray<UTimeFrame*> Frames;
		TimeFrames.GenerateValueArray(Frames);
	
		UTimeFrame* CurrentTimeFrame = Frames[i];

		for (auto Action : CurrentTimeFrame->Actions)
		{
			Action->Execute();
		}
	}
}

AActor* AUxmlInterpeter::SpawnObject(ObjectType ToSpawn, FTransform Transform)
{
	const FTransform* _Transform = &Transform;

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.bNoFail = true;

	UWorld* _WorldContext = WorldContext->GetWorld();

	if(_WorldContext == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NoWorldContext"))
		return nullptr;
	}

	switch (ToSpawn)
	{
	case ObjectType::Machine:
		return _WorldContext->SpawnActor(AMachineObject::StaticClass(), _Transform, SpawnParams);

	case ObjectType::Path:
		return _WorldContext->SpawnActor(APathObject::StaticClass(), _Transform, SpawnParams);
		
	case ObjectType::Extrude:
		return _WorldContext->SpawnActor(AExtrudeObject::StaticClass(), _Transform, SpawnParams);
		
	case ObjectType::Surface:
		return _WorldContext->SpawnActor(ASurfaceObject::StaticClass(), _Transform, SpawnParams);

	default:
		return _WorldContext->SpawnActor(AWitnessObject::StaticClass(), _Transform, SpawnParams);
	}
}


FString AUxmlInterpeter::GeometryPathToName(FString GeometryPath)
{
	FString Left, Right;
	GeometryPath.Split(TEXT("\\"),&Left,&Right,ESearchCase::CaseSensitive,ESearchDir::FromEnd);
	return Right;
}

FString AUxmlInterpeter::GetStaticMeshPath(FString MeshName)
{
	if(MeshName.Contains(" "))
	{
		MeshName = MeshName.Replace(TEXT(" "),TEXT("_"));
	}
	
	FString ReturnValue = "/Game/Models/" + MeshName + '/' + MeshName;
	return ReturnValue;
}

UMaterialInstanceDynamic* AUxmlInterpeter::GetMaterial(const FString TextureFilePath)
{
	FString tempMatfp = "/Game/Materials/Mat_Temp";
	const TCHAR* LtempMatfp = *tempMatfp;
	UMaterial* TempMat = LoadObject<UMaterial>(NULL,LtempMatfp);

	UTexture* wantedTexture = GetTexture(TextureFilePath);

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(TempMat,GEngine->GetWorld(),FName("tempmat"));

	DynamicMaterial->SetTextureParameterValue(TEXT("TextureParam"),wantedTexture);
	//DynamicMaterial->SetVectorParameterValue(TEXT("Base Color"),FLinearColor(255,255,255,255));
	return DynamicMaterial;
}

UTexture* AUxmlInterpeter::GetTexture(const FString TextureFilePath)
{
	FString noExtensionString = TextureFilePath.Left(TextureFilePath.Len() - 4);
	FString cutString = noExtensionString.Right(noExtensionString.Len() -80);
	for(int i = 0; i < cutString.Len(); i++)
	{
		const char checkCHar = cutString[i];
		const char testChar = '\\';
		if(checkCHar == testChar)
		{
			cutString[i] = '/';
		}
	}
	FString OriginalTexturefp = "/Game/Materials/" + cutString;

	const TCHAR* LOriginalTexturefp = *OriginalTexturefp;
	
	UTexture* loadedTexture = LoadObject<UTexture>(NULL, LOriginalTexturefp);

	if(loadedTexture == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Couldn't find texture"));
	}
	
	return loadedTexture;
}

void AUxmlInterpeter::GenerateCameras(float Scale,float Height, AActor* WorldContext)
{
	UWorld* world = WorldContext->GetWorld();
	// Check wich surface is larger
	TArray<AActor*> surfaces;
	UGameplayStatics::GetAllActorsOfClass(world,ASurfaceObject::StaticClass(),surfaces);

	ASurfaceObject* mainSurface = nullptr;
	FBoxSphereBounds mainBounds;

	for(auto Currentsurface : surfaces)
	{
		ASurfaceObject* Tempsurface = Cast<ASurfaceObject>(Currentsurface);
		if(mainSurface == nullptr)
		{
			mainSurface = Tempsurface;
			mainBounds = Tempsurface->ProcMesh->Bounds;
			continue;
		}
		else
		{
			// Getting the values for checking
			FBoxSphereBounds TempBounds = Tempsurface->ProcMesh->Bounds;
			float TempXLength = (TempBounds.Origin.X - TempBounds.BoxExtent.X) - (TempBounds.Origin.X + TempBounds.BoxExtent.X);
			float TempYLength = (TempBounds.Origin.Y - TempBounds.BoxExtent.Y) - (TempBounds.Origin.Y + TempBounds.BoxExtent.Y);

			float CheckXLength = (mainBounds.Origin.X - mainBounds.BoxExtent.X) - (mainBounds.BoxExtent.X + mainBounds.BoxExtent.X);
			float CheckYLength = (mainBounds.Origin.Y - mainBounds.BoxExtent.Y) - (mainBounds.BoxExtent.Y + mainBounds.BoxExtent.Y);

			bool XBool = TempXLength < CheckXLength;
			bool YBool = TempYLength < CheckYLength;
			if(XBool && YBool)
			{
				mainSurface = Tempsurface;
				mainBounds = TempBounds;
			}
		}
	}

	// Spawn Cameras
	const FActorSpawnParameters Parameters = FActorSpawnParameters();
	
	// TopView
	float TopViewX = mainBounds.Origin.X;
	float TopViewY = mainBounds.Origin.Y;
	FVector TopView = FVector(TopViewX,TopViewY, Height * 2.5);
	FRotator TopViewRotation = FVector::DownVector.Rotation();
	AActor* topCamera = world->SpawnActor(ACameraActor::StaticClass(), &TopView, &TopViewRotation,
										 Parameters);
	
	

	// BottomRight
	float BottomRightX = mainBounds.Origin.X + mainBounds.BoxExtent.X + Scale;
	float BottomRightY = mainBounds.Origin.Y + mainBounds.BoxExtent.Y + Scale;
	FVector BottomRight = FVector(BottomRightX, BottomRightY, Height);
	FRotator BottomRightRotation = (mainBounds.Origin - BottomRight).Rotation();
	AActor* bottomRightCamera = world->SpawnActor(ACameraActor::StaticClass(), &BottomRight, &BottomRightRotation,
											 Parameters);
	
	// TopRight
	float TopRightX = mainBounds.Origin.X;
	float TopRightY = mainBounds.Origin.Y + mainBounds.BoxExtent.Y + Scale;
	FVector TopRight = FVector(TopRightX, TopRightY,Height);
	FRotator TopRightRotation = (mainBounds.Origin - TopRight).Rotation();
	AActor* topRightCamera = world->SpawnActor(ACameraActor::StaticClass(), &TopRight, &TopRightRotation,
											 Parameters);

	// Top Left
	float TopLeftX = mainBounds.Origin.X - mainBounds.BoxExtent.X - Scale;
	float TopLeftY = mainBounds.Origin.Y + mainBounds.BoxExtent.Y + Scale;
	FVector TopLeft = FVector(TopLeftX,TopLeftY, Height);
	FRotator TopleftRotation = (mainBounds.Origin - TopLeft).Rotation();
	AActor* topleftCamera = world->SpawnActor(ACameraActor::StaticClass(), &TopLeft, &TopleftRotation,
											 Parameters);

	// Bottom Left
	float BottomLeftX = mainBounds.Origin.X - mainBounds.BoxExtent.X - Scale;
	float BottomLeftY = mainBounds.Origin.Y;
	FVector BottomLeft = FVector(BottomLeftX, BottomLeftY, Height);
	FRotator BottomLeftRotation = (mainBounds.Origin - BottomLeft).Rotation();
	AActor* bottomLeftCamera = world->SpawnActor(ACameraActor::StaticClass(), &BottomLeft, &BottomLeftRotation,
										 Parameters);
	
	// West
	float WestX = mainBounds.Origin.X - mainBounds.BoxExtent.X - Scale;
	float WestY = mainBounds.Origin.Y - mainBounds.BoxExtent.Y - Scale;
	FVector West = FVector(WestX,WestY, Height);
	FRotator WestRotation = (mainBounds.Origin - West).Rotation();
	AActor* westCamera = world->SpawnActor(ACameraActor::StaticClass(), &West, &WestRotation,
										 Parameters);

	// North
	float NorthX = mainBounds.Origin.X;
	float NorthY = mainBounds.Origin.Y - mainBounds.BoxExtent.Y - Scale;
	FVector North = FVector(NorthX, NorthY, Height);
	FRotator NorthRotation = (mainBounds.Origin - North).Rotation();
	AActor* northCamera = world->SpawnActor(ACameraActor::StaticClass(), &North, &NorthRotation,
										 Parameters);
	
	// East
	float EastX = mainBounds.Origin.X + mainBounds.BoxExtent.Y + Scale;
	float EastY = mainBounds.Origin.Y - mainBounds.BoxExtent.Y - Scale;
	FVector East = FVector(EastX,EastY, Height);
	FRotator EastRotation = (mainBounds.Origin - East).Rotation();
	AActor* eastCamera = world->SpawnActor(ACameraActor::StaticClass(), &East, &EastRotation,
										 Parameters);

	// South
	float SouthX = mainBounds.Origin.X + mainBounds.BoxExtent.X + Scale;
	float SouthY =mainBounds.Origin.Y;
	FVector South = FVector(SouthX, SouthY, Height);
	FRotator SouthRotation = (mainBounds.Origin - South).Rotation();
	AActor* southCamera = world->SpawnActor(ACameraActor::StaticClass(), &South, &SouthRotation,
										 Parameters);
}

