using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace RemoteLabSystem
{
    static class Program
    {
        // class constants
        

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new RLS_Main());
        }
    }
}

