// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayerLevelScriptActor.h"

#include <filesystem>


//File picker includes
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "sys/stat.h"

#include "WitnessReplayerActor.h"
#include "XMLLoader.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/RecordPanelUI.h"


// Sets default values
AReplayerLevelScriptActor::AReplayerLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReplayerLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(InitialMouseLockMode);
	
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);

	IntroWidget = CreateWidget<UIntroWidget>(World, IntroWidgetClass, "Intro Widget");
	IntroWidget->AddToViewport(10);
	IntroWidget->OnStart.AddDynamic(this, &AReplayerLevelScriptActor::InitializeXml);

	
}

FString AReplayerLevelScriptActor::GetPathFromCommandLinePath()
{
	TArray<FString> Tokens {};
	TArray<FString> Switches {};
	TMap<FString, FString> Params {};
	
	UKismetSystemLibrary::ParseCommandLine(UKismetSystemLibrary::GetCommandLine(), Tokens, Switches, Params);
	
	FString* CommandLinePath = Params.Find("-w3dpath");
	FString Path = CommandLinePath ? *CommandLinePath : DefaultXmlPath;
	
	FPaths::NormalizeFilename(Path);
	
	return Path;
}

TMap<FString, FString> AReplayerLevelScriptActor::GetConfigFile()
{
	FString Commandline = UKismetSystemLibrary::GetCommandLine();
	if(Commandline.IsEmpty())
		Commandline = "-w3dpath " + DefaultXmlPath;
	
	
	FString FilePath;
	TArray<FString> FilteredCommands;
	for (int i = 0; i < Commandline.Len(); i++)
	{
		if(Commandline[i] == '-')
		{
			FString Prefix;
			Prefix.AppendChar(Commandline[i]);
			Prefix.AppendChar(Commandline[i + 1]);
			Prefix.AppendChar(Commandline[i + 2]);
			Prefix.AppendChar(Commandline[i + 3]);
			Prefix.AppendChar(Commandline[i + 4]);
			Prefix.AppendChar(Commandline[i + 5]);
			Prefix.AppendChar(Commandline[i + 6]);
			Prefix.AppendChar(Commandline[i + 7]);
			Prefix.AppendChar(Commandline[i + 8]);
			
			if(Prefix == "-w3dpath ")
			{
				FString ConstructedString = "";
				int Key = 9;
				while (Key + i < Commandline.Len())
				{
					FString TagCheck;
					TagCheck.AppendChar(Commandline[Key + i]);
					TagCheck.AppendChar(Commandline[Key + i + 1]);
					TagCheck.AppendChar(Commandline[Key + i + 2]);
					TagCheck.AppendChar(Commandline[Key + i + 3]);
					if(TagCheck == ".xml")
					{
						ConstructedString.Append(".xml");
						break;
					}
						
					ConstructedString.AppendChar(Commandline[Key + i]);
					Key++;
				}
				FilteredCommands.Add(ConstructedString);
			}
		}
	}
	if(FilteredCommands.Num() > 0)
	{
		FString configFilePath = FilteredCommands[0];
		TMap<FString,FString>  ConfigFileOutput;

		if(configFilePath.Len() <= 0)
		{
			return TMap<FString, FString>();
		}

		// Check if it has standard xml tag
		FString GeneratedFilePath = FPlatformProcess::UserTempDir();
		GeneratedFilePath.Append("Replay");


		struct stat buffer;
		std::string s = TCHAR_TO_UTF8(*GeneratedFilePath);
		bool FolderExists = (stat (s.c_str(),&buffer) == 0);

		if(!FolderExists)
		{
			std::filesystem::create_directory(s);
		}


		GeneratedFilePath.Append("/TempConfig.xml");
		FString StringFile;
		FFileHelper::LoadFileToString(StringFile,*configFilePath);
		if(!StringFile.IsEmpty())
		{
			if(StringFile.Contains("<?xml"))
			{
				int Key = 0;
				for(int i = 0; i < StringFile.Len(); i++)
				{
					if(StringFile[i] == '>')
					{
						Key = i + 1;
						break;
					}
				}

				StringFile = StringFile.Mid(Key, StringFile.Len());
				FFileHelper::SaveStringToFile(StringFile, *GeneratedFilePath);
			}
			else
			{
				FFileHelper::SaveStringToFile(StringFile, *GeneratedFilePath);
			}
		}
		else
		{
			GeneratedFilePath = configFilePath;
		}
		
		FXmlFile XmlFile;
		if(XmlFile.LoadFile(GeneratedFilePath))
		{
			FXmlNode* Root = XmlFile.GetRootNode();
			TArray<FXmlNode*> XmlNodes = Root->GetChildrenNodes();
			for (auto Childnode : XmlNodes)
			{
				FString Tag = Childnode->GetTag();
				if(Tag == "File")
				{
					FString FilePathToSimulation = Childnode->GetAttribute("FilePath");
					ConfigFileOutput.Add("File",FilePathToSimulation);
				}
				else if(Tag == "Time")
				{
					FString TimeStamp = Childnode->GetAttribute("TimeStamp");
					ConfigFileOutput.Add("Time",TimeStamp);
				}
				else if(Tag == "Format")
				{
					FString DisplayFormat = Childnode->GetAttribute("DisplayFormat");
					if(DisplayFormat.Contains(".")) { DisplayFormat = DisplayFormat.Replace(TEXT("."), TEXT("")); };
					if(DisplayFormat.Contains(",")) { DisplayFormat = DisplayFormat.Replace(TEXT(","), TEXT("")); };
					FString UnitFormat = Childnode->GetAttribute("TimeUnitFormat");
					FString HourFormat = Childnode->GetAttribute("TimeDisplay");
					ConfigFileOutput.Add("DisplayFormat",DisplayFormat);
					ConfigFileOutput.Add("TimeUnitFormat",UnitFormat);
					ConfigFileOutput.Add("HourFormat",HourFormat);
				}
			}
			return  ConfigFileOutput;
		}
		
		return TMap<FString,FString>();
	}
	else
	{
		return TMap<FString, FString>();
	}
}

#if WITH_EDITOR
FString AReplayerLevelScriptActor::OpenFileDialogForEditor()
{
		TArray<FString> result;
		void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			uint32 SelectionFlag = 1; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
			DesktopPlatform->OpenFileDialog(ParentWindowPtr, FString("tmp"), FString(""), FString("C:/Users/Public/Documents/Lanner Group/WITNESS 23 Horizon/Startup/En"), "w3d|*.w3d|xml|*.xml", SelectionFlag, result);
		}
	return result[0];
}
#endif

void AReplayerLevelScriptActor::InitializeXml()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	
	WitnessReplayerActor = Cast<AWitnessReplayerActor>(World->SpawnActor(AWitnessReplayerActor::StaticClass()));
	WitnessReplayerActor->GetBlueprintClasses(RobotClass, HumanClass, BillboardClass);
	
	TMap<FString, FString> Config = GetConfigFile();
	
	//if(!WitnessReplayerActor->LoadXML(Config["File"]) && bOpenFileDialog)
	if(Config.Num() > 0)
	{
		if(!WitnessReplayerActor->LoadXML(Config["File"]))
		{
			//Open File Dialog;
#if WITH_EDITOR
			WitnessReplayerActor->LoadXML(OpenFileDialogForEditor());
#endif
		}
	}
	else
	{
		if(!WitnessReplayerActor->LoadXML(FPaths::LaunchDir().Append("/Replay/Content/XML/WitnessRun.xml")))
		{
#if WITH_EDITOR
			WitnessReplayerActor->LoadXML(OpenFileDialogForEditor());
#endif
		}
	}

	
	
	WitnessReplayerActor->GenerateCameras(CameraScale, CameraHeight, this);
	VideoControlWidget = CreateWidget<UVideoControlWidget>(PlayerController, VideoControlWidgetClass, "VideoControlWidget");
	VideoControlWidget->AddToViewport(1);
	VideoControlWidget->UploadConfigData(Config);

	URecordPanelUI* RecordingPanel = Cast<URecordPanelUI>( CreateWidget<UUserWidget>(PlayerController,RecordPanelClass,"RecordPanel"));
	RecordingPanel->AddToViewport(1);
	RecordingPanel->VideoControls = VideoControlWidget;

	
	
}

// Called every frame
void AReplayerLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

