@echo off
mkdir SexyKanjiBeta2.00.XXX_July12_2010\Package\Kanji
svn export --force .\Kanji .\SexyKanjiBeta2.00.XXX_July12_2010\Package\Kanji
build_distro
pause