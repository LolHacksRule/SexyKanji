@echo off

echo Starting Distribution Build...
mkdir SexyKanjiBeta2.00.XXX_July12_2010_release

mkdir SexyKanjiBeta2.00.XXX_July12_2010_release\Documentation

mkdir SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Kanji

echo Exporting SVN...
svn export --force .\Kanji .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Kanji
svn export --force .\Content .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Content
svn export --force .\Demos .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Demos
svn export --force .\SexyKanji .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\SexyKanji
svn export --force .\Tools .\SexyKanjiBeta2.00.XXX_July12_2010_release\Tools 
svn export --force .\Demos.xcodeproj .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Demos.xcodeproj 
copy .\Demos_2005.sln .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Demos_2005.sln
copy .\Demos_2008.sln .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Demos_2008.sln
echo Finished!

echo Copying Documentation...
copy ".\Documentation\Sexy Kanji Resource Management.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_release\Documentation\Sexy Kanji Resource Management.pdf" 
copy ".\Documentation\SexyKanjiFramework.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_release\Documentation\SexyKanjiFramework.pdf" 

copy ".\Documentation\Sexy App Framework License.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_release\PopCap Framework License.pdf" 
copy ".\Documentation\Sexy Kanji Credits.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_release\Sexy Kanji Credits.pdf" 
copy ".\Documentation\Sexy Kanji Framework License.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_release\SexyKanji License.pdf" 

copy ".\Documentation\Readme_First.txt" .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package\Kanji

copy .\Documentation\*.txt .\SexyKanjiBeta2.00.XXX_July12_2010_release\Package

echo Starting Zip.
zip -r -9 -q SexyKanjiBeta2.00.XXX_July12_2010_release.zip .\SexyKanjiBeta2.00.XXX_July12_2010_release
Zip Complete.

echo Finished!

pause