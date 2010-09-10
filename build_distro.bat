@echo off

echo Starting Distribution Build...
mkdir SexyKanjiBeta2.00.XXX_July12_2010

mkdir SexyKanjiBeta2.00.XXX_July12_2010\Documentation
mkdir SexyKanjiBeta2.00.XXX_July12_2010\Package\Kanji\lib

echo Exporting SVN...
svn export --force .\Kanji\headers .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Kanji\headers
svn export --force .\Content .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Content
svn export --force .\Demos .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Demos
svn export --force .\SexyKanji .\SexyKanjiBeta2.00.XXX_July12_2010\Package\SexyKanji
svn export --force .\Tools .\SexyKanjiBeta2.00.XXX_July12_2010\Tools 
svn export --force .\Demos.xcodeproj .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Demos.xcodeproj 
copy .\Demos_2005.sln .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Demos_2005.sln
copy .\Demos_2008.sln .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Demos_2008.sln
echo Finished!

echo Copying Documentation...
copy ".\Documentation\Sexy Kanji Resource Management.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010\Documentation\Sexy Kanji Resource Management.pdf" 
copy ".\Documentation\SexyKanjiFramework.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010\Documentation\SexyKanjiFramework.pdf" 

copy ".\Documentation\Sexy App Framework License.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010\PopCap Framework License.pdf" 
copy ".\Documentation\Sexy Kanji Credits.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010\Sexy Kanji Credits.pdf" 
copy ".\Documentation\Sexy Kanji Framework License.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010\SexyKanji License.pdf" 

copy ".\Documentation\Readme_First.txt" .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Kanji

copy .\Documentation\*.txt .\SexyKanjiBeta2.00.XXX_July12_2010\Package
echo Finished!

pause