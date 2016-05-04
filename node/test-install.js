
var libclientserver = require("/usr/lib/node_modules/libclientserver");

var Cli = new libclientserver.Client("/tmp/SimpleServer");
Cli.Connect();



console.log("Started!");
