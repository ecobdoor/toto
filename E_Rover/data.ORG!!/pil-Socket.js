SocketCLI = function(NAME, WS_URL, Q_TYP, SLOT) {
	var SELF = this;
	this._name = NAME;
	this.cntWScnx = 0;
	this.cntErrors = 0;
	this.slot = SLOT;
	var WS;//CONNECT() will pass by reference to the socket slot !!!
	this.PROTOCOL = Q_TYP;
	this._status = function(CHN) {
		return "id(" + SELF.cntWScnx
			+ "): " + SELF.cntErrors
			+ " errors \"" + CHN + "\"";
	}
	this.Status = function(CHN) {
		SELF.slot.affStatus(SELF._status(CHN));
	}
	this.stop = function(CHN) {
		SELF.slot.stopSender(SELF._status(CHN));
	}
	this.addMsg = function(HEAD, CHN, FOOT) {
		SELF.slot.addMsg(HEAD, CHN, FOOT);
	}
	//
	// WEBSOCKET CLIENT CONNEXION 
	//
	this.CONNECT = function() {
		SELF.Status("Connect");
		SELF.cntWScnx++;
		SELF.WS = new WebSocket(WS_URL, Q_TYP);
		SELF.Status("Create");
		/*
		 * ==== ON OPEN ====
		 */
		SELF.WS.onopen = function() {
			SELF.Status("OPEN");
		};
		/*
		 * ==== ON MESSAGE ====
		 */
		SELF.WS.onmessage = function(evt) {
			//SELF.addMsg("<-rover",evt.data);
			var rtn = ON_MESSAGE(SELF, (new Date()).getTime(), evt.data);
			//SELF.addMsg("<-rover",evt.data);
			if (rtn.TYP == "?") {
				SELF.addMsg("<-rover", "Parsing error [" + evt.data + "]");
			} else {
				var rep = rtn.Rsignals;
				SELF.addMsg(rep.SNS, rtn.RJsonchn, rtn.InfoRchn);
				if (rtn.TYP == "Q") {
					var que = rtn.Qsignals;
					SELF.addMsg(que.SNS, rtn.QJsonchn, rtn.InfoQchn);
				}
			}
		};
		//
		// Client ON_CLOSE SOCKET
		//
		SELF.WS.onclose = function(event) {
			SELF.Status("Close");
			SELF.stop("CLOSE(" + event.code + ",'" + event.reason + "',"
				+ event.wasClean + ")");
			setTimeout(function() {
				SELF.CONNECT();
			}, 100);
		};
		//
		// Client ON_ERROR SOCKET
		//
		SELF.WS.onerror = function(event) {
			SELF.stop('ERROR');
			SELF.WS.close();
		};
	}
};
