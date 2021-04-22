function WriteLeftMenu(divID, aID, divClassName, aClassName)
{
document.write("<div id=\"divID2417\" class=\"headerLeftMenuInActive\"><a id=\"aID2417\" href=\"#\" OnMouseOver=\"link('_statsindex','index',this)\" class=\"leftMenuLinkHeadInActive\">Statistics</a></div>\n");
document.write("<div class=\"paragraphLeftMenu\">Pages</div>\n");
document.write("<div id=\"divID2602\" class=\"leftMenuInActive\"><a id=\"aID2602\" href=\"#\" OnMouseOver=\"link('_statsgeneral','stats_general',this)\" class=\"leftMenuLinkInActive\">General</a></div>\n");
document.write("<div id=\"divID2603\" class=\"leftMenuInActive\"><a id=\"aID2603\" href=\"#\" OnMouseOver=\"link('_statsundoc','stats_undoc',this)\" class=\"leftMenuLinkInActive\">Undocumented</a></div>\n");
document.write("<div id=\"divID2604\" class=\"leftMenuInActive\"><a id=\"aID2604\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_msg',this)\" class=\"leftMenuLinkInActive\">Msg-Messages</a></div>\n");
document.write("<div id=\"divID2605\" class=\"leftMenuInActive\"><a id=\"aID2605\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_warn',this)\" class=\"leftMenuLinkInActive\">Msg-Warnings</a></div>\n");
document.write("<div id=\"divID2606\" class=\"leftMenuInActive\"><a id=\"aID2606\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_err',this)\" class=\"leftMenuLinkInActive\">Msg-Errors</a></div>\n");
document.write("<div id=\"divID2607\" class=\"leftMenuInActive\"><a id=\"aID2607\" href=\"#\" OnMouseOver=\"link('_statsmsg','stats_msg_dbg',this)\" class=\"leftMenuLinkInActive\">Msg-Debug</a></div>\n");
if(divID != "" && aID != "")
{
document.getElementById(divID).className = divClassName;
document.getElementById(aID).className = aClassName;
}
}
