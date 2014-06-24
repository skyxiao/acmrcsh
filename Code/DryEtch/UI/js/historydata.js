var Chart1, scroller,tip;

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
    Chart1.scroll.mouseButton = 0;
    Chart1.scroll.direction = "horizontal";
    scroller = new Tee.Scroller("canvas2", Chart1);
   
    Chart1.draw();
}

function draw(results) {
    Chart1 = new Tee.Chart("canvas");
    Chart1.title.visible = false;
    Chart1.axes.bottom.labels.dateFormat = "isoTime";
	Chart1.axes.bottom.labels.format.font.fill = "white";
	Chart1.axes.left.labels.format.font.fill = "white";	
    var series1 = Chart1.addSeries(new Tee.Line());
    series1.addRandom(results.length);
    series1.title = $("#ddlType").val();
    series1.data.x = new Array(series1.count());
    for (t = 0; t < series1.count(); t++) {
        tmp = new Date(results[t]['time']);
        series1.data.x[t] = tmp;
        series1.data.values[t] = results[t]['data'];
    }
    Chart1.title.text = $("#ddlType").val();
    Chart1.panel.transparent = true;
    Chart1.legend.visible = false;
    var x1 = series1.data.x;
    var length = x1.length;
    Chart1.axes.bottom.setMinMax(x1[0].getTime(), x1[length - 1].getTime());
    Chart1.axes.bottom.labels.roundFirst = true;
    Chart1.zoom.enabled = false;
    Chart1.scroll.mouseButton = 0;
    Chart1.scroll.direction = "horizontal";
    scroller = new Tee.Scroller("canvas2", Chart1);
    scroller.onChanging = function (s, min, max) {
//        var mi = new Date(min);//.toDateString(),
//        ma = new Date(max);//.toDateString();
//        document.getElementById("data").textContent = "Showing data from " + mi + " to " + ma;
    }
    tip = new Tee.ToolTip(Chart1);
    Chart1.tools.add(tip);
    tip.ongettext = function (tool, text, series, index) {
        if (tip.render == "dom")
            return 'Value: ' + series.data.values[index];
        else
            return 'Value: ' + series.data.values[index];
    }
    Chart1.legend.visible = true;
    Chart1.legend.legendStyle = "series";
    Chart1.legend.position = "right";
    Chart1.draw();
}


function testConnectMysql() {
    var fs = require('fs');
	try
	{
		var connect = $.parseJSON(fs.readFileSync('./config/database.json'));
		var mysql = require('mysql');
		var TEST_DATABASE = connect.database.database; //  'test';

		var connection = mysql.createConnection({
			host: connect.database.host,
			user: connect.database.user,
			password: connect.database.password,
			port: connect.database.port
		});

		connection.query('USE ' + TEST_DATABASE, function(err){
			if (err)
			{
				writeLog("error", err);
				connection.end();
				return;
			}
		});
		//var data = Math.random() * 100;
		//console.log('call dry_etch.record_data(DATE_ADD(now(), INTERVAL ' + 1 + ' SECOND), "HF",' + data + ');');
		//connection.query('call dry_etch.record_data(DATE_ADD(now(), INTERVAL ' + 1 + ' SECOND), "HF",' + data + ');');
		for (var i = 0; i < 10000; i++) {
			var data = Math.random() * 100;
			connection.query('call dry_etch.record_data(DATE_ADD(now(), INTERVAL ' + i + ' SECOND), "HF",' + data + ');', function(err){
				if (err)
				{
					writeLog("error", err);
					connection.end();
					return;
				}
			});
		}
	}
	catch (e)
	{
		writeLog("exception", e);
	}
    
}

function getData() {
    var begin = $("#txBeginTime").val();
    var end = $("#txEndTime").val();
    if (begin == "") {
        alert("Begin Time can be null,please input begin time");
        return;
    }
    if (end == "") {
        alert("End Time can be null,please input end time");
        return;
    }
    var type = $("#ddlType").val();
    var fs = require('fs');
	try
	{
		var connect = $.parseJSON(fs.readFileSync('./config/database.json'));
		var mysql = require('mysql');
		var TEST_DATABASE = connect.database.database; //  'test';

		var connection = mysql.createConnection({
			host: connect.database.host,
			user: connect.database.user,
			password: connect.database.password,
			port: connect.database.port
		});
		connection.query('USE ' + TEST_DATABASE, function(err){
			if (err)
			{
				writeLog("error", err);
				connection.end();
				return;
			}
		});
		var sql = 'SELECT  * FROM process_data where type="' + type + '" and time>="' + begin + '" and time <="' + end + '" limit 1500';
		console.log(sql);
		connection.query(
		  sql,
		  function selectCb(err, results, fields) {
			if (err) 
			{ 
				writeLog("error", err);
				connection.end();
				return; 
			}
			
			if (results.length > 0) {
				  draw(results);
			}
			else {
				alert("No data get it");
			}
			connection.end();
		 }
	   );
	}
	catch (e)
	{
		writeLog("exception", e);
	}
}