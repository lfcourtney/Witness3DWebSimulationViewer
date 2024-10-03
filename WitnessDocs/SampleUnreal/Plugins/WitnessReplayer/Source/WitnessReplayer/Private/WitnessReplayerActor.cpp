#include "WitnessReplayerActor.h"

//File picker includes
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
//Witness Entity Includes
#include "Actors/WitnessEntityActor.h"
#include "Actors/WitnessExtrudeActor.h"
#include "Actors/WitnessStaticActor.h"
#include "Actors/WitnessSurfaceActor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SpotLight.h"

AWitnessReplayerActor::AWitnessReplayerActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(*FPaths::Combine(FPaths::ProjectContentDir(), "Blueprints", "2", "BP_Robot_Simple.BP_Robot_Simple")); // This path can be obtained from the editor doing right click + "Copy Reference"
	//static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("Blueprint'/Game/Blueprints/2/BP_Robot_Simple.BP_Robot_Simple'")); // This path can be obtained from the editor doing right click + "Copy Reference"
	mAnimationActorClass = (UClass*) blueprint_finder.Object->GeneratedClass;
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder2(TEXT("Blueprint'/Game/Blueprints/WitnessTextObjectBillboard.WitnessTextObjectBillboard'")); // This path can be obtained from the editor doing right click + "Copy Reference"
	mBlueprintBillboardClass = (UClass*) blueprint_finder2.Object->GeneratedClass;
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder3(TEXT("Blueprint'/Game/Blueprints/BP_AI_Char.BP_AI_Char'")); // This path can be obtained from the editor doing right click + "Copy Reference"
	mBlueprintHumanClass = (UClass*) blueprint_finder3.Object->GeneratedClass;*/
}

void AWitnessReplayerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWitnessReplayerActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!LoadedFile) return;
	
	if(bIsPaused) return;
	
	if(NextTime >= 0 && NextTime <= Duration)
	{
		StepTo(NextTime);
		Time = NextTime;
	}
	NextTime = Time + PlaybackSpeed * PlaybackMultiplier;
	
}

void AWitnessReplayerActor::GetBlueprintClasses(UClass* RobotClass, UClass* HumanClass, UClass* BillboardClass)
{
	mAnimationActorClass = RobotClass;
	mBlueprintHumanClass = HumanClass;
	mBlueprintBillboardClass = BillboardClass;
}

bool AWitnessReplayerActor::LoadXML(FString path)
{
	if(path.Len() <= 0)
	{
		return false;
	}
	
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/Robot x2.w3d"; }
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/Text.w3d"; }
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/WitnessRun.xml"; }
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/RobotConvs.xml"; }
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/Text.w3d"; }
	//if(path == "") { path = FPaths::ProjectContentDir() + "/XML/LiveBuild3.xml"; }
	//if(path == "") { path = FPaths::Combine(FPaths::ProjectContentDir(), "XML", "LiveBuild3.xml"); }

	UE_LOG(LogTemp, Warning, TEXT("Loading XML file: %s"), *path);
	
	//Init
	XmlFile = new FXmlFile();

	CheckTags(path);
	
	if(XmlFile->LoadFile(path) == false) { return false; }
	if(XmlFile->IsValid() == false) { return false; }

	UE_LOG(LogTemp, Warning, TEXT("Loaded XML file: %s"), *path);

	LoadedFile = true;
	
	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return false;

	WitnessActorSpawner = static_cast<AWitnessActorSpawner*>(World->SpawnActor(AWitnessActorSpawner::StaticClass()));
	if(!ensure(WitnessActorSpawner != nullptr)) return false;

	WitnessStaticManager = static_cast<AWitnessStaticManager*>(World->SpawnActor(AWitnessStaticManager::StaticClass()));
	if(!ensure(WitnessStaticManager != nullptr)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Created StaticManager"));
	
	if(WitnessStaticManager->LoadXMLFile(path))
	{
		for (TTuple<FString, UWitnessStatic*> Node : WitnessStaticManager->StaticMap)
		{
			if(Node.Value->Geometry.Contains("RobotArmAnim"))
			{
				AWitnessAnimationActor* SpawnedActor = GetWorld()->SpawnActor<AWitnessAnimationActor>(mAnimationActorClass);

				AnimationMap.Add(Node.Key, SpawnedActor);
				ActorMap.Add(Node.Key, SpawnedActor);
				FVector Location = Node.Value->Translate;
				Location = ActorMap[Node.Key]->TransformLocation(Location- FVector(0,Node.Value->Scale.Y,0));
				ActorMap[Node.Key]->SetActorLocation(Location);
				AnimationMap[Node.Key]->EffectorPosition = FVector(0,0,0);
				AnimationMap[Node.Key]->EffectorRotation = FRotator(0,0,0);
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("Create not implemented yet"));
				AWitnessActor* SpawnedActor = WitnessActorSpawner->SpawnWitnessActor(Node.Key, AWitnessStaticActor::StaticClass());
				if(ensure(SpawnedActor != nullptr))
				{
					ActorMap.Add(Node.Key, SpawnedActor);
					FString Geometry = Node.Value->Geometry;
					if(Geometry.Contains( "dg-lq-Machine")) Geometry = "";
					if(Geometry != "")
					{
						AWitnessStaticActor* StaticActor = (AWitnessStaticActor*)SpawnedActor;
						FString ContentPath = GetStaticMeshPath(GeometryPathToName(Geometry));
						UStaticMesh* meshToUse = Cast<UStaticMesh>(
						StaticLoadObject(UStaticMesh::StaticClass(), NULL, *ContentPath));
						if(meshToUse != nullptr)
						{
							StaticActor->Mesh = meshToUse;
							StaticActor->SetGeometry(StaticActor->Mesh);
						}
					}
				}
				UE_LOG(LogTemp, Error, TEXT("Update not implemented yet"));
				FVector Location; 

				if(Node.Value->Scale.Y < 1.1f && Node.Value->Scale.Y > 0.85f)
				{
					Location = Node.Value->Translate;
				}
				else
				{
					Location = Node.Value->Translate;
					Location.Y = (Location.Y *= Node.Value->Scale.Y);
					float t = 1 - Node.Value->Scale.Y; // -0.3
					float r = 0.925 + t; // 0.7
					Location.Y *= r;
				}
				Location = ActorMap[Node.Key]->TransformLocation(Location);// +Node.Value->QueuePartPosition * 0);// - FVector(0,Node.Value->Scale.Y/2,0));
				ActorMap[Node.Key]->SetActorLocation(Location);
				FVector scale = Node.Value->Scale;
				scale = FVector(scale.X,scale.Z,scale.Y);
				ActorMap[Node.Key]->SetActorScale3D(scale);
				UE_LOG(LogTemp, Error, TEXT("Placed: %s at %s"), *Node.Key, *Location.ToString());
			}
		}
	}
	
	//WitnessPathManager
	WitnessPathManager = static_cast<AWitnessPathManager*>(World->SpawnActor(AWitnessPathManager::StaticClass()));
	if(!ensure(WitnessPathManager != nullptr)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Created PathManager"));

	if(WitnessPathManager->LoadXMLFile(path))
	{
		for (TTuple<FString, UWitnessPath*> Node : WitnessPathManager->PathMap)
		{
			AWitnessActor* SpawnedActor = WitnessActorSpawner->SpawnWitnessActor(Node.Key, AWitnessPathActor::StaticClass());
			if(ensure(SpawnedActor != nullptr))
			{
				ActorMap.Add(Node.Key, SpawnedActor);
			}	
			
			AWitnessPathActor* PathActor = (AWitnessPathActor*)ActorMap[Node.Key];
			PathActor->WitnessPath = Node.Value;
			//FString ContentPath = GetStaticMeshPath(GeometryPathToName(Node.Value->Geometry));
			FString ContentPath;
			if(Node.Key.Contains("Conv"))
			{
				ContentPath = "/Game/Models/Path/Conveyor";
			}
			else if(Node.Key.Contains("Sect"))
			{
				ContentPath = "/Game/Models/Path/Path";
			}
			else
			{
				ContentPath = "";
			}
			UStaticMesh* meshToUse = Cast<UStaticMesh>(
			StaticLoadObject(UStaticMesh::StaticClass(), NULL, *ContentPath));
			if(meshToUse != nullptr)
			{
				PathActor->Mesh = meshToUse;
			}
			PathActor->CreateSpline(100.0f,ESplineMeshAxis::X);
		}
	}

	WitnessAnimationManager = static_cast<AWitnessAnimationManager*>(World->SpawnActor(AWitnessAnimationManager::StaticClass()));
	if(!ensure(WitnessAnimationManager != nullptr)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Created AnimationManager"));
	
	WitnessAnimationManager->LoadXMLFile(path);


	//Witness Text Manager
	WitnessTextManager = static_cast<AWitnessTextManager*>(World->SpawnActor(AWitnessTextManager::StaticClass()));
	if(!ensure(WitnessTextManager != nullptr)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Created WitnessTextManager"));

	if(WitnessTextManager->LoadXMLFile(path))
	{
		Duration = WitnessTextManager->GetDuration();
		NextTime = Time + PlaybackSpeed * PlaybackMultiplier;

		for (TTuple<FString, UWitnessText*> Node : WitnessTextManager->GetEntitiesAtTime(Duration))
		{

			AWitnessTextBillboard* SpawnedActor = GetWorld()->SpawnActor<AWitnessTextBillboard>(mBlueprintBillboardClass);
			BillboardMap.Add(Node.Key,SpawnedActor);

			
			//SpawnedActor->textRender->GetTextRender()->Text = FText::FromString("Halofd");
			SpawnedActor->TextValue =  "Halofd";
			SpawnedActor->SetActorLocation(SpawnedActor->TransformLocation(FVector(Node.Value->Px,Node.Value->Py,Node.Value->Pz )));
			
			
			
		}
	}
	
	WitnessEntityManager = static_cast<AWitnessEntityManager*>(World->SpawnActor(AWitnessEntityManager::StaticClass()));
	if(!ensure(WitnessEntityManager != nullptr)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Created EntityManager"));

	if(WitnessEntityManager->LoadXMLFile(path))
	{
		Duration = WitnessEntityManager->GetDuration();
		NextTime = Time + PlaybackSpeed * PlaybackMultiplier;
		for (TTuple<FString, UWitnessEntity*> Node : WitnessEntityManager->GetEntitiesAtTime(Duration))
		{
			if(Node.Key.Contains("Labor"))
			{
				AWitnessAnimationActor* SpawnedActor = GetWorld()->SpawnActor<AWitnessAnimationActor>(mBlueprintHumanClass);
				LaborMap.Add(Node.Key, SpawnedActor);
			}
			else
			{
				AWitnessActor* SpawnedActor = WitnessActorSpawner->SpawnWitnessActor(Node.Key, AWitnessEntityActor::StaticClass());
				if(ensure(SpawnedActor != nullptr))
				{
					SpawnedActor->InstanceName = Node.Key;
					ActorMap.Add(Node.Key, SpawnedActor);
				}
				AWitnessEntityActor* EntityActor = (AWitnessEntityActor*)SpawnedActor;
		
				//EntityActor->SetActorLocation(FVector(Node.Value->Px, Node.Value->Py, Node.Value->Pz*1.5f));
				EntityActor->SetActorLocation(FVector(Node.Value->Px, Node.Value->Py, Node.Value->Pz));
		
				if(Node.Value->Geometry != "")
				{
					FString ContentPath = GetStaticMeshPath(GeometryPathToName(Node.Value->Geometry));
					UStaticMesh* meshToUse = Cast<UStaticMesh>(
					StaticLoadObject(UStaticMesh::StaticClass(), NULL, *ContentPath));
					if(meshToUse != nullptr)
					{
						EntityActor->StaticMeshComponent->SetVisibility(false);
						EntityActor->SetGeometry(meshToUse);
					}
					if(Node.Value->Geometry.Contains("dg-pt-Part1"))
					{
						SpawnedActor->SetActorScale3D(SpawnedActor->GetActorScale3D()*7.5f);
					}
				}
			}


		}
	}



	//WitnessEnviromentManager
	WitnessEnviromentManager = static_cast<AWitnessEnviromentManager*>(World->SpawnActor(AWitnessEnviromentManager::StaticClass()));
	if(!ensure(WitnessEnviromentManager != nullptr)) return false;

	UE_LOG(LogTemp, Warning, TEXT("Created EnviromentManager"));

	if(WitnessEnviromentManager->LoadXMLFile(path))
	{
		for (TTuple<FString, UWitnessEnviromentObject*> Node : WitnessEnviromentManager->EnviromentMap)
		{
			if(Node.Value->GetClass() == UWitnessExtrude::StaticClass())
			{
				AWitnessActor* SpawnedActor = WitnessActorSpawner->SpawnWitnessActor(Node.Key, AWitnessExtrudeActor::StaticClass());
				if(SpawnedActor != nullptr)
				{
					SpawnedActor->InstanceName = Node.Key;
					ActorMap.Add(Node.Key, SpawnedActor);

					AWitnessExtrudeActor* ExtrudeActor = (AWitnessExtrudeActor*)SpawnedActor;

					ExtrudeActor->WitnessExtrude = (UWitnessExtrude*)Node.Value;

					//UE_LOG(LogTemp, Warning, TEXT("Extrude Start %s"), *ExtrudeActor->WitnessExtrude->StartLocation.ToString());

					UStaticMesh* meshToUse = Cast<UStaticMesh>(
					StaticLoadObject(UStaticMesh::StaticClass(), ExtrudeActor->GetWorld(), TEXT("/Game/Temp/Wall")));

					UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
																		 ExtrudeActor->GetWorld(),
																		 TEXT(
																			 "/Game/Temp/WallMat")));
					UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));

					if(meshToUse != nullptr && Material != nullptr)
					{
						ExtrudeActor->GenerateExtrude(meshToUse, Material);
					}
				}
			}
			else if(Node.Value->GetClass() == UWitnessSurface::StaticClass())
			{
				AWitnessActor* SpawnedActor = WitnessActorSpawner->SpawnWitnessActor(Node.Key, AWitnessSurfaceActor::StaticClass());
				if(SpawnedActor != nullptr)
				{
					SpawnedActor->InstanceName = Node.Key;
					ActorMap.Add(Node.Key, SpawnedActor);

					AWitnessSurfaceActor* SurfaceActor = (AWitnessSurfaceActor*)SpawnedActor;

					SurfaceActor->WitnessSurface = (UWitnessSurface*)Node.Value;

					UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
																		 SurfaceActor->GetWorld(),
																		 TEXT(
																			 "/Game/Temp/FloorMat")));
					UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));

					if(Material != nullptr)
					{
						SurfaceActor->GenerateSurface(Material);
					}
				}
			}
		}
	}
	
	
	return true;
}

void AWitnessReplayerActor::StepTo(int64 _Time)
{
	WitnessEntityManager->StepTo(_Time);
	WitnessAnimationManager->StepTo(_Time);
	WitnessTextManager->StepTo(_Time);

	Time = _Time;
	NextTime = Time + PlaybackSpeed * PlaybackMultiplier;
	for (TTuple<FString, UWitnessEntity*> Node : WitnessEntityManager->GetEntitiesAtCurrentTimeInterpolated())
	{
		UWitnessEntity* Entity = Node.Value;
		if(ActorMap.Contains(Node.Key))
		{
			AWitnessActor* WitnessActor = ActorMap[Node.Key];
		
			if(ActorMap.Contains(Entity->PartPositionInstanceName))
			{
				AWitnessActor* partPositionActor = ActorMap[Entity->PartPositionInstanceName];
				if(partPositionActor->GetClass() == AWitnessEntityActor::StaticClass())
				{
					AWitnessEntityActor* EntityActor = (AWitnessEntityActor*)partPositionActor;
					WitnessActor->AttachToActor(EntityActor, FAttachmentTransformRules::KeepRelativeTransform);
					WitnessActor->SetActorRelativeLocation(FVector(0,0, 0));
					//if(EntityActor->StaticMeshComponent->GetStaticMesh() != nullptr)
						//WitnessActor->SetActorRelativeLocation(FVector(0,0, +EntityActor->StaticMeshComponent->GetStaticMesh()->GetExtendedBounds().SphereRadius));
						//WitnessActor->SetActorRelativeLocation(FVector(0,0, EntityActor->StaticMeshComponent->GetStaticMesh()->GetExtendedBounds().SphereRadius * 2));
				}
				else if (partPositionActor->GetClass() == AWitnessPathActor::StaticClass())
				{
					AWitnessPathActor* PathActor = (AWitnessPathActor*)ActorMap[Entity->PartPositionInstanceName];
					if(PathActor != nullptr)
					{
						//UE_LOG(LogTemp, Error, TEXT("PathActor-InstanceName: %s"), *Entity->PartPositionInstanceName);
						if(PathActor->WitnessPath != nullptr)
						{
							float distance = PathActor->SplineComponent->GetSplineLength() * Entity->PartPosition;
							//FVector Location = PathActor->SplineComponent->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
							FVector Location = PathActor->TransformLocation(PathActor->WitnessPath->GetPointAt(Entity->PartPosition));
							//Location.Z += PathActor->Mesh->GetExtendedBounds().BoxExtent.Z * 2.0f;
							WitnessActor->SetActorLocation(Location);
							WitnessActor->AttachToActor(PathActor, FAttachmentTransformRules::KeepRelativeTransform);

							FVector AngleLocation = PathActor->TransformLocation(PathActor->WitnessPath->GetPointAt(Entity->PartPosition - 0.01f));
							//FVector AngleLocation = PathActor->SplineComponent->GetLocationAtDistanceAlongSpline(distance - 1, ESplineCoordinateSpace::World);
							FVector ForwardAxis = (Location - AngleLocation).GetSafeNormal();
							
							FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PathActor->TransformLocation(PathActor->WitnessPath->GetPointAt(Entity->PartPosition)), AngleLocation);
							//FRotator NewRotation = UKismetMathLibrary::MakeRotationFromAxes(ForwardAxis, WitnessActor->GetActorRightVector(), WitnessActor->GetActorUpVector());
							//NewRotation = FRotator(0, NewRotation.Yaw, 0); // +90 degrees on Yaw when needed
							WitnessActor->SetActorRotation(NewRotation);
							if(PathActor->InstanceName.Contains("Conv"))
								WitnessActor->SetActorLocation(Location+WitnessActor->GetActorUpVector()*PathActor->Mesh->GetExtendedBounds().SphereRadius);
							if(PathActor->InstanceName.Contains("Sect"))
								WitnessActor->SetActorLocation(Location-WitnessActor->GetActorUpVector()*PathActor->Mesh->GetExtendedBounds().SphereRadius);
								//WitnessActor->SetActorLocation(Location+WitnessActor->GetActorUpVector()*PathActor->Mesh->GetExtendedBounds().SphereRadius * 2);


							for (TTuple<FString, AWitnessAnimationActor*> n : LaborMap)
							{
								if((Location - n.Value->GetActorLocation()).SquaredLength() < 10*10)
								{
									if(!WitnessActor->IsAttachedTo(n.Value))
									{
										WitnessActor->AttachToActor(n.Value, FAttachmentTransformRules::SnapToTargetIncludingScale);
										//WitnessActor->SetActorLocation(n.Value->EffectorPosition);
										//WitnessActor->SetActorRelativeLocation(n.Value->EffectorPosition - n.Value->GetActorLocation());
										WitnessActor->SetActorRotation(n.Value->EffectorRotation);
										n.Value->AttachedActor = WitnessActor;
									}
								}
								else if(WitnessActor->IsAttachedTo(n.Value)) WitnessActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
							}
						}

						/*if(entity ->contains interface == true)
						{
							play animation depending on distance along spline;
						}*/
					}
				}
				else if (partPositionActor->GetClass() == AWitnessStaticActor::StaticClass())
				{
					AWitnessStaticActor* StaticActor = (AWitnessStaticActor*)partPositionActor;
					WitnessActor->AttachToActor(StaticActor, FAttachmentTransformRules::KeepRelativeTransform);
					//if(StaticActor->StaticMeshComponent->GetStaticMesh() != nullptr)
						//WitnessActor->SetActorRelativeLocation(FVector(0,0, StaticActor->StaticMeshComponent->GetStaticMesh()->GetExtendedBounds().SphereRadius));
						//WitnessActor->SetActorRelativeLocation(FVector(0,0, StaticActor->StaticMeshComponent->GetStaticMesh()->GetExtendedBounds().SphereRadius * 2));
				}
				else if (partPositionActor->GetClass() == mAnimationActorClass)//AWitnessAnimationActor::StaticClass())
				{
					AWitnessAnimationActor* AnimationActor = (AWitnessAnimationActor*)partPositionActor;
					WitnessActor->AttachToActor(AnimationActor, FAttachmentTransformRules::KeepRelativeTransform);
					WitnessActor->SetActorRelativeLocation(AnimationActor->EffectorPosition);
					WitnessActor->SetActorRelativeRotation(AnimationActor->EffectorRotation);

					AnimationActor->AttachedActor = WitnessActor;
				}
			}
		
			if(WitnessActor->GetClass() == AWitnessEntityActor::StaticClass())
			{
				AWitnessEntityActor* EntityActor = (AWitnessEntityActor*)WitnessActor;
				EntityActor->StaticMeshComponent->SetVisibility(!Entity->Deleted);
				UE_LOG(LogTemp, Warning, TEXT("%s is currently: %s"), *Node.Key, (Entity->Deleted ? *FString("Deleted") : *FString("Not Deleted")));
			}
		}

		if(LaborMap.Contains(Node.Key))
		{
			if(Node.Value->PartPositionInstanceName.Contains("Labor Queue"))
			{
				auto labor = LaborMap[Node.Key];
				auto queue = WitnessStaticManager->StaticMap[Entity->PartPositionInstanceName];
				FVector pos = labor->TransformLocation(queue->Translate);
				labor->SetActorLocation(pos);
				labor->AnimationAt = -1;
				
			}
			if(Node.Value->PartPositionInstanceName.Contains("Path"))
			{
				float at = Entity->PartPosition;
				auto labor = LaborMap[Node.Key];
				auto path = WitnessPathManager->PathMap[Entity->PartPositionInstanceName];
				FVector pos = labor->TransformLocation(path->GetPointAt(at));
				FVector tan = labor->TransformLocation(path->GetTangentAt(at));
				labor->SetActorLocation(pos);
				labor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(pos, pos+tan));
				labor->AnimationAt = FGenericPlatformMath::Fmod((path->Length*at*30.0f),100.0f )/100.0f;
			}
			else
			{
				//LaborMap[Node.Key]->SetActorLocation(FVector(-10000));
			}
		}
	}
	auto Frames = WitnessAnimationManager->GetAnimationFramesAtCurrentTimeInterpolated();
	for (TTuple<FString, AWitnessAnimationActor*> Node : AnimationMap)
	{
		float at;
		if(Frames[Node.Key]->Started) at = float(Frames[Node.Key]->UpdateTime - Frames[Node.Key]->StartTime)/float(Frames[Node.Key]->Duration);
		else at = 1.0f - float(Frames[Node.Key]->UpdateTime - Frames[Node.Key]->StopTime)/float(Frames[Node.Key]->Duration);
		Node.Value->AnimationAt = at;
	}

	auto texts = WitnessTextManager->GetEntitiesAtCurrentTimeInterpolated();
	for (TTuple<FString, UWitnessText*> Text : texts)
	{
		auto Billboard =  BillboardMap[Text.Key];
		
		// Set Text from Billboard
		//Billboard->textRender->GetTextRender()->SetText(FText::FromString(Text.Value->Text));
		Billboard->TextValue = Text.Value->Text;

	}

}

FString AWitnessReplayerActor::GeometryPathToName(FString GeometryPath)
{
	FString Left, Right;
	GeometryPath.Split(TEXT("\\"),&Left,&Right,ESearchCase::CaseSensitive,ESearchDir::FromEnd);
	return Right;
}

FString AWitnessReplayerActor::GetStaticMeshPath(FString MeshName)
{
	if(MeshName.Contains(" "))
	{
		MeshName = MeshName.Replace(TEXT(" "),TEXT("_"));
	}
	
	FString ReturnValue = "/Game/Models/" + MeshName + '/' + MeshName;
	return ReturnValue;
}

void AWitnessReplayerActor::GenerateCameras(float Scale,float Height, AActor* WorldContext)
{
	UWorld* world = WorldContext->GetWorld();
	// Check wich surface is larger
	TArray<AActor*> surfaces;
	UGameplayStatics::GetAllActorsOfClass(world,AWitnessSurfaceActor::StaticClass(),surfaces);

	AWitnessSurfaceActor* mainSurface = nullptr;
	FBoxSphereBounds mainBounds;

	for(auto Currentsurface : surfaces)
	{
		AWitnessSurfaceActor* Tempsurface = Cast<AWitnessSurfaceActor>(Currentsurface);
		if(mainSurface == nullptr)
		{
			mainSurface = Tempsurface;
			mainBounds = Tempsurface->ProceduralMeshComponent->Bounds;
			continue;
		}
		else
		{
			// Getting the values for checking
			FBoxSphereBounds TempBounds = Tempsurface->ProceduralMeshComponent->Bounds;
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

bool AWitnessReplayerActor::GetFileFromOpenDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames)
{
#ifdef	UE_BUILD_SHIPPING
 return false;
#endif
	
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				return DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
		}
	}
	return false;
}

void AWitnessReplayerActor::SetPaused(bool bShouldPause)
{
	bIsPaused = bShouldPause;
}

void AWitnessReplayerActor::CheckTags(FString Path)
{
	FString File;
	FFileHelper::LoadFileToString(File, *Path);
	if(File != "")
	{
		if(!File.StartsWith("<root>"))
		{
			File = "<root>" + File + "</root>";
			FFileHelper::SaveStringToFile(File, *Path);
		}
	}
}
