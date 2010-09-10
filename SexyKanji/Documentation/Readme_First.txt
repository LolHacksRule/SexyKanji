***********************************************************
                      Compiling Demos
***********************************************************

Before you can begin comiling and playing with SexyKanji,
you first have to insert your Kanji headers and libraries 
into the correct directories.


LIBRARIES
***********************************************************
SexyKanji expects the prebuilt libraries to be placed in the:

             "SexyKanji Beta/package/Kanji/lib"

directory. Both the 'lib' and 'a' versions of these libraries
can be placed in this directory (both the Win32 and MacOSX 
prebuilt libs).


HEADERS
***********************************************************
Sexy Kanji expects the Win32 headers to be placed in the:

       "SexyKanji Beta/package/Kanji/headers/win"
			 
directory, and the MacOSX headers to be placed in the:

       "SexyKanji Beta/package/Kanji/headers/mac"
	  
directory.


OUTPUT
***********************************************************
The Windows built binaries are moved by the linker to the 

          "SexyKanji Beta/Package/Content"
		
directory, and the MacOSX binaries are left in their 
respective folders in the build directory.  For the MacOSX
version of the demos, the Contents are copied into the 
resource bundle.