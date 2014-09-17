var recentTime = "";
var wRecentTime = "";
var eRecentTime = "";
var warningCount = 0;
var errorCount = 0;

function getRecentLog()
{
	var log = getControl().fetch_recent_event_log();
	log = $.parseJSON(log);

	if(!log || log["event_logs"].length == 0)
		return;

	var logObject = {};
	var logLevel = "";
	var logCount = log["event_logs"].length;
	for (var i = 0; i < logCount; ++i)
	{
		logObject = log["event_logs"][i];
		logLevel = logObject["level"];
		if(logLevel == "warning")
			warningCount = warningCount + 1;
		else if(logLevel == "error")
			errorCount = errorCount + 1;
	}

	var time = logObject["time"].substr(0, 4) + "-" + logObject["time"].substr(4, 2) + "-" + logObject["time"].substr(6, 2) + " " +
		logObject["time"].substr(9, 2) + ":" + logObject["time"].substr(11, 2) + ":" + logObject["time"].substr(13, 2);
	var logstr = "[" + time + "] " + "[" + logObject["level"] + "] " + "[" + logObject["id"] + "] :" + logObject["info"];
	if (logObject["level"] == "warning")
	{
		$(".log_content").removeClass("log_content_contrast")
			.removeClass("log_info")
			.removeClass("log_error")
			.addClass("log_warning");
	}
	else if (logObject["level"] == "error")
	{
		$(".log_content").removeClass("log_content_contrast")
			.removeClass("log_info")
			.removeClass("log_warning")
			.addClass("log_error");
	}
	else
	{
		$(".log_content").removeClass("log_content_contrast")
			.removeClass("log_warning")
			.removeClass("log_error")
			.addClass("log_info");
	}
	$(".log_content").text(logstr);
	$(".warning_count").text(warningCount);
	$(".error_count").text(errorCount);
	delete log;
}

function init()
{
	$(".log_btn_clear").click(function(){
		warningCount = 0;
		errorCount = 0;
		$(".warning_count").text(warningCount);
		$(".error_count").text(errorCount);
		$(".log_content").text("");
		$(".long_log").removeClass("long_log");
		getControl().clear_alarm();
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

	$(".username").text(top.USER_INFO["name"]);
	$(".logout").click(function(){
		$(".tooltip").remove();
		if (confirm("Are you sure sign out?"))
		{
			top.window.location.href = "Login.htm";
			delete top.USER_INFO;
		}
	});

	$(".logout").mouseover(function(){
		var e = event || window.event;
		var tooltip = "<div class='tooltip' style='position:absolute;left:" + (e.clientX - 40) + "px;top:" + (e.clientY + 10) + "px;'><label>Sign out</label></div>";
		$("body").append(tooltip);
	});


	$(".logout").mouseleave(function(){
		$(".tooltip").remove();
	});
}

setInterval("getRecentLog()", 1000);