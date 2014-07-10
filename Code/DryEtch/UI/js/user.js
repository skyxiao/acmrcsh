var currentUser = {};

function  permissionCheck()
{
	var user_info = top.USER_INFO;
	if (user_info["user"] && user_info["user"] == "readonly")
	{
		$(".userbtn").find("button").attr("class", "user_disable_btn");
		$(".userbtn").find("button").attr("disabled", "disabled");
		$(".sure_btn_box").find("button").attr("class", "sure_disable_btn");
		$(".sure_btn_box").find("button").attr("disabled", "disabled");
		$(".userinfobox1").find("input").attr("disabled", "disabled");
		$(".userinfobox1").find("select").attr("disabled", "disabled");
	}
}

function getAllUser()
{
	var connection = getSqlConnection();
	var sql = "select * from user where 1";
		
	connection.query(
		sql,
		function selectCb(err, results, fields) {
		if (err)
		{
			writeLog("error", err);
			connection.end();
			return;
		}

		var li = "";
		for (var i = 0; i < results.length; ++i)
		{
			currentUser[results[i]["name"]] = results[i];
			li += "<li isSave='true'>" + results[i]["name"] + "</li>";
		}

		jQuery(".userlist").append(li);
		jQuery(".userlist").children("li").click(function(){
			checkIssave(this);
		});
				
		connection.end();
	});
}

function showUserInfo(This)
{
	$(".userselect").removeClass("userselect");
	$(This).addClass("userselect");

	var userName = $(This).text().trim();
	var userInfo = currentUser[userName];

	var str = "<tr>" +
					"<td style='display:none;' class='id'>" + userInfo["id"] + "</td>" +
					"<td align='right'>" +
						"<label>UserName:</label>" +
					"</td>" +
					"<td>" +
						"<input class='uname' value='" + userInfo["name"] + "'/>" +
					"</td>" +
					"<td align='right'>" +
						"<label>PassWord:</label>" +
					"</td>" +
					"<td>" +
						"<input class='pword' value='" + userInfo["password"] + "' />" +
					"</td>" +
			 "</tr>" +
			 "<tr>" +
					"<td align='right'>" +
						"<label>overview:</label>" +
					"</td>" +
					"<td>" +
						"<select class='overview'>" +
							"<option value='invisible' " + (userInfo["overview"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["overview"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["overview"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
					"<td align='right'>" +
						"<label>systemconfig:</label>" +
					"</td>" +
					"<td>" +
						"<select class='config'>" +
							"<option value='invisible' " + (userInfo["config"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["config"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["config"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
			 "</tr>" +
			 "<tr>" +
					"<td align='right'>" +
						"<label>systemdata:</label>" +
					"</td>" +
					"<td>" +
						"<select class='data'>" +
							"<option value='invisible' " + (userInfo["data"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["data"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["data"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
					"<td align='right'>" +
						"<label>gasbox:</label>" +
					"</td>" +
					"<td>" +
						"<select class='gasbox'>" +
							"<option value='invisible' " + (userInfo["gasbox"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["gasbox"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["gasbox"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
			 "</tr>" +
			 "<tr>" +
					"<td align='right'>" +
						"<label>recipe:</label>" +
					"</td>" +
					"<td>" +
						"<select class='recipe'>" +
							"<option value='invisible' " + (userInfo["recipe"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["recipe"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["recipe"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
					"<td align='right'>" +
						"<label>historydata:</label>" +
					"</td>" +
					"<td>" +
						"<select class='history'>" +
							"<option value='invisible' " + (userInfo["history"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["history"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["history"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
			 "</tr>" +
			 "<tr>" +
					"<td align='right'>" +
						"<label>log:</label>" +
					"</td>" +
					"<td>" +
						"<select class='log'>" +
							"<option value='invisible' " + (userInfo["log"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["log"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["log"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
					"<td align='right'>" +
						"<label>leakcheck:</label>" +
					"</td>" +
					"<td>" +
						"<select class='leakcheck'>" +
							"<option value='invisible' " + (userInfo["leakcheck"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["leakcheck"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["leakcheck"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
			 "</tr>" +
			 "<tr>" +
					"<td align='right'>" +
						"<label>monitor:</label>" +
					"</td>" +
					"<td>" +
						"<select class='monitor'>" +
							"<option value='invisible' " + (userInfo["monitor"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["monitor"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["monitor"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
					"<td align='right'>" +
						"<label>user:</label>" +
					"</td>" +
					"<td>" +
						"<select class='user_select'>" +
							"<option value='invisible' " + (userInfo["user"] == "invisible" ? "selected='selected'" : "") + ">invisible</option>" +
							"<option value='readonly' " + (userInfo["user"] == "readonly" ? "selected='selected'" : "") + ">readonly</option>" +
							"<option value='operable' " + (userInfo["user"] == "operable" ? "selected='selected'" : "") + ">operable</option>" +
						"</select>" +
					"</td>" +
			 "</tr>";
	$(".userinfobox1").find("table").empty();
	$(".userinfobox1").find("table").append(str);
	$(".sure_btn_box").show();
	$(".userinfobox1").children("table").find("input").change(function() {		
		$(".userselect").css("color", "#AF0505");
		$(".userselect").attr("isSave", "false");
	});
		
	$(".userinfobox1").find("select").change(function() {
		$(".userselect").css("color", "#AF0505");
		$(".userselect").attr("isSave", "false");
	});

	$(".overview").attr("disabled", "disabled");
	$(".uname").keyup(function(){
		var text = $(this).val();
		if (text.length > 12)
		{
			var str = text.substring(0, 12);
			$(this).val(str);
		}
	});

	permissionCheck();
}

function getUserObject()
{
	var userInfo = {};
	userInfo["id"] = $(".id").text().trim();
	userInfo["name"] = $(".uname").val().trim();
	userInfo["password"] = $(".pword").val().trim();
	userInfo["overview"] = $(".overview").val().trim();
	userInfo["config"] = $(".config").val().trim();
	userInfo["data"] = $(".data").val().trim();
	userInfo["gasbox"] = $(".gasbox").val().trim();
	userInfo["recipe"] = $(".recipe").val().trim();
	userInfo["history"] = $(".history").val().trim();
	userInfo["log"] = $(".log").val().trim();
	userInfo["leakcheck"] = $(".leakcheck").val().trim();
	userInfo["monitor"] = $(".monitor").val().trim();
	userInfo["user"] = $(".user_select").val().trim();

	return userInfo;
}

function saveUser(This)
{
	if ($(".userselect").attr("isSave") == "false")
	{
		var oldUserName = $(".userselect").text().trim();
		var userInfo = getUserObject();
		if (userInfo["name"] == "")
		{
			alert("The UserName can't be empty!!!");
			return;
		}

		if (userInfo["password"] == "")
		{
			alert("The PassWord can't be empty!!!");
			return;
		}

		var connection = getSqlConnection();
		var sql = "update `user` set " + "name='" +
					userInfo["name"] + "', " + "password='" + userInfo["password"] + "', " + "overview='" +
					userInfo["overview"] + "', " + "config='" + userInfo["config"] + "', " + "data='" + userInfo["data"] + "', " + "gasbox='" +
					userInfo["gasbox"] + "'," + "recipe='" + userInfo["recipe"] + "', " + "history='" + userInfo["history"] + "', " +
					"log='" + userInfo["log"] + "', " + "leakcheck='" + userInfo["leakcheck"] + "', " + "monitor='" + userInfo["monitor"] + "', " +
					"user='" + userInfo["user"] + "' where id=" + userInfo["id"];

		connection.query(
			sql,
			function callback(err, results, fields) {
				if (err)
				{
					writeLog("error", err);
					connection.end();
					return;
				}

				delete currentUser[oldUserName];
				$(".userselect").text(userInfo["name"]);

				currentUser[userInfo["name"]] = userInfo;
				$(".messagealert").text("The user '" + userInfo["name"] + "' save successfully");
				setTimeout("$('.messagealert').slideUp(300)", 3000);
				$(".messagealert").slideDown(300);
				$(".userselect").css("color", "#fff");
				$(".userselect").attr("isSave", "true");
				if (This)
				{
					showUserInfo(This);
				}
				connection.end();
		});
	}	
}

function checkIssave(This)
{
	var isSave = $(".userselect").attr("isSave");
	if (isSave && isSave != "true")
	{
		if (confirm("The user '" + $(".userselect").text().trim() + "' is modified, do you want to save it?"))
		{
			saveUser(This);
		}
		else
		{
			var userInfo = getUserObject();
			if (userInfo["name"])
			{
				currentUser[userInfo["name"]] = userInfo;
			}
			showUserInfo(This);
		}
	}
	else
	{
		showUserInfo(This);
	}	
}

function deleteUser()
{
	var userName = $(".userselect").text();

	if (userName && userName.trim() != "")
	{
		if (confirm("Are you sure you want to delete the user '" + userName.trim() + "'?"))
		{
			var userInfo = currentUser[userName.trim()];
			var connection = getSqlConnection();
			var sql = "delete from `user` where id=" + userInfo["id"];

			connection.query(
				sql,
				function callback(err, results, fields) {
					if (err)
					{
						writeLog("error", err);
						connection.end();
						return;
					}

					$(".userinfobox1").find("table").empty();
					$(".sure_btn_box").hide();
					$(".userselect").remove();	
					delete currentUser[userName.trim()];
					connection.end();
			});
		}
	}
}

function userNameCheck(userName, cbFun)
{
	if (userName == "")
	{
		$(".un_error").text("Username can't be blank");
		$(".un_error").show();
		return;
	}

	var connection = getSqlConnection();
	var sql = "select name from `user` where name='" + userName + "'";

	connection.query(
		sql,
		function callback(err, results, fields) {
			if (err)
			{
				writeLog("error", err);
				connection.end();
				return;
			}

			if (results.length > 0)
			{
				$(".un_error").text("Username is already taken");
				$(".un_error").show();
				return;
			}
			else
			{
				$(".un_error").hide();
			}

			if (cbFun)
			{
				cbFun();
			}
			
			connection.end();
	});
}

$(".add_uname").blur(function(){
	var uName = $(this).val().trim();

	userNameCheck(uName);
});

$(".add_cpword").blur(function(){
	var pw = $(".add_pword").val().trim();
	var pcw = $(this).val().trim();

	if (pw != pcw)
	{
		$(".pcw_error").show();
	}
	else
	{
		$(".pcw_error").hide();
	}
});

$(".add_pword").blur(function(){
	var pw = $(this).val().trim();

	if (pw == "")
	{
		$(".pw_error").show();
	}
	else
	{
		$(".pw_error").hide();
	}
});

function addUser()
{
	var uName = $(".add_uname").val().trim();
	userNameCheck(uName, addUserCb);
}

function addUserCb()
{
	var pw = $(".add_pword").val().trim();
	var pcw = $(".add_cpword").val().trim();

	if (pw == "")
	{
		$(".pw_error").show();
		return;
	}
	else
	{
		$(".pw_error").hide();
	}

	if (pw != pcw)
	{
		$(".pcw_error").show();
		return;
	}
	else
	{
		$(".pcw_error").hide();
	}

	var connection = getSqlConnection();
	var sql = "insert into `user` (`name`,`password`,`overview`,`config`,`data`,`gasbox`,`recipe`,`history`,`log`,`leakcheck`,`monitor`,`user`)" +
			  " values('" + $(".add_uname").val().trim() + "','" + $(".add_pword").val().trim() + "','" + $(".add_overview").val().trim() + "','" +
			  $(".add_config").val().trim() + "','" + $(".add_data").val().trim() + "','" + $(".add_gasbox").val().trim() + "','" +
			  $(".add_recipe").val().trim() + "','" + $(".add_history").val().trim() + "','" + $(".add_log").val().trim() + "','" +
			  $(".add_leakcheck").val().trim() + "','" + $(".add_monitor").val().trim() + "','" + $(".add_user").val().trim() + "')";

	connection.query(
		sql,
		function callback(err, results, fields) {
			if (err)
			{
				writeLog("error", err);
				connection.end();
				return;
			}
			
			addUserCb2(connection);
	});
}

function addUserCb2(conn)
{
	var sql = "select * from `user` order by id desc limit 1";
	if (!conn)
	{
		alert("Add user failed!!!");
		return;
	}

	conn.query(
		sql,
		function callback(err, results, fields) {
			if (err)
			{
				writeLog("error", err);
				conn.end();
				return;
			}
			
			if (results.length > 0)
			{
				currentUser[results[0]["name"]] = results[0];
				var li = "<li isSave='true'>" + results[0]["name"] + "</li>";
				jQuery(".userlist").append(li);
				jQuery(".userlist").children("li").click(function(){
					checkIssave(this);
				});
				resetNewUserBox();
			}
			else
			{
				alert("Add user failed!!!");
			}

			conn.end();
	});
}

function resetNewUserBox()
{
	$(".add_uname").val("");
	$(".add_pword").val("");
	$(".add_cpword").val("");
	$(".add_overview").val("operable");
	$(".add_config").val("invisible");
	$(".add_data").val("invisible");
	$(".add_gasbox").val("operable");
	$(".add_recipe").val("invisible");
	$(".add_history").val("operable");
	$(".add_log").val("operable");
	$(".add_leakcheck").val("invisible");
	$(".add_monitor").val("invisible");
	$(".add_user").val("invisible");
	$(".un_error").hide();
	$(".pw_error").hide();
	$(".pcw_error").hide();
	$(".mask_fade").hide();
	$(".new_user").hide();
}

function newUser()
{
	$(".mask_fade").show();
	$(".new_user").show();
}