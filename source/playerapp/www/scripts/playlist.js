//-------------------------------------------------------------------------------------------
// Global
//-------------------------------------------------------------------------------------------

const playListElement = document.getElementById('pl-list');

//-------------------------------------------------------------------------------------------
// init page
//-------------------------------------------------------------------------------------------

loadPlaylist();

//-------------------------------------------------------------------------------------------

function loadPlaylist()
{
	var playlistURL = window.location.protocol + "//" + window.location.host + "/playlist";

	getJSON(playlistURL, function(err, data) {
		if(err === null) 
		{
			data.forEach(createPlaylistElement);
		}
	});
}

//-------------------------------------------------------------------------------------------

function getJSON(url, callback)
{
	var xhr = new XMLHttpRequest();
	xhr.open('GET', url, true);
	xhr.responseType = 'json';
	xhr.onload = function() {
		var status = xhr.status;
		if (status === 200) 
		{
			callback(null, xhr.response);
		} 
		else 
		{
			callback(status, xhr.response);
		}
	};
	xhr.send();	
}

//-------------------------------------------------------------------------------------------

function createPlaylistElement(value, index, array)
{
	const listItem = document.createElement('li');
	listItem.setAttribute('data-index', index);
	listItem.innerHTML = generatePlaylistHTMLFromJSON(index, value);
	playListElement.appendChild(listItem);
}

//-------------------------------------------------------------------------------------------

function generatePlaylistHTMLFromJSON(index, value)
{
	let idx = index + 1;
	let h = '<div class="pl-item-index">' + idx.toString() + '</div>';
	h += '<div class="pl-item-name">' + value.title + '</div>';
	h += '<div class="pl-item-time">' + timeStampAsString(value.length) + '</div>';
	return h;
}

//-------------------------------------------------------------------------------------------

function timeStampAsString(timeValue)
{
	let sec = parseInt(timeValue) % 60;
	let min = parseInt(timeValue / 60);
	let hour = parseInt(min / 60);
	let tS = "";
	
	if(hour > 0)
	{
		tS += hour.toString();
		tS += ":";
		if(min < 10)
		{
			tS += "0";
		}
		tS += min.toString();
	}
	else
	{
		tS += min.toString();
	}
	
	tS += ":";
	if(sec < 10)
	{
		tS += "0";
	}
	tS += sec.toString();
	
	return tS;
}

//-------------------------------------------------------------------------------------------
