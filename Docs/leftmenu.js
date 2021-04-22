function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID0\" class=\"headerLeftMenuInActive\"><a id=\"aID0\" href=\"#\" OnMouseOver=\"link('','index',this)\" class=\"leftMenuLinkHeadInActive\">Default mainpage</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID2262\" class=\"leftMenuInActive\"><a id=\"aID2262\" href=\"#\" OnMouseOver=\"link('_dir','SexyKanji/SexyKanji0',this)\" class=\"leftMenuLinkInActive\">SexyKanji</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Miscellaneous</div>\n");
document.write("<div id=\"divID159\" class=\"leftMenuInActive\"><a id=\"aID159\" href=\"#\" OnMouseOver=\"link('_statsindex','common/statistics/index',this)\" class=\"leftMenuLinkInActive\">Statistics</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
