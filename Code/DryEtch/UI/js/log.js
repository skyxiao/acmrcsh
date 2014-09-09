$(document).ready(function (){
	getLogData("");
});

var nDivHight = $(".table_info_box").height();
$(".table_info_box").scroll(function(){
	var nScrollHight = $(this)[0].scrollHeight;
    var nScrollTop = $(this)[0].scrollTop;
    if (nScrollTop + nDivHight >= nScrollHight)
	{
		var tds = $(".table_info").find("tr:last").find("td");
		var time = $(tds[2]).text();
		getLogData(time);
    }
});

function getLogData(time, selectTime)
{
	try
	{
		var fs = require('fs');
		var connect = $.parseJSON(fs.readFileSync('./config/database.json'));
		var mysql = require('mysql');
		var TEST_DATABASE = connect.database.database;

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

		var sql = "";
		var beginTime = $("#txBeginTime").val().trim();
		var endTime = $("#txEndTime").val().trim();

		if (endTime == "")
		{
			endTime = getDateString(new Date());
		}

		if (beginTime == "")
		{

			sql = "select id, time, level, info from event_log where time < '" + endTime + "'order by time desc limit 25";
		}
		else
		{

			sql = "select id, time, level, info from event_log where time >= '" + beginTime + "' and time <= '" + endTime + "'order by time desc limit 25";

		}
		
		//var sql = "select id, time, level, info from event_log order by time desc limit 25";
		if (time && time != "")
		{
			if (beginTime != "")
			{
				sql = "select id, time, level, info from event_log where time >= '" + beginTime + "' and time < '" + time + "' order by time desc limit 25";
			}
			else
			{
				sql = "select id, time, level, info from event_log where time < '" + time + "' order by time desc limit 25";
			}
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
		
			var info = "";
			for (var i = 0; i < results.length; ++i)
			{
				var row = results[i];
				var classname = "";
				if (selectTime && selectTime == getDateString(row["time"]))
				{
					classname = "selectLog";
				}
				else
				{
					classname = "";
				}

				info += '<tr class="' + row["level"] + ' ' + classname + '">' +
							'<td width="10%">' + row["id"] + '</td>' +
							'<td width="10%">' + row["level"] + '</td>' +
							'<td width="20%">' + getDateString(row["time"]) + '</td>' +
							'<td width="60%">' + row["info"] + '</td>' +
						'</tr>';
			}

			if (time == "")
			{
				$(".table_info").children().remove();
			}

			$(".table_info").append(info);
			
			connection.end();
		  }
		);
	}
	catch (e)
	{
		writeLog("exception", e);
	}
	
}

function getDateString(date)
{
	var str = "";
	str += date.getFullYear();
	var month = date.getMonth() + 1;
	if (month < 10)
	{
		str += "-0" + month;
	}
	else
	{
		str += "-" + month;
	}
	
	var day = date.getDate();
	if (day < 10)
	{
		str += "-0" + day;
	}
	else
	{
		str += "-" + day;
	}
	
	var hour = date.getHours();
	if (hour < 10)
	{
		str += " 0" + hour;
	}
	else
	{
		str += " " + hour;
	}
	
	var minute = date.getMinutes();
	if (minute < 10)
	{
		str += ":0" + minute;
	}
	else
	{
		str += ":" + minute;
	}
	
	var second = date.getSeconds();
	if (second < 10)
	{
		str += ":0" + second;
	}
	else
	{
		str += ":" + second;
	}
	
	return str;
}