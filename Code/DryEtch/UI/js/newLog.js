var recentLog = [];
var timer = 0;

function getRecentLog()
{
	var log = getControl().fetch_recent_event_log();
	log = $.parseJSON(log);
	var logstr = "";

	if (log && log["event_logs"] != "")
	{
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

		var logObject = {};
		if (log["event_logs"].length == 1)
		{
			 logObject = log["event_logs"][0];
		}
		else
		{
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
			}
		}

		var time = logObject["time"].substr(0, 4) + "-" + logObject["time"].substr(4, 2) + "-" + logObject["time"].substr(6, 2) + " " +
			logObject["time"].substr(9, 2) + ":" + logObject["time"].substr(11, 2) + ":" + logObject["time"].substr(13, 2);
		logstr += "[" + time + "] " + "[" + logObject["level"] + "] " + "[" + logObject["id"] + "] :" + logObject["info"];
		if (logObject["level"] == "info" && level == 0)
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
		else if (logObject["level"] == "warning" && level <= 1)
		{
			$(".log_content").removeClass("log_content_contrast")
				.removeClass("log_info")
				.removeClass("log_error")
				.addClass("log_warning");
			clearInterval(setIntervalid);
			delete (recentLog.shift());
			recentLog.push(logObject);
			$(".log_content").text(logstr);
			timer = 0;
			$(".long_log").removeClass("long_log");
		}
		else if (logObject["level"] == "error")
		{
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
		$(".log_content").text("");
		$(".long_log").removeClass("long_log");
		delete (recentLog.shift());
		getControl().reset_signal_tower();
	});
}

setInterval("getRecentLog()", 1000);