@echo off

echo Starting Distribution Build...
mkdir SexyKanjiBeta2.00.XXX_July12_2010_demo

mkdir SexyKanjiBeta2.00.XXX_July12_2010_demo\Documentation
mkdir SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib

REM Copy Demo Libs in place of release libs
copy .\Kanji\lib\kanji_vc8_demo_ansi.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanji_vc8_ansi.lib
copy .\Kanji\lib\kanji_vc8_demo_ansi_d.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanji_vc8_ansi_d.lib
copy .\Kanji\lib\kanji_vc8_demo_unicode.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanji_vc8_unicode.lib
copy .\Kanji\lib\kanji_vc8_demo_unicode_d.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanji_vc8_unicode_d.lib

copy .\Kanji\lib\kanjisounddx_vc8_ansi.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjisounddx_vc8_ansi.lib
copy .\Kanji\lib\kanjisounddx_vc8_ansi_d.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjisounddx_vc8_ansi_d.lib
copy .\Kanji\lib\kanjisounddx_vc8_unicode.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjisounddx_vc8_unicode.lib
copy .\Kanji\lib\kanjisounddx_vc8_unicode_d.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjisounddx_vc8_unicode_d.lib

copy .\Kanji\lib\kanjivideo_vc8_ansi.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjivideo_vc8_ansi.lib
copy .\Kanji\lib\kanjivideo_vc8_ansi_d.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjivideo_vc8_ansi_d.lib
copy .\Kanji\lib\kanjivideo_vc8_unicode.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjivideo_vc8_unicode.lib
copy .\Kanji\lib\kanjivideo_vc8_unicode_d.lib .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\lib\kanjivideo_vc8_unicode_d.lib


echo Exporting SVN...
svn export --force .\Kanji\headers .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji\headers
svn export --force .\Content .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Content
svn export --force .\Demos .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Demos
svn export --force .\SexyKanji .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\SexyKanji
svn export --force .\Tools .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Tools 
svn export --force .\Demos.xcodeproj .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Demos.xcodeproj 
copy .\Demos_2005.sln .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Demos_2005.sln
copy .\Demos_2008.sln .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Demos_2008.sln
echo Finished!

echo Copying Documentation...
copy ".\Documentation\Sexy Kanji Resource Management.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_demo\Documentation\Sexy Kanji Resource Management.pdf" 
copy ".\Documentation\SexyKanjiFramework.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_demo\Documentation\SexyKanjiFramework.pdf" 

copy ".\Documentation\Sexy App Framework License.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_demo\PopCap Framework License.pdf" 
copy ".\Documentation\Sexy Kanji Credits.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_demo\Sexy Kanji Credits.pdf" 
copy ".\Documentation\Sexy Kanji Framework License.pdf" ".\SexyKanjiBeta2.00.XXX_July12_2010_demo\SexyKanji License.pdf" 

copy ".\Documentation\Readme_First.txt" .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package\Kanji

copy .\Documentation\*.txt .\SexyKanjiBeta2.00.XXX_July12_2010_demo\Package

echo Starting Zip.
zip -r -9 -q SexyKanjiBeta2.00.XXX_July12_2010_demo.zip .\SexyKanjiBeta2.00.XXX_July12_2010_demo
Zip Complete.

echo Finished!

pause