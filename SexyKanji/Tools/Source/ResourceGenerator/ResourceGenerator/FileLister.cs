/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   FileLister.cs
///
/// Purpose:    Create a file list out of a directory tree.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace ResourceGenerator
{
    /// <summary>
    /// Create a file list out of a directory tree.
    /// </summary>
    public class FileLister
    {
        /************************************************************************/
        /* Path from which the search begins.
        /************************************************************************/
        private String      mPath;

        /************************************************************************/
        /* The file search pattern (e.g. *.*).
        /************************************************************************/
        private String      mPattern;

        /************************************************************************/
        /* The recursion level
        /* 0 : unlimited subdirectory recursion
        /* 1 : only current directory
        /* n : n subdirectory levels recursion (where n > 1)
        /************************************************************************/
        private int         mRecursion;

        /************************************************************************/
        /* Store the found files.
        /************************************************************************/
        private ArrayList   mFiles;

        /// <summary>
        /// Create the file list, according to the specified RecursionLevel.
        /// </summary>
        /// <param name="theSearchPath">The search path.</param>
        /// <param name="theRecursionLevel">The recursion level.</param>
        private void CreateListRecursive(String theSearchPath, int theRecursionLevel)
        {
            DirectoryInfo aThisLevel    = new DirectoryInfo(theSearchPath);
            DirectoryInfo[] aChildLevel = aThisLevel.GetDirectories();

            if (theRecursionLevel != 1)
            {
                foreach (DirectoryInfo Child in aChildLevel)
                {
                    CreateListRecursive(Child.FullName, theRecursionLevel - 1);
                }
            }

            FileInfo[] aFiles = aThisLevel.GetFiles(mPattern);

            foreach (FileInfo aFile in aFiles)
            {
                mFiles.Add(aFile);
            }
        }

        /// <summary>
        /// Create a (recursive) file list.
        /// </summary>
        /// <param name="theSearchPath">Path from which the search begins.</param>
        /// <param name="theSearchPattern">File name pattern (e.g. "*.*").</param>
        /// <param name="RecursionLevel">
        /// RecursionLevel = 0 : unlimited subdirectory recursion
        /// RecursionLevel = 1 : only current directory
        /// RecursionLevel = n : n subdirectory levels recursion (where n > 1)
        /// </param>
        /// <returns>ArrayList of FileInfo with the files found</returns>
        public ArrayList CreateList(String theSearchPath, String theSearchPattern, int theRecursionLevel)
        {
            mPath       = theSearchPath;
            mPattern    = theSearchPattern;
            mRecursion  = theRecursionLevel;

            if (mPath == null)
                return null;
                
            if (mPath[mPath.Length - 1] != Path.DirectorySeparatorChar)
                mPath += Path.DirectorySeparatorChar;

            if (Directory.Exists(mPath) == false)
                return null; //  throw new Exception("Directory not found: " + mPath);
            
            mFiles = new ArrayList();

            CreateListRecursive(mPath, mRecursion);

            return mFiles;
        }
    }
}
