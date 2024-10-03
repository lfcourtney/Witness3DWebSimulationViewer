#pragma once
#include "Action.h"

#include "SocketSubsystem.h"
#include "XMLInterpeter.h"
#include "XML_Interpeter.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/TimelineTemplate.h"

void UCreateAction::Execute()
{
	Super::Execute();
	//UE_LOG(LogTemp, Warning, TEXT("Create Action. happening at Time: %d"), Time);

	if ((CreateData.Num() >= 1) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateActions Empty!"))
		return;
	}
	
	AMachineObject* MachineObject = static_cast<AMachineObject*>(WitnessObject);
		USceneComponent* SceneComponent = WitnessObject->FindComponentByClass<USceneComponent>();
		SceneComponent->SetMobility(EComponentMobility::Movable);
	
	if (MachineObject != nullptr)
	{
		UMeshComponent* comp = MachineObject->FindComponentByClass<UMeshComponent>();
		if(comp)
		{
			comp->SetMobility(EComponentMobility::Movable);
		}
		WitnessObject->ObjectName = InstanceName;
		//UE_LOG(LogTemp, Warning, TEXT("Trying to find mesh at: %s"), *MachineObject->ContentPath);
		UStaticMesh* meshToUse = Cast<UStaticMesh>(
			StaticLoadObject(UStaticMesh::StaticClass(), NULL, *MachineObject->ContentPath));
		if (meshToUse != nullptr)
		{
			
			//UE_LOG(LogTemp, Warning, TEXT("Adding Found Mesh to Machine Object"));
			MachineObject->MeshComp->SetMobility(EComponentMobility::Movable);
			MachineObject->SetGeometry(meshToUse);
		}
	}

	if(WitnessObject->GetClass() == APathObject::StaticClass())
	{
		
		APathObject* PathObject = static_cast<APathObject*>(WitnessObject);
		UMeshComponent* comp = PathObject->FindComponentByClass<UMeshComponent>();
		if(comp)
		{
			comp->SetMobility(EComponentMobility::Movable);
		}
		
		if(PathObject->ObjectName.Contains("Conv"))
		{
			
			UStaticMesh* meshToUse = Cast<UStaticMesh>(
			StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Models/Path/Conveyor")));
			if(meshToUse != nullptr)
			{
				
				PathObject->SetGeometry(meshToUse, 100.0f);
			}
		}
		else
		{
			UStaticMesh* meshToUse = Cast<UStaticMesh>(
			StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Models/Path/Path")));
			UMaterial* Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Models/Path/PathMat")));
			if(meshToUse != nullptr)
			{
				PathObject->SplineComp->SetMobility(EComponentMobility::Movable);
				PathObject->SetGeometry(meshToUse, 100.0f);
				if(Material != nullptr)
				{
					PathObject->SetMaterial(Material);
				}
			}
		}
	}

	for (auto Data : CreateData)
	{
		if(Data->GetClass() == UQueueInfo::StaticClass())
		{
			UQueueInfo* QueueInfo = static_cast<UQueueInfo*>(Data);
			if (QueueInfo != nullptr)
			{
				// Steven taak
				FVector SocketPosition = FVector(QueueInfo->Position.X, QueueInfo->Position.Z, QueueInfo->Position.Y) * 100;
				UStaticMesh* Mesh = MachineObject->Geometry;
				UStaticMeshSocket* TestSocket = NewObject<UStaticMeshSocket>();
				//UE_LOG(LogTemp, Warning, TEXT("Trying to make socket"));
				if (TestSocket && Mesh)
				{
					if(SocketPosition == FVector::ZeroVector)
						return;
				
					TestSocket->SocketName = "TestSocket";
					TestSocket->RelativeLocation = SocketPosition;
					Mesh->AddSocket(TestSocket);
					MachineObject->MeshComp->RegisterComponent();
					//UE_LOG(LogTemp, Display, TEXT("Created Socket!!!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to make SOCKET!!"));
				}
			
			}
		}
		else if(Data->GetClass() == UPath::StaticClass())
		{
			UPath* Path = static_cast<UPath*>(Data);
			if (Path != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Trying to fill path!"));
				APathObject* PathObject = Cast<APathObject>(WitnessObject);
				if (PathObject != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Got Path!"));
					PathObject->Path = Path;

					if (PathObject->Path == nullptr)
					{
						UE_LOG(LogTemp, Error, TEXT("Could not get Path Pointer!"));
					}
					else if (PathObject->Geometry == nullptr)
					{
						UE_LOG(LogTemp, Error, TEXT("Could not get Geometry!"));
					}
					else if (PathObject->Path->PathData.Num() > 0)
					{
						//UE_LOG(LogTemp, Warning, TEXT("All variables are Okay. Create Spline!"));
					
						UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
																		 PathObject->GetWorld(),
																		 TEXT(
																			 "/Game/Temp/WallMat")));
						UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));
					
						PathObject->CreateSpline(PathObject, Path->PathData, PathObject->Geometry, 100.0f,
												 ESplineMeshAxis::X, Material);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Could not get Path!"));
				}
			}
		}
		else if(Data->GetClass() == UExtrude::StaticClass())
		{
			UExtrude* Extrude = static_cast<UExtrude*>(Data);
			if (Extrude != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Trying to fill path!"));
				AExtrudeObject* ExtrudeObject = Cast<AExtrudeObject>(WitnessObject);
				if (ExtrudeObject != nullptr)
				{
					ExtrudeObject->SplineComp->SetMobility(EComponentMobility::Movable);
					ExtrudeObject->SplineMeshComp->SetMobility(EComponentMobility::Movable);
					ExtrudeObject->Extrude = Extrude;
					ExtrudeObject->CreateSpline(ExtrudeObject, ExtrudeObject->Extrude, ExtrudeObject->Geometry);
				
					UMaterial* ParentMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(),
																		 ExtrudeObject->GetWorld(),
																		 TEXT(
																			 "/Game/Temp/WallMat")));
					UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(ParentMaterial, ParentMaterial->GetWorld(), FName("MaterialName"));
					if(Material != nullptr)
					{
						//UE_LOG(LogTemp, Display, TEXT("Got A Material!"));
						ExtrudeObject->SetMaterial(Material);
					}
				}
			}
		}
		else if(Data->GetClass() == USurface::StaticClass())
		{
			USurface* Surface = static_cast<USurface*>(Data);
			if (Surface != nullptr)
			{
				if(WitnessObject->GetClass() == ASurfaceObject::StaticClass())
				{
					ASurfaceObject* SurfaceObject = static_cast<ASurfaceObject*>(WitnessObject);
					FVector TL = Surface->StartLoc * 100.0f;
					FVector BR = Surface->EndLoc * 100.0f;
					
					FVector Vert1 = TL;
					FVector Vert2 = FVector(BR.X, TL.Y, TL.Z);
					FVector Vert3 = FVector(TL.X, BR.Y, TL.Z);
					FVector Vert4 = BR;

					TArray<FVector> locations;
					locations.Add(Vert1);
					locations.Add(Vert2);
					locations.Add(Vert3);
					locations.Add(Vert4);
					// Spawn Cameras
					
					TArray<FVector> Verts = TArray<FVector>();
					Verts.Add(Vert1);
					Verts.Add(Vert2);
					Verts.Add(Vert3);
					Verts.Add(Vert4);
					
					TArray<int32> Tris = TArray<int32>();
					Tris.Add(0);
					Tris.Add(1);
					Tris.Add(2);
					Tris.Add(2);
					Tris.Add(1);
					Tris.Add(3);
					
					TArray<FVector> Normals = TArray<FVector>();
					Normals.Add(Surface->Normal);
					Normals.Add(Surface->Normal);
					Normals.Add(Surface->Normal);
					Normals.Add(Surface->Normal);
					
					TArray<FVector2D> UVs = TArray<FVector2D>();
					UVs.Add(FVector2D(0,0));
					UVs.Add(FVector2D(1,0));
					UVs.Add(FVector2D(0,1));
					UVs.Add(FVector2D(1,1));
					
					TArray<FColor> Colors = TArray<FColor>();
					Colors.Add(Surface->color1);
					Colors.Add(Surface->color1);
					Colors.Add(Surface->color1);
					Colors.Add(Surface->color1);
					
					TArray<FProcMeshTangent> Tangents = TArray<FProcMeshTangent>();
					Tangents.Add(FProcMeshTangent(1, 1, 1));
					Tangents.Add(FProcMeshTangent(1, 1, 1));
					Tangents.Add(FProcMeshTangent(1, 1, 1));
					Tangents.Add(FProcMeshTangent(1, 1, 1));
					
					if(SurfaceObject != nullptr)
					{
						if(SurfaceObject->ProcMesh != nullptr)
						{
							SurfaceObject->ProcMesh->UpdateMeshSection(0, Verts, Normals, UVs, Colors, Tangents);
							if(SurfaceObject->Material != nullptr)
							{
								//UE_LOG(LogTemp, Display, TEXT("Got A Material!"));
								SurfaceObject->SetMaterial(nullptr);
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("Could not get Floor Material!"));
							}
						}
					}
				}
			}
		}
	}

	//UE_LOG(LogTemp, Display, TEXT("Succesfully casted all Action Data!"));
}

void UUpdateAction::Execute()
{
	
}

void UUpdateAction::Execute(AWitnessObject* Object)
{
	//UE_LOG(LogTemp, Display, TEXT("Update Action. At Time: %d"), Time);

	if (Object == nullptr)
	{
		if (GameInstance->ContainsName(InstanceName) == true)
		{
			//UE_LOG(LogTemp, Display, TEXT("Found Name. Getting Actor"));
			WitnessObject = GameInstance->GetWitnessObjectFromName(InstanceName);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Cant get name so wont find actor!"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Object not nullptr using it!"));
		WitnessObject = Object; // UXMLInterpeter::GetWitnessObjectFromName(InstanceName);
	}

	if (WitnessObject != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Got Actor. Trying to Update"));
		FVector Location = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;
		FVector Scaler = FVector::ZeroVector;

		AWitnessObject* Parent = nullptr;
		float PathPercentage;
		bool IsReversed;

		for (auto Data : UpdateData)
		{
			UTranslate* Translate = Cast<UTranslate>(Data);
			URotate* Rotate = Cast<URotate>(Data);
			UScale* Scale = Cast<UScale>(Data);
			UPartPosition* PartPosition = Cast<UPartPosition>(Data);
			UQueueInfo* QueueInfo = Cast<UQueueInfo>(Data);
			if(Data->GetClass() == UTranslate::StaticClass())
			{
				if (Translate != nullptr)
				{
					Location.X = Translate->Translation.X;
					Location.Y = Translate->Translation.Z;
					Location.Z = Translate->Translation.Y;
					Location *= 100.0f;
				}
			}
			else if(Data->GetClass() == URotate::StaticClass())
			{
				if (Rotate != nullptr)
				{
					Rotation.Roll = Rotate->RotationChange.X;
					Rotation.Pitch = Rotate->RotationChange.Z;
					Rotation.Yaw = Rotate->RotationChange.Y;
				}
			}
			else if(Data->GetClass() == UScale::StaticClass())
			{
				if (Scale != nullptr)
				{
					Scaler.X = Scale->ScaleChange.X;
					Scaler.Y = Scale->ScaleChange.Z;
					Scaler.Z = Scale->ScaleChange.Y;
				}
			}
			else if(Data->GetClass() == UPartPosition::StaticClass())
			{
				if (PartPosition != nullptr)
				{
					//UE_LOG(LogTemp, Error, TEXT("Part Position"));
					if (GameInstance->TimeLineObjectsMap.Num() > 0)
					{
						//UE_LOG(LogTemp, Warning, TEXT("Map Has Keys!"));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Map Couldn't be found"));
					}
					Parent = GameInstance->GetWitnessObjectFromName(PartPosition->InstanceName);
					PathPercentage = PartPosition->Position;
					IsReversed = PartPosition->bIsReverse;
				}
			}
			else if(Data->GetClass() == UQueueInfo::StaticClass())
			{
				if (QueueInfo != nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("QUEUE INFO EXISTS IN UPDATE PLEASE IMPLEMENT!"));
				}
			}
		}

		if (Location != FVector::ZeroVector)
		{
			WitnessObject->SetActorLocation(Location);
		}
		if (Rotation != FRotator::ZeroRotator)
		{
			WitnessObject->AddActorLocalRotation(Rotation);
		}
		if (Parent != nullptr)
		{
			//UE_LOG(LogTemp, Error, TEXT("Parent exists. Attaching"));
			WitnessObject->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
			
			APathObject* Path = Cast<APathObject>(Parent);
			if (Path)
			{
				float Size = 0.0f;
				
				FVector SplineLoc = Path->SplineComp->GetLocationAtDistanceAlongSpline(
					PathPercentage * Path->SplineComp->GetSplineLength(), ESplineCoordinateSpace::World);

				FVector Min;
				FVector Max;
				FBoxSphereBounds bounds = Path->Geometry->GetBounds();
				Size = bounds.BoxExtent.Z * 2;
				WitnessObject->AddActorLocalOffset(FVector(0,0,Size), false);
				SplineLoc.Z += Size;//needs to come from mesh bounds. how tall is a mesh
				//UE_LOG(LogTemp, Error, TEXT("Is a spline opbject! Moving Actor"));
				const FRotator TargetRotation = (SplineLoc - WitnessObject->GetActorLocation() + FVector(0,0,Size)).Rotation();
				WitnessObject->SetActorRotation(TargetRotation);
				WitnessObject->SetActorLocation(SplineLoc);
				
				/*if (WitnessObject->GetActorLocation() == FVector::ZeroVector || FVector::Dist(
					FVector::ZeroVector, WitnessObject->GetActorLocation()) < 100)
				{
					WitnessObject->SetActorLocation(SplineLoc);
				}
				else
				{
					const FRotator TargetRotation = (SplineLoc - WitnessObject->GetActorLocation()).Rotation();
					GameInstance->NavigationTasks.Add(FNavigationTask(WitnessObject,
						25,WitnessObject->GetActorLocation(),SplineLoc,
						WitnessObject->GetActorRotation(),TargetRotation));
				}*/
			}
		}
	}
}

void UDeleteAction::Execute()
{
	AActor* ToDelete;

	if (GameInstance->ContainsName(InstanceName) == true)
	{
		ToDelete = GameInstance->TimeLineObjectsMap.FindAndRemoveChecked(InstanceName);
		if (ToDelete != nullptr)
		{
			ToDelete->Destroy(true, true);
		}
	}
}

void ULoadAction::Execute()
{
	UE_LOG(LogTemp, Display, TEXT("Load Action. Time: %d"), Time);
}

void UStartAction::Execute()
{
	UE_LOG(LogTemp, Display, TEXT("Start Action. Time: %d"), Time);
}

void UEndAction::Execute()
{
	UE_LOG(LogTemp, Display, TEXT("End Action. Time: %d"), Time);
}
