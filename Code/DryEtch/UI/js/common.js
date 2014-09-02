$(document).keydown(function (e) { 
    var keyEvent; 
    if (e.keyCode == 8)
    { 
        var d = e.srcElement || e.target; 
        if (d.tagName.toUpperCase() == 'INPUT'|| d.tagName.toUpperCase() == 'TEXTAREA')
        { 
            keyEvent = d.readOnly || d.disabled; 
        }
        else
        { 
            keyEvent = true; 
        } 
    }
    else
    { 
        keyEvent = false; 
    } 

    if (keyEvent)
    { 
        e.preventDefault(); 
    } 
}); 

function getSqlConnection()
{
	var fs = require("fs");
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

	return connection;
}

function getControl() {
    return top.control;
}

function getJsonData(obj) {
    var fs = require('fs');
    fs.readFile('./../data/parameters.json', function (err, data) {
        alert(data);
        obj.Data = $.parseJSON(data);
    });
}

function setPropertyChange(func) {
    setInterval(function () {
        var data = getSyncJsonData();
        func(data);
    },500);
}

        function getSyncJsonData() {
            var fs = require('fs');
           return $.parseJSON(fs.readFileSync('./../data/parameters.json'));
       }

       function getSyncXmlData(path) {
           var fs = require('fs');
           var str = fs.readFileSync(path);
           var parser = new DOMParser();
           return parser.parseFromString(str, "text/xml");
       }



       //去掉左边的空白 
       function trimLeft(s) {
           if (s == null) {
               return "";
           }
           var whitespace = new String(" \t\n\r");
           var str = new String(s);
           if (whitespace.indexOf(str.charAt(0)) != -1) {
               var j = 0, i = str.length;
               while (j < i && whitespace.indexOf(str.charAt(j)) != -1) {
                   j++;
               }
               str = str.substring(j, i);
           }
           return str;
       }
       //去掉右边的空白 
       function trimRight(s) {
           if (s == null) return "";
           var whitespace = new String(" \t\n\r");
           var str = new String(s);
           if (whitespace.indexOf(str.charAt(str.length - 1)) != -1) {
               var i = str.length - 1;
               while (i >= 0 && whitespace.indexOf(str.charAt(i)) != -1) {
                   i--;
               }
               str = str.substring(0, i + 1);
           }
           return str;
       }


       function fetchSystemDataByRange(from, to, flag) {
           var json = getControl().fetch_system_data(parseInt(from), parseInt(to), flag);
//           console.log("flag:" + flag + ",json:" + json);
           return $.parseJSON(json);
       }

       function modifySystemData(id,value) {
           getControl().modify_system_data(parseInt(id), value);
           console.log("modifySystemData id:" + id + ",value:" + value);
       }

       function modifyParameter(id, value) {
           getControl().modify_parameter(parseInt(id), value);
           console.log("modifyParameter id:" + id + ",value:" + value);
       }

       function fetchParametersByids(value) {
           var arr = new Array();
           var ids = value.split(",");
           for (index in ids) {
               arr.push(parseInt(ids[index]));
           }
           var json = getControl().fetch_parameters(arr);
           console.log("fetchParametersByids:" + json);
           return $.parseJSON(json);
       }

       function fetchParametersByrange(from,to) {
           var json = getControl().fetch_parameters(parseInt(from), parseInt(to));
           console.log("fetchParametersByrange:" + json);
           return $.parseJSON(json);
       }

       function fetchSystemDatabyids(arr,flag) {
           var json = getControl().fetch_system_data(arr, flag);
           if (!flag) {
//               console.log("flag:" + flag + ",json:" + json);
           }
//           
           return $.parseJSON(json);
       }

       function writeLog(type, logmsg) {
           var log = { type: type, msg: logmsg, time: getTime() };
		   var fs = require("fs");
           fs.appendFile('./log/UILog.log', formatLogMsg(log), 'utf8', function (err) {
               if (err) {
                   console.log(err);
               }
           });
         
       }

       function formatLogMsg(log) {
           return [log.time, log.type, log.msg] + "\n";
       }

       function getTime() {
           var t = new Date();
           return [t.getFullYear(), '-', add0(t.getMonth() + 1), '-', add0(t.getDate()), ' ',
                add0(t.getHours()), ':', add0(t.getMinutes()), ':', add0(t.getSeconds())].join('');
       }

       function add0(num) {
           return num > 9 ? num : '0' + num;
       }  