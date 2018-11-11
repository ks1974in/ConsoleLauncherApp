ConsoleLauncher: App which tries to launch the target app [SysTrayApp]
TestApp : c++ console app without dependencies
BizBrainAgentSysTrayApp     : App which shows up in system Tray and shows message box, when lanched from windows explorer, but throws exception when launched                   via code
Win32ApiUtils  : ATL dll , Contains class ProcessLauncher which contains the method CreateProcessAsUser
BizBrain-Agent-Logging :Contains logger class

Folder Test:Contains all binaries, including psexec64
Folder Logs: Contains logs [
Win32ApiUtilsLog.log - log from Win32ApiUtils.dll
ConsoleLauncher.log  - log from ConsoleLauncher.exe

To launch
psexec64 -i -s cmd.exe
in the cmd.exe,
navigate to the Test folder and type
ConsoleLauncher.exe TestApp.exe or ConsoleLauncher.exe BizBrainAgentSysTrayApp.exe

psexec64 is necessary to launch cmd in local system account, otherwise it will not work. 
In the actual project, this is called from  a windows service running under the Local system account



