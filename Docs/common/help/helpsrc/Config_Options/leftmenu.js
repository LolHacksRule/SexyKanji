function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID2589\" class=\"headerLeftMenuInActive\"><a id=\"aID2589\" href=\"#\" OnMouseOver=\"link('_dir','ConfigOptions0',this)\" class=\"leftMenuLinkHeadInActive\">Config Options</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID2529\" class=\"leftMenuInActive\"><a id=\"aID2529\" href=\"#\" OnMouseOver=\"link('_page','DocParser0',this)\" class=\"leftMenuLinkInActive\">DocParser</a></div>\n");
document.write("<div id=\"divID2530\" class=\"leftMenuInActive\"><a id=\"aID2530\" href=\"#\" OnMouseOver=\"link('_page','DotOptions0',this)\" class=\"leftMenuLinkInActive\">DotOptions</a></div>\n");
document.write("<div id=\"divID2531\" class=\"leftMenuInActive\"><a id=\"aID2531\" href=\"#\" OnMouseOver=\"link('_page','FormulaLatexOptions0',this)\" class=\"leftMenuLinkInActive\">FormulaLatexOptions</a></div>\n");
document.write("<div id=\"divID2532\" class=\"leftMenuInActive\"><a id=\"aID2532\" href=\"#\" OnMouseOver=\"link('_page','GeneralOptions0',this)\" class=\"leftMenuLinkInActive\">GeneralOptions</a></div>\n");
document.write("<div id=\"divID2533\" class=\"leftMenuInActive\"><a id=\"aID2533\" href=\"#\" OnMouseOver=\"link('_page','HeaderAndFooter0',this)\" class=\"leftMenuLinkInActive\">HeaderAndFooter</a></div>\n");
document.write("<div id=\"divID2534\" class=\"leftMenuInActive\"><a id=\"aID2534\" href=\"#\" OnMouseOver=\"link('_page','IncludeInOutput0',this)\" class=\"leftMenuLinkInActive\">IncludeInOutput</a></div>\n");
document.write("<div id=\"divID2535\" class=\"leftMenuInActive\"><a id=\"aID2535\" href=\"#\" OnMouseOver=\"link('_page','InputOptions0',this)\" class=\"leftMenuLinkInActive\">InputOptions</a></div>\n");
document.write("<div id=\"divID2536\" class=\"leftMenuInActive\"><a id=\"aID2536\" href=\"#\" OnMouseOver=\"link('_page','Inputparsing0',this)\" class=\"leftMenuLinkInActive\">Inputparsing</a></div>\n");
document.write("<div id=\"divID2537\" class=\"leftMenuInActive\"><a id=\"aID2537\" href=\"#\" OnMouseOver=\"link('_page','LanguageOptions0',this)\" class=\"leftMenuLinkInActive\">LanguageOptions</a></div>\n");
document.write("<div id=\"divID2538\" class=\"leftMenuInActive\"><a id=\"aID2538\" href=\"#\" OnMouseOver=\"link('_page','LayoutHtml0',this)\" class=\"leftMenuLinkInActive\">LayoutHtml</a></div>\n");
document.write("<div id=\"divID2539\" class=\"leftMenuInActive\"><a id=\"aID2539\" href=\"#\" OnMouseOver=\"link('_page','LeftMenu0',this)\" class=\"leftMenuLinkInActive\">LeftMenu</a></div>\n");
document.write("<div id=\"divID2540\" class=\"leftMenuInActive\"><a id=\"aID2540\" href=\"#\" OnMouseOver=\"link('_page','MessagesOptions0',this)\" class=\"leftMenuLinkInActive\">MessagesOptions</a></div>\n");
document.write("<div id=\"divID2541\" class=\"leftMenuInActive\"><a id=\"aID2541\" href=\"#\" OnMouseOver=\"link('_page','OutputCustomization0',this)\" class=\"leftMenuLinkInActive\">OutputCustomization</a></div>\n");
document.write("<div id=\"divID2542\" class=\"leftMenuInActive\"><a id=\"aID2542\" href=\"#\" OnMouseOver=\"link('_page','PreprocessorOptions0',this)\" class=\"leftMenuLinkInActive\">PreprocessorOptions</a></div>\n");
document.write("<div id=\"divID2543\" class=\"leftMenuInActive\"><a id=\"aID2543\" href=\"#\" OnMouseOver=\"link('_page','SearchPageOptions0',this)\" class=\"leftMenuLinkInActive\">SearchPageOptions</a></div>\n");
document.write("<div id=\"divID2544\" class=\"leftMenuInActive\"><a id=\"aID2544\" href=\"#\" OnMouseOver=\"link('_page','SpecialOptions0',this)\" class=\"leftMenuLinkInActive\">SpecialOptions</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
