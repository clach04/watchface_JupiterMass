Pebble.addEventListener('showConfiguration', function(e) {
  var color = localStorage.getItem('color');
  if (! color)
  {
      // http://developer.getpebble.com/tools/color-picker/#0000FF
      color = "0000FF";  // GColorBlue
  }
  var URL = 'http://clach04.github.io/pebble/JupiterMass/pebble-config.html?' +
      'color=' + color;
  console.log('Configuration window opened. ' + URL);
  Pebble.openURL(URL);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var dictionary = {
      "KEY_TIME_COLOR": parseInt(configuration.color, 16)
    };
    console.log('Color ' + configuration.color);
    localStorage.setItem('color', configuration.color);
    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Configuration sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending configuration info to Pebble!");
      }
    );
  }
);
