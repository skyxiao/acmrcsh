var setIntervalID;
//var x0 = 500;//大圈中心点
//var y0 = 300;
var x0 = 210;//大圈中心点
var y0 = 430;
var r = 110;
var lr = 70;
var setIntervalID2;

function  permissionCheck()
{
	var user_info = top.USER_INFO;
	if (user_info)
	{
		return;
	}

	if (user_info["overview"] && user_info["overview"] == "readonly")
	{
		$(".auto").attr("disabled", "disabled");
		$(".auto").removeClass("enable_button");
		$(".auto").addClass("disabled_button");
		$(".abort").removeClass("enable_button");
		$(".abort").addClass("disabled_button");
		$(".abort").attr("disabled", "disabled");
		$(".recipe_opreation").removeClass("long_btn_enable");
		$(".recipe_opreation").attr("disabled", "disabled");
		$(".recipe_opreation").addClass("long_btn_disable");
		$(".btn_ok").attr("disabled", "disabled");
		$(".btn_ok").removeClass("enable_button");
		$(".btn_ok").addClass("disabled_button");
	}
}

function clearSetInterval()
{
	clearInterval(setIntervalID);
	clearInterval(setIntervalID2);
}

function getPoints(angle) {
    var hudu = (2 * Math.PI / 360) * angle;
    var x1 = r * Math.cos(hudu);
    var y1 = r * Math.sin(hudu);
    var x3 = x1 + x0;
    var y3 = y1 + y0;
    return [x3 - lr, y3 - lr];
}

function getMousePos(event) {
    var e = event || window.event;
    alert("x:" + e.clientX + ", y:" + clientY);
}

var arrayObj = ["empty", "unprocessed", "processing", "processed", "semiprocessed", "broken"];

function turntableRun() 
{
	var json = {};
	var arr = [61, 62, 37, 60, 100040, 100041, 100042, 100043, 100044, 100045, 7044, 65, 66, 63, 100046, 100047]
	try
	{
		json = getControl().fetch_system_data(arr, false);
		json = $.parseJSON(json);
	}
	catch (e)
	{
		delete json;
		delete arr;
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}

	var systemdata = json["systemdata"];
	var Slot1WaferID = "";
	var Slot2WaferID = "";
	var Slot3WaferID = "";
	var Slot1WaferState = 0;
	var Slot2WaferState = 0;
	var Slot3WaferState = 0;
	var angle = 0;
	var pickUp = 0;
	var pickDown = 0;
	var doorClose = 0;
	var doorOpen = 0;
	var armIn = 0;
	var armOut = 0;
	var forkV = 0;
	var waferId = "";
	var waferState = 0;

	for (var i = 0; i < systemdata.length; ++i)
	{
		switch (systemdata[i]["id"])
		{
		case "61":
			pickUp = parseInt(systemdata[i]["value"]);
			break;

		case "62":
			pickDown = parseInt(systemdata[i]["value"]);
			break;

		case "37":
			doorClose = parseInt(systemdata[i]["value"]);
			break;

		case "60":
			doorOpen = parseInt(systemdata[i]["value"]);
			break;

		case "66":
			armOut = parseInt(systemdata[i]["value"]);
			break;

		case "63":
			forkV = parseInt(systemdata[i]["value"]);
			break;

		case "65":
			armIn = parseInt(systemdata[i]["value"]);
			break;

		case "100046":
			waferId = systemdata[i]["value"];
			break;

		case "100047":
			waferState = systemdata[i]["value"];
			break;

		case "100040":
			Slot1WaferID = systemdata[i]["value"];
			break;

		case "100041":
			Slot2WaferID = systemdata[i]["value"];
			break;

		case "100042":
			Slot3WaferID = systemdata[i]["value"];
			break;

		case "100043":
			Slot1WaferState = parseInt(systemdata[i]["value"]);
			Slot1WaferState = isNaN(Slot1WaferState) ? 0 : Slot1WaferState;
			break;

		case "100044":
			Slot2WaferState = parseInt(systemdata[i]["value"]);
			Slot2WaferState = isNaN(Slot2WaferState) ? 0 : Slot2WaferState;
			break;

		case "100045":
			Slot3WaferState = parseInt(systemdata[i]["value"]);
			Slot3WaferState = isNaN(Slot3WaferState) ? 0 : Slot3WaferState;
			break;

		case "7044":
			angle = systemdata[i]["value"];
			break;

		default:
		
		}
	}

	if (armIn == 1 && armOut == 0)
	{
		$(".arm").attr("class", "arm arm_in");
	}
	else if (armIn == 0 && armOut == 0)
	{
		$(".arm").attr("class", "arm arm_middle");
	}
	else if (armIn == 0 && armOut == 1)
	{
		$(".arm").attr("class", "arm arm_out");
	}

	$(".wafer").text(waferId);

	if (forkV == 0)
	{
		$(".fork").attr("class", "fork fork_v");
		$(".wafer").attr("class", "wafer wafer_v wafer_" + arrayObj[waferState]);
	}
	else if (forkV == 1)
	{
		$(".fork").attr("class", "fork fork_h");
		$(".wafer").attr("class", "wafer wafer_h wafer_" + arrayObj[waferState]);
	}

	if (doorClose == 0 && doorOpen == 1)
	{
		$(".door").attr("class", "door door_open");
	}
	else if (doorClose == 0 && doorOpen == 0)
	{
		$(".door").attr("class", "door door_middle");
	}
	else
	{
		$(".door").attr("class", "door door_close");
	}

	if (pickUp == 0 && pickDown == 0)
	{
		$(".pinStatus").find("img:first").attr("src", "../images/pin_normal.png");
	}
	else if (pickUp == 1 && pickDown == 0)
	{
		$(".pinStatus").find("img:first").attr("src", "../images/pin_up.png");
	}
	else if (pickUp == 0 && pickDown == 1)
	{
		$(".pinStatus").find("img:first").attr("src", "../images/pin_down.png");
	}

	var i = parseInt(angle);
	var j = parseFloat(angle);
	if (isNaN(i) || isNaN(j))
	{
		i = 0;
		j = 0;
	}

	var k = j - i;
	var p1 = getPoints(i % 360 + k);
	var p2 = getPoints((i + 120) % 360 + k);
	var p3 = getPoints((i + 240) % 360 + k);
	$("#secondWinfer").attr("style", "position: absolute; left:" + p1[0] + "px; top:" + p1[1] + "px");
	$("#firstWinfer").attr("style", "position: absolute; left:" + p2[0] + "px; top:" + p2[1] + "px");
	$("#thirdWinfer").attr("style", "position: absolute; left:" + p3[0] + "px; top:" + p3[1] + "px");
	$("#firstWinfer").find("div:first").text(Slot1WaferID);
	$("#secondWinfer").find("div:first").text(Slot2WaferID);
	$("#thirdWinfer").find("div:first").text(Slot3WaferID);
	$("#firstWinfer").find("div:first").attr("class", arrayObj[Slot1WaferState]);
	$("#secondWinfer").find("div:first").attr("class", arrayObj[Slot2WaferState]);
	$("#thirdWinfer").find("div:first").attr("class", arrayObj[Slot3WaferState]);

	delete json;
	delete arr;
	delete systemdata;
}

$(document).ready(function () {
	setIntervalID = setInterval(turntableRun, 500);
	setIntervalID2 = setInterval(getInitData, 1000);
	/*
    $("#btnRun").click(function () {
			if (setIntervalID != "")
			{
				return;
			}
            var arrayObj = ["broken", "processed", "processing", "unprocessed", "semiprocessed"];
            var i = 1;
            setIntervalID = setInterval(function () {
                var p1 = getPoints((6 * i) % 360);
                var p2 = getPoints((6 * i + 120) % 360);
                var p3 = getPoints((6 * i + 240) % 360);
                $("#firstWinfer").attr("style", "position: absolute; left:" + p1[0] + "px; top:" + p1[1] + "px");
                $("#secondWinfer").attr("style", "position: absolute; left:" + p2[0] + "px; top:" + p2[1] + "px");
                $("#thirdWinfer").attr("style", "position: absolute; left:" + p3[0] + "px; top:" + p3[1] + "px");
                $("#firstWinfer").attr("class", arrayObj[i % 5]);
                $("#secondWinfer").attr("class", arrayObj[(i + 1) % 5]);
                $("#thirdWinfer").attr("class", arrayObj[(i - 1) % 5]);
                i++;
            }, 500);
		
		$("#btnStop").click(function(){
			clearInterval(setIntervalID);
			setIntervalID = "";
		});
    });
	*/
	$(".auto").click(function (){
		var text = $(".auto").text();
		if ("Auto" == text)
		{
			getControl().invoke(0, 0);
		}
		else if ("Stop" == text)
		{
			getControl().invoke(0, 1);
		}		
	});

	$(".abort").click(function (){
		getControl().invoke(0, 3);
	});

    var first = getPoints(0);
    var sec = getPoints(120);
    var three = getPoints(240);
    $("#firstWinfer").attr("style", "position: absolute; left:" + first[0] + "px; top:" + first[1] + "px");
    //console.log("sec,position: absolute; left:" +p2x + "px; top:" + sec[1] + "px");
    $("#secondWinfer").attr("style", "position: absolute; left:" + sec[0] + "px; top:" + sec[1] + "px");
    $("#thirdWinfer").attr("style", "position: absolute; left:" + three[0] + "px; top:" + three[1] + "px");
    //console.log("three,position: absolute; left:" + three[0] + "px; top:" + three[1] + "px");
	//setTimeout(initMenu, 200);   
//    setPropertyChange(updateProperty);
});

function initMenu()
	{
		$(".Mask1").contextMenu('myMenu1',
		{
			shadow:false,
			menuStyle: {
				border:'1px solid #158895',
				width : '150px',
				"z-index":900,
				'border-radius' : '5px',
				display: top.USER_INFO.overview && top.USER_INFO.overview == "readonly" ? "none" : "",
				background:'-webkit-gradient(linear, left top, left bottom, from(#01202A), to(#07495F))'
			},
			itemStyle: {
				background:"none",
				color: 'white',
				border: 'none',
				'font-weight':'normal',
				'padding-left':'5px'

			},
			itemHoverStyle: {
				color: '#fff',
				background: '-webkit-gradient(linear, left top, left bottom, from(#0A68BA), to(#044D71))',
				border: 'none',
				'font-weight':'bold'
			},
			bindings:
			{
				'HOME': function (t) {
					getControl().invoke(0, 1001);
				},
				'PUMP': function (t) {
					getControl().invoke(0, 1007, 0);
				},
				'VENT': function (t) {
					getControl().invoke(0, 1008, 0);
				},
				'PINUP': function (t) {
					getControl().invoke(0, 1010);
				},
				'PINDOWN': function (t){
					getControl().invoke(0, 1011);
				},
				'OPEN_DOOR': function (t){
					getControl().invoke(0, 1017);
				},
				'CLOSE_DOOR': function (t) {
					getControl().invoke(0, 1018);
				},
				'TURNON_HEATER': function (t) {
					getControl().invoke(0, 1012);
				},
				'TURNOFF_HEATER': function (t) {
					getControl().invoke(0, 1013);
				},
				'ROTATE_FORWARD': function (t) {
					getControl().invoke(0, 1015);
				},
				'ROTATE_BACKWARD': function (t) {
					getControl().invoke(0, 1016);
				},
				'LOAD': function (t) {
					getControl().invoke(0, 1002);
				},
				'UNLOAD': function (t) {
					getControl().invoke(0, 1003);
				}
			}
		});

		$(".Mask2").contextMenu('myMenu2',
		{
			shadow:false,
			menuStyle: {
				border:'1px solid #158895',
				width : '150px',
				"z-index":900,
				display: top.USER_INFO.overview && top.USER_INFO.overview == "readonly" ? "none" : "",
				'border-radius' : '5px',
				background:'-webkit-gradient(linear, left top, left bottom, from(#01202A), to(#07495F))'
			},
			itemStyle: {
				background:"none",
				color: 'white',
				border: 'none',
				'font-weight':'normal',
				'padding-left':'5px'

			},
			itemHoverStyle: {
				color: '#fff',
				background: '-webkit-gradient(linear, left top, left bottom, from(#0A68BA), to(#044D71))',
				border: 'none',
				'font-weight':'bold'
			},
			bindings:
			{
				'ARM_IN': function (t) {
					getControl().invoke(0, 1019);
				},
				'ARM_OUT': function (t) {
					getControl().invoke(0, 1020);
				},
				'FORK_VERTICAL': function (t) {
					getControl().invoke(0, 1021);
				},
				'FORK_HORIZONTAL': function (t) {
					getControl().invoke(0, 1022);
				}
			}
		});
	}

//document.onmousemove = mouseMove;
function mouseMove(ev) {
    ev = ev || window.event;
    var mousePos = mouseCoords(ev);
}
function mouseCoords(ev) {
    if (ev.pageX || ev.pageY) {
        console.log(ev.pageX + "," + ev.pageY)
        return { x: ev.pageX, y: ev.pageY };
    }
    return {
        x: ev.clientX + document.body.scrollLeft - document.body.clientLeft,
        y: ev.clientY + document.body.scrollTop - document.body.clientTop
    };
}

var updateProperty = function (data) {
    console.log(data.parameters[0].id);
};

var fs = window.parent.FS;
var recipedir = "./recipe";

function loadRecipe()
{
	fs.readdir(recipedir, function(err, files){
		if (err)
		{
			Dialog.alert("<label style='font-size:14px;'>Read The dir '" + recipedir + "' ERROE:" + err + "</label>");
			return;
		}
		
		var tr = "<tr>";
		for (var i = 0; i < files.length; ++i)
		{
			var file = files[i];
			var recipeName = file.substring(0, file.lastIndexOf("."));
			tr += '<td style="width:25%;" align="center">' + recipeName + '</td>';
			if ((i + 1) % 4 == 0 && i != files.length - 1)
			{
				tr += "</tr><tr>"
			}
		}

		tr += "</tr>";
		
		$(".recipe_list").find("table").children().remove();
		$(".recipe_list").find("table").append(tr);
		$(".recipe_list").find("table").find("td").click(function(){
			$(".recipe_list_select").removeClass("recipe_list_select");
			$(this).addClass("recipe_list_select");
		});
		$(".recipe_list").show();
	});
}

function unLoadRecipe()
{
	var rName = $(".overview_rname").next().val();
	if ("" != rName)
	{
		getControl().unload_recipe(rName);
	}
}
	
function selectRecipeSure()
{
	var rName = $(".recipe_list_select").text();
	if (rName == "")
	{
		Dialog.alert("<label style='font-size:14px;'>Please Select the recipe loaded</label>");
		return;
	}

	getControl().load_recipe(rName);
	try
	{
		var json = getControl().fetch_system_data([100013], false);
		json = $.parseJSON(json);
		$(".overview_rname").next().val(json["systemdata"][0]["value"]);
		delete json;
	}
	catch (e)
	{
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}
	
	
	$(".recipe_list").hide();
}

function selectRecipeCancel()
{
	$(".recipe_list").hide();
}

function getInitData()
{
	var arr = [100024, 100013, 100014, 100015, 100016, 100017, 100050, 100018, 100055, 4000, 5000, 6000, 8020, 8023];
	var json = {};
	try
	{
		json = getControl().fetch_system_data(arr, false);
		json = $.parseJSON(json);
	}
	catch (e)
	{
		delete json;
		delete arr;
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}
	
	var system_data = json["systemdata"];
	var recipe_total_time = 0;
	var recipe_done_time = 0;
	var step_total_time = 0;
	var step_done_time = 0;
	for (var i = 0; i < system_data.length; ++i)
	{
		switch (system_data[i]["id"])
		{
		case "100024":
			$(".overview_info_2").text(system_data[i]["value"]);
			break;

		case "100013":
			$(".overview_rname").next().val(system_data[i]["value"]);
			break;

		case "100014":
			recipe_total_time = parseInt(system_data[i]["value"]);
			break;

		case "100015":
			recipe_done_time = parseInt(system_data[i]["value"]);
			break;

		case "100016":
			step_total_time = parseInt(system_data[i]["value"]);
			break;

		case "100017":
			step_done_time = parseInt(system_data[i]["value"]);
			break;

		case "100050":
			if (system_data[i]["value"] == 0)
			{
				$(".status_content").text("idle");
				$(".status_content").removeClass("running");
				$(".status_content").removeClass("error");
				$(".status_content").addClass("idle");
				$(".auto").text("Auto");
				$(".auto").removeClass("disabled_button");
				$(".auto").addClass("enable_button");
				$(".auto").removeAttr("disabled");
				$(".abort").attr("disabled", "disabled");
				$(".abort").removeClass("enable_button");
				$(".abort").addClass("disabled_button");
				$(".recipe_opreation").removeClass("long_btn_disable");
				$(".recipe_opreation").removeAttr("disabled");
				$(".recipe_opreation").addClass("long_btn_enable");
			}
			else if (system_data[i]["value"] == 1)
			{
				$(".status_content").text("running");
				$(".status_content").removeClass("idle");
				$(".status_content").removeClass("error");
				$(".status_content").addClass("running");
				$(".auto").text("Stop");
				$(".auto").removeClass("disabled_button");
				$(".auto").addClass("enable_button");
				$(".auto").removeAttr("disabled");
				$(".abort").removeClass("disabled_button");
				$(".abort").addClass("enable_button");
				$(".abort").removeAttr("disabled");
			}
			else if (system_data[i]["value"] == 2)
			{
				$(".status_content").text("error");
				$(".status_content").removeClass("idle");
				$(".status_content").removeClass("running");
				$(".status_content").addClass("error");
				$(".auto").text("Auto");
				$(".auto").attr("disabled", "disabled");
				$(".auto").removeClass("enable_button");
				$(".auto").addClass("disabled_button");
				$(".abort").removeClass("disabled_button");
				$(".abort").addClass("enable_button");
				$(".abort").removeAttr("disabled");
			}

			if (system_data[i]["value"] != 0)
			{
				$(".recipe_opreation").removeClass("long_btn_enable");
				$(".recipe_opreation").attr("disabled", "disabled");
				$(".recipe_opreation").addClass("long_btn_disable");
			}
			break;

		case "100018":
			if (system_data[i]["value"] == 0)
			{
				$(".btn_ok").attr("disabled", "disabled");
				$(".btn_ok").removeClass("enable_button");
				$(".btn_ok").addClass("disabled_button");
			}
			else
			{
				$(".btn_ok").removeAttr("disabled");
				$(".btn_ok").removeClass("disabled_button");
				$(".btn_ok").addClass("enable_button");
			}
			break;

		case "100055":
			$(".current_step").text(system_data[i]["value"]);
			break;

		case "4000":
			$(".aiBodyHTTemp").text(system_data[i]["value"] + " ℃");
			break;

		case "5000":
			$(".aiLidHTTemp").text(system_data[i]["value"] + " ℃");
			break;

		case "6000":
			$(".aiChuckHTTemp").text(system_data[i]["value"] + " ℃");
			break;

		case "8020":
			$(".aiProcChamPressure").text(system_data[i]["value"] + " torr");
			break;

		case "8023":
			$(".aiExpChamPressure").text(system_data[i]["value"] + " torr");
			break;

		default:
		
		}
	}

	permissionCheck();

	if (recipe_total_time != 0 && recipe_done_time <= recipe_total_time)
	{
		$(".recipe_done_progress").parent().find("div").text(recipe_done_time + " / " + recipe_total_time);
	}	

	if (0 != recipe_total_time && !isNaN(recipe_total_time) && !isNaN(recipe_done_time))
	{
		var rate = recipe_done_time / recipe_total_time;
		if (rate > 1)
		{
			rate = 1;
		}

		rate = rate * 100;
		var ratestr = rate + "%";
		$(".recipe_done_progress").width(ratestr);
	}
	else
	{
		$(".recipe_done_progress").width("0%");
	}

	if (step_total_time != 0 && step_done_time <= step_total_time)
	{
		$(".step_done_progress").parent().find("div").text(step_done_time + " / " + step_total_time);
	}

	if (0 != step_total_time && !isNaN(step_total_time) && !isNaN(step_done_time))
	{
		var rate = step_done_time / step_total_time;
		if (rate > 1)
		{
			rate = 1;
		}

		rate = rate * 100;
		var ratestr = rate + "%";
		$(".step_done_progress").width(ratestr);
	}
	else
	{
		$(".step_done_progress").width("0%");
	}

	delete json;
	delete system_data;
	delete arr;
}

function overViewSkip()
{
	getControl().modify_system_data(100019, 1);
	getControl().modify_system_data(100020, 1);
}

function overViewOk()
{
	var WaferLotID = $(".WaferLotID").val();
	if (WaferLotID != "")
	{		
		getControl().modify_system_data(100022, WaferLotID);
		getControl().modify_system_data(100019, 0);
		getControl().modify_system_data(100020, 1);
	}
	else
	{
		alert("The WaferLotID can't be blank!!!");
	}
}