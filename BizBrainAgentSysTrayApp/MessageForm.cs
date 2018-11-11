using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AgentDLLServiceSystemTray
{
    public partial class MessageForm : Form
    {
        private WindowsFormsSynchronizationContext _context = (WindowsFormsSynchronizationContext)(WindowsFormsSynchronizationContext.Current != null ? WindowsFormsSynchronizationContext.Current : new WindowsFormsSynchronizationContext());
        private BizBrainAgentSysTrayApp app;
        public MessageForm(BizBrainAgentSysTrayApp app)
        {
            InitializeComponent();
            this.FormClosing += MessageForm_FormClosing;
        }

        private void MessageForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            Hide();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            txtMessage.Text = null;
        }
        private void _ShowMessage(object oMessage)
        {
            txtMessage.Text = (string)oMessage;
            this.Visible = true;
            this.WindowState = FormWindowState.Minimized;
            this.WindowState = FormWindowState.Normal;
        }
        private void _HideMessage(object o)
        {
            Hide();
        }
        public void HideMessage()
        {
            _context.Post(new System.Threading.SendOrPostCallback(_HideMessage), null);
        }
        public void ShowMessage(string message)
        {

            _context.Post(new System.Threading.SendOrPostCallback(_ShowMessage), message);
        }
    }
}
