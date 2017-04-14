const call = new XMLHttpRequest();
const file = "http://data.sparkfun.com/output/3JynOpWyqrHnrvjNdzrm.json";
let output = "";
let t_cont = "";
let date;
let sum_c = 0;
let sum_f = 0;
let temp_c = 0;
let temp_f = 0;
let hours = 0;
let minutes = 0;
let seconds = 0;

call.onreadystatechange = function()
{
	if (this.readyState != 4 || this.status != 200) {
		return;
	}
	const data = JSON.parse(this.responseText);
	for(let i = 0; i < data.length; i++)
	{
		date = new Date(data[i].timestamp);
		hours = ("0" + date.getHours()).slice(-2);
		minutes = ("0" + date.getMinutes()).slice(-2);
		seconds = ("0" + date.getSeconds()).slice(-2);
		t_cont += '<div class="line"><div class="t"><p><img src="img/time.png" />' + hours + ":" + minutes + ":" + seconds + '</p></div><div class="temp"><img src="img/temp.png"/><span class="c">' + data[i].temp_c + ' °C</span><span class="f">' + data[i].temp_f + ' °F</span></div></div>';
		sum_c += Number(data[i].temp_c);
		sum_f += Number(data[i].temp_f);
	}
	let avg_c = Math.round(sum_c / (data.length));
	let avg_f = Math.round(sum_f / (data.length));
	let t_avg = '<p id="c">' + avg_c.toFixed(2) + ' °C</p><p id="f">' + avg_f.toFixed(2) + ' °F</p></div>';
	output = "<div id='a'><h2>Average</h2>" + t_avg + "</div>" + t_cont;
	document.getElementById("diiiv").innerHTML = output;
};
call.open("GET", file, true);
call.send();
