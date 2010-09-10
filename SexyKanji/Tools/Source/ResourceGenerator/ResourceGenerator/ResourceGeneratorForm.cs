/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   ResourceGeneratorForm.cs
///
/// Purpose:    Handle user interface and call the worker functions.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace ResourceGenerator
{
    /// <summary>
    /// Class to handle UI and call the worker functions.
    /// </summary>
    public partial class ResourceGeneratorForm : Form
    {
        /// <summary>
        /// Helper class to load/save project settings to/from 
        /// corresponding UI elements and to/from project.xml file.
        /// </summary>
        public class ProjectMRUItem
        {
            public ProjectMRUItem() { }
            ~ProjectMRUItem() { }

            public String   mProjectName;
            public String   mResourceXMLPath;
            public String   mOutDir;
            public String   mOutFileName;
            public String   mFunctionPrefix;
            public bool     mReportUnusedFiles;
            public bool     mWriteDelBatchFile;
            public String   mBaseResPath;
        }

        /************************************************************************/
        /* Save the project settings in this list
        /************************************************************************/
        ArrayList mProjectMRUList;

        /// <summary>
        /// CTor, add handler for form closing.
        /// </summary>
        public ResourceGeneratorForm()
        {
            InitializeComponent();

            this.FormClosing += new FormClosingEventHandler(ResourceGeneratorForm_FormClosing);
        }

        /// <summary>
        /// Try to load the settings from projects.xml file and initialize UI.
        /// </summary>
        /// <param name="e">event data</param>
        protected override void OnLoad(EventArgs e)
        {
            // call base, raises the OnLoad event
            base.OnLoad(e);  

            mProjectMRUList         = new ArrayList();
            outputFileTextBox.Text  = "Res";
            outnameLabel.Text       = "Res.h / Res.cpp";

            if (File.Exists("projects.xml"))
            {
                XmlTextReader aReader = new XmlTextReader(OpenStreamReaderWithEncoding("projects.xml"));

                while (aReader.Read())
                {
                    switch (aReader.NodeType)
                    {
                        case XmlNodeType.Element: // The node is an element.

                            if (aReader.Name.ToLower() == "project")
                            {
                                ProjectMRUItem aNewItem = new ProjectMRUItem();

                                aNewItem.mProjectName       = aReader.GetAttribute("id");
                                aNewItem.mResourceXMLPath   = aReader.GetAttribute("xml_path");
                                aNewItem.mOutDir            = aReader.GetAttribute("out_path");
                                aNewItem.mOutFileName       = aReader.GetAttribute("res_name");
                                aNewItem.mFunctionPrefix    = aReader.GetAttribute("func_prefix");
                                aNewItem.mReportUnusedFiles = Convert.ToBoolean(aReader.GetAttribute("report_unused"));
                                aNewItem.mWriteDelBatchFile = Convert.ToBoolean(aReader.GetAttribute("write_delbatch"));
                                aNewItem.mBaseResPath       = aReader.GetAttribute("base_respath");

                                mProjectMRUList.Add(aNewItem);
                            }
                            break;
                        case XmlNodeType.Text: // Display the text in each element.

                            break;
                        case XmlNodeType.EndElement: // Display the end of the element.

                            break;
                    }
                }

                aReader.Close();
            }

            // update project names
            ArrayList aProjectNameList = new ArrayList();

            foreach (ProjectMRUItem i in mProjectMRUList)
            {
                aProjectNameList.Add(i.mProjectName);
            }

            mruComboBox.DataSource = aProjectNameList;

            // select the first project or, if no project exists, 
            // create a new one via simulated newProjectButton click
            if (aProjectNameList.Count > 0)
                mruComboBox.SelectedIndex = 0;
            else
            {
                // moved from CTor to OnLoad, CTor cant raise control events
                // because the controls are not created yet :)
                newProjectButton.PerformClick();
            }
        }

        /// <summary>
        /// Write a string to console output. 
        /// Splits the string in pieces to avoid horizontal scrollbar.
        /// </summary>
        /// <param name="theText">The text to print.</param>
        public void WriteLine(String theText)
        {
            string[] aSplitString = theText.Split(new Char[] {' '});
            String aOutString = "";

            foreach (string aString in aSplitString) 
            {
                if (aString.Trim() != "")
                {
                    aOutString += " " + aString;
                    if (aOutString.Length > 50)
                    {
                        consoleOutListBox.Items.Add(aOutString);
                        aOutString = "";
                    }
                }
            }

            consoleOutListBox.Items.Add(aOutString);

            int itemsPerPage = (int)(consoleOutListBox.Height / consoleOutListBox.ItemHeight);
            consoleOutListBox.TopIndex = consoleOutListBox.Items.Count - itemsPerPage;
        }

        /// <summary>
        /// Detects the byte order mark of a file and returns
        /// an appropriate encoding for the file.
        /// </summary>
        /// <param name="srcFile"></param>
        /// <returns></returns>
        public static Encoding GetFileEncoding(String srcFile)
        {
            // *** Use Default of Encoding.Default (Ansi CodePage)
            Encoding enc = Encoding.Default;

            // *** Detect byte order mark if any - otherwise assume default
            byte[] buffer = new byte[5];
            FileStream file = new FileStream(srcFile, FileMode.Open);
            file.Read(buffer, 0, 5);
            file.Close();

            if (buffer[0] == 0xef && buffer[1] == 0xbb && buffer[2] == 0xbf)
                enc = Encoding.UTF8;
            else if (buffer[0] == 0xfe && buffer[1] == 0xff)
                enc = Encoding.Unicode;
            else if (buffer[0] == 0xff && buffer[1] == 0xfe)
                enc = Encoding.BigEndianUnicode;
            else if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0xfe && buffer[3] == 0xff)
                enc = Encoding.UTF32;
            else if (buffer[0] == 0x2b && buffer[1] == 0x2f && buffer[2] == 0x76)
                enc = Encoding.UTF7;

            return enc;
        }

        /// <summary>
        /// Opens a stream reader with the appropriate text encoding applied.
        /// </summary>
        /// <param name="srcFile"></param>
        public static System.IO.StreamReader OpenStreamReaderWithEncoding(String srcFile)
        {
            Encoding enc = GetFileEncoding(srcFile);
            return new StreamReader(srcFile, enc);
        }

        /// <summary>
        /// Called on closing event, save the project informations.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        void ResourceGeneratorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            XmlTextWriter aWriter = new XmlTextWriter("projects.xml", Encoding.UTF8);
            aWriter.IndentChar = '\t';
            aWriter.Formatting = Formatting.Indented;

            aWriter.WriteComment("Sexy-Kanji Resource Generator Project File (JPoag)");
            aWriter.WriteStartElement("Projects");

            foreach (ProjectMRUItem i in mProjectMRUList)
            {
                aWriter.WriteStartElement("Project");

                aWriter.WriteAttributeString("id",              i.mProjectName);
                aWriter.WriteAttributeString("xml_path",        i.mResourceXMLPath);
                aWriter.WriteAttributeString("out_path",        i.mOutDir);
                aWriter.WriteAttributeString("res_name",        i.mOutFileName);
                aWriter.WriteAttributeString("func_prefix",     i.mFunctionPrefix);
                aWriter.WriteAttributeString("report_unused",   Convert.ToString(i.mReportUnusedFiles));
                aWriter.WriteAttributeString("write_delbatch",  Convert.ToString(i.mWriteDelBatchFile));
                aWriter.WriteAttributeString("base_respath",    i.mBaseResPath);

                aWriter.WriteEndElement(/*"Project"*/);
            }

            aWriter.WriteEndElement(/*"Projects"*/);

            aWriter.Close();
        }

        /// <summary>
        /// Open a file dialog to choose the Resource.xml file.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void browseResXML_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "Resource.xml file to Generate Code From.";
            openFileDialog1.Multiselect = false;
            openFileDialog1.DefaultExt = "xml";
            openFileDialog1.Filter = "XML files (*.xml)|*.xml|All files (*.*)|*.*";

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                resourceXMLTextBox.Text = openFileDialog1.FileName;
            }
        }

        /// <summary>
        /// Create a new project.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void newProjectButton_Click(object sender, EventArgs e)
        {
            ProjectMRUItem aNewItem = new ProjectMRUItem();
            aNewItem.mProjectName = "New Project";
            aNewItem.mOutFileName = "Res";

            int aNewIndex = mProjectMRUList.Count;
            mProjectMRUList.Add(aNewItem);

            ArrayList aNewMRUList = new ArrayList();
            foreach (ProjectMRUItem i in mProjectMRUList)
            {
                aNewMRUList.Add(i.mProjectName);
            }
            mruComboBox.DataSource = aNewMRUList;

            if (aNewIndex >= 0 && aNewIndex < aNewMRUList.Count)
            {
                mruComboBox.SelectedIndex = aNewIndex;
            }
        }

        /// <summary>
        /// Delete current project.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void deleteProjectButton_Click(object sender, EventArgs e)
        {
            int anIndex = mruComboBox.SelectedIndex;

            if (anIndex >= 0 && anIndex < mProjectMRUList.Count)
            {
                mProjectMRUList.RemoveAt(anIndex);
                anIndex = 0;

                ArrayList aNewMRUList = new ArrayList();
                foreach (ProjectMRUItem i in mProjectMRUList)
                {
                    aNewMRUList.Add(i.mProjectName);
                }
                mruComboBox.DataSource = aNewMRUList;

                if (anIndex >= 0 && anIndex < aNewMRUList.Count)
                {
                    mruComboBox.SelectedIndex = anIndex;
                }
                else
                {
                    newProjectButton.PerformClick();
                }
            }
        }

        /// <summary>
        /// Open a folder browser dialog to choose the output directory.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void browseOutDirButton_Click(object sender, EventArgs e)
        {
            if (outputDirTextBox.Text != "")
                folderBrowserDialog1.SelectedPath = outputDirTextBox.Text;

            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                outputDirTextBox.Text = folderBrowserDialog1.SelectedPath;
            }
        }
        
        /// <summary>
        /// UI enable/disable.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void reportUnusedFilesCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (reportUnusedFilesCheckBox.Checked == true)
            {
                baseResPathTextBox.Enabled = true;
                browseBaseResPathButton.Enabled = true;
                writeDelBatchCheckBox.Enabled = true;
            }
            else
            {
                baseResPathTextBox.Text = "";
                writeDelBatchCheckBox.Checked = false;
                baseResPathTextBox.Enabled = false;
                browseBaseResPathButton.Enabled = false;
                writeDelBatchCheckBox.Enabled = false;
            }
        }

        /// <summary>
        /// Open a folder browser dialog to choose the base resource directory.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void browseBaseResPathButton_Click(object sender, EventArgs e)
        {
            if (baseResPathTextBox.Text != "")
                folderBrowserDialog1.SelectedPath = baseResPathTextBox.Text;

            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                baseResPathTextBox.Text = folderBrowserDialog1.SelectedPath;
            }
        }
        
        /// <summary>
        /// Parse the XML Resource file, generate C++ source files,
        /// report unused files (if checked), save project information.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void generate_Click(object sender, EventArgs e)
        {
            //////////////////////////////////////////////////////////////////////////
            // Instantiate the classes which are doing the work for us.
            //////////////////////////////////////////////////////////////////////////
            ResourceParser  aResGen         = new ResourceParser(this);
            CodeWriter      aCodeWriter     = new CodeWriter(this);
            UnusedFiles     aUnusedFiles    = new UnusedFiles();

            int anIndex = mruComboBox.SelectedIndex;

            if (anIndex >= 0 && anIndex < mProjectMRUList.Count)
            {
                ProjectMRUItem anItem = mProjectMRUList[anIndex] as ProjectMRUItem;

                anItem.mOutFileName         = outputFileTextBox.Text;
                anItem.mOutDir              = outputDirTextBox.Text;
                anItem.mResourceXMLPath     = resourceXMLTextBox.Text;
                anItem.mFunctionPrefix      = functionPrefixTextBox.Text;
                anItem.mReportUnusedFiles   = reportUnusedFilesCheckBox.Checked;
                anItem.mWriteDelBatchFile   = writeDelBatchCheckBox.Checked;
                anItem.mBaseResPath         = baseResPathTextBox.Text;

                Cursor.Current = Cursors.WaitCursor;
                consoleOutListBox.Items.Clear();

                WriteLine("Parsing resource...");

                if (aResGen.ParseResource(anItem.mResourceXMLPath) == true)
                {
                    WriteLine("Parsing resource file successful!");
                    WriteLine("Writing source code...");

                    if (aCodeWriter.WriteSourceCode(anItem.mOutDir, anItem.mOutFileName, anItem.mFunctionPrefix) == true)
                    {
                        WriteLine("Writing source code successful!");
                    }
                    else
                    {
                        Cursor.Current = Cursors.Default;
                        
                        WriteLine("Error writing source code: " + aCodeWriter.GetError());
                        MessageBox.Show(this, aCodeWriter.GetError(), "Code Generator Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                        return;
                    }

                    if (anItem.mReportUnusedFiles == true)
                    {
                        String aReport = aUnusedFiles.ReportUnusedFiles(anItem.mBaseResPath, anItem.mWriteDelBatchFile);

                        Cursor.Current = Cursors.Default;

                        if (aUnusedFiles.HadError() == true)
                        {
                            aReport = aUnusedFiles.GetError();
                            WriteLine("Error Checking Unused Files :" + aReport);
                            MessageBox.Show(this, aReport, "Error Checking Unused Files");
                        } 
                        else
                        {
                            WriteLine("Unused Resources:");
                            WriteLine(aReport);
                            MessageBox.Show(this, aReport, "Unused Resources");
                        }
                    }
                }
                else
                {
                    Cursor.Current = Cursors.Default;

                    WriteLine("Error parsing resource: " + aResGen.GetError());
                    MessageBox.Show(this, aResGen.GetError(), "Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                    return;
                }
            }

            WriteLine("Generation Complete!");
        }

        /// <summary>
        /// Update UI to selected project data.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void mruComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            int anIndex = mruComboBox.SelectedIndex;

            if (anIndex >= 0 && anIndex < mProjectMRUList.Count)
            {
                ProjectMRUItem anItem = mProjectMRUList[anIndex] as ProjectMRUItem;

                outputFileTextBox.Text          = anItem.mOutFileName;
                outputDirTextBox.Text           = anItem.mOutDir;
                projectNameTextBox.Text         = anItem.mProjectName;
                resourceXMLTextBox.Text         = anItem.mResourceXMLPath;
                functionPrefixTextBox.Text      = anItem.mFunctionPrefix;
                reportUnusedFilesCheckBox.Checked = anItem.mReportUnusedFiles;
                writeDelBatchCheckBox.Checked   = anItem.mWriteDelBatchFile;
                baseResPathTextBox.Text         = anItem.mBaseResPath;
            }
        }

        /// <summary>
        /// Update label to display the names for the source files.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void outputFileTextBox_TextChanged(object sender, EventArgs e)
        {
            outnameLabel.Text = outputFileTextBox.Text + ".h / " + outputFileTextBox.Text + ".cpp";
        }

        /// <summary>
        /// Update MRU combo box for current project name.
        /// </summary>
        /// <param name="sender">sender info</param>
        /// <param name="e">event data</param>
        private void projectNameTextBox_LostFocus(object sender, EventArgs e)
        {
            int anIndex = mruComboBox.SelectedIndex;

            if (anIndex >= 0 && anIndex < mProjectMRUList.Count)
            {
                ProjectMRUItem anItem = mProjectMRUList[anIndex] as ProjectMRUItem;

                anItem.mProjectName = projectNameTextBox.Text;
                mProjectMRUList[anIndex] = anItem;

                ArrayList aNewMRUList = new ArrayList();
                foreach (ProjectMRUItem i in mProjectMRUList)
                {
                    aNewMRUList.Add(i.mProjectName);
                }
                mruComboBox.DataSource = aNewMRUList;

                if (anIndex >=0 && anIndex < aNewMRUList.Count)
                {
                    mruComboBox.SelectedIndex = anIndex;
                }
            }
        }
    }
}