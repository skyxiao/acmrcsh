var results = {
	"2000" : [],
	"2002" : [],
	"2004" : [],
	"2006" : [],
	"2008" : [],
	"8001" : [],
	"8020" : [],
	"8023" : [],
	"4000" : [],
	"5000" : [],
	"6000" : []
};

var activeList = [];
var allList = [2000,2002,2004,2006,2008,8001,8020,8023,4000,5000,6000];
var intervalTime = 200;
var lineColor = {
	"2000" : "#1371D0", 
	"2002" : "#D87E2A", 
	"2004" : "#F73003", 
	"2006" : "#AFAC05", 
	"2008" : "#62C209", 
	"8001" : "#6D1E76", 
	"8020" : "#0D8B90", 
	"8023" : "#DA25A1", 
	"4000" : "#070707", 
	"5000" : "#234301", 
	"6000" : "#24F41D"
};

function initChart() {
	$("#canvas").remove();
	$(".canvas_box").append('<canvas id="canvas" width="952" height="600" style="padding:0px;">This browser does not seem to support HTML5 Canvas.</canvas>');
    var Chart1 = new Tee.Chart("canvas");
	Chart1.axes.bottom.title.text = "Time (s)";
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
    Chart1.panel.format.gradient.colors = ["beige", "orange"];
    Chart1.panel.format.gradient.direction = "bottomtop";
    Chart1.legend.position = "left";
	var series1 = Chart1.addSeries(new Tee.Line());
    series1.addRandom(10);
    series1.data.x = new Array(series1.count());
    for (t = 0; t < series1.count(); t++) {
        series1.data.x[t] = t;
        series1.data.values[t] = 0;
    }

    Chart1.title.visible = false;
    Chart1.panel.transparent = true;
    Chart1.legend.visible = false;
    Chart1.axes.bottom.labels.roundFirst = false;
    Chart1.zoom.enabled = false;
	Chart1.scroll.enabled = false;
    Chart1.draw();
	delete Chart1;
	delete series1;
}

var count = 0;

function draw(List) {
	$("#canvas").remove();
	$(".canvas_box").append('<canvas id="canvas" width="952" height="600" style="padding:0px;">This browser does not seem to support HTML5 Canvas.</canvas>');
    var Chart1 = new Tee.Chart("canvas");
	Chart1.axes.bottom.title.text = "Time (s)";
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
    Chart1.title.visible = false;
	for (var i = 0; i < List.length; ++i)
	{
		var series1 = Chart1.addSeries(new Tee.Line());
		series1.addRandom(results[List[i]].length);
		series1.format.fill = lineColor[List[i]];
		series1.data.x = new Array(series1.count());
		for (t = 0; t < series1.count(); t++) {
			series1.data.x[t] = results[List[i]][t]['time'];
			series1.data.values[t] = results[List[i]][t]['value'];
		}
		//delete series1.data.x;
		//delete series1;
	}

	Chart1.axes.left.setMinMax(-1000, 1000);
	if (List.length == 0)
	{
		Chart1.axes.bottom.setMinMax(0, 100);
	}
	else
	{
		var length = results[List[0]].length;
		var minTime = results[List[0]][0]["time"];
		var maxTime = results[List[0]][length - 1]["time"];
		if (maxTime < 100)
		{
			maxTime = 100;
		}		
		
		Chart1.axes.bottom.setMinMax(minTime, maxTime);
	}
	

	if (List.length == 0)
	{
		var series1 = Chart1.addSeries(new Tee.Line());
		series1.addRandom(10);
		series1.data.x = new Array(series1.count());
		for (t = 0; t < series1.count(); t++) {
			series1.data.x[t] = t;
			series1.data.values[t] = 0;
		}
		Chart1.axes.bottom.setMinMax(0, 10);
		Chart1.axes.left.setMinMax(0, 1000);
	}
    
    Chart1.panel.transparent = true;
    Chart1.legend.visible = false;
    Chart1.axes.bottom.labels.roundFirst = false;
    Chart1.zoom.enabled = false;
	Chart1.scroll.enabled = false;
    var tip = new Tee.ToolTip(Chart1);
	if (List.length != 0)
	{
		Chart1.tools.add(tip);
		tip.ongettext = function (tool, text, series, index) {
			if (tip.render == "dom")
				return 'value:' + series.data.values[index] + '  time:' + series.data.x[index];
			else
				return 'value:' + series.data.values[index] + '  time:' + series.data.x[index];
		}
	}
    Chart1.legend.visible = false;
    Chart1.legend.legendStyle = "series";
    Chart1.legend.position = "left";
    Chart1.draw();
	delete Chart1;
	delete tip;
}

var intervalTimeId = "";
var intervalTimeId2 = "";
var time = 0;

function getData()
{
	var valueJson = {};
	try
	{
		valueJson = getControl().fetch_system_data(allList, false);
		valueJson = $.parseJSON(valueJson);
	}
	catch (e)
	{
		Dialog.alert("<label style='font-size:14px;'>" + e + "</label>");
		return;
	}

	time = time + intervalTime;
	var system_data = valueJson["systemdata"];
	for (var i = 0; i < system_data.length; ++i)
	{
		var length = results[system_data[i]["id"]].length;
		if (length > 0 && results[system_data[i]["id"]][length - 1]["time"] > 100)
		{
			delete (results[system_data[i]["id"]].shift());
		}

		var value = parseFloat(system_data[i]["value"]);
		if (isNaN(value))
		{
			value = 0;
		}
		results[system_data[i]["id"]].push({value:value,time:time/1000});
	}
	delete valueJson;
	delete system_data;
	delete value;
}

function start(This)
{
	var text = $(This).text();
	if ("Start" == text)
	{
		intervalTimeId = setInterval(getData, intervalTime);
		intervalTimeId2 = setInterval("draw(activeList)", 500);
		$(This).text("Stop");
	}
	else
	{
		clearInterval(intervalTimeId);
		clearInterval(intervalTimeId2);
		for (var key in results)
		{
			while(results[key].length > 0)
			{
				delete (results[key].shift());
			}
		}
		time = 0;
		count = 0;
		$(This).text("Start");
	}
}

Array.prototype.remove = function(val) {
	var index = this.indexOf(val);
	if (index > -1) {
		this.splice(index, 1);
	}
};


function init()
{
	$(".monitor_item").find("input").click(function(){
		var id = parseInt($(this).attr("idvalue"));
		if (!this.checked)
		{
			activeList.remove(id);
		}
		else
		{
			activeList.push(id);
		}

		if ($(".btn_box").find("button").text() == "Stop")
		{
			draw(activeList);
		}
	});

	$(".interval_time").blur(function(){
		var value = parseInt($(this).val());
		if (!isNaN(value))
		{
			if (value < 200)
			{
				value = 200;
				$(this).val(value);
			}

			intervalTime = value;
			clearInterval(intervalTimeId);
			var text = $(".btn_box").find("button").text();
			if ("Stop" == text)
			{
				intervalTimeId = setInterval(getData, intervalTime);
			}
		}
	});
}