using System.IO;
using UnrealBuildTool;

public class comdlg32 : ModuleRules
{
    public comdlg32(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string libraryPath = Path.Combine(ModuleDirectory, "lib");
        
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(libraryPath, "comdlg32.lib"));
            PublicDelayLoadDLLs.Add("comdlg32.dll");
            RuntimeDependencies.Add("$(TargetOutputDir)/comdlg32.dll", Path.Combine(libraryPath, "comdlg32.dll"));
        }
    }
}