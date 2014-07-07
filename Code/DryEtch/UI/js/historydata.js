var Chart1, scroller,tip;
var drawType = [];
var currentData = {};
var TypeMax = {
	"N2": 5000,
	"EtOH": 700,
	"HF": 2000,
	"Pressure": 200,
	"Chuck": 1000,
	"Lid":1000,
	"Body":1000,
	"Tank":2000
};

function  permissionCheck()
{
	var user_info = top.USER_INFO;
	if (user_info["history"] && user_info["history"] == "readonly")
	{
		$("#Button1").attr("class", "get_data_disable");
		$("#Button1").attr("disabled", "disabled");
		$(".historydata_content").find("input").attr("disabled", "disabled");
		$(".historydata_content").find("select").attr("disabled", "disabled");
		$(".typelist").find("input").attr("disabled", "disabled");
	}
}

function initChart() {
    Chart1 = new Tee.Chart("canvas");
    Chart1.axes.bottom.labels.dateFormat = "isoTime";
    var series1 = Chart1.addSeries(new Tee.Line());
    Chart1.panel.format.gradient.colors = ["beige", "orange"];
    Chart1.panel.format.gradient.direction = "bottomtop";
	Chart1.axes.bottom.labels.format.font.fill = "white";
	Chart1.axes.left.labels.format.font.fill = "white";	
    Chart1.legend.position = "right";
    series1.addRandom(10);
    series1.data.x = new Array(series1.count());
    for (t = 0; t < series1.count(); t++) {
        series1.data.x[t] = t;
        series1.data.values[t] = 0;
    }
    Chart1.title.visible = false;
    Chart1.panel.transparent = true;
    Chart1.legend.visible = false;
    var x1 = series1.data.x;
    Chart1.axes.bottom.setMinMax(x1[2], x1[5]);
    Chart1.axes.bottom.labels.roundFirst = true;
    Chart1.zoom.enabled = false;
   // Chart1.scroll.mouseButton = 0;
    //Chart1.scroll.direction = "horizontal";
    //scroller = new Tee.Scroller("canvas2", Chart1);
   
    Chart1.draw();
}

function draw(results) {
    Chart1 = new Tee.Chart("canvas");
    Chart1.title.visible = false;
    Chart1.axes.bottom.labels.dateFormat = "isoDateTime1";
	Chart1.axes.bottom.labels.format.font.fill = "white";
	Chart1.axes.left.labels.format.font.fill = "white";	
	Chart1.axes.left.labels.visible = false;
	var flag = false;
    //var series1 = Chart1.addSeries(new Tee.Line());
	for (var i = 0; i < drawType.length; ++i)
	{
		var res = results[drawType[i]];
		if (!res || res.length == 0)
		{
			continue;
		}

		flag = true;
		var series1 = Chart1.addSeries(new Tee.PointXY());
		//var series1 = Chart1.addSeries(new Tee.Line());
		series1.pointer.format.stroke.fill = "";
		series1.pointer.format.shadow.color = "";
		series1.pointer.width = 4;
		series1.pointer.height = 4;

		series1.addRandom(res.length);
		series1.title = drawType[i];
		series1.data.x = new Array(series1.count());

		for (t = 0; t < series1.count(); t++) 
		{
			var tmp = new Date(res[t]['time']);
			series1.data.x[t] = tmp;
			series1.data.values[t] = parseFloat(res[t]['data']) / TypeMax[drawType[i]];
		}

		series1["typeFlag"] = drawType[i];
    }
	
	if (!flag)
	{
		var series1 = Chart1.addSeries(new Tee.Line());
		series1.pointer.width = 0;
		series1.pointer.height = 0;
		series1.addRandom(10);
		series1.data.x = new Array(series1.count());
		for (t = 0; t < series1.count(); t++) {
			series1.data.x[t] = t;
			series1.data.values[t] = 0;
		}
	}
    
    Chart1.title.text = $("#ddlType").val();
    Chart1.panel.transparent = true;
    Chart1.legend.visible = true;
	if (!flag)
	{
		Chart1.legend.visible = false;
	}
	
    Chart1.axes.bottom.labels.roundFirst = true;
	Chart1.zoom.direction = "horizontal";
	Chart1.zoom.enabled = true;
	if (!flag)
	{
		Chart1.zoom.enabled = false;
		Chart1.scroll.enabled = false;
	}
    //Chart1.scroll.mouseButton = 0;

	Chart1.scroll.direction = "horizontal";
   // scroller = new Tee.Scroller("canvas2", Chart1);
    tip = new Tee.ToolTip(Chart1);
    Chart1.tools.add(tip);
    tip.ongettext = function (tool, text, series, index) {
		var value = "";
		if (series["typeFlag"])
		{
			value += "Value: " + (parseFloat(series.data.values[index]) * TypeMax[series["typeFlag"]]);
		}
		
		return value;
    }

    //Chart1.legend.visible = true;
	//Chart1.scroll.enabled = true;
    Chart1.legend.legendStyle = "series";
    Chart1.legend.position = "right";
    Chart1.draw();
}

function checkDrawType()
{
	drawType = [];
	$(".typelist").find("input").each(function(){
		if (this.checked)
		{
			var type = $(this).next().text().trim();
			drawType.push(type);
		}
	});
}

function getAllWafer(WaferLotID)
{
	var connection = getSqlConnection();
	//var sql = "select id from wafer_process where 1";
	var sql = "call query_batch_wafer('" + WaferLotID + "')"
		
	connection.query(
		sql,
		function selectCb(err, results, fields) {
		if (err)
		{
			writeLog("error", err);
			connection.end();
			return;
		}

		var waferList = "<tr>";
		var results = results[0];

		for (var i = 0; i < results.length; ++i)
		{
			if (i != 0 && i % 3 == 0)
			{
				waferList += "</tr><tr>";
			}

			waferList += "<td align='center'>" + results[i]["id"] + "</td>";
		}

		waferList += "</tr>";

		if (results.length != 0)
		{
			$(".wafer_list").find("table").empty();
			$(".wafer_list").find("table").append(waferList);
			$(".wafer_list").show();
			$(".wafer_list").find("table").find("td").click(function(){
				var text = $(this).text().trim();
				$(".batch_id").val(text);
				$(".wafer_list").hide();
				displayData(text);
			});
		}
		else
		{
			$(".wafer_list").hide();
		}

		connection.end();
	});
}

function displayData(id)
{
	checkDrawType();
	var connection = getSqlConnection();
	var sql = "select start_time, end_time, recipe from wafer_process where id='" + id + "'";
	connection.query(
		sql,
		function selectCb(err, results, fields) {
			if (err)
			{
				writeLog("error", err);
				connection.end();
				return;
			}

			if (results.length > 0)
			{
				var startTime = (new Date(results[0]["start_time"])).format("yyyy-mm-dd HH:MM:ss");
				var endTime = "";
				if (results[0]["end_time"])
				{
					endTime = (new Date(results[0]["end_time"])).format("yyyy-mm-dd HH:MM:ss");
				}

				$("#txBeginTime").val(startTime);
				$("#txEndTime").val(endTime);
				$(".recipe").val(results[0]["recipe"]);
				drawData(startTime, endTime);
			}
			else
			{
				$("#txBeginTime").val("");
				$("#txEndTime").val("");
				$(".recipe").val("");
			}

			connection.end();
	});
}

function drawData(startTime, endTime)
{
	var connection = getSqlConnection();
	var dataCount = 0;
	var relData = {};
	var typeArr = ["N2", "EtOH", "HF", "Pressure", "Chuck", "Lid", "Body","Tank"];

	for (var i = 0; i < typeArr.length; ++i)
	{
		var sql = "select time, type, data from process_data where type='" + typeArr[i] + "' and time >= '" + startTime + "' and time <= '" + endTime + "'";
		if (endTime == "")
		{
			sql = "select time, type, data from process_data where type='" + typeArr[i] + "' and time >= '" + startTime + "'";
		}

		connection.query(
			sql,
			function selectCb(err, results, fields) {
				if (err)
				{
					writeLog("error", err);
					connection.end();
					return;
				}

				if (results.length > 0)
				{
					var type = results[0]["type"];
					relData[type] = results;
				}

				dataCount += 1;
				if (dataCount == typeArr.length)
				{
					connection.end();
					currentData = relData;
					draw(relData);
				}
		});
	}
}

function init()
{
	$(".typelist").find("input").click(function(){
		checkDrawType();
		draw(currentData);
	});

	$(".batch_id").keyup(function(){
		var text = $(this).val().trim();
		if (text != "")
		{
			getAllWafer(text);
		}
	});
}

function Query()
{
	var id = $(".batch_id").val().trim();
	displayData(id);
	/*
	var startTime = $("#txBeginTime").val();
	if (startTime)
	{
		startTime = startTime.trim();
	}

	var endTime = $("#txEndTime").val();

	if (endTime)
	{
		endTime = endTime.trim();
	}

	if (startTime || endTime)
	{
		drawData(startTime, endTime);
	}*/
}