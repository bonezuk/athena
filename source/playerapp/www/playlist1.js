const playListElement = document.getElementById('play-list');

loadPlaylist();

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

function createPlaylistElement(value, index, array)
{
	const listItem = document.createElement('li');
	listItem.setAttribute('data-index', index);
	listItem.innerHTML = `
			<span class="number">${index + 1}</span>
			<div class="play-list">
				<p class="track-name">${value.title}</p>
			</div>
		`;
	playListElement.appendChild(listItem);
}
