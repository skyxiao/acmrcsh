var prpos = new Array();
var ids = new Array();
var setIntervalID;
var isFetchAll = false;
var setIntervalID2;
var unit = {
	"8009" : "sccm",
	"8030" : "sccm",
    "8012" : "sccm",
    "2004" : "kPa",
    "8015" : "sccm",
    "8034" : "sccm",
    "8018" : "sccm",
    "8036" : "sccm",
    "8005" : "%",
    "4000" : "℃",
    "5000" : "℃",
    "6000" : "℃",
    "8020" : "torr",
    "2006" : "kPa",
	"8023" : "torr",
	"8032" : "sccm",
	"2022" : "℃",
	"2024" : "℃",
	"2000" : "kPa",
	"2008" : "kPa"
};

var user_info = {};

function  permissionCheck()
{
	user_info = top.USER_INFO;
	if (user_info["gasbox"] && user_info["gasbox"] == "readonly")
	{
		$(".btn_box").find("button").attr("disabled", "disabled");
		$(".btn_box").find("button").removeClass("enable_button");
		$(".btn_box").find("button").addClass("disabled_button");
		$(".btn_box").find("button").attr("isable", "false");		
		$("input").attr("disabled", "disabled");
	}
}

function valve(left, top,class1,class2,id, namePosition, name) {
    this.left = left - 7;
    this.top = top - 7;
    this.id = id;
    this.class1 = class1;
    this.class2 = class2;
    this.namePosition = namePosition;
    this.name = name;
    this.click = function () {
		//clearInterval(setIntervalID);
		if (user_info["gasbox"] && user_info["gasbox"] == "readonly")
		{
			return;
		}

        if (prpos[id] == "1") {
        	if (confirm("Are you sure close the valve '" + name + "'?"))
        	{
            	modifySystemData(id, "0");
            }
        }
        else {
        	if (confirm("Are you sure open the valve '" + name + "'?"))
        	{
            	modifySystemData(id, "1");
            }
        }
		//getVoleStatus();
		//setIntervalID = setInterval(getVoleStatus, 500);
    }
}
$(document).ready(function () {
    var arr = initValve();
    for (var i = 0; i < arr.length; i++) {
        var div = document.createElement("div");
        div.onclick = arr[i].click;
        div.setAttribute("class", arr[i].class1);
        div.setAttribute("class1", arr[i].class1);
        div.setAttribute("class2", arr[i].class2);
        var style = 'left:' + arr[i].left + 'px; top:' + arr[i].top + 'px';
        div.setAttribute("style", style);
        div.setAttribute("id", arr[i].id);
        //        div.setAttribute("title", arr[i].id);
        $("#gasbox")[0].appendChild(div);
        prpos[arr[i].id] = "1";
        ids.push(arr[i].id);
        if (arr[i].namePosition && arr[i].namePosition != "")
        {
        	var label = document.createElement("label");
        	var style = "";
        	switch (arr[i].namePosition)
        	{
        		case "top":
        			style = "left:" + (arr[i].left - 5) + "px;top:" + (arr[i].top - 14) + "px";
        			label.setAttribute("style", style);
        			label.setAttribute("class", "valve_name");
        			break;

        		case "bottom":
        			style = "left:" + (arr[i].left - 5) + "px;top:" + (arr[i].top + 18) + "px";
        			label.setAttribute("style", style);
        			label.setAttribute("class", "valve_name");
        			break;

        		case "left":
        			style = "left:" + (arr[i].left - 42) + "px;top:" + (arr[i].top + 3) + "px";
        			label.setAttribute("style", style);
        			label.setAttribute("class", "valve_name");
        			break;

        		case "right":
        			style = "left:" + (arr[i].left + 18) + "px;top:" + (arr[i].top + 3)  + "px";
        			label.setAttribute("style", style);
        			label.setAttribute("class", "valve_name");
        			break;

        		case "lefttop":
        			style = "left:" + (arr[i].left - 20) + "px;top:" + (arr[i].top - 14)  + "px";
        			label.setAttribute("style", style);
        			label.setAttribute("class", "valve_name");
        			break;

        		default:
        			break;
        	}

        	label.innerHTML = arr[i].name ? arr[i].name : "";

        	$("#gasbox")[0].appendChild(label);
        }
    }
    initVAC();
    initText();
    setIntervalID = setInterval(getVoleStatus, 500);

	$(".PurgeHF").click(function() {
		if ($(this).attr("isable") == "true")
		{
            try
            {
                writeLog("info", "send PurgeHF command.");
    			getControl().invoke(0, 1009, 2);
            }
            catch (e)
            {
                writeLog("error", e);
            }
		}
	});

	$(".PurgeEtOH").click(function() {
		if ($(this).attr("isable") == "true")
		{
             try
            {
                writeLog("info", "send PurgeEtOH command.");
			     getControl().invoke(0, 1009, 3);
            }
            catch (e)
            {
                writeLog("error", e);
            }
		}
	});

	$(".PurgeExpchamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
             try
            {
                writeLog("info", "send PurgeExpchamber command.");
			    getControl().invoke(0, 1009, 1);
            }
            catch (e)
            {
                writeLog("error", e);
            }
		}
	});

	$(".PurgeProcesschamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
            try
            {
                 writeLog("info", "send PurgeProcesschamber command.");
			     getControl().invoke(0, 1009, 0);
            }
            catch (e)
            {
                writeLog("error", e);
            }
		}
	});

	$(".PumpExpchamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
            try
            {
                writeLog("info", "send PumpExpchamber command.");
			     getControl().invoke(0, 1007, 1);
            }
             catch (e)
            {
                writeLog("error", e);
            }
		}
	});

	$(".PumpProcesschamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
            try
            {
                writeLog("info", "send PumpProcesschamber command.");
			     getControl().invoke(0, 1007, 0);
            }
            catch (e)
            {
                writeLog("error", e);
            }
		}	
	});

	$(".Abort").click(function(){
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 3);
		}
	});
});

function initVAC() {
    var div = document.createElement("div");
    div.setAttribute("class", "vac_green");
    div.setAttribute("id", "vac");
    var style = 'left:48px; top:60px';
    div.setAttribute("style", style);
    ids.push(1011);
    ids.push(50);
    ids.push(51);
    prpos[1011] = "1";
    prpos[50] = "1";
    prpos[51] = "1";
    div.onclick = function () {
		if (user_info["gasbox"] && user_info["gasbox"] == "readonly")
		{
			return;
		}

        if (prpos[1011] == "1") {
        	if (confirm("Are you sure turn off the pump?"))
        	{
            	modifySystemData(1011, "0");
            }
        }
        else {
        	if (confirm("Are you sure turn on the pump?"))
        	{
            	modifySystemData(1011, "1");
            }
        }
    };
    $("#gasbox")[0].appendChild(div);
    var label = document.createElement("label");
    label.setAttribute("class", "valve_name");
    label.setAttribute("style", "left:29px;top:82px;");
    label.innerHTML = "VAC PUMP";
    $("#gasbox")[0].appendChild(label);
}

function initText() {
    ids.push(8009);
    ids.push(8030);
    ids.push(8012);
    ids.push(8032);

    ids.push(8015);
    ids.push(8034);
    ids.push(8018);
    ids.push(8036);

    prpos[8009] = "1";
    prpos[8030] = "1";
    prpos[8012] = "1";
    prpos[2004] = "1";
    prpos[8015] = "1";
    prpos[8034] = "1";
    prpos[8018] = "1";
    prpos[8036] = "1";

    ids.push(8005);
    ids.push(4000);
    ids.push(5000);
    ids.push(6000);
    ids.push(8020);
    ids.push(2006);
    ids.push(2004);
	ids.push(8023);
	ids.push(2022);
	ids.push(2024);
	ids.push(2000);
	ids.push(2008);


    prpos[8005] = "0";
    prpos[4000] = "0";
    prpos[5000] = "0";
    prpos[6000] = "0";
    prpos[8020] = "0";
    prpos[2006] = "0";
    prpos[2004] = "0";
	prpos[8023] = "0";
	prpos[2022] = "0";
	prpos[2024] = "0";
	prpos[2000] = "0";
	prpos[2008] = "0";

    $(":text").keydown(function (event) {
        if (event.keyCode == 13) {
            var min = 0;
            var max = parseInt($(this).attr("max"));
            var value = Number($(this).val());
            if (!isNaN(value)) {
                if (value > max || value < min) {
                    alert("Please input value in range [" + min + ", " + max + "].");
                    return;
                }
                else {
                    modifySystemData($(this).attr("pid"), $(this).val());
                }
            }
            else {
                alert("Please input a number.")
                return;
            }
        }
    }); 
}

function initValve() {
    var arr = new Array();
    arr.push(new valve(271, 45, "valve_green_h", "valve_gray_h", 1056, "top", "OPV1"));
    arr.push(new valve(271, 72, "valve_green_h", "valve_gray_h", 1057, "bottom", "OPV2"));
    arr.push(new valve(179, 118, "valve_green_h", "valve_gray_h", 1018, "bottom", "OPV3"));
    arr.push(new valve(730, 119, "valve_green_h", "valve_gray_h", 1019, "top", "OPV4"));
    arr.push(new valve(270, 168, "valve_green_h", "valve_gray_h", 1021, "lefttop", "OPV10"));
    arr.push(new valve(420, 167, "valve_green_h", "valve_gray_h", 1022, "top", "OPV11A"));
    arr.push(new valve(245, 184, "valve_green_h", "valve_gray_h", 1036, "lefttop", "OPV12"));
    arr.push(new valve(348, 203, "valve_green_h", "valve_gray_h", 1023, "bottom", "OPV15"));
    arr.push(new valve(540, 184, "valve_green_h", "valve_gray_h", 1034, "bottom", "OPV8"));
    arr.push(new valve(650, 184, "valve_green_h", "valve_gray_h", 1035, "bottom", "OPV9"));
    arr.push(new valve(650, 230, "valve_green_h", "valve_gray_h", 1038, "bottom", "OPV14"));
    arr.push(new valve(151, 243, "valve_green_h", "valve_gray_h", 1037, "lefttop", "OPV13"));
    arr.push(new valve(191, 440, "valve_green_h", "valve_gray_h", 1039, "top", "OPV20"));
    arr.push(new valve(321, 423, "valve_green_h", "valve_gray_h", 1026, "lefttop", "OPV18"));
    arr.push(new valve(474, 423, "valve_green_h", "valve_gray_h", 1027, "top", "OPV19"));
    arr.push(new valve(394, 459, "valve_green_h", "valve_gray_h", 1028, "bottom", "OPV21"));
    arr.push(new valve(305, 504, "valve_green_h", "valve_gray_h", 1029, "top", "OPV23"));
    arr.push(new valve(492, 502, "valve_green_h", "valve_gray_h", 1030, "top", "OPV24"));
    arr.push(new valve(492, 593, "valve_green_h", "valve_gray_h", 1051, "top", "OPV28"));
    arr.push(new valve(711, 546, "valve_green_h", "valve_gray_h", 1049, "bottom", "OPV26"));
    arr.push(new valve(304, 592, "valve_green_h", "valve_gray_h", 1050, "top", "OPV27"));
    arr.push(new valve(448, 220, "valve_green_h", "valve_gray_h", 1067, "left", "OPV11B"));
    arr.push(new valve(570, 151, "valve_green", "valve_gray", 1032, "right", "OPV5"));
    arr.push(new valve(620, 151, "valve_green", "valve_gray", 1033, "right", "OPV6"));
    arr.push(new valve(833, 167, "valve_green", "valve_gray", 1041, "right", "OPV7"));
    arr.push(new valve(529, 270, "valve_green", "valve_gray", 1024, "bottom", "OPV16"));
    arr.push(new valve(557, 270, "valve_green", "valve_gray", 1025, "right", "OPV17"));
    arr.push(new valve(242, 461, "valve_green", "valve_gray", 1048, "right", "OPV22"));
    return arr;
}

var lastStatus = null;
var lastProcCommand = null;
var lastprocParam1 = null;
var lastprocParam2 = null;

function getSettingsData()
{
	var arr = [100050, 100052, 100053, 100054, 46, 47, 48];
	var json = {};
	try
	{
		json = getControl().fetch_system_data(arr, false);
		json = $.parseJSON(json);
	

    	var system_data = json["systemdata"];
    	var status = 0;
    	var procCommand = 1000;
    	var procParam1 = 0;
    	var procParam2 = 0;
    	var TKLow = 0;
    	var TKHigh = 0;
    	var TKHighH = 0;
    	for (var i = 0; i < system_data.length; ++i)
    	{
    		switch (system_data[i]["id"])
    		{

    		case "100050":
    			status = parseInt(system_data[i]["value"]);
    			break;

    		case "100052":
    			procCommand = parseInt(system_data[i]["value"]);
    			break;

    		case "100053":
    			procParam1 = parseInt(system_data[i]["value"]);
    			break;

    		case "100054":
    			procParam2 = parseInt(system_data[i]["value"]);
    			break;

    		case "46":
    			TKLow = parseInt(system_data[i]["value"]);
    			break;

    		case "47":
    			TKHigh = parseInt(system_data[i]["value"]);
    			break;

    		case "48":
    			TKHighH = parseInt(system_data[i]["value"]);
    			break;

    		default:
    		
    		}
    	}

    	if (TKHighH == 1)
    	{
    		$(".gas_level").text("HH");
    	}
    	else if (TKHigh == 1)
    	{
    		$(".gas_level").text("H");
    	}
    	else if (TKLow == 1)
    	{
    		$(".gas_level").text("L");
    	}
    	else
    	{
    		$(".gas_level").text("LL");
    	}

    	if (lastStatus === status && lastProcCommand === procCommand && lastprocParam1 === procParam1 &&  lastprocParam2 === procParam2)
    	{
    		return;
    	}
    	else
    	{
    		lastStatus = status;
    		lastProcCommand = procCommand;
    		lastprocParam1 = procParam1;
    		lastprocParam2 = procParam2;
    	}

    	if (status == 0)
    	{
    		$(".btn_box").find("button").removeAttr("disabled");
    		$(".btn_box").find("button").removeClass("disabled_button");
    		$(".btn_box").find("button").addClass("enable_button");
    		$(".btn_box").find("button").attr("isable", "true");
    		$(".Abort").attr("disabled", "disabled");
    		$(".Abort").removeClass("enable_button");
    		$(".Abort").addClass("disabled_button");
    		$(".Abort").attr("isable", "false");
    	}
    	else if (status == 1)
    	{
    		$(".btn_box").find("button").attr("disabled", "disabled");
    		$(".btn_box").find("button").removeClass("enable_button");
    		$(".btn_box").find("button").addClass("disabled_button");
    		$(".btn_box").find("button").attr("isable", "false");
    		$(".Abort").removeAttr("disabled");
    		$(".Abort").removeClass("disabled_button");
    		$(".Abort").addClass("enable_button");
    		$(".Abort").attr("isable", "true");		
    	}
    	else if (status == 2)
    	{
    		$(".btn_box").find("button").attr("disabled", "disabled");
    		$(".btn_box").find("button").removeClass("enable_button");
    		$(".btn_box").find("button").addClass("disabled_button");
    		$(".btn_box").find("button").attr("isable", "false");
    	}

    	permissionCheck();

    	delete json;
    	delete arr;
    	delete system_data;
    }
    catch (e)
    {
        writeLog("error", e);
        Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
        return;
    }
}

function getVoleStatus()
{
	try {
            var data = fetchSystemDatabyids(ids, isFetchAll);
            if (!isFetchAll) {
                isFetchAll = true;
            }
            for (var i = 0; i < data.systemdata.length; i++) {
                prpos[data.systemdata[i].id] = data.systemdata[i].value;
                if ($("#" + data.systemdata[i].id).length > 0) {
                    if ($("#" + data.systemdata[i].id).attr("tag") == "text") {
                        $("#" + data.systemdata[i].id).text(data.systemdata[i].value + " " + unit[data.systemdata[i].id]);
                    }
                    else {
                        if (data.systemdata[i].value == "1") {
                            $("#" + data.systemdata[i].id).attr("class", $("#" + data.systemdata[i].id).attr("class1"));
                        }
                        else {
                            $("#" + data.systemdata[i].id).attr("class", $("#" + data.systemdata[i].id).attr("class2"));
                        }
                    }
                }
                else {
                    if (data.systemdata[i].id == 1011 || data.systemdata[i].id == 50 || data.systemdata[i].id == 51) {
                        if (prpos[1011] == "0") {
                            $("#vac").attr("class", "vac_gray");
                        }
                        else if (prpos[50] == "0" && prpos[51] == "0") {
                            $("#vac").attr("class", "vac_green");
                        }
                        else {
                            $("#vac").attr("class", "vac_yellow");
                        }
                    }
                }
            }
        } catch (e) {
            alert(e);
            clearInterval(setIntervalID);
        }

		delete data;
}

function clearSetInterval()
{
	clearInterval(setIntervalID2);
	clearInterval(setIntervalID);
}

setIntervalID2 = setInterval(getSettingsData, 500);






                   
                       