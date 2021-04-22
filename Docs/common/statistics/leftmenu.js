function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID159\" class=\"headerLeftMenuInActive\"><a id=\"aID159\" href=\"#\" OnMouseOver=\"link('_statsindex','index',this)\" class=\"leftMenuLinkHeadInActive\">Statistics</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID2317\" class=\"leftMenuInActive\"><a id=\"aID2317\" href=\"#\" OnMouseOver=\"link('_statsgeneral','stats_general',this)\" class=\"leftMenuLinkInActive\">General</a></div>\n");
document.write("<div id=\"divID2318\" class=\"leftMenuInActive\"><a id=\"aID2318\" href=\"#\" OnMouseOver=\"link('_statsundoc','stats_undoc',this)\" class=\"leftMenuLinkInActive\">Undocumented</a></div>\n");
document.write("<div id=\"divID2319\" class=\"leftMenuInActive\"><a id=\"aID2319\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_msg',this)\" class=\"leftMenuLinkInActive\">Msg-Messages</a></div>\n");
document.write("<div id=\"divID2320\" class=\"leftMenuInActive\"><a id=\"aID2320\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_warn',this)\" class=\"leftMenuLinkInActive\">Msg-Warnings</a></div>\n");
document.write("<div id=\"divID2321\" class=\"leftMenuInActive\"><a id=\"aID2321\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_err',this)\" class=\"leftMenuLinkInActive\">Msg-Errors</a></div>\n");
document.write("<div id=\"divID2322\" class=\"leftMenuInActive\"><a id=\"aID2322\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_dbg',this)\" class=\"leftMenuLinkInActive\">Msg-Debug</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
