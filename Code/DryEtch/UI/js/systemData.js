var setIntervalID;
$(document).ready(function () {
    var xmlDom = getSyncXmlData('./config/systemdata.xml');
    for (var i = 0; i < xmlDom.firstChild.children.length; i++) {
        getHtml(xmlDom.firstChild.children[i], $('#tree')[0]);
    }
    var tree = $('#tree').goodtree({ 'reveal': $('.focus') });
	$("#tree").find("li").click(function(){
		$(".select").removeClass("select");
		$(this).addClass("select");
});
    setIntervalID = setInterval(SynData, 500);
});

function SynData() {
    if (beginID != endID) {
        try {
            var data = fetchSystemDataByRange(beginID, endID, isFetchAll);
            if (!isFetchAll) {
                isFetchAll = true;
            }
            for (var i = 0; i < data.systemdata.length; i++) {
                if ($("#" + data.systemdata[i].id).length > 0) {
                    $("#" + data.systemdata[i].id).val(data.systemdata[i].value);
                }
            }
        } catch (e) {
            writeLog("error", e);
        }
    }
}
var htmlText = "";
var beginID = 0;
var endID = 0;
var isFetchAll = false;
function buttonClick(obj) {
    var id = parseInt(obj.getAttribute("pid"));
    var txtValue = $("#" + obj.getAttribute("txtID")).val();
    if (obj.getAttribute("min") == "null") {
        modifySystemData(id, txtValue)
        return;
    }
    var min = parseInt(obj.getAttribute("min"));
    var max = parseInt(obj.getAttribute("max"));
    var value = Number(txtValue);
    console.log("value:" + value);
    if (!isNaN(value)) {
        if (value > max || value < min) {
            alert("Please input value in range ["+min+", "+max+"].");
        }
        else {
            modifySystemData(id, txtValue);
        }
    }
    else {
        alert("Please input a number.")
    }
}
function isLeaf(node) {
    var _isLeaf = true;
    for (var i = 0; i < node.children.length; i++) {
        if ((node.children[i].attributes.length > 0 && node.children[i].attributes[0].name == "type") || (node.attributes.length > 0 && node.attributes[0].name == "type")) {

        }
        else {
            return false;
        }
    }
    return true;
}

function getHtml(node, element) {
    if (isLeaf(node)) {
        var el = document.createElement("li");
        el.setAttribute("hb", node.attributes["hb"]);
        el.setAttribute("lb", node.attributes["lb"]);
        el.onclick = function () {
            $("#data").empty();
            createFromGroup(node);
            beginID = node.attributes["lb"].textContent;
            endID = node.attributes["hb"].textContent;
            isFetchAll = false;
        }
        var textnode = document.createTextNode(node.tagName);
        el.appendChild(textnode);
        element.appendChild(el);
    }
    else {
        var li = document.createElement("li");
        var span = document.createElement("span");
        var textnode = document.createTextNode(node.tagName);
        span.appendChild(textnode);
        li.appendChild(span);
        var ul = document.createElement("ul");
        for (var i = 0; i < node.children.length; i++) {
            getHtml(node.children[i], ul);
        }
        li.appendChild(ul);
        element.appendChild(li);
        span.onclick = function () {
            $("#data").empty();
            createFromGroup(node);
            beginID = node.attributes["lb"];
            endID = node.attributes["hb"];
            isFetchAll = false;
        }
    }
}

function createFromGroup(node) {
    htmlText = "";
    for (var i = 0; i < node.children.length; i++) {
        if (node.children[i].attributes.length > 0) {
            createDataEx(node.children[i], i, i == node.children.length - 1);
        }
    }
//    console.log(htmlText);
    $("#data").append(htmlText);
}

function getContent(name,min, max, precision, readonly, id, unit, i) {
    var isRead = readonly == "True" ? true : false;
    var text;
    if (isRead) {
        if (i % 2 == 1) {
            return '<td width="80px"  style="text-align:left">' + name + '</td><td width="70px"><input class="conf_disable"  type="text" id=' + id + ' disabled /></td><td  width="70px"></td><td  width="50px">' + unit + '</td><td style="text-align:left"></td></tr>';
        }
        return '<tr><td width="80px"  style="text-align:left">' + name + '</td><td width="70px"><input class="conf_disable" type="text" id=' + id + ' disabled /></td><td  width="70px"></td><td  width="50px">' + unit + '</td><td style="text-align:left"></td>';
    }
    else {
        if (i % 2 == 1) {
            return '<td width="80px"  style="text-align:left">' + name + '</td><td width="70px"><input  class="conf_disable" type="text" id=' + id + ' disabled /></td><td  width="80px"><input class="conf_input"   type="text" id="textbox'
              + i + '" /></td><td  width="50px">' + unit + '</td><td style="text-align:left">'
               + '<input class="conf_btn" pid="' + id + '" precision="' + precision + '" max="' + max + '" min="' + min + '"  txtID="textbox' + i + '" type="button" onclick="buttonClick(this);" value="set" /></td></tr>';
        }
        return '<tr><td width="80px"  style="text-align:left">' + name + '</td><td width="70px"><input class="conf_disable" type="text" id=' + id + ' disabled /></td><td  width="80px"><input class="conf_input"   type="text" id="textbox'
              + i + '" /></td><td  width="50px">' + unit + '</td><td style="text-align:left">'
               + '<input class="conf_btn" pid="' + id + '" precision="' + precision + '" max="' + max + '" min="' + min + '"  txtID="textbox' + i + '" type="button" onclick="buttonClick(this);" value="set" /></td>';
    }
   
}

function createDataEx(node, i, islast) {
    var unitText;
    if (node.children[5].textContent == "") {
        unitText = "   ";
    }
    else {
        unitText = node.children[5].textContent;
    }
    htmlText += getContent(node.nodeName, node.children[0].textContent, node.children[1].textContent,
     node.children[2].textContent, node.children[3].textContent, node.children[4].textContent, unitText, i);
    if (islast) {
        if (i % 2 == 0) {
            htmlText += "<td></td><td></td><td></td><td></td><td></td></tr>";
        }
    }
}

function clearSetInterval()
{
	clearInterval(setIntervalID);
}