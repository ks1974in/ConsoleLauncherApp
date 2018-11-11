using BizBrainCommonUtils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Win32ApiUtilsLib;

namespace BizBrainAgentWindowsService
{
    public class ProcessLauncher
    {

        


        private string pathToTargetApp ;




        private Process procTarget ;


        private _ILoggerEx logger = new Logger("ProcessLauncher");
     



        public void LaunchTargetApp(string cmdLine)
        {

            ProcessManager processManager = new ProcessManager();

            //string path = @"D:\Projects\Visual Studio 2015\Projects\BizBrainAgent\BizBrainTarget \bin\Debug";
            //string cmdLine = "TestApp.exe";//"BizBrainAgentSysTrayApp.exe";// "BizBrainAgentTarget .exe";

            string pathCurrent = AppDomain.CurrentDomain.BaseDirectory;//System.IO.Path.GetDirectoryName(System.Reflection.Assembly.Gete.Location);

            pathToTargetApp =pathCurrent +  cmdLine;
            //pathToTarget App = pathCurrent + "\\" + cmdLine;

            //pathToSysTrayApp = cmdLine;
            logger.Info("pathToTarget :" + pathToTargetApp);
            logger.Info("Launch "+cmdLine+ "App called with managed thread id:" + Thread.CurrentThread.ManagedThreadId);
            try
            {
                long processId = processManager.LaunchProcessAsActiveUser(pathToTargetApp);
                logger.Info("AgentDLLService::LaunchProcessAsActiveUser::Target App "+cmdLine+"Launched with ProcessId:" + processId);


                long error = 0;

                if (processId == 0)
                {
                    logger.Info("Failed to Create Target  process");
                    error = Marshal.GetLastWin32Error();
                    logger.Info("GetLastWin32Error returned " + error); ;

                }
                else
                {
                    procTarget  = Process.GetProcessById((int)processId);
                    if (procTarget  == null)
                    {
                        logger.Info("Target  process is null");
                        error = Marshal.GetLastWin32Error();
                        logger.Info("GetLastWin32Error returned " + error);
                    }
                    else
                        try
                        {
                            // logger.Info("Created Target  process on Machine Name:" + procTarget .MachineName + " with id:" + procSysTray.Id);
                            logger.Info("Created Target  process ");
                        }
                        catch (Exception e)
                        {
                           
                            logger.Info(e.Message);
                        }
                }
            }

            catch (Exception e)
            {
              
                logger.Error("Failed to create process Target " + e.Message);
                logger.Error(e.StackTrace);
            }
        }

    }
}
