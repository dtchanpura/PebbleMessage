var message = localStorage.getItem("dtchanpura.github.io.message");

if (!message) {
ipServer = 'Hello World!';
}
console.log("Message at top is "+ message);
//var base_url = "http://"+ipServer+":"+portServer;
Pebble.addEventListener("ready",
function(e) {
sendMessage();
}
);
Pebble.addEventListener("appmessage",
function(e) {

sendMessage();
});

function sendMessage() {

var dict = {"KEY_MESSAGE" : message};

    //Send data to watch for display
    Pebble.sendAppMessage(dict);
}

Pebble.addEventListener("showConfiguration", function() {
Pebble.openURL('http://dtchanpura.github.io/PebbleConfigs/MessageConfig/');
});
Pebble.addEventListener("webviewclosed", function(e) {
console.log("configuration closed");
// webview closed
var options = JSON.parse(decodeURIComponent(e.response));
message = options.message;
console.log("Message is "+message)//portServer = options.port;
localStorage.setItem("dtchanpura.github.io.message", message);
//localStorage.setItem("ar.com.sergioaraki.remote.port", portServer);

});
