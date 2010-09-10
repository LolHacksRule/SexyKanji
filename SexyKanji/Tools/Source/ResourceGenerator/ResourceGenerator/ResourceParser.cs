/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   ResourceParser.cs
///
/// Purpose:    Parse a (resource) XML file and save the resource data.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Windows.Forms;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace ResourceGenerator
{
    /// <summary>
    /// Class to parse a (resource) XML file and save the resource data.
    /// </summary>
    class ResourceParser : ResourceData
    {
        /************************************************************************/
        /* The form to write console output information.
        /************************************************************************/
        ResourceGeneratorForm mOutputForm = null;

        /************************************************************************/
        /* String to store error information.
        /************************************************************************/
        String          mErrorString;

        /************************************************************************/
        /* Read the resource XML file.
        /************************************************************************/
        XmlTextReader   aXMLReader;

        /// <summary>
        /// CTor, initialize member.
        /// </summary>
        /// <param name="theOutputForm">The form to write console output.</param>
        public ResourceParser(ResourceGeneratorForm theOutputForm)
        {
            mOutputForm     = theOutputForm;
            mErrorString    = "";
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
        /// Add a dir/file to the corresponding Sets to save the data.
        /// This data is used to determine the unused files.
        /// </summary>
        /// <param name="thePath">File/path to add.</param>
        private void AddFile(String thePath)
        {
            if (thePath == null)
                return;

            String aPath = mDirPrefix + thePath;
            aPath = Common.GetStandardFileName(aPath);

            mDirSet.Add(Path.GetDirectoryName(aPath));

            aPath = Common.RemoveExtensionAndUnderscores(aPath);

            if (aPath.Length > 0 && aPath[0] != '!') // program supplied resource
            {
                mFileSet.Add(aPath);
            }
        }

        /// <summary>
        /// Parse the set defaults, saves mDefaultIdPrefix and mDirPrefix.
        /// </summary>
        /// <returns>True, if all set default information is provided, otherwise false.</returns>
        private bool ParseSetDefaults()
        {
            String anIdPrefix = aXMLReader.GetAttribute("idprefix");

            if (anIdPrefix != null)
            {
                mDefaultIdPrefix = Common.RemoveTrailingSlash(anIdPrefix);  
            }

            String aPath = aXMLReader.GetAttribute("path");

            if (aPath != null)
            {
                aPath = Common.GetStandardFileName(aPath);
                mDirPrefix = aPath;
                mDirPrefix = Common.AddTrailingSlash(mDirPrefix);

                return true;
            }
            else
            {
                mErrorString = "No path specified";
                if (anIdPrefix != null)
                    mErrorString += " ID: " + anIdPrefix;

                return false;
            }
        }

        /// <summary>
        /// Parse common resource attributes, save the data in resource dictionary and variable list.
        /// </summary>
        /// <param name="theType">The resource type.</param>
        /// <returns>False if attribute path is missing or the element already exists, otherwise true.</returns>
        private bool ParseCommonResource(ResType theType)
        {
            String aPath = aXMLReader.GetAttribute("path");
            String anId = aXMLReader.GetAttribute("id");
            String aTempString;

            if (aPath == null)
            {
                mErrorString = "No path specified";
                if (anId != null)
                    mErrorString += " ID: " + anId;

                return false;
            }
            
            if (anId == null)
            {
                anId = mDefaultIdPrefix + Common.GetFileName(aPath, true);
            }
            else
            {
                anId = mDefaultIdPrefix + anId;
            }

            mOutputForm.WriteLine("Parsing Element " + anId);
            Application.DoEvents();

            AddFile(aPath);

            if (theType == ResType.Image)
            {
                aTempString = aXMLReader.GetAttribute("alphaimage");

                if (aTempString != null)
                    AddFile(aTempString);

                aTempString = aXMLReader.GetAttribute("alphagrid");

                if (aTempString != null)
                    AddFile(aTempString);
            }

            BaseRes aRes = new BaseRes();
               
            aRes.mType = theType;
            aRes.mId = anId;

            aTempString = aXMLReader.GetAttribute("alias");

            if (aTempString != null)
                aRes.mAlias.Add(mDefaultIdPrefix + aTempString);

            for (ushort i = 0; i < MAX_ALIASES; i++)
            {
                aTempString = aXMLReader.GetAttribute("alias" + i.ToString());

                if (aTempString != null)
                    aRes.mAlias.Add(mDefaultIdPrefix + aTempString);
            }

            try
            {
                mCurrentResGroup.Add(anId, aRes);
            }
            catch (ArgumentException)
            {
                mErrorString = "An element with Key = " + anId + " already exists.";
                return false;
            }
                        
            // save the resource information for the codewriter
            mVariableList.Add(aRes);

            return true;
        }

        /// <summary>
        /// Determine resource type and call the parse method.
        /// </summary>
        /// <returns>False, if there is a parsing error, otherwise true.</returns>
        private bool ParseResources()
        {
            while (aXMLReader.Read())
            {
                if (aXMLReader.NodeType == XmlNodeType.Element)
                {
                    if (aXMLReader.Name == "Image")
                    {
                        if (ParseCommonResource(ResType.Image) == false)
                            return false;
                    }
                    else if (aXMLReader.Name == "Sound")
                    {
                        if (ParseCommonResource(ResType.Sound) == false)
                            return false;
                    }
                    else if (aXMLReader.Name == "Music")
                    {
                        if (ParseCommonResource(ResType.Music) == false)
                            return false;
                    }
                    else if (aXMLReader.Name == "Font")
                    {
                        if (ParseCommonResource(ResType.Font) == false)
                            return false;
                    }
                    else if (aXMLReader.Name == "Movie")
                    {
                        if (ParseCommonResource(ResType.Movie) == false)
                            return false;
                    }
                    else if (aXMLReader.Name == "SetDefaults")
                    {
                        if (ParseSetDefaults() == false)
                            return false;
                    }
                }
                else if (aXMLReader.NodeType == XmlNodeType.EndElement)
                {
                    // the end element of the resource group
                    return true;
                }
            }

            return true;
        }
        
        /// <summary>
        /// Start parsing the resource XML file.
        /// </summary>
        /// <param name="theResFile">The file with the resource definitions.</param>
        /// <returns>False in case of any error, otherwise true.</returns>
        public bool ParseResource(String theResFile)
        {
            if (theResFile == "")
            {
                mErrorString = "Please specify a XML Resource File!";
                return false;
            }

            try
            {
                if (File.Exists(theResFile))
                {
                    aXMLReader = new XmlTextReader(ResourceGeneratorForm.OpenStreamReaderWithEncoding(theResFile));

                    // find ResourceManifest tag
                    while (aXMLReader.Read())
                    {
                        if (aXMLReader.NodeType == XmlNodeType.Element)
                        {
                            if (aXMLReader.Name.ToLower() == "resourcemanifest")
                            {
                                mOutputForm.WriteLine("Found ResourceManifest");
                                Application.DoEvents();
                                break;
                            }
                        }
                    }

                    if (aXMLReader.EOF == true)
                    {
                        aXMLReader.Close();
                        mErrorString = "Expecting tag ResourceManifest";
                        
                        return false;
                    }

                    // start parsing the nodes
                    while (aXMLReader.Read())
                    {
                        switch (aXMLReader.NodeType)
                        {
                            case XmlNodeType.Element: // The node is an element.

                                // each resource group is an element
                                if (aXMLReader.Name.ToLower() == "resources")
                                {
                                    String aResGroup = aXMLReader.GetAttribute("id");
                                    if (aResGroup == null)
                                    {
                                        mErrorString = "No id for resources specified";
                                        if (aXMLReader.HasLineInfo() == true)
                                            mErrorString += " - Line: " + aXMLReader.LineNumber + " Position: " + aXMLReader.LinePosition;
                        
                                        return false;
                                    }

                                    // create a new dictionary for the resource group
                                    mCurrentResGroup = new SortedDictionary<String, BaseRes>();
                                    mResGroups.Add(aResGroup, mCurrentResGroup);

                                    mOutputForm.WriteLine("Parsing Group " + aResGroup);
                                    Application.DoEvents();

                                    // start parsing the group
                                    if (ParseResources() == false)
                                        break;
                                }
                                else
                                {
                                    mErrorString = "Invalid Section '" + aXMLReader.Name + "'";
                                    break;
                                }
                                break;
                        }

                        if (mErrorString != "")
                        {
                            // an error occured, try to get line information from xml parser
                            if (aXMLReader.HasLineInfo() == true)
                                mErrorString += " - Line: " + aXMLReader.LineNumber + " Position: " + aXMLReader.LinePosition;
                            break;
                        }
                    }

                    mOutputForm.WriteLine("Closing Reader");
                    Application.DoEvents();

                    aXMLReader.Close();
                }
                else
                {
                    mErrorString = "File not found: " + theResFile;

                    return false;
                }
            }
            catch (Exception aException)
            {
                mErrorString += aException.Message;

                return false;
            }

            return (mErrorString == "");
        }
    }
}
