function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID2591\" class=\"headerLeftMenuInActive\"><a id=\"aID2591\" href=\"#\" OnMouseOver=\"link('_dir','ExampleCode0',this)\" class=\"leftMenuLinkHeadInActive\">Example Code</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID2592\" class=\"leftMenuInActive\"><a id=\"aID2592\" href=\"#\" OnMouseOver=\"link('_dir','Inheritance/Inheritance0',this)\" class=\"leftMenuLinkInActive\">Inheritance</a></div>\n");
document.write("<div id=\"divID2593\" class=\"leftMenuInActive\"><a id=\"aID2593\" href=\"#\" OnMouseOver=\"link('_dir','Separate_Docs/SeparateDocs0',this)\" class=\"leftMenuLinkInActive\">Separate Docs</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Classes</div>\n");
document.write("<div id=\"divID2433\" class=\"leftMenuInActive\"><a id=\"aID2433\" href=\"#\" OnMouseOver=\"link('_class','ClassInUnit0',this)\" class=\"leftMenuLinkInActive\">ClassInUnit</a></div>\n");
document.write("<div id=\"divID2419\" class=\"leftMenuInActive\"><a id=\"aID2419\" href=\"#\" OnMouseOver=\"link('_class','HowToDocument0',this)\" class=\"leftMenuLinkInActive\">HowToDocument</a></div>\n");
document.write("<div id=\"divID2425\" class=\"leftMenuInActive\"><a id=\"aID2425\" href=\"#\" OnMouseOver=\"link('_class','Overload0',this)\" class=\"leftMenuLinkInActive\">Overload</a></div>\n");
document.write("<div id=\"divID2429\" class=\"leftMenuInActive\"><a id=\"aID2429\" href=\"#\" OnMouseOver=\"link('_class','Template23971',this)\" class=\"leftMenuLinkInActive\">Template<></a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Units</div>\n");
document.write("<div id=\"divID2594\" class=\"leftMenuInActive\"><a id=\"aID2594\" href=\"#\" OnMouseOver=\"link('_unit','Unit_h0',this)\" class=\"leftMenuLinkInActive\">Unit.h</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
