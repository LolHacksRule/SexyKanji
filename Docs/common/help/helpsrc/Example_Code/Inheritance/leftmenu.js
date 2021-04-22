function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID2592\" class=\"headerLeftMenuInActive\"><a id=\"aID2592\" href=\"#\" OnMouseOver=\"link('_dir','Inheritance0',this)\" class=\"leftMenuLinkHeadInActive\">Inheritance</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Classes</div>\n");
document.write("<div id=\"divID2420\" class=\"leftMenuInActive\"><a id=\"aID2420\" href=\"#\" OnMouseOver=\"link('_class','BaseClass0',this)\" class=\"leftMenuLinkInActive\">BaseClass</a></div>\n");
document.write("<div id=\"divID2421\" class=\"leftMenuInActive\"><a id=\"aID2421\" href=\"#\" OnMouseOver=\"link('_class','DerivedLevel_10',this)\" class=\"leftMenuLinkInActive\">DerivedLevel_1</a></div>\n");
document.write("<div id=\"divID2422\" class=\"leftMenuInActive\"><a id=\"aID2422\" href=\"#\" OnMouseOver=\"link('_class','DerivedLevel_20',this)\" class=\"leftMenuLinkInActive\">DerivedLevel_2</a></div>\n");
document.write("<div id=\"divID2423\" class=\"leftMenuInActive\"><a id=\"aID2423\" href=\"#\" OnMouseOver=\"link('_class','Final_10',this)\" class=\"leftMenuLinkInActive\">Final_1</a></div>\n");
document.write("<div id=\"divID2424\" class=\"leftMenuInActive\"><a id=\"aID2424\" href=\"#\" OnMouseOver=\"link('_class','Final_20',this)\" class=\"leftMenuLinkInActive\">Final_2</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
