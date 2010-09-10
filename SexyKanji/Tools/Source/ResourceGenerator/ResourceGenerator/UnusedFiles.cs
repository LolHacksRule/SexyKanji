/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   UnusedFiles.cs
///
/// Purpose:    Create a Report of files, not used in the resource XML definition.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace ResourceGenerator
{
    /// <summary>
    /// Check the directory of files not used in the resource XML definition.
    /// Careful: If you reference a image file for a font with a different name
    /// than the font descriptor XML file, the image will be marked as unused!
    /// Example: "myfont.xml" with reference to the image "myfont1.png" then
    /// myfont1.png is marked as unused.
    /// </summary>
    class UnusedFiles : ResourceData
    {
        /************************************************************************/
        /* String to store error information.
        /************************************************************************/
        String          mErrorString;

        /************************************************************************/
        /* Write the batch file to delete unused resources.
        /************************************************************************/
        StreamWriter    aStreamWriter;

        /************************************************************************/
        /* Count the number of files found.
        /************************************************************************/
        uint mFileCounter;

        /// <summary>
        /// CTor, initialize member
        /// </summary>
        public UnusedFiles()
        {
            mErrorString = "";
            mFileCounter = 0;
        }

        /// <summary>
        /// Get the current error.
        /// </summary>
        /// <returns>The error as string.</returns>
        public String GetError()
        {
            return mErrorString;
        }

        /// <summary>
        /// Get error state.
        /// </summary>
        /// <returns>true if any error, otherwise false.</returns>
        public bool HadError()
        {
            return mErrorString != "";
        }
   
        /// <summary>
        /// Get the unused files for a specified directory.
        /// </summary>
        /// <param name="theBaseResPath">The path that contains the directories with the resources.</param>
        /// <param name="theDir">The directory to search for unused files.</param>
        /// <param name="theWriteBatchFile">True, if a batch file should be written.</param>
        /// <returns>Report of unused files in the specified directory.</returns>
        private string GetUnusedFilesForDir(String theBaseResPath, String theDir, bool theWriteBatchFile)
        {
            String aResult  = "";
            String aPath    = theBaseResPath;
            String aDir     = theDir;

            //////////////////////////////////////////////////////////////////////////
            // helper to get a recursive file list.
            //////////////////////////////////////////////////////////////////////////
            FileLister aFileLister = new FileLister();
            ArrayList aFileList;

            // create path
            aPath = Common.AddTrailingSlash(aPath);
            aPath += theDir;
            aPath = Common.AddTrailingSlash(aPath);
            aPath = Common.GetStandardFileName(aPath);

            aDir = Common.AddTrailingSlash(aDir);
            aDir = Common.GetStandardFileName(aDir);

            // get the files in the directory and subdirectories
            try
            {
                aFileList = aFileLister.CreateList(aPath, "*.*", 0);
            }
            catch (DirectoryNotFoundException aException)
            {
                mErrorString += aException.Message;
                return "";
            }

            // no files found
            if (aFileList == null)
                return "";

            // iterate through the files in the directory
            foreach (FileInfo aFileInfo in aFileList)
            {
                if (aFileInfo.Attributes == FileAttributes.Directory)
                    continue;

                String aName = aFileInfo.Name;
                aName = Common.GetStandardFileName(aName);
                aName = Common.RemoveExtensionAndUnderscores(aName);
                aName = aDir + aName;

                mFileCounter++;

                // file present?
                if (mFileSet.ContainsKey(aName) != true)
                {
                    if (aResult != "")
                        aResult += ", ";

                    aResult += aFileInfo.Name;

                    if (theWriteBatchFile == true)
                    {
                        String aDelPath = theBaseResPath;
                        aDelPath += "\\";
                        aDelPath += theDir;
                        aDelPath += "\\";
                        aDelPath += aFileInfo.Name;

                        aStreamWriter.WriteLine("del \"" + aDelPath + "\"" + Environment.NewLine);
                    }
                }
            } 

            return aResult;
        }

        /// <summary>
        /// Get the unused files.
        /// </summary>
        /// <param name="theBaseResPath">The path that contains the directories with the resources.</param>
        /// <param name="theWriteBatchFile">True, if a batch file should be written.</param>
        /// <returns>Report of unused files.</returns>
        private String GetUnusedFiles(String theBaseResPath, bool theWriteBatchFile)
        {
            //////////////////////////////////////////////////////////////////////////
            // the overall result information for unused files.
            //////////////////////////////////////////////////////////////////////////
            String aResult = "";
        
            // for all directories used in the resource file
            foreach (KeyValuePair<String, bool> aDirectory in mDirSet)
            {
                String aDir = aDirectory.Key;
                // get the unused files for the current directory
                String aDirResult = GetUnusedFilesForDir(theBaseResPath, aDir, theWriteBatchFile);

                if (mErrorString != "")
                    return aResult;

                if (aDirResult != "")
                {
                    // save the result
                    aResult += Environment.NewLine + " " + aDir.ToUpper() + ": " + aDirResult + Environment.NewLine;

                    if (theWriteBatchFile)
                        aStreamWriter.WriteLine(Environment.NewLine);
                }
            }

            return aResult;
        }

        /// <summary>
        /// Get the Report of unused files.
        /// </summary>
        /// <param name="theBaseResPath">The path that contains the directories with the resources.</param>
        /// <param name="theWriteBatchFile">True, if a batch file should be written.</param>
        /// <returns>Report of unused files.</returns>
        public String ReportUnusedFiles(String theBaseResPath, bool theWriteBatchFile)
        {
            String aResult = "";
            String aBatchPath = theBaseResPath;

            if (aBatchPath == "")
            {
                return "Please specify the Base Resource Path!";
            }

            try
            {
                if (theWriteBatchFile == true)
                {
                    // create file name for batch file and open stream
                    aBatchPath = Common.AddTrailingSlash(aBatchPath);
                    aBatchPath = Common.GetStandardFileName(aBatchPath);

                    aBatchPath += "del unused resources.bat";
                    
                    try
                    {
                        aStreamWriter = new StreamWriter(aBatchPath);
                        aStreamWriter.WriteLine("REM Generated by Sexy Kanji Resource Generator " + DateTime.Now + Environment.NewLine);
                    }
                    catch (ArgumentException aException)
                    {
                        mErrorString = "Unable to open stream: " + aBatchPath + " " + aException.Message;

                        return "";
                    }
                }

                // get the unused files
                aResult = GetUnusedFiles(theBaseResPath, theWriteBatchFile);
                
                if (theWriteBatchFile == true)
                {
                    aStreamWriter.Close();
                    aStreamWriter.Dispose();
                }

                if (mErrorString != "" || aResult == "")
                {
                    // delete file in case of "no unused files" or error
                    if (theWriteBatchFile == true)
                        File.Delete(aBatchPath);

                    if (mFileCounter == 0)
                        return "No files in the specified path found.";
                    else
                        return "All files used.";
                }
                else
                {
                    if (aResult == "")
                        aResult = "All files used.";
                    else
                    {
                        if (theWriteBatchFile == true)
                            aResult += Environment.NewLine + " Batch file written: " + aBatchPath;
                    }
                }
            }
            catch (Exception aException)
            {
                mErrorString += aException.Message;

                return "";
            }

            return aResult;
        }
    }
}
