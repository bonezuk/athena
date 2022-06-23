function urlRelPath(path)
{
	return "../" + path;
}

function playButtonImageSource(isEnabled, isPause, isPressed, isHovering)
{
	let imageName = "images/pc_";
	imageName += (!isPause || !isEnabled) ? "play" : "pause";
	imageName += "_";
	if(!isEnabled)
	{
		imageName += "disabled";
	}
	else if(isPressed)
	{
		imageName += "press";
	}
	else
	{
		imageName += (isHovering) ? "hover" : "normal";
	}
	imageName += ".png";
	console.log(isEnabled + " " + isPause + " " +  isPressed + " " +  isHovering  + " " + imageName);
	return urlRelPath(imageName);
}

//-------------------------------------------------------------------------------------------
// PlayTime enums
//-------------------------------------------------------------------------------------------

const PlayTimeHourUpperDigit = "PlayTimeHourUpperDigit";
const PlayTimeHourLowerDigit = "PlayTimeHourLowerDigit";
const PlayTimeMinuteUpperDigit = "PlayTimeMinuteUpperDigit";
const PlayTimeMinuteLowerDigit = "PlayTimeMinuteLowerDigit";
const PlayTimeSecondUpperDigit = "PlayTimeSecondUpperDigit";
const PlayTimeSecondLowerDigit = "PlayTimeSecondLowerDigit";

//-------------------------------------------------------------------------------------------

function getPlayTimeDigitImageName(digitType, timeInSeconds)
{
	let imageName = "";
	let tS = -1;
	
    if(digitType === PlayTimeHourUpperDigit || digitType === PlayTimeHourLowerDigit)
	{
		tS = parseInt(timeInSeconds / (60 * 60));
        if(digitType === PlayTimeHourLowerDigit)
		{
			tS = tS % 10;
		}
		else
		{
			tS = parseInt(tS / 10) % 100;
		}
	}
    else if(digitType === PlayTimeMinuteUpperDigit || digitType === PlayTimeMinuteLowerDigit)
	{
		tS = parseInt(timeInSeconds / 60) % 60;
        if(digitType === PlayTimeMinuteLowerDigit)
		{
			tS = tS % 10;
		}
		else
		{
			tS = parseInt(tS / 10);
		}	
	}
    else if(digitType === PlayTimeSecondUpperDigit || digitType === PlayTimeSecondLowerDigit)
	{
		tS = parseInt(timeInSeconds % 60);
        if(digitType === PlayTimeSecondLowerDigit)
		{
			tS = tS % 10;
		}
		else
		{
			tS = parseInt(tS / 10);
		}		
	}
	
	if(tS >= 0.0)
	{
        imageName = urlRelPath("images/digit_" + tS + ".png");
	}
	return imageName;
}

//-------------------------------------------------------------------------------------------
