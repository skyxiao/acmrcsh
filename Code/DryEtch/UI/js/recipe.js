var ClipBoard = [];
var fs = require("fs");
var recipedir = "./recipe";
var unSaveRecipeList = {};
var range = {
	position : [0, 100],
	pressure : [0, 200],
	etoh : [0, 700],
	HF : [0, 2000],
	N2 : [0, 5000],
	"N2 Purge" : [0, 5000]
};

function  permissionCheck()
{
	var user_info = top.USER_INFO;
	if (user_info["recipe"] && user_info["recipe"] == "readonly")
	{
		$("input").attr("disabled", "disabled");
		$("select").attr("disabled", "disabled");
		$(".recipebtn").find("button").attr("class", "recipe_disable_btn");
		$(".recipebtn").find("button").attr("disabled", "disabled");
		$(".stepbtn").find("button").attr("class", "step_disable_btn");
		$(".stepbtn").find("button").attr("disabled", "disabled");
	}
}

function checkSelectRecipe()
{
	if (jQuery(".recipeselect").length > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

function addOneStep()
{
	if (!checkSelectRecipe())
	{
		Dialog.alert("<label style='font-size:14px;'>Please select the recipe firstly.</label>");
		return;
	}

	var num = parseInt(jQuery(".recipevaluebox1").find(".stepitem:last").find("label").text()) + 1;
	if (isNaN(num))
	{
		num = 1;
	}
	var stepstr = '<td class="stepitem">' +
					'<span>' +
						'<input type="checkbox"/><label>' + num + '<label>' +
					'</span>' +
					'<ul>' +
						'<li><input type="text" value=""/></li>' +
						'<li><input type="text" value="0"/></li>' +
						'<li>' +
							'<select>' +
								'<option value="position">position</option>' +
								'<option value="pressure">pressure</option>' +
							'</select>' +
						'</li>' +
						'<li><input type="text" value="0" min="' + range["position"][0] + '" max="' + range["position"][1] + '"/></li>' +
						'<li><input type="text" value="0" min="' + range["pressure"][0] + '" max="' + range["pressure"][1] + '"/></li>' +
						'<li><input type="text" value="0"/></li>' +
						'<li>' +
							'<select>' +
								'<option value="none">none</option>' +
								'<option value="chamber">chamber</option>' +
								'<option value="bypass">bypass</option>' +
							'</select>' +
						'</li>' +
						'<li><input type="text" value="0" min="' + range["HF"][0] + '" max="' + range["HF"][1] + '"/></li>' +
						'<li>' +
							'<select>' +
								'<option value="none">none</option>' +
								'<option value="chamber">chamber</option>' +
								'<option value="bypass">bypass</option>' +
							'</select>' +
						'</li>' +
						'<li><input type="text" value="0" min="' + range["etoh"][0] + '" max="' + range["etoh"][1] + '"/></li>' +
						'<li><input type="text" value="0" min="' + range["N2"][0] + '" max="' + range["N2"][1] + '"/></li>' +
						'<li><input type="text" value="0" min="' + range["N2 Purge"][0] + '" max="' + range["N2 Purge"][1] + '"/></li>' +
					'</ul>' +
				'</td>';
	jQuery(".recipevaluebox").find("table").find("tr").append(stepstr);
	jQuery(".recipeselect").css("color", "#AF0505");
	jQuery(".recipeselect").attr("isSave", "false");
	
	jQuery(".recipevaluebox1").find(".stepitem").children("ul").find("input").change(function() {
		var This = this;
		var value = parseInt($(this).val());
		var min = parseInt($(this).attr("min"));
		var max = parseInt($(this).attr("max"));
		if (!isNaN(min))
		{
			if (value > max || value < min)
			{
				Dialog.alert("<label style='font-size:14px;'>The value range should be [" + min + "," + max + "]</label>", function(){
					if (value > max)
					{
						$(This).val(max);
					}
					else
					{
						$(This).val(min);
					}
				});
			}
		}
		jQuery(".recipeselect").css("color", "#AF0505");
		jQuery(".recipeselect").attr("isSave", "false");
	});
		
	jQuery(".recipevaluebox1").find("select").change(function() {
		jQuery(".recipeselect").css("color", "#AF0505");
		jQuery(".recipeselect").attr("isSave", "false");
	});
}

function deleteStep()
{
	var flag = false;
	jQuery(".recipevaluebox1").find(".stepitem").each(function(){
		if (jQuery(this).find("span").find("input")[0].checked)
		{
			flag = true;
			return false;
		}
	});
	
	if (!flag)
	{
		Dialog.alert("<label style='font-size:14px;'>Please select the step you want to delete</label>");
		return;
	}
	
	Dialog.confirm("<label style='font-size:14px;'>Do you want to delete this Step?</label>", function(){
		jQuery(".recipevaluebox1").find(".stepitem").each(function(){
			if (jQuery(this).find("span").find("input")[0].checked)
			{
				jQuery(this).remove();
			}
		});
		
		var i = 1;
		jQuery(".recipevaluebox1").find(".stepitem").each(function(){
			jQuery(this).find("span").find("label").text(i);
			i = i + 1;
		});
		
		jQuery(".recipeselect").css("color", "#AF0505");
		jQuery(".recipeselect").attr("isSave", "false");
	});
}

function copyStep()
{
	jQuery(".recipevaluebox1").find(".stepitem").each(function(){
		if (jQuery(this).find("span").find("input")[0].checked)
		{
			ClipBoard.push(this);
		}
	});
}

function pasteStep()
{
	var num = parseInt(jQuery(".recipevaluebox1").find(".stepitem:last").find("label").text()) + 1;
	if (isNaN(num))
	{
		num = 1;
	}
	
	var length = ClipBoard.length;
	if (0 != length)
	{
		if (!checkSelectRecipe())
		{
			Dialog.alert("<label style='font-size:14px;'>Please select the recipe firstly.</label>");
			return;
		}
	}

	while (0 != ClipBoard.length)
	{
		var This = ClipBoard.shift().cloneNode(true);
		This = jQuery(This);
		This.find("span").find("label").text(num);
		This.find("span").find("input")[0].checked = false;
		jQuery(".recipevaluebox1").find("tr")[0].appendChild(jQuery(This)[0]);
		num = num + 1;
	}
	
	if (0 != length)
	{
		jQuery(".recipeselect").css("color", "#AF0505");
		jQuery(".recipeselect").attr("isSave", "false");
		
		jQuery(".recipevaluebox1").find(".stepitem").children("ul").find("input").change(function() {
			var This = this;
			var value = parseInt($(this).val());
			var min = parseInt($(this).attr("min"));
			var max = parseInt($(this).attr("max"));
			if (!isNaN(min))
			{
				if (value > max || value < min)
				{
					Dialog.alert("<label style='font-size:14px;'>The value range should be [" + min + "," + max + "]</label>", function(){
						if (value > max)
						{
							$(This).val(max);
						}
						else
						{
							$(This).val(min);
						}
					});
				}
			}
			jQuery(".recipeselect").css("color", "#AF0505");
			jQuery(".recipeselect").attr("isSave", "false");
		});
			
		jQuery(".recipevaluebox1").find("select").change(function() {
			jQuery(".recipeselect").css("color", "#AF0505");
			jQuery(".recipeselect").attr("isSave", "false");
		});
	}
}

function cutStep()
{
	var flag = false;
	jQuery(".recipevaluebox1").find(".stepitem").each(function(){
		if (jQuery(this).find("span").find("input")[0].checked)
		{
			flag = true;
			ClipBoard.push(this);
			jQuery(this).remove();
		}
	});
	
	if (flag)
	{
		jQuery(".recipeselect").css("color", "#AF0505");
		jQuery(".recipeselect").attr("isSave", "false");
	}
}

function loadRecipe()
{
	fs.readdir(recipedir, function(err, files){
		if (err)
		{
			//Dialog.alert("<label style='font-size:14px;'>Read The dir '" + recipedir + "' ERROE:" + err + "</label>");
			return;
		}
		
		var li = "";
		for (var i = 0; i < files.length; ++i)
		{
			var file = files[i];
			var recipeName = file.substring(0, file.lastIndexOf("."));
			li += "<li>" + recipeName + "</li>";
		}
		
		jQuery(".recipelist").append(li);
		jQuery(".recipelist").children("li").click(function(){
			checkUnSavedRecipe(this);
		});
	});
}

function checkUnSavedRecipe(This)
{
	var saveFlag = jQuery(".recipeselect").attr("isSave");
	var rName = jQuery(".recipeselect").text();
	if (saveFlag && saveFlag == "false" && !jQuery(This).hasClass("recipeselect"))
	{
		Dialog.confirm("<label style='font-size:14px;'>The " + rName + " is unsaved, do you want to save it?</label>", function(){
			function hasSaved()
			{
				jQuery(".recipeselect").attr("isSave", "true");
				jQuery(".recipeselect").css("color", "#fff");
				showSelectRecipe(fs, This);
			}
			recipeSave(hasSaved);
		}, function(){
			unSaveRecipeList[rName] = domToJson();
			showSelectRecipe(fs, This);
		});
	}
	else
	{
		showSelectRecipe(fs, This);
	}
}

function showSelectRecipe(fs, This)
{
	jQuery(".recipeselect").removeClass("recipeselect");
	jQuery(This).addClass("recipeselect");
	var rName = jQuery(This).text();
	if (rName == "")
	{
		return;
	}
	
	var rJson = {};
	if (unSaveRecipeList[rName])
	{
		rJson = unSaveRecipeList[rName];
	}
	else
	{
		try
		{
			var rJsonStr = fs.readFileSync(recipedir + '/' + rName + '.rcp');
			if (rJsonStr == "")
			{
				jQuery(".recipevaluebox1").find(".stepitem").remove();
				return;
			}
			/*
			var crypto = require('crypto');
			var algorithm = 'aes-128-ecb';
			var key = 'acm-recipe-acm';
			var clearEncoding = 'utf8';
			var cipherEncoding = 'base64';
			var decipher = crypto.createDecipher(algorithm, key);
			if (typeof rJsonStr != 'string')
			{
				rJsonStr = rJsonStr.toString();
			}
			rJsonStr = decipher.update(rJsonStr, cipherEncoding, clearEncoding) + decipher.final(clearEncoding);
			*/
			rJson = jQuery.parseJSON(rJsonStr);
		}
		catch (e)
		{
			Dialog.alert("<label style='font-size:12px;'>" + e + "</label>");
			return;
		}
	}
			
	if (rJson && typeof rJson == "object")
	{
		jQuery(This).attr("version", rJson["Version"]);
		var steps = rJson["Steps"];
		var steplist = "";
		for (var i = 0; i < steps.length; ++i)
		{
			steplist += '<td class="stepitem">' +
							'<span>' +
								'<input type="checkbox"/><label>' + (i + 1) + '<label>' +
							'</span>' +
							'<ul>' +
								'<li><input type="text" value="' + steps[i].Name + '"/></li>' +
								'<li><input type="text" value="' + steps[i].Time + '"/></li>' +
								'<li>';
			if (steps[i]["APC Mode"] == "position")
			{
				steplist += '<select>' +
								'<option value="position" selected = "selected">position</option>' +
								'<option value="pressure">pressure</option>' +
							'</select>';
			}
			else
			{
				steplist += '<select>' +
								'<option value="position">position</option>' +
								'<option value="pressure" selected = "selected">pressure</option>' +
							'</select>';
			}
					
			steplist += '</li>' +
						'<li><input type="text" value="' + steps[i].Position + '" min="' + range["position"][0] + '" max="' + range["position"][1] + '"/></li>' +
						'<li><input type="text" value="' + steps[i].Pressure + '" min="' + range["pressure"][0] + '" max="' + range["pressure"][1] + '"/></li>' +
						'<li><input type="text" value="' + steps[i]["Rotate Speed"] + '"/></li>' +
						'<li>';
			var select1 = "";
			var select2 = "";
			var select3 = "";
			switch (steps[i]["HF Bypass"])
			{
				case 'none':
					select1 = 'selected = "selected"';
					break;
				case 'chamber':
					select2 = 'selected = "selected"';
					break;
				case 'bypass':
					select3 = 'selected = "selected"';
					break;
				default:
			}
			
			steplist += '<select>' +
							'<option value="none" ' + select1 + '>none</option>' +
							'<option value="chamber" ' + select2 + '>chamber</option>' +
							'<option value="bypass" ' + select3 + '>bypass</option>' +
						'</select>' + 
					'</li>' +
					'<li><input type="text" value="' + steps[i]["HF"] + '" min="' + range["HF"][0] + '" max="' + range["HF"][1] + '"/></li>';
			
			select1 = "";
			select2 = "";
			select3 = "";
			switch (steps[i]["EtOH Bypass"])
			{
				case 'none':
					select1 = 'selected = "selected"';
					break;
				case 'chamber':
					select2 = 'selected = "selected"';
					break;
				case 'bypass':
					select3 = 'selected = "selected"';
					break;
				default:
			}
			
			steplist += '<li>' +
							'<select>' +
								'<option value="none" ' + select1 + '>none</option>' +
								'<option value="chamber" ' + select2 + '>chamber</option>' +
								'<option value="bypass" ' + select3 + '>bypass</option>' +
							'</select>' +
						'</li>' +
						'<li><input type="text" value="' + steps[i]["EtOH"] + '" min="' + range["etoh"][0] + '" max="' + range["etoh"][1] + '"/></li>' +
						'<li><input type="text" value="' + steps[i]["N2"] + '" min="' + range["N2"][0] + '" max="' + range["N2"][1] + '"/></li>' +
						'<li><input type="text" value="' + steps[i]["N2 Purge"] + '" min="' + range["N2 Purge"][0] + '" max="' + range["N2 Purge"][1] + '"/></li>' +
						'</ul>' +
						'</td>';
		}
				
		jQuery(".recipevaluebox1").find(".stepitem").remove();
		jQuery(".recipevaluebox1").find("tr").append(steplist);
		
		jQuery(".recipevaluebox1").find(".stepitem").children("ul").find("input").change(function() {
			var This = this;
			var value = parseInt($(this).val());
			var min = parseInt($(this).attr("min"));
			var max = parseInt($(this).attr("max"));
			if (!isNaN(min))
			{
				if (value > max || value < min)
				{
					Dialog.alert("<label style='font-size:14px;'>The value range should be [" + min + "," + max + "]</label>", function(){
						if (value > max)
						{
							$(This).val(max);
						}
						else
						{
							$(This).val(min);
						}
					});
				}
			}
			
			jQuery(".recipeselect").css("color", "#AF0505");
			jQuery(".recipeselect").attr("isSave", "false");
		});
		
		jQuery(".recipevaluebox1").find("select").change(function() {
			jQuery(".recipeselect").css("color", "#AF0505");
			jQuery(".recipeselect").attr("isSave", "false");
		});
		permissionCheck();
	}
}

function domToJson()
{
	var rJson = {};
	var This = jQuery(".recipeselect");
	var version = This.attr("Version");
	
	rJson["Version"] = version ? version : "1.0";
	rJson["Steps"] = [];
	
	jQuery(".recipevaluebox1").find(".stepitem").each(function(){
		var stepObj = {};
		var lis = jQuery(this).children("ul").children("li");
		stepObj["Name"] = jQuery(lis[0]).children("input").val();
		stepObj["Time"] = jQuery(lis[1]).children("input").val();
		stepObj["APC Mode"] = jQuery(lis[2]).children("select").val();
		stepObj["Position"] = jQuery(lis[3]).children("input").val();
		stepObj["Pressure"] = jQuery(lis[4]).children("input").val();
		stepObj["Rotate Speed"] = jQuery(lis[5]).children("input").val();
		stepObj["HF Bypass"] = jQuery(lis[6]).children("select").val();
		stepObj["HF"] = jQuery(lis[7]).children("input").val();
		stepObj["EtOH Bypass"] = jQuery(lis[8]).children("select").val();
		stepObj["EtOH"] = jQuery(lis[9]).children("input").val();
		stepObj["N2"] = jQuery(lis[10]).children("input").val();
		stepObj["N2 Purge"] = jQuery(lis[11]).children("input").val();
		rJson["Steps"].push(stepObj);
	});
	
	return rJson;
}

function dirCheckAndMK()
{
	var flag = fs.existsSync(recipedir);
	if (!flag)
	{
		fs.mkdirSync(recipedir);
	}
}

function recipeSave(fn)
{
	dirCheckAndMK();
	var rJson = domToJson();
	var This = jQuery(".recipeselect");
	var rName = This.text();

	if (rName)
	{
		/*
		var crypto = require('crypto');
		var algorithm = 'aes-128-ecb';
		var key = 'acm-recipe-acm';
		var clearEncoding = 'utf8';
		var cipherEncoding = 'base64';
		var cipher = crypto.createCipher(algorithm, key);
		var cipherStr = cipher.update(JSON.stringify(rJson), clearEncoding, cipherEncoding) + cipher.final(cipherEncoding);
		*/
		fs.writeFile(recipedir + '/' + rName + '.rcp', JSON.stringify(rJson), function(err){
			if (err)
			{
				Dialog.alert("<label style='font-size:14px;'>The " + rName + " save failed£¡</label>");
			}
			else
			{
				/*
				Dialog.alert("<label style='font-size:14px;'>The " + rName + " save successfully</label>", function(){
					delete unSaveRecipeList[rName];
					if (fn)
					{
						fn();
					}
					else
					{
						jQuery(".recipeselect").attr("isSave", "true");
						jQuery(".recipeselect").css("color", "#fff");
					}
				});
				*/
				delete unSaveRecipeList[rName];
				jQuery(".messagealert").text("The " + rName + " save successfully");
				setTimeout("jQuery('.messagealert').slideUp(300)",3000);
				jQuery(".messagealert").slideDown(300);
				if (fn)
				{
					fn();
				}
				else
				{
					jQuery(".recipeselect").attr("isSave", "true");
					jQuery(".recipeselect").css("color", "#fff");
				}
				
			}
		});
	}
}

function recipeDelete()
{
	if (!fs.existsSync(recipedir))
	{
		jQuery(".recipevaluebox1").find(".stepitem").remove();
		jQuery(".recipeselect").remove();
		return;
	}
	
	var rselect = jQuery(".recipeselect");
	if (rselect.length != 0)
	{
		Dialog.confirm("<label style='font-size:14px;'>Do you want to delete this Recipe?</label>", function(){
			var rName = jQuery(".recipeselect").text();
			fs.unlink(recipedir + '/' + rName + ".rcp", function(err){
				jQuery(".recipevaluebox1").find("table").find("tr").children().remove();
				jQuery(".recipeselect").remove();
			});
		});
	}
}

function recipeAdd()
{
	var lis = jQuery(".recipelist").children("li");
	var rName = "NewRecipe";
	for (var i = 1; 1; ++i)
	{
		var j = 0;
		for (; j < lis.length; ++j)
		{
			if (rName == jQuery(lis[j]).text())
			{
				break;
			}
		}
		
		if (j == lis.length)
		{
			break;
		}
		
		rName = "NewRecipe";
		rName += i;
	}
	var li = "<li><input class='rnameedit' style='padding-left:5px;width:80%;background-color:#C6F0FB;border:none;' value='" + rName + "'/></li>";
	
	jQuery(".recipelist").append(li);
	jQuery(".rnameedit").focus();
	jQuery(".rnameedit").blur(function(){
		var This = this;
		rName = jQuery(this).val();
		dirCheckAndMK();
		if (rName == "")
		{
			//rName = $(this).attr("value");
			//Dialog.alert("<label style='font-size:14px;'>The Recipe Name can not be empty");
			$(this).parent().remove();
			return;
		}
		
		for (var i = 0; i < lis.length; ++i)
		{
			if (rName == jQuery(lis[i]).text())
			{
				Dialog.alert("<label style='font-size:14px;'>The " + rName + " has existed", function(){
					$(This).focus();
				});
				return;
			}
		}
		
		fs.writeFileSync(recipedir + '/' + rName + ".rcp", "");
		var li = jQuery(this).parent();
		jQuery(this).parent().text(rName);
		jQuery(".recipeselect").removeClass("recipeselect");
		li.addClass("recipeselect");
		li.click(function(){
			showSelectRecipe(fs, this);
		});
		jQuery(".recipevaluebox1").find(".stepitem").remove();
		jQuery(this).remove();
	});
}

function recipeSaveAs()
{
	var rselect = jQuery(".recipeselect");
	var orName = rselect.text();
	var input = "<input class='rnameedit' style='padding-left:5px;width:80%;background-color:#C6F0FB;border:none;' value='" + orName + "'/>";
	rselect.text("");
	rselect.append(input);
	jQuery(".rnameedit").focus();
	jQuery(".rnameedit").blur(function(){
		var lis = jQuery(".recipelist").children("li");
		var This = this;
		var nrName = jQuery(this).val();
		dirCheckAndMK();
		if (nrName == "")
		{
			Dialog.alert("<label style='font-size:14px;'>The Recipe Name can not be empty");
			$(This).val(orName);
			$(This).focus();
			return;
		}
		
		for (var i = 0; i < lis.length - 1; ++i)
		{
			if (nrName == jQuery(lis[i]).text())
			{
				Dialog.alert("<label style='font-size:14px;'>The " + nrName + " has existed", function(){
					$(This).focus();
				});
				return;
			}
		}
		fs.renameSync(recipedir + '/' + orName + ".rcp", recipedir + '/' + nrName + ".rcp");
		jQuery(".recipeselect").removeClass("recipeselect");
		rselect.addClass("recipeselect");
		rselect.text(nrName);
		jQuery(this).remove();
	});
}

/*
function checkRecipeSave(){
	var str = "";
	jQuery(".recipelist").children("li").each(function(){
		var flag = jQuery(this).attr("isSave");
		if (flag && flag == "false")
		{
			str += jQuery(this).text() + ",";
		}
	});
	
	if (str != "")
	{
		Dialog.confirm("The " + str + "  haven't saved, do you want save its?", function(){
			
		});
	}
}*/