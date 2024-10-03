// Fill out your copyright notice in the Description page of Project Settings.


#include "XMLLoader.h"

#include "XmlFile.h"
#include "Kismet/KismetSystemLibrary.h"


TMap<FString,FString>  UXMLLoader::LoadFile()
{
	FString Commandline = UKismetSystemLibrary::GetCommandLine();
	if(Commandline.IsEmpty())
		return TMap<FString,FString>();
	
	
	FString FilePath;
	TArray<FString> FilteredCommands;
	for (int i = 0; i < Commandline.Len(); i++)
	{
		if(Commandline[i] == '-')
		{
			FString Prefix;
			int StartIndex = Commandline.Find("-w3dpath ");
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
					if(Commandline[Key + i] == '-')
					{
						i = i + Key;
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
		UE_LOG(LogTemp, Warning, TEXT("Loading Config: %s"), *configFilePath);
		FXmlFile XmlFile;
		if(XmlFile.LoadFile(configFilePath))
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

TMap<FString,FString>  UXMLLoader::LoadConfig(FString configFilePath)
{
	//test
	TMap<FString,FString>  ConfigFileOutput;

	if(configFilePath.Len() <= 0)
	{
		return TMap<FString, FString>();
	}
	UE_LOG(LogTemp, Warning, TEXT("Loading Config: %s"), *configFilePath);
	FXmlFile XmlFile;
	if(XmlFile.LoadFile(configFilePath))
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


