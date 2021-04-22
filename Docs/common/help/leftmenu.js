function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID0\" class=\"headerLeftMenuInActive\"><a id=\"aID0\" href=\"#\" OnMouseOver=\"link('','index',this)\" class=\"leftMenuLinkHeadInActive\">Help</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Directories</div>\n");
document.write("<div id=\"divID2590\" class=\"leftMenuInActive\"><a id=\"aID2590\" href=\"#\" OnMouseOver=\"link('_dir','helpsrc/Command_Reference/CommandReference0',this)\" class=\"leftMenuLinkInActive\">Command Reference</a></div>\n");
document.write("<div id=\"divID2589\" class=\"leftMenuInActive\"><a id=\"aID2589\" href=\"#\" OnMouseOver=\"link('_dir','helpsrc/Config_Options/ConfigOptions0',this)\" class=\"leftMenuLinkInActive\">Config Options</a></div>\n");
document.write("<div id=\"divID2587\" class=\"leftMenuInActive\"><a id=\"aID2587\" href=\"#\" OnMouseOver=\"link('_dir','helpsrc/Documenting/Documenting0',this)\" class=\"leftMenuLinkInActive\">Documenting</a></div>\n");
document.write("<div id=\"divID2591\" class=\"leftMenuInActive\"><a id=\"aID2591\" href=\"#\" OnMouseOver=\"link('_dir','helpsrc/Example_Code/ExampleCode0',this)\" class=\"leftMenuLinkInActive\">Example Code</a></div>\n");
document.write("<div id=\"divID2588\" class=\"leftMenuInActive\"><a id=\"aID2588\" href=\"#\" OnMouseOver=\"link('_dir','helpsrc/Navigating_In_Doxygen/NavigatingInDoxygen0',this)\" class=\"leftMenuLinkInActive\">Navigating In Doxygen</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID2516\" class=\"leftMenuInActive\"><a id=\"aID2516\" href=\"#\" OnMouseOver=\"link('_page','CompilingDoxyS0',this)\" class=\"leftMenuLinkInActive\">Compiling DoxyS</a></div>\n");
document.write("<div id=\"divID2514\" class=\"leftMenuInActive\"><a id=\"aID2514\" href=\"#\" OnMouseOver=\"link('_page','InstallingDoxyS0',this)\" class=\"leftMenuLinkInActive\">Installing DoxyS</a></div>\n");
document.write("<div id=\"divID2515\" class=\"leftMenuInActive\"><a id=\"aID2515\" href=\"#\" OnMouseOver=\"link('_page','RunningDoxyS0',this)\" class=\"leftMenuLinkInActive\">Running DoxyS</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Miscellaneous</div>\n");
document.write("<div id=\"divID2417\" class=\"leftMenuInActive\"><a id=\"aID2417\" href=\"#\" OnMouseOver=\"link('_statsindex','common/statistics/index',this)\" class=\"leftMenuLinkInActive\">Statistics</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
