function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID2593\" class=\"headerLeftMenuInActive\"><a id=\"aID2593\" href=\"#\" OnMouseOver=\"link('_dir','SeparateDocs0',this)\" class=\"leftMenuLinkHeadInActive\">Separate Docs</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Classes</div>\n");
document.write("<div id=\"divID2426\" class=\"leftMenuInActive\"><a id=\"aID2426\" href=\"#\" OnMouseOver=\"link('_class','NamespaceClass5138',this)\" class=\"leftMenuLinkInActive\">NamespaceClass</a></div>\n");
document.write("<div id=\"divID2427\" class=\"leftMenuInActive\"><a id=\"aID2427\" href=\"#\" OnMouseOver=\"link('_class','SepDocClass0',this)\" class=\"leftMenuLinkInActive\">SepDocClass</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
