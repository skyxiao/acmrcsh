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
	"8032" : "sccm"
};

function valve(left, top,class1,class2,id) {
    this.left = left - 7;
    this.top = top - 7;
    this.id = id;
    this.class1 = class1;
    this.class2 = class2;
    this.click = function () {
		clearInterval(setIntervalID);
        if (prpos[id] == "1") {
            modifySystemData(id, "0");
        }
        else {
            modifySystemData(id, "1");
        }
		getVoleStatus();
		setIntervalID = setInterval(getVoleStatus, 500);
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
    }
    initVAC();
    initText();
    setIntervalID = setInterval(getVoleStatus, 500);

	$(".PurgeHF").click(function() {
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 1009, 2);
		}
		else if ($(this).attr("isable") == "false")
		{
			getControl().invoke(0, 3);
		}
	});

	$(".PurgeEtOH").click(function() {
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 1009, 3);
		}
		else if ($(this).attr("isable") == "false")
		{
			getControl().invoke(0, 3);
		}
	});

	$(".PurgeExpchamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 1009, 1);
		}
		else if ($(this).attr("isable") == "false")
		{
			getControl().invoke(0, 3);
		}
	});

	$(".PurgeProcesschamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 1009, 0);
		}
		else if ($(this).attr("isable") == "false")
		{
			getControl().invoke(0, 3);
		}	
	});

	$(".PumpExpchamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 1007, 1);
		}
		else if ($(this).attr("isable") == "false")
		{
			getControl().invoke(0, 3);
		}
	});

	$(".PumpProcesschamber").click(function() {
		if ($(this).attr("isable") == "true")
		{
			getControl().invoke(0, 1007, 0);
		}
		else if ($(this).attr("isable") == "false")
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
        if (prpos[1011] == "1") {
            modifySystemData(1011, "0");
        }
        else {
            modifySystemData(1011, "1");
        }
    };
    $("#gasbox")[0].appendChild(div);
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


    prpos[8005] = "0";
    prpos[4000] = "0";
    prpos[5000] = "0";
    prpos[6000] = "0";
    prpos[8020] = "0";
    prpos[2006] = "0";
    prpos[2004] = "0";
	prpos[8023] = "0";

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
    arr.push(new valve(271, 45, "valve_green_h", "valve_gray_h", 1056));
    arr.push(new valve(271, 72, "valve_green_h", "valve_gray_h", 1057));
    arr.push(new valve(179, 118, "valve_green_h", "valve_gray_h", 1018));
    arr.push(new valve(730, 119, "valve_green_h", "valve_gray_h", 1019));
    arr.push(new valve(270, 168, "valve_green_h", "valve_gray_h", 1021));
    arr.push(new valve(420, 167, "valve_green_h", "valve_gray_h", 1022));
    arr.push(new valve(245, 184, "valve_green_h", "valve_gray_h", 1036));
    arr.push(new valve(348, 203, "valve_green_h", "valve_gray_h", 1023));
    arr.push(new valve(540, 184, "valve_green_h", "valve_gray_h", 1034));
    arr.push(new valve(650, 184, "valve_green_h", "valve_gray_h", 1035));
    arr.push(new valve(650, 230, "valve_green_h", "valve_gray_h", 1038));
    arr.push(new valve(151, 243, "valve_green_h", "valve_gray_h", 1037));
    arr.push(new valve(191, 440, "valve_green_h", "valve_gray_h", 1039));
    arr.push(new valve(151, 549, "valve_green_h", "valve_gray_h", 1048));
    arr.push(new valve(321, 423, "valve_green_h", "valve_gray_h", 1026));
    arr.push(new valve(474, 423, "valve_green_h", "valve_gray_h", 1027));
    arr.push(new valve(394, 459, "valve_green_h", "valve_gray_h", 1028));
    arr.push(new valve(305, 504, "valve_green_h", "valve_gray_h", 1029));
    arr.push(new valve(492, 502, "valve_green_h", "valve_gray_h", 1030));
    arr.push(new valve(492, 593, "valve_green_h", "valve_gray_h", 1051));
    arr.push(new valve(711, 546, "valve_green_h", "valve_gray_h", 1049));
    arr.push(new valve(304, 592, "valve_green_h", "valve_gray_h", 1050));
    arr.push(new valve(448, 220, "valve_green_h", "valve_gray_h", 1067));
    arr.push(new valve(570, 151, "valve_green", "valve_gray", 1032));
    arr.push(new valve(620, 151, "valve_green", "valve_gray", 1033));
    arr.push(new valve(833, 167, "valve_green", "valve_gray", 1041));
    arr.push(new valve(529, 270, "valve_green", "valve_gray", 1024));
    arr.push(new valve(557, 270, "valve_green", "valve_gray", 1025));
    arr.push(new valve(242, 461, "valve_green", "valve_gray", 1040));
    return arr;
}

var lastStatus = null;
var lastprocParam1 = null;
var lastprocParam2 = null;

function getSettingsData()
{
	var arr = [100050, 100052, 100053, 100054];
	var json = {};
	try
	{
		json = getControl().fetch_system_data(arr, false);
		json = $.parseJSON(json);
	}
	catch (e)
	{
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}

	var system_data = json["systemdata"];
	var status = 0;
	var procCommand = 1000;
	var procParam1 = 0;
	var procParam2 = 0;
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

		default:
		
		}
	}

	if (lastStatus === status && lastprocParam1 === procParam1 &&  lastprocParam2 === procParam2)
	{
		return;
	}
	else
	{
		lastStatus = status;
		lastprocParam1 = procParam1;
		lastprocParam2 = procParam2;
	}

	if (status == 0)
	{
		$(".PurgeHF").text("PurgeHF");
		$(".PurgeEtOH").text("PurgeEtOH");
		$(".PurgeExpchamber").text("PurgeExpchamber");
		$(".PurgeProcesschamber").text("PurgeProcesschamber");
		$(".PumpExpchamber").text("PumpExpchamber");
		$(".PumpProcesschamber").text("PumpProcesschamber");
		$(".btn_box").find("button").removeAttr("disabled");
		$(".btn_box").find("button").removeClass("disabled_button");
		$(".btn_box").find("button").addClass("enable_button");
		$(".btn_box").find("button").attr("isable", "true");
	}
	else if (status == 1)
	{
		$(".PurgeHF").text("PurgeHF");
		$(".PurgeEtOH").text("PurgeEtOH");
		$(".PurgeExpchamber").text("PurgeExpchamber");
		$(".PurgeProcesschamber").text("PurgeProcesschamber");
		$(".PumpExpchamber").text("PumpExpchamber");
		$(".PumpProcesschamber").text("PumpProcesschamber");
		$(".btn_box").find("button").attr("disabled", "disabled");
		$(".btn_box").find("button").removeClass("enable_button");
		$(".btn_box").find("button").addClass("disabled_button");
		$(".btn_box").find("button").attr("isable", "false");

		if (procCommand == 1009 && procParam1 == 2 && procParam2 == 0)
		{
			$(".PurgeHF").text("PurgeHF Abort");
			$(".PurgeHF").removeAttr("disabled");
			$(".PurgeHF").removeClass("disabled_button");
			$(".PurgeHF").addClass("enable_button");
			$(".PurgeHF").attr("isable", "true");
		}
		else if (procCommand == 1009 && procParam1 == 3 && procParam2 == 0)
		{
			$(".PurgeEtOH").text("PurgeEtOH Abort");
			$(".PurgeEtOH").removeAttr("disabled");
			$(".PurgeEtOH").removeClass("disabled_button");
			$(".PurgeEtOH").addClass("enable_button");
			$(".PurgeEtOH").attr("isable", "true");
		}
		else if (procCommand == 1009 && procParam1 == 1 && procParam2 == 0)
		{
			$(".PurgeExpchamber").text("PurgeExpchamber Abort");
			$(".PurgeExpchamber").removeAttr("disabled");
			$(".PurgeExpchamber").removeClass("disabled_button");
			$(".PurgeExpchamber").addClass("enable_button");
			$(".PurgeExpchamber").attr("isable", "true");
		}
		else if (procCommand == 1009 && procParam1 == 0 && procParam2 == 0)
		{
			$(".PurgeProcesschamber").text("PurgeProcesschamber Abort");
			$(".PurgeProcesschamber").removeAttr("disabled");
			$(".PurgeProcesschamber").removeClass("disabled_button");
			$(".PurgeProcesschamber").addClass("enable_button");
			$(".PurgeProcesschamber").attr("isable", "true");
		}
		else if (procCommand == 1007 && procParam1 == 1 && procParam2 == 0)
		{
			$(".PumpExpchamber").text("PumpExpchamber Abort");
			$(".PumpExpchamber").removeAttr("disabled");
			$(".PumpExpchamber").removeClass("disabled_button");
			$(".PumpExpchamber").addClass("enable_button");
			$(".PumpExpchamber").attr("isable", "true");
		}
		else if (procCommand == 1007 && procParam1 == 0 && procParam2 == 0)
		{
			$(".PumpProcesschamber").text("PumpProcesschamber Abort");
			$(".PumpProcesschamber").removeAttr("disabled");
			$(".PumpProcesschamber").removeClass("disabled_button");
			$(".PumpProcesschamber").addClass("enable_button");
			$(".PumpProcesschamber").attr("isable", "true");
		}
	}
	else if (status == 2)
	{
		$(".PurgeHF").text("PurgeHF");
		$(".PurgeEtOH").text("PurgeEtOH");
		$(".PurgeExpchamber").text("PurgeExpchamber");
		$(".PurgeProcesschamber").text("PurgeProcesschamber");
		$(".PumpExpchamber").text("PumpExpchamber");
		$(".PumpProcesschamber").text("PumpProcesschamber");
		$(".btn_box").find("button").attr("disabled", "disabled");
		$(".btn_box").find("button").removeClass("enable_button");
		$(".btn_box").find("button").addClass("disabled_button");
		$(".btn_box").find("button").attr("isable", "false");
	}

	delete json;
	delete arr;
	delete system_data;
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
}

function clearSetInterval()
{
	clearInterval(setIntervalID2);
	clearInterval(setIntervalID);
}

setIntervalID2 = setInterval(getSettingsData, 500);






                   
                       