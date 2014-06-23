$(document).ready(function () {
    var xmlDom = getSyncXmlData('./config/parameters.xml');
    for (var i = 0; i < xmlDom.firstChild.children.length; i++) {
        getHtml(xmlDom.firstChild.children[i], $('#tree')[0]);
    }
    var tree = $('#tree').goodtree({ 'reveal': $('.focus') });
	$("#tree").find("li").click(function(){
		$(".select").removeClass("select");
		$(this).addClass("select");
	});
});
var htmlText = "";

function getContentText(id) {
    var data = fetchParametersByrange(id, id, false);
    $("#" + data.parameters[0].id).val(data.parameters[0].value);
}
function buttonClick(obj) {
    var id = parseInt(obj.getAttribute("pid"));
    var txtValue = $("#" + obj.getAttribute("txtID")).val();
    if (obj.getAttribute("min") == "null") {
        modifyParameter(id, txtValue)
        getContentText(id);
        return;
    }
    var min = parseInt(obj.getAttribute("min"));
    var max = parseInt(obj.getAttribute("max"));
    var value = Number(txtValue);
    if (!isNaN(value)) {
        if (value > max || value < min) {
            alert("Please input value in range [" + min + ", " + max + "].");
        }
        else {
            modifyParameter(id, txtValue);
            getContentText(id);
        }
    }
    else {
        alert("Please input a number.");
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
        el.onclick = function () {
            $("#data").empty();
            createFromGroup(node);
            initContent(node.attributes["lb"].textContent, node.attributes["hb"].textContent);
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
            initContent(node.attributes["lb"].textContent, node.attributes["hb"].textContent);
        }
    }
}

function initContent(lb, hb) {
    try {
        var data = fetchParametersByrange(lb, hb, false);
        for (var i = 0; i < data.parameters.length; i++) {
            if ($("#" + data.parameters[i].id).length > 0) {
                $("#" + data.parameters[i].id).val(data.parameters[i].value);
            }
        }
    } catch (e) {
        alert(e);
    }
}

function createFromGroup(node) {
    htmlText = "";
    for (var i = 0; i < node.children.length; i++) {
        if (node.children[i].attributes.length > 0) {
            createDataEx(node.children[i], i, i == node.children.length-1);
        }
    }
    console.log(htmlText);
    $("#data").append(htmlText);
}

function getContent(title, name, text, unit, min, max, i,id) {
   if(i%2==1)
   {
       return '<td>' + id + '</td><td width="80px" style="text-align:left"><span title="' + title + '">' + name + '</span></td><td  width="80px"><input class="conf_input" type="text" id="'
              + id + '" value="' + text + '" /></td><td  width="50px">'+unit+'</td><td style="text-align:left">'
               + '<input class="conf_btn" pid="' + id + '" max="' + max + '" min="' + min + '"  txtID="' + id + '" type="button" onclick="buttonClick(this);" value="set" /></td></tr>';
   }
   return '<tr><td>' + id + '</td><td width="80px"  style="text-align:left"><span title="' + title + '">' + name + '</span></td><td  width="80px"><input class="conf_input"  type="text" id="'
              + id + '" value="' + text + '" /></td><td  width="50px">'+unit+'</td><td style="text-align:left">'
               + '<input class="conf_btn" pid="' + id + '" max="' + max + '" min="' + min + '"  txtID="' + id + '" type="button" onclick="buttonClick(this);" value="set" /></td>';
}

function createDataEx(node, i,islast) {
    var unitText;
    if (node.children[5].textContent == "") {
        unitText = "   ";
    }
    else {
        unitText = node.children[5].textContent;
    }

    htmlText += getContent(node.children[1].textContent, node.nodeName, node.children[4].textContent, unitText, node.children[1].textContent, node.children[3].textContent, i, node.children[4].textContent);
    if (islast) {
        if (i % 2 == 0) {
            htmlText+="<td></td><td></td><td></td><td></td></tr>";
        }
    }
}