/// ********************************************************************
/// Sexy Kanji Resource Generator Version 0.1
/// -------------------------------------------------------------    
/// Created:    2010/03/16
/// Filename:   ResourceData.cs
///
/// Purpose:    Abstract base class for shared resource data.
///             
///             Heiko Fischer - heiko@fischeronline.de
/// ********************************************************************

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace ResourceGenerator
{
    /// <summary>
    /// Abstract base class for shared resource data.
    /// </summary>
    public abstract class ResourceData
    {
        /// <summary>
        /// Simple wrapper class for a set collection based on a sorted dictionary.
        /// </summary>
        public class Set<T> : SortedDictionary<T, bool>
        {
            /// <summary>
            /// Overwritten Add method to catch the ArgumentException
            /// </summary>
            /// <param name="theItem">The item to add.</param>
            public void Add(T theItem)
            {
                try
                {
                    this.Add(theItem, true);
                }
                catch (ArgumentException)
                {
                    // ignore, its a set
                }
            }
        }

        /// <summary>
        /// The valid resource types.
        /// </summary>
        public enum ResType
        {
            Image,
            Sound,
            Music,
            Font,
            Movie,
            Unknown
        };

        /// <summary>
        /// Helper class for resource definitions.
        /// </summary>
        public class BaseRes
        {
            public ResType      mType;    // type of resource
            public String       mId;      // ID of resource
            public ArrayList    mAlias;   // alias list

            public BaseRes() { mType = ResType.Unknown; mId = ""; mAlias = new ArrayList(); }
        };

        /************************************************************************/
        /* max number of allowed aliases for a resource
        /************************************************************************/
        public const ushort         MAX_ALIASES = 10;

        /************************************************************************/
        /* the main dictionary with all the data for the resources
        /* and the currently used dictionary for the resource group
        /************************************************************************/
        public static SortedDictionary<String, SortedDictionary<String, BaseRes>> mResGroups;
        public static SortedDictionary<String, BaseRes> mCurrentResGroup;

        /************************************************************************/
        /* save BaseRes information for every resource
        /************************************************************************/
        public static ArrayList     mVariableList;

        /************************************************************************/
        /* save (unique) file and directory information
        /************************************************************************/
        public static Set<String>   mFileSet; 
        public static Set<String>   mDirSet;

        /************************************************************************/
        /* the current Id prefix (e.g. "FONT_")
        /************************************************************************/
        public static String        mDefaultIdPrefix;

        /************************************************************************/
        /* the current directory prefix (e.g. "fonts/")
        /************************************************************************/
        public static String        mDirPrefix;
        
        /// <summary>
        /// CTor, initialize member.
        /// </summary>
        public ResourceData()
        {
            mResGroups      = new SortedDictionary<String, SortedDictionary<String, BaseRes>>();
            mVariableList   = new ArrayList();
            mFileSet        = new Set<String>();
            mDirSet         = new Set<String>();
        }
    }
}
