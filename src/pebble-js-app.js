Pebble.addEventListener('showConfiguration', function(e) {
  var color = localStorage.getItem('color');
  if (! color)
  {
      // http://developer.getpebble.com/tools/color-picker/#0000FF
      color = "0000FF";  // GColorBlue
  }
  var vibrate_disconnect_str = 'off';
  var vibrate_disconnect = localStorage.getItem('vibrate_disconnect');
  if (! vibrate_disconnect)
  {
      console.log('!vibrate_disconnect: ');
      vibrate_disconnect = 0;
  }
  if (vibrate_disconnect == 1)
  {
      vibrate_disconnect_str = 'on';
  }
  else
  {
      vibrate_disconnect_str = 'off';
  }

  //var URL = 'http://clach04.github.io/pebble/JupiterMass/nojquery_pebble-config.html' +
  //var URL = 'http://clach04.github.io/pebble/JupiterMass/pebble-config.html' +
  var URL = 'http://clach04.github.io/pebble/JupiterMass/slate/index.html' +
      '?' +
      'color=' + color + '&' +
      'vibrate_disconnect=' + vibrate_disconnect;
  console.log('Configuration window opened. ' + URL);
  Pebble.openURL(URL);
});

Pebble.addEventListener('webviewclosed',
    function(e) {
        console.log('e.response: ' + e.response);
        console.log('e.response.length: ' + e.response.length);
        try {
            var configuration = JSON.parse(decodeURIComponent(e.response));
            var vibrate_disconnect = 0;

            console.log('dictionary to validate ' + JSON.stringify(configuration));

            if ('vibrate_disconnect' in configuration)
            {
                switch (configuration.vibrate_disconnect) {
                    case true:
                    case 'true':
                    case 'True':
                    case 'TRUE':
                    case 1:
                    case '1':
                    case 'on':
                        vibrate_disconnect = 1;
                        break;
                    default:
                        vibrate_disconnect = 0;
                        break;
                }
            }
            var dictionary = {
              "KEY_TIME_COLOR": parseInt(configuration.color, 16),
              "KEY_VIBRATE_ON_DISCONNECT": vibrate_disconnect
            };
            console.log('Color ' + configuration.color);
            localStorage.setItem('color', configuration.color);
            console.log('vibrate_disconnect ' + configuration.vibrate_disconnect);
            localStorage.setItem('vibrate_disconnect', configuration.vibrate_disconnect);
            console.log('dictionary to send ' + JSON.stringify(dictionary));
            // Send to Pebble
            Pebble.sendAppMessage(dictionary,
                function(e) {
                    console.log("Configuration sent to Pebble successfully!");
                },
                function(e) {
                    console.log("Error sending configuration info to Pebble!");
                }
            );
        } catch (ex) {
            // If we have SyntaxError JSON is invalid, anything unknown!?
            if (ex instanceof SyntaxError) {
                console.log('Probably Cancelled');
            } else {
                throw ex;
                // See https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SyntaxError
            }
        }
    }
);
