var net = require("net");

function Client(path)
{
	this.ID = 1;
	this.path = path;
	this.Sock = new net.Socket();
	this.IsConn = false;
	this.RequestMap = {};
	var self = this;
    
	this.Sock.on('connect', function(e) {
		
		console.log("OnConnect");
		self.IsConn = true;
	});
  
	this.Sock.on('error', function(e) {
		console.log("Error: " + e);
		self.IsConn = false;
		if (this.Sock != undefined)
		{
			this.Sock.destroy();
		}
		//FIXME: Fail all current requests
	});

	this.Sock.on('data', function(data) {
		try
		{
			var str = data.toString();
			var args = JSON.parse(str.substring(13));
			if (args["_ID"] != undefined)
			{
				var ID = args["_ID"];
				if (self.RequestMap[ID] != undefined)
				{
					self.RequestMap[ID](args);
					delete self.RequestMap[ID];
				}
				else
				{
					console.log("Missing _ID in repsonse" + str);
				}
			}
			else
			{
				console.log("Missing callback");
			}
		}
		catch(err)
		{
			console.log("Failed to Parse: " + data);
			console.log("Error: " + err);
		}
	});

	this.Sock.on('close', function(Sock) {
		console.log("Closed .. Reconnecting");
		self.IsConn = false;
		if (this.Sock != undefined)
		{
			this.Sock.destroy();
		}
		setTimeout(function() {
			self.Connect(path);
		}, 1000);
	});
};

Client.prototype.Connect = function() {
	if (this.IsConn == true)
	{
		return;
	}
	console.log("Trying to connect to " + this.path);
	var self = this;
	this.Sock.connect( { "path" : this.path}, function(Sock) {});
};

Client.prototype.Disconnect = function()
{
	if (this.IsConn == false)
	{
		return;
	}
};

Client.prototype.IsConnected = function()
{
	return this.IsConn;
};

Client.prototype.SendRequest = function(args, callback)
{
	var self = this;
	if (this.IsConn == false)
	{
		console.log("SendRequest::Not Connected");
		//Send Error now
		console.log("Not Connected");
		return;
	}

	this.ID++;
	var ID = self.ID;
	args["_ID"] = ID;
	var str = "JSONREQUEST " + JSON.stringify(args) + "\n";
	self.RequestMap[ID] = callback;
	setTimeout(function() {
		if (self.RequestMap[ID] != undefined)
		{
			console.log("Timeout for Call " + ID);
			var data = { "_ERROR" : "Timeout" };
			self.RequestMap[ID](data);
			delete self.RequestMap[ID];
			console.log(self.RequestMap);
		}
	}, 10000);
	self.Sock.write(str);
};

Client.prototype.SendCommand = function(args)
{
	if (this.IsConn == false)
	{
		console.log("SendCommand::Not Connected");
		return;
	}

	var str = "JSONCOMMAND " + JSON.stringify(args) + "\n";
	this.Sock.write(str);
};

var Cli = new Client("/tmp/SimpleServer");
Cli.Connect();

function Test2(data)
{
	if (data["_ID"] == undefined)
	{
		console.log("Sending First Request");
	}
	else
	{
		console.log("Request " + data["_ID"] + " Complete Sending Another Command");
	}
	var args = { "Test" : "Yes" };
	args["action"] = "PING";
	Cli.SendRequest(args, Test2);
}

function Test()
{
	console.log("Sending Command");
	var args = { "Test" : "Yes" };
	args["action"] = "PING";
	//Cli.SendCommand(args);
	Cli.SendRequest(args, function(data) {
		console.log("Request Complete:" + JSON.stringify(data));
	});
	setTimeout(Test, 500);
}

function Run()
{
	console.log("Run");
	console.log("Connected: " + Cli.IsConnected());
	//console.log(Cli);
	if (Cli.IsConnected() == true)
	{
		console.log("Really Connected");
//		Test();
		Test2({});
	}
	else
	{
		setTimeout(Run, 1000);
	}
}

setTimeout(Run, 1000);

console.log("Started!");



