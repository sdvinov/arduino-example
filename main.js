var call = new XMLHttpRequest();
var file = "http://data.sparkfun.com/output/3JynOpWyqrHnrvjNdzrm.json";
var output = "";
var t_cont = "";
var date;
var sum_c = 0;
var sum_f = 0;
var temp_c = 0;
var temp_f = 0;
var hours = 0;
var minutes = 0;
var seconds = 0;

call.onreadystatechange = function() 
{
	if (this.readyState != 4 || this.status != 200) {
		return;
	}
	var stuff = JSON.parse(this.responseText);
	for(var i = 0; i < stuff.length; i++) 
	{
		date = new Date(stuff[i].timestamp);
		hours = ("0" + date.getHours()).slice(-2);
		minutes = ("0" + date.getMinutes()).slice(-2);
		seconds = ("0" + date.getSeconds()).slice(-2);
		t_cont += '<tr><td id="c">' + stuff[i].temp_c + 'C</td><td id="f">' + stuff[i].temp_f + 'F</td><td id="d">' + hours + ":" + minutes + ":" + seconds + '</td></tr>';
		sum_c += Number(stuff[i].temp_c);
		sum_f += Number(stuff[i].temp_f);
	}
	var avg_c = Math.round(sum_c / (stuff.length));
	var avg_f = Math.round(sum_f / (stuff.length));
	var t_avg = '<tr id="a"><td>Average ' + avg_c.toFixed(2) + 'C</td><td>Average ' + avg_f.toFixed(2) + 'F</td><td></td></tr>';
	output = "<table>" + t_avg + t_cont + "</table>";
	document.getElementById("diiiv").innerHTML = output;
};
call.open("GET", file, true);
call.send();