var LeakCheckTime = 5;
var LeakCheckPressure = 0;
var LeakCheckThreshold = 0;
var LeakCheckPumpHTime = 0;
var intervalTimeId = "";
var leadTime = 2 + 30 / 60;
var range = {
	"LeakCheckTime" : [0, 3600],
	"LeakCheckPressure" : [0, 100],
	"LeakCheckThreshold" : [0, 10],
	"LeakCheckPumpHTime" : [0, 600]
};

function getParam()
{
	var confJson = {};
	arr = [9000, 9001, 9002, 9003];
	try
	{
		confJson = getControl().fetch_parameters(arr);
		confJson = $.parseJSON(confJson);
	}
	catch (e)
	{
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}

	var system_data = confJson["parameters"];
	for (var i = 0; i < system_data.length; ++i)
	{
		switch (system_data[i]["id"])
		{
		case "9000":
			$(".pump_down_pressure").val(system_data[i]["value"]);
			LeakCheckPressure = parseFloat(system_data[i]["value"]);
			break;

		case "9001":
			$(".leak_check_phtime").val(system_data[i]["value"]);
			LeakCheckPumpHTime = parseInt(system_data[i]["value"]);
			break;

		case "9002":
			$(".test_time").val(system_data[i]["value"]);
			LeakCheckTime = parseInt(system_data[i]["value"]);
			break;

		case "9003":
			$(".leak_check_threshold").val(system_data[i]["value"]);
			LeakCheckThreshold = parseInt(system_data[i]["value"]);
			break;

		default:
		
		}
	}
}

function getSettings()
{
	var arr = [100030, 100031, 100050, 100052, 100053, 100054];
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

	/*
	var confJson = {};
	arr = [9000, 9001, 9002, 9003];
	try
	{
		confJson = getControl().fetch_parameters(arr);
		confJson = $.parseJSON(confJson);
	}
	catch (e)
	{
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}*/
	
	var system_data = json["systemdata"];
	var status = 0;
	var procCommand = 1000;
	var procParam1 = 0;
	var procParam2 = 0;
	for (var i = 0; i < system_data.length; ++i)
	{
		switch (system_data[i]["id"])
		{
		case "100030":
			$(".result").text(system_data[i]["value"]);
			break;

		case "100031":
			$(".increase_rate").find("span:first").text(system_data[i]["value"]);
			break;

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
	
	/*
	var system_data = confJson["parameters"];
	for (var i = 0; i < system_data.length; ++i)
	{
		switch (system_data[i]["id"])
		{
		case "9000":
			$(".pump_down_pressure").val(system_data[i]["value"]);
			LeakCheckPressure = parseFloat(system_data[i]["value"]);
			break;

		case "9001":
			$(".leak_check_phtime").val(system_data[i]["value"]);
			LeakCheckPumpHTime = parseInt(system_data[i]["value"]);
			break;

		case "9002":
			$(".test_time").val(system_data[i]["value"]);
			LeakCheckTime = parseInt(system_data[i]["value"]);
			break;

		case "9003":
			$(".leak_check_threshold").val(system_data[i]["value"]);
			LeakCheckThreshold = parseInt(system_data[i]["value"]);
			break;

		default:
		
		}
	}*/

	if (status == 0)
	{
		$(".exp_chamber_btn").text("Expansion chamber test start");
		$(".pro_chamber_btn").text("Process chamber test start");
		$(".pro_chamber_btn").removeAttr("disabled");
		$(".exp_chamber_btn").removeAttr("disabled");
		$(".btn_box").find("button").removeClass("disabled_button");
		$(".btn_box").find("button").addClass("enable_button");
	}
	else if (status == 1)
	{
		$(".exp_chamber_btn").text("Expansion chamber test start");
		$(".pro_chamber_btn").text("Process chamber test start");
		$(".pro_chamber_btn").attr("disabled", "disabled");
		$(".exp_chamber_btn").attr("disabled", "disabled");
		$(".btn_box").find("button").removeClass("enable_button");
		$(".btn_box").find("button").addClass("disabled_button");
		if (procCommand == 1014 && procParam1 == 0 && procParam2 == 0)
		{
			$(".pro_chamber_btn").text("Process chamber test cancel");
			$(".pro_chamber_btn").removeAttr("disabled");
			$(".pro_chamber_btn").removeClass("disabled_button");
			$(".pro_chamber_btn").addClass("enable_button");
		}
		else if (procCommand == 1014 && procParam1 == 1 && procParam2 == 0)
		{
			$(".exp_chamber_btn").text("Expansion chamber test cancel");
			$(".exp_chamber_btn").removeAttr("disabled");
			$(".exp_chamber_btn").removeClass("disabled_button");
			$(".exp_chamber_btn").addClass("enable_button");
		}
	}
	else if (status == 2)
	{
		$(".exp_chamber_btn").text("Expansion chamber test start");
		$(".pro_chamber_btn").text("Process chamber test start");
		$(".pro_chamber_btn").attr("disabled", "disabled");
		$(".exp_chamber_btn").attr("disabled", "disabled");
		$(".btn_box").find("button").removeClass("enable_button");
		$(".btn_box").find("button").addClass("disabled_button");
	}
	delete json;
	//delete confJson;
	delete system_data;
	delete arr;
}

function initChart() {
    var Chart1 = new Tee.Chart("canvas");
	Chart1.axes.bottom.title.text = "Time (min)";
	Chart1.axes.left.title.text = "Pressure (torr)";
	Chart1.axes.left.title.format.font.fill = "white";
	Chart1.axes.left.title.format.font.style = "14px Tahoma";
	Chart1.axes.left.ticks.stroke.fill = "#09272C";
	Chart1.axes.left.format.stroke.fill = "#09272C";
	Chart1.axes.left.title.margins.right = 4;
	Chart1.axes.left.labels.format.font.fill = "white";	
	Chart1.axes.bottom.title.format.font.fill = "white";
	Chart1.axes.bottom.title.format.font.style = "14px Tahoma";
	Chart1.axes.bottom.labels.format.font.fill = "white";
	Chart1.axes.bottom.ticks.stroke.fill = "#09272C";
	Chart1.axes.bottom.format.stroke.fill = "#09272C";
	var bmax = parseInt($(".test_time").val());
	if (!isNaN(bmax) && 0 != bmax)
	{
		Chart1.axes.bottom.setMinMax(0, bmax);
	}
	
	var lmax = parseInt($(".pump_down_pressure").val());
	if (!isNaN(lmax) && 0 != lmax)
	{
		Chart1.axes.left.setMinMax(0, lmax);
	}
	
    var series1 = Chart1.addSeries(new Tee.Line());
    Chart1.panel.format.gradient.colors = ["beige", "orange"];
    Chart1.panel.format.gradient.direction = "bottomtop";
    Chart1.legend.position = "left";
    series1.addRandom(10);
    series1.data.x = new Array(series1.count());
    for (t = 0; t < series1.count(); t++) {
        series1.data.x[t] = t;
        series1.data.values[t] = 0;
    }
    Chart1.title.visible = false;
    Chart1.panel.transparent = true;
    Chart1.legend.visible = false;
    Chart1.axes.bottom.setMinMax(0, LeakCheckTime / 60);
    Chart1.axes.bottom.labels.roundFirst = false;
    Chart1.zoom.enabled = false;
	Chart1.scroll.enabled = false;
    Chart1.draw();
	delete Chart1;
	delete series1;
}

function draw(results) {
	$("#canvas").remove();
	$(".canvas_box").append('<canvas id="canvas" width="1008" height="500" style="padding:0px;">This browser does not seem to support HTML5 Canvas.</canvas>');
    var Chart1 = new Tee.Chart("canvas");
	Chart1.axes.bottom.title.text = "Time (min)";
	Chart1.axes.left.title.text = "Pressure (torr)";
	Chart1.axes.left.title.format.font.fill = "white";
	Chart1.axes.left.title.format.font.style = "14px Tahoma";
	Chart1.axes.left.ticks.stroke.fill = "#09272C";
	Chart1.axes.left.format.stroke.fill = "#09272C";
	Chart1.axes.left.title.margins.right = 6;
	Chart1.axes.left.labels.format.font.fill = "white";	
	Chart1.axes.bottom.title.format.font.fill = "white";
	Chart1.axes.bottom.title.format.font.style = "14px Tahoma";
	Chart1.axes.bottom.labels.format.font.fill = "white";
	Chart1.axes.bottom.ticks.stroke.fill = "#09272C";
	Chart1.axes.bottom.format.stroke.fill = "#09272C";
	var bmax = parseInt($(".test_time").val());
	if (!isNaN(bmax) && 0 != bmax)
	{
		Chart1.axes.bottom.setMinMax(0, bmax);
	}
	
	var lmax = parseInt($(".pump_down_pressure").val());
	if (!isNaN(lmax) && 0 != lmax)
	{
		Chart1.axes.left.setMinMax(0, lmax);
	}
	
    Chart1.title.visible = false;
    var series1 = Chart1.addSeries(new Tee.Line());
    series1.addRandom(results.length);
    series1.data.x = new Array(series1.count());
    for (t = 0; t < series1.count(); t++) {
        series1.data.x[t] = results[t]['time'];
        series1.data.values[t] = results[t]['pressure'];
    }
    Chart1.panel.transparent = true;
    Chart1.legend.visible = false;
    var x1 = series1.data.x;
	Chart1.axes.bottom.setMinMax(leadTime, LeakCheckTime / 60);
    Chart1.axes.bottom.labels.roundFirst = false;
    Chart1.zoom.enabled = false;
	Chart1.scroll.enabled = false;
    var tip = new Tee.ToolTip(Chart1);
    Chart1.tools.add(tip);
    tip.ongettext = function (tool, text, series, index) {
        if (tip.render == "dom")
            return 'Pressure:' + series.data.values[index] + '  Time:' + series.data.x[index];
        else
            return 'Pressure:' + series.data.values[index] + '  Time:' + series.data.x[index];
    }
    Chart1.legend.visible = false;
    Chart1.legend.legendStyle = "series";
    Chart1.legend.position = "left";
    Chart1.draw();
	delete Chart1;
	delete tip;
	delete series1;
}

function saveSettingsData()
{
	getControl().modify_parameter(9002, LeakCheckTime);
	getControl().modify_parameter(9000, LeakCheckPressure);
	getControl().modify_parameter(9001, LeakCheckPumpHTime);
	getControl().modify_parameter(9003, LeakCheckThreshold);
}

var timeId1 = "";
var timeId2 = "";
var pchamberData = [];
var echamberData = [];
var time1 = 0;
function getProChamberData()
{
	if (time1 >= LeakCheckTime)
	{
		draw(pchamberData);
		clearInterval(timeId1);
		return;
	}
	saveSettingsData();
	var text = $(".pro_chamber_btn").text();
	if ("Process chamber test cancel" == text)
	{
		getControl().invoke(0, 3);
		clearInterval(timeId2);
		clearInterval(timeId1);
		while(pchamberData.length > 0)
		{
			delete (pchamberData.shift());
		}
		time1 = 0;
	}
	else if ("Process chamber test start" == text)
	{
		getControl().invoke(0, 1014, 0);
		clearInterval(timeId2);
		if (time1 == 0)
		{
			pchamberData.push({pressure:LeakCheckPressure,time:0});
		}
		
		function createData()
		{
			if (time1 >= LeakCheckTime)
			{
				clearInterval(timeId1);
				return;
			}
			var arr = [8020];
			var valueJson = {};
			try
			{
				valueJson = getControl().fetch_system_data(arr, false);
				valueJson = $.parseJSON(valueJson);
			}
			catch (e)
			{
				Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
				return;
			}

			time1 = time1 + 0.5;
			var data = {
				pressure : parseFloat(valueJson["systemdata"][0]["value"]),
				time : (time1 / 60).toFixed(3) + leadTime
			};			
			
			if (pchamberData.length > LeakCheckTime / 0.5)
			{
				delete (pchamberData.shift());
			}
			
			pchamberData.push(data);
			
			draw(pchamberData);
			delete valueJson;
		}
	
		timeId1 = setInterval(createData, 500);
	}
}

function cancelGetProChamberData()
{
	clearInterval(timeId1);
}

var time2 = 0;
function getExpChamberData()
{
	if (time2 >= LeakCheckTime)
	{
		draw(pchamberData);
		clearInterval(timeId2);
		return;
	}
	saveSettingsData();
	var text = $(".exp_chamber_btn").text();
	if ("Expansion chamber test cancel" == text)
	{
		getControl().invoke(0, 3);
		clearInterval(timeId2);
		clearInterval(timeId1);
		while(echamberData.length > 0)
		{
			delete (echamberData.shift());
		}
		time2 = 0;
	}
	else if ("Expansion chamber test start" == text)
	{
		getControl().invoke(0, 1014, 1);
		clearInterval(timeId1);

		if (time2 == 0)
		{
			echamberData.push({pressure:LeakCheckPressure,time:0});
		}
		
		function createData()
		{
			if (time1 >= LeakCheckTime)
			{
				clearInterval(timeId2);
				return;
			}
			var arr = [8023];
			var valueJson = {};
			try
			{
				valueJson = getControl().fetch_system_data(arr, false);
				valueJson = $.parseJSON(valueJson);
			}
			catch (e)
			{
				Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
				return;
			}

			time2 = time2 + 0.5;
			var data = {
				pressure : parseFloat(valueJson["systemdata"][0]["value"]),
				time : (time2 / 60).toFixed(3) + leadTime
			};			
			
			if (echamberData.length > LeakCheckTime / 0.5)
			{
				delete (echamberData.shift());
			}
			
			echamberData.push(data);
			
			draw(echamberData);
			delete valueJson;
		}
		
		timeId2 = setInterval(createData, 500);
	}
}

function cancelGetExpChamberData()
{
	clearInterval(timeId2);
}

function init()
{
	getParam();
	$(".test_time").blur(function(){
		LeakCheckTime = parseInt($(this).val());
		if (isNaN(LeakCheckTime))
		{
			LeakCheckTime = 1200;
		}
		else if (LeakCheckTime < range["LeakCheckTime"][0] || LeakCheckTime > range["LeakCheckTime"][1])
		{
			alert("The TestTime should be [" + range["LeakCheckTime"][0] + "," + range["LeakCheckTime"][1] + "]");
			if (LeakCheckTime < range["LeakCheckTime"][0])
			{
				$(this).val(range["LeakCheckTime"][0]);
			}
			else
			{
				$(this).val(range["LeakCheckTime"][1]);
			}
		}
	});

	$(".pump_down_pressure").blur(function(){
		LeakCheckPressure = parseInt($(this).val());
		if (isNaN(LeakCheckPressure))
		{
			LeakCheckPressure = 10;
		}
		else if (LeakCheckPressure < range["LeakCheckPressure"][0] || LeakCheckPressure > range["LeakCheckPressure"][1])
		{
			alert("The LeakCheckPressure should be [" + range["LeakCheckPressure"][0] + "," + range["LeakCheckPressure"][1] + "]");
			if (LeakCheckPressure < range["LeakCheckPressure"][0])
			{
				$(this).val(range["LeakCheckPressure"][0]);
			}
			else
			{
				$(this).val(range["LeakCheckPressure"][1]);
			}
		}
	});

	$(".leak_check_threshold").blur(function(){
		LeakCheckThreshold = parseInt($(this).val());
		if (isNaN(LeakCheckThreshold))
		{
			LeakCheckThreshold = 5;
		}
		else if (LeakCheckThreshold < range["LeakCheckThreshold"][0] || LeakCheckThreshold > range["LeakCheckThreshold"][1])
		{
			alert("The LeakCheckThreshold should be [" + range["LeakCheckThreshold"][0] + "," + range["LeakCheckThreshold"][1] + "]");
			if (LeakCheckThreshold < range["LeakCheckThreshold"][0])
			{
				$(this).val(range["LeakCheckThreshold"][0]);
			}
			else
			{
				$(this).val(range["LeakCheckThreshold"][1]);
			}
		}
	});

	$(".leak_check_phtime").blur(function(){
		LeakCheckPumpHTime = parseInt($(this).val());
		if (isNaN(LeakCheckPumpHTime))
		{
			LeakCheckPumpHTime = 30;
		}
		else if (LeakCheckPumpHTime < range["LeakCheckPumpHTime"][0] || LeakCheckPumpHTime >range["LeakCheckPumpHTime"][1])
		{
			alert("The LeakCheckPumpHoldTime should be [" + range["LeakCheckPumpHTime"][0] + "," + range["LeakCheckPumpHTime"][1] + "]");
			if (LeakCheckPumpHTime < range["LeakCheckPumpHTime"][0])
			{
				$(this).val(range["LeakCheckPumpHTime"][0]);
			}
			else
			{
				$(this).val(range["LeakCheckPumpHTime"][1]);
			}
		}
	});
}

function clearSetInterval()
{
	clearInterval(intervalTimeId);
}

intervalTimeId = setInterval(getSettings, 500);