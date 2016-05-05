var net = require("net");

var exports = module.exports = {};

exports.Client = function (path)
{
	this.ID = 0;
	this.path = path;
	this.Sock = new net.Socket();
	this.IsConn = false;
	this.RequestMap = {};
	this.ReadBuffer = "";
	var self = this;
	
	this.Connect = function() {
		if (this.IsConn == true)
		{
			return;
		}
		console.log("Trying to connect to " + this.path);
		var self = this;
		this.Sock.connect( { "path" : this.path}, function(Sock) {});
	};
	
	
	this.Disconnect = function()
	{
		if (this.IsConn == false)
		{
			return;
		}
	};

	this.IsConnected = function()
	{
		return this.IsConn;
	};

	this.SendRequest = function(args, callback)
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
		var str = "REQUEST " + JSON.stringify(args) + "\n";
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

	this.SendCommand = function(args)
	{
		if (this.IsConn == false)
		{
			console.log("SendCommand::Not Connected");
			return;
		}

		var str = "COMMAND " + JSON.stringify(args) + "\n";
		this.Sock.write(str);
	};

    
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
			self.ReadBuffer += data.toString();
			
			//Process incoming data
			var newline = self.ReadBuffer.indexOf('\n');
			while(newline > 0)
			{
				var line = self.ReadBuffer.substring(0, newline);
				self.ReadBuffer = self.ReadBuffer.substring(newline + 1);
				if (line.substring(0, 9) === "RESPONSE ")
				{
					var args = JSON.parse(line.substring(9));
					if (args["_ID"] != undefined)
					{
						var ID = args["_ID"];
						var error = "";
						
						if (args["_ERROR"] != undefined)
						{
							if (args["_ERROR"] != "Success")
							{
								error = args["_ERROR"];
							}
						}
						
						if (args["_EXCEPTION"] != undefined)
						{
							error = args["_EXCEPTION"];
						}
						
						if (self.RequestMap[ID] != undefined)
						{
							self.RequestMap[ID](args, error);
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
				else if (line.substring(0, 6) === "EVENT ")
				{
					var args = JSON.parse(line.substring(6));
					//FIXME: Raise event
				}
				else
				{
					console.log("Unknown Response: " + line);
					console.log('"' + line.substring(0, 9) + '"');
				}
				
				newline = self.ReadBuffer.indexOf('\n');
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





