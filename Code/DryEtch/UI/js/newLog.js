var recentLog = [];
var recentTime = "";
var wRecentTime = "";
var eRecentTime = "";
var timer = 0;
var warningCount = 0;
var errorCount = 0;

function getRecentLog()
{
	var log = getControl().fetch_recent_event_log();
	log = $.parseJSON(log);
	var logstr = "";

	if (log && log["event_logs"] != "")
	{
		/*
		var level = 0;
		if (recentLog.length > 0)
		{
			if (recentLog[0]["level"] == "warning")
			{
				level = 1;
			}
			else if (recentLog[0]["level"] == "error")
			{
				level = 2;
			}
		}
		*/

		var logObject = {};
		if (log["event_logs"].length == 1)
		{
			 logObject = log["event_logs"][0];
		}
		else
		{
			/*
			logObject = log["event_logs"][0];
			for (var i = 1; i < log["event_logs"].length; ++i)
			{
				if (log["event_logs"][i]["level"] == "error")
				{
					logObject = log["event_logs"][i];
				}
				else if (log["event_logs"][i]["level"] == "warning")
				{
					if (logObject["level"] != "error")
					{
						logObject = log["event_logs"][i];
					}
				}
				else if (log["event_logs"][i]["level"] == "info")
				{
					if (logObject["level"] == "info")
					{
						logObject = log["event_logs"][i];
					}
				}
			}*/

			logObject = log["event_logs"][0];
			var time = parseInt(logObject["time"]);
			for (var i = 1; i < log["event_logs"].length; ++i)
			{
				if (parseInt(log["event_logs"][i]["time"]) > time)
				{
					logObject = log["event_logs"][i];
					time = parseInt(logObject["time"]);
				}
			}
		}

		var time = logObject["time"].substr(0, 4) + "-" + logObject["time"].substr(4, 2) + "-" + logObject["time"].substr(6, 2) + " " +
			logObject["time"].substr(9, 2) + ":" + logObject["time"].substr(11, 2) + ":" + logObject["time"].substr(13, 2);
		logstr += "[" + time + "] " + "[" + logObject["level"] + "] " + "[" + logObject["id"] + "] :" + logObject["info"];
		recentTime = time;
		if (logObject["level"] == "info")
		{
			$(".log_content").removeClass("log_content_contrast")
				.removeClass("log_warning")
				.removeClass("log_error")
				.addClass("log_info");
			clearInterval(setIntervalid);
			delete (recentLog.shift());
			recentLog.push(logObject);
			$(".log_content").text(logstr);
			timer = 0;
			$(".long_log").removeClass("long_log");
		}
		else if (logObject["level"] == "warning")
		{
			warningCount = warningCount + 1;
			$(".log_content").removeClass("log_content_contrast")
				.removeClass("log_info")
				.removeClass("log_error")
				.addClass("log_warning");
			clearInterval(setIntervalid);
			delete (recentLog.shift());
			recentLog.push(logObject);
			$(".log_content").text(logstr);
			$(".warning_count").text(warningCount);
			timer = 0;
			wRecentTime = recentTime;
			$(".long_log").removeClass("long_log");
		}
		else if (logObject["level"] == "error")
		{
			errorCount = errorCount + 1;
			$(".log_content").removeClass("log_content_contrast")
				.removeClass("log_info")
				.removeClass("log_warning")
				.addClass("log_error");
			clearInterval(setIntervalid);
			setIntervalid = setInterval("twinkle()", 800);
			delete (recentLog.shift());
			recentLog.push(logObject);
			$(".log_content").text(logstr);
			timer = 0;
			eRecentTime = recentTime;
			$(".error_count").text(errorCount);
			$(".long_log").removeClass("long_log");
		}
	}

	if (recentLog.length > 0)
	{
		timer = timer + 800;
		if (timer > 1000 * 60 * 4)
		{
			if ($(".long_log").length == 0)
			{
				$(".log_content").addClass("long_log");
			}

			timer = 0;
		}
	}

	delete log;
}

var flag = false;
var setIntervalid = "";
function twinkle()
{
	if (!flag)
	{
		$(".log_content").addClass("log_content_contrast");
	}
	else
	{
		$(".log_content").removeClass("log_content_contrast");
	}
	flag = !flag;
}

function init()
{
	$(".log_btn_clear").click(function(){
		clearInterval(setIntervalid);
		warningCount = 0;
		errorCount = 0;
		$(".warning_count").text(warningCount);
		$(".error_count").text(errorCount);
		$(".log_content").text("");
		$(".long_log").removeClass("long_log");
		delete (recentLog.shift());
		getControl().reset_signal_tower();
	});

	$(".log_content").click(function(){
		var text = $(this).text();
		if (text.trim() != "")
		{
			top.frames["menu"].SelectMenu("Log", null, "log", recentTime);		
		}
	});

	$(".warning_count").click(function(){
		if (warningCount != 0)
		{
			top.frames["menu"].SelectMenu("Log", null, "log", wRecentTime);
		}
	});

	$(".error_count").click(function(){
		if (errorCount != 0)
		{
			top.frames["menu"].SelectMenu("Log", null, "log", eRecentTime);
		}
	});
}

setInterval("getRecentLog()", 1000);