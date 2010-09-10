/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   Common.cs
///
/// Purpose:    Path/file/directory helper.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace ResourceGenerator
{
    /// <summary>
    /// Common String manipulation functions for files/directories.
    /// </summary>
    class Common
    {
        /// <summary>
        /// Lower the file name an replace backslash with slash.
        /// </summary>
        /// <param name="theName">The path with/without file name.</param>
        /// <returns>The standard file name.</returns>
        public static String GetStandardFileName(String theName)
        {
            String aName = "";
            
            theName = theName.ToLower();

            // Path.AltDirectorySeparatorChar = /
            // Path.DirectorySeparatorChar = \
            for (int i=0; i < theName.Length; i++)
            {
                char aChar = theName[i];
                if (aChar == Path.DirectorySeparatorChar)
                    aChar = Path.AltDirectorySeparatorChar;

                aName += aChar;
            }

            return aName;
        }

        /// <summary>
        /// Get the file name of the specified path.
        /// </summary>
        /// <param name="thePath">The path with file.</param>
        /// <param name="noExtension">True to remove the file extension.</param>
        /// <returns>The file name.</returns>
        public static String GetFileName(String thePath, bool noExtension)
        {
            if (noExtension == true)
                return Path.GetFileNameWithoutExtension(thePath);
            else
                return Path.GetFileName(thePath);
        }

        /// <summary>
        /// Remove the file extension and underscores from a path.
        /// </summary>
        /// <param name="thePath">The path with file.</param>
        /// <returns></returns>
        public static String RemoveExtensionAndUnderscores(String thePath)
        {
            if (thePath.Length == 0)
                return "";

            thePath = GetStandardFileName(thePath);
            int aSlashPos = thePath.LastIndexOf(Path.AltDirectorySeparatorChar) + 1;

            if (thePath[aSlashPos] == '_')
                thePath = thePath.Substring(0, aSlashPos) + thePath.Substring(aSlashPos + 1);

            if (thePath.Length == 0)
                return "";

            int aDotPos = thePath.LastIndexOf('.');
            if (aDotPos != -1)
                thePath = thePath.Substring(0, aDotPos);

            if (thePath[thePath.Length - 1] == '_')
                thePath = thePath.Substring(0, thePath.Length - 1);

            return thePath;
        }

        /// <summary>
        /// Add a trailing slash to a directory.
        /// </summary>
        /// <param name="theDirectory">The directory.</param>
        /// <returns>The directory with trailing slash.</returns>
        public static String AddTrailingSlash(String theDirectory)
        {
            if (theDirectory.Length == 0)
                return "";
            
            int aLength = theDirectory.Length - 1;

            if (theDirectory[aLength] == Path.DirectorySeparatorChar)
                theDirectory = theDirectory.Substring(0, aLength) + Path.AltDirectorySeparatorChar;  
            else
                theDirectory += Path.AltDirectorySeparatorChar;

            return theDirectory;
        }

        /// <summary>
        /// Remove a trailing slash from a directory.
        /// </summary>
        /// <param name="theDirectory">The directory.</param>
        /// <returns>The directory without trailing slash.</returns>
        public static String RemoveTrailingSlash(String theDirectory)
        {
            int aLen = theDirectory.Length;
            
            if ((aLen > 0) && ((theDirectory[aLen - 1] == Path.DirectorySeparatorChar) || (theDirectory[aLen - 1] == Path.AltDirectorySeparatorChar)))
                return theDirectory.Substring(0, aLen - 1);
            else
                return theDirectory;
        }
    }
}
