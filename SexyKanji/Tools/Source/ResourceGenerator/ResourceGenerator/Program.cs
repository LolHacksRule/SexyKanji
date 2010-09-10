/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   Program.cs
///
/// Purpose:    The main entry point for the Resource Generator.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace ResourceGenerator
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the Resource Generator.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new ResourceGeneratorForm());
        }
    }
}