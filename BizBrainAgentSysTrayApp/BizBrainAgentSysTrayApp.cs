using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.ServiceProcess;
using System.Threading;
using BizBrainCommonUtils;

[
ComImport,
Guid("1620BE13-A68F-4FA3-B5C8-31092D626CDA")]
public class AgentInfo { }

namespace AgentDLLServiceSystemTray
{   [
  ComVisible(true),
  Guid("1C815380-9321-48D5-ADC3-970C6680DDBA"),
  ProgId("BizBrain.Agent.SysTrayApp"),
  ClassInterface(ClassInterfaceType.AutoDispatch)]
    public class BizBrainAgentSysTrayApp : ApplicationContext
    {

        [DllImport("shell32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool IsUserAnAdmin();
       
        private NotifyIcon trayIcon;

        private bool initialized = false;
        string source = "SysTrayApp";
       
        private _ILoggerEx logger;
       
        private SynchronizationContext _context = SynchronizationContext.Current != null ? SynchronizationContext.Current : new SynchronizationContext();
      



       
        public void ShowNotification(string trayIconText, string baloonTipTitle, string baloonTipText)
        {
            ShowNotifyMessage(trayIconText, baloonTipTitle, baloonTipText);
        }

        public BizBrainAgentSysTrayApp()
        {
            //log4net.Config.XmlConfigurator.Configure();
            logger = new Logger("BizBrainAgentSysTrayApp");

            logger.Log("BizBrainAgentSysTrayApp ctor");
            /* if (!EventLog.SourceExists(source))
             {
                 EventLog.CreateEventSource(source, log);
             }*/
            // Initialize Tray Icon
            try
            {
                //EventLog.WriteEntry(source,"Before Initialize");
                logger.Log("Before Initialize");
                bool isAdmin = IsUserAnAdmin();
                SetIcon();
                MessageBox.Show("Running");
                logger.Log("After Initialize");
            }
            catch (Exception e)
            {
                logger.Log("Initialize Exception:" + e.Message);
                logger.Log(e.StackTrace);
                //EventLog.WriteEntry(source,e.Message);
                //EventLog.WriteEntry(source,e.StackTrace);
                Application.Exit();
                System.Environment.Exit(0);
            }
            if (initialized)
            {
                try
                {
                    SetIcon();
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                    MessageBox.Show(e.StackTrace);
                    logger.Log("SetIcon:" + e.Message);

                    //EventLog.WriteEntry(source, e.Message);
                    //EventLog.WriteEntry(source, e.StackTrace);

                    Application.Exit();
                    System.Environment.Exit(0);
                }
            }

        }
        void SetIcon()
        {
            try
            {
               
                CreateNotifyIcon();
                {
                    SetRegisteredIcon();
                }
                trayIcon.Visible = false;
                trayIcon.Visible = true;

            }
            catch (Exception e)
            {
                logger.Error("Failed to get agentInfo:" + e.Message);
               
            }

        }
        void LicenseInfo(object sender, EventArgs e)
        {

            logger.Log("Begin LicenseInfo");
            if (initialized)
            {
                try
                {
                    MessageBox.Show("LicenseInfo screen here");
                }
                catch (Exception ex)
                {
                    //if (!AutoRestartService())
                    {
                        string msg = "Failed to Launch ManagementUIApp with Screen LicenseInfo- Exception:" + ex.Message;
                        logger.Error(msg);
                        logger.Error(ex.StackTrace);
                        MessageBox.Show(msg);
                    }
                }

            }
            else
            {
                MessageBox.Show("Failed connect to server");
            }
            logger.Log("End LicenseInfo");
        }

        void ShowNotifyMessage(string message)
        {

            trayIcon.Text = message;// "BizBrain Agent Service [Registered]";
            trayIcon.ShowBalloonTip(1000);
        }

        void ShowNotifyMessage(string trayIconText, string ballonTipTitle, string baloonTipText)
        {

            trayIcon.Text = trayIconText;// "BizBrain Agent Service [Registered]";
            trayIcon.BalloonTipTitle = ballonTipTitle;//"BizBrain Agent Service[Registered]";
            trayIcon.BalloonTipText = baloonTipText;// "Service Started";
            trayIcon.BalloonTipIcon = ToolTipIcon.Info;
            trayIcon.ShowBalloonTip(1000);

        }
        void CreateNotifyIcon()
        {
            if (trayIcon == null)
            {
                trayIcon = new NotifyIcon()
                {
                    Icon = AgentDLLService.Properties.Resources.AppIcon,
                    ContextMenu = new ContextMenu(new MenuItem[] {
                        new MenuItem("License Info",LicenseInfo)


            }),
                    Visible = true
                };
                trayIcon.Text = "BizBrain Agent Service";
                trayIcon.Click += TrayIcon_Click;
            }
        }

        private void TrayIcon_Click(object sender, EventArgs e)
        {
            // MessageBox.Show("I was clicked");
            logger.Log("Begin TrayIcon_Click");
            //  Manage(sender,e);
            logger.Log("End TrayIcon_Click");
        }

        void SetRegisteredIcon()
        {

            try
            {
                CreateNotifyIcon();
                trayIcon.Icon = AgentDLLService.Properties.Resources.AppIcon;
                trayIcon.Text = "BizBrain Agent Service [Registered]";
                trayIcon.BalloonTipTitle = "BizBrain Agent Service[Registered]";
                trayIcon.BalloonTipText = "Service Started";
                trayIcon.BalloonTipIcon = ToolTipIcon.Info;
                trayIcon.ShowBalloonTip(5000);
            }
            catch (Exception e)
            {
                //MessageBox.Show("SetRegisteredIcon:" + e.Message);
                //EventLog.WriteEntry(source, e.Message);
            }


        }
        void SetUnregisteredIcon()
        {
            try
            {
                CreateNotifyIcon();
                trayIcon.Icon = AgentDLLService.Properties.Resources.AgentUnregistered;
                trayIcon.Text = "BizBrain Agent Service [Unregistered]";
                trayIcon.BalloonTipTitle = "BizBrain Agent Service[Unregistered]";
                trayIcon.BalloonTipText = "Please register the Agent";
                trayIcon.BalloonTipIcon = ToolTipIcon.Info;
                trayIcon.ShowBalloonTip(5000);
            }
            catch (Exception e)
            {
                //MessageBox.Show("SetUnregisteredIcon:" + e.Message);
                //EventLog.WriteEntry(source, e.Message);
            }



        }
        private void SetOnlineIcon(object o = null)
        {
            //  MessageBox.Show(" before SetOnlineIcon:"+(trayIcon!=null).ToString()+":"+ (AgentDLLService.Properties.Resources.Online!=null).ToString());

            try
            {
                CreateNotifyIcon();
                //@@@17th January 2018
                //Instead of Resources.Online which shows an ugly connected icon, AppIcon is used;
                trayIcon.Icon = AgentDLLService.Properties.Resources.AppIcon;
                trayIcon.Text = "BizBrain Agent Service [Online]";
                trayIcon.BalloonTipTitle = "BizBrain Agent Service [Online]";
                trayIcon.BalloonTipText = "The Agent is Online";
                trayIcon.BalloonTipIcon = ToolTipIcon.Info;
                trayIcon.ShowBalloonTip(1000);
            }
            catch (Exception e)
            {
                //EventLog.WriteEntry(source, e.Message);
                //MessageBox.Show("SetOnlineIcon:" + e.Message);
                logger.Error("Exception in SetOnlineIcon:" + e.Message);
                logger.Error(e.StackTrace);
            }

        }
        private void SetOfflineIcon(object o = null)
        {
            try
            {
                CreateNotifyIcon();
                trayIcon.Icon = AgentDLLService.Properties.Resources.Online;
                trayIcon.Text = "BizBrain Agent Service [Offline]";
                trayIcon.BalloonTipTitle = "BizBrain Agent Service [Offline]";
                trayIcon.BalloonTipText = "The Agent is Offline";
                trayIcon.BalloonTipIcon = ToolTipIcon.Info;
                trayIcon.ShowBalloonTip(1000);
            }
            catch (Exception e)
            {
                //MessageBox.Show("SetOfflineIcon:" + e.Message);
                //EventLog.WriteEntry(source, e.Message);
                logger.Error("Exception in SetOfflineIcon:" + e.Message);
                logger.Error(e.StackTrace);
            }

        }
        void SetNoPluginsIcon()
        {
            CreateNotifyIcon();
            trayIcon.Icon = AgentDLLService.Properties.Resources.Online;
            trayIcon.Text = "BizBrain Agent Service [No Plugins Installed]";
        }
        void Exit(object sender, EventArgs e)
        {
            // Hide tray icon, otherwise it will remain shown until user mouses over it
            Shutdown(true);
        }
        private void Shutdown(bool revoke = true)
        {
            {

                try
                {
                    if (trayIcon != null)
                    {
                        trayIcon.Visible = false;
                    }
                }
                catch (Exception ex)
                {
                    //MessageBox.Show(ex.Message); 
                    //EventLog.WriteEntry(source, ex.Message);
                }
                finally
                {
                    Application.Exit();
                    System.Environment.Exit(0);
                }
            }
        }
    }
}
