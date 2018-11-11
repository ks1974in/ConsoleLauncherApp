using BizBrainAgentWindowsService;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace ConsoleLauncher
{
    class Program
    {
        static void Main(string[] args)
        {
            ProcessLauncher processLauncher = new ProcessLauncher();

            Console.WriteLine("Before");
            Console.ReadLine();
            if (args.Length != 1)
            {
                Console.WriteLine("Please enter name of app in the target folder");
                return;
            }
            else
            {
                processLauncher.LaunchTargetApp(args[0]);
            }
            Console.WriteLine("After");
        }
    }
}
