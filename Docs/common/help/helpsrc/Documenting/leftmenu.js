function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID2587\" class=\"headerLeftMenuInActive\"><a id=\"aID2587\" href=\"#\" OnMouseOver=\"link('_dir','Documenting0',this)\" class=\"leftMenuLinkHeadInActive\">Documenting</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID2517\" class=\"leftMenuInActive\"><a id=\"aID2517\" href=\"#\" OnMouseOver=\"link('_page','Autolinkgeneration0',this)\" class=\"leftMenuLinkInActive\">Auto link generation</a></div>\n");
document.write("<div id=\"divID2518\" class=\"leftMenuInActive\"><a id=\"aID2518\" href=\"#\" OnMouseOver=\"link('_page','Directories0',this)\" class=\"leftMenuLinkInActive\">Directories</a></div>\n");
document.write("<div id=\"divID2519\" class=\"leftMenuInActive\"><a id=\"aID2519\" href=\"#\" OnMouseOver=\"link('_page','Documentingthecode0',this)\" class=\"leftMenuLinkInActive\">Documenting the code</a></div>\n");
document.write("<div id=\"divID2520\" class=\"leftMenuInActive\"><a id=\"aID2520\" href=\"#\" OnMouseOver=\"link('_page','ExcludingInput0',this)\" class=\"leftMenuLinkInActive\">Excluding Input</a></div>\n");
document.write("<div id=\"divID2521\" class=\"leftMenuInActive\"><a id=\"aID2521\" href=\"#\" OnMouseOver=\"link('_page','Formulas0',this)\" class=\"leftMenuLinkInActive\">Formulas</a></div>\n");
document.write("<div id=\"divID2522\" class=\"leftMenuInActive\"><a id=\"aID2522\" href=\"#\" OnMouseOver=\"link('_page','Images0',this)\" class=\"leftMenuLinkInActive\">Images</a></div>\n");
document.write("<div id=\"divID2523\" class=\"leftMenuInActive\"><a id=\"aID2523\" href=\"#\" OnMouseOver=\"link('_page','MainPage0',this)\" class=\"leftMenuLinkInActive\">MainPage</a></div>\n");
document.write("<div id=\"divID2524\" class=\"leftMenuInActive\"><a id=\"aID2524\" href=\"#\" OnMouseOver=\"link('_page','Pages0',this)\" class=\"leftMenuLinkInActive\">Pages</a></div>\n");
document.write("<div id=\"divID2525\" class=\"leftMenuInActive\"><a id=\"aID2525\" href=\"#\" OnMouseOver=\"link('_page','SpecialSections0',this)\" class=\"leftMenuLinkInActive\">Special Sections</a></div>\n");
document.write("<div id=\"divID2526\" class=\"leftMenuInActive\"><a id=\"aID2526\" href=\"#\" OnMouseOver=\"link('_page','TextFormatting0',this)\" class=\"leftMenuLinkInActive\">Text Formatting</a></div>\n");
document.write("<div id=\"divID2527\" class=\"leftMenuInActive\"><a id=\"aID2527\" href=\"#\" OnMouseOver=\"link('_page','Units0',this)\" class=\"leftMenuLinkInActive\">Units</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
