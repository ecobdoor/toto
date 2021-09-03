// ===================================================================
MODULES = new Array(); // Array of modules indexed by MID (module ID)
var eltQUERY = new Array();
var eltREPLY = new Array();
// PILOT => <= ROVER
eltQUERY["pQr"] = {
	SNS : "=",
	ACK : "rRp"
};
eltREPLY["rRp"] = {
	SNS : "="
};
// ROVER => <= PILOT
eltQUERY["rQp"] = {
	SNS : "≈",
	ACK : "pRr"
};
eltREPLY["pRr"] = {
	SNS : "≈"
};
// PILOT => <= BASIS
eltQUERY["pQb"] = {
	SNS : "-",
	ACK : "bRp"
};
eltREPLY["bRp"] = {
	SNS : "-"
};
// BASIS => <= PILOT
eltQUERY["bQp"] = {
	SNS : "~",
	ACK : "pRb"
};
eltREPLY["pRb"] = {
	SNS : "~"
};
// BASIS => <= ROVER
eltQUERY["bQr"] = {
	SNS : "≈",
	ACK : "rRb"
};
eltREPLY["rRb"] = {
	SNS : "≈"
};
// ROVER => <= BASIS
eltQUERY["rQb"] = {
	SNS : "≈",
	ACK : "bRr"
};
eltREPLY["bRr"] = {
	SNS : "≈"
};
function SNS(TYP) {
	if (TYP[1] == "Q")
		return eltQUERY[TYP].SNS;
	else if (TYP[1] == 'R')
		return eltREPLY[TYP].SNS;
	else
		$alert("Error CTL '" + CTL + "' unknown");
	return "?";
}
// ====================================================================
var dt = 0;
var scheduler = -1;
var cntQUERIES = 0;
var skippedCnt = 0;
fifoWait = new Array();
fifoLost = new Array();
/**
 * Parent "class" of all slots
 * 
 * Define scheduler of module START_loop STOP The _Function function must be
 * initialized/ overloaded by true module the name of which is __oneShot()
 * Generaly __oneShot calls __oneShot
 */
Timer=function () {
	// private:
	var SELF = this;
	var _Scheduler = -1;
	var _Running = false;// to forbid if CHANGE first
	var _Function = function() {
		alert("_Function not initialized.");
	};
	// public:
	this._Timeout;
	//this._Status = STATUX.XOK;
	// ---------------------------------------------------------------
	/**
	 * TIMER START_loop -> setInterval(_Function, TMSOUT)
	 */
	this.START_loop = function(TMSOUT) {
		SELF._Timeout = TMSOUT;
		clearInterval(_Scheduler);
		_Running = true;
		_Scheduler = setInterval(_Function, TMSOUT);
	}
	// ---------------------------------------------------------------
	/**
	 * TIMER START_oneShoot -> setTimeout(_Function, TMSOUT)
	 */
	this.START_oneShoot = function(TMSOUT) {
		SELF._Timeout = TMSOUT;
		clearInterval(_Scheduler);
		_Running = false;
		_Scheduler = setTimeout(_Function, TMSOUT);
	}
	// ---------------------------------------------------------------
	/**
	 * TIMER STOP
	 */
	this.STOP = function() {
		_Running = false;
		clearInterval(_Scheduler);
	}
	// ---------------------------------------------------------------
	/**
	 * TIMER CHANGE FCT
	 */
	this.CHANGE = function(FCT) {
		_Function = FCT;
		if (_Running) {
			clearInterval(_Scheduler);
			_Scheduler = setInterval(FCT, SELF._Timeout);
		}
	}
};
// /////////////////////////////////////////////////////////////////////
function S_tick(VALUE){
	var CHN=VALUE.toString(10);
	while (CHN.length<10)
		CHN="."+CHN;
	return CHN;
}
function logHeader(SOK,OBJ){
	return "("+fifoWait.length+"/"+fifoLost.length+") "+S_tick(OBJ.CTL.TIK)+ " "+SOK._name+"("+SOK.cntWScnx+") ";
}
// ===================================================================
/**
 * PROCESS RECEIVED MESSAGES
 * 
 * return object STA
 * 
 * Rsignals object reply sense RJsonchn reply chain InfoRchn info about reply
 * and more if it was a query reception Qsignals object query sense QJsonchn *
 * query chain InfoQchn info about query
 */
ON_MESSAGE=function (SOK, TRC, CHN) {
	var RTN = {
		TYP : "?"
	};
	var OBJ;
	try {
		console.log(CHN);
		OBJ = JSON.parse(CHN.substr(CHN.indexOf("{")));
	} catch (e) {
		console.error("Parsing error:", e);
		// //console.error("["+event.data+"]");
		// alert("Parsing error!");
		// process.exit(0);
		// var x=0/0;
		return RTN;
	}
	var typ = OBJ.CTL.TYP;
	RTN.TYP = typ[1];
	var query;
	var reply;
	// -----------------------------------------------------------------
	// If it's a REPLY
	//
	if (RTN.TYP == 'R') {
		if (eltREPLY[typ] === undefined)
			alert(OBJ.CTL.TYP + " Type erreur");
		else {// clone !!
			reply = Object.assign({}, eltREPLY[typ]);
		}
		RTN.RJsonchn = CHN;
		reply.SNS =logHeader(SOK,OBJ)+"<" + reply.SNS + " REPLY "+OBJ.KMD.MID+" [";
		//console.log(reply.SNS + CHN);
		var check = lookForTIK(OBJ.CTL);// what to do ?
		var slot=OBJ.KMD.MID;
		MODULES[slot].update2reply(OBJ.KMD);
		var deltaTIK = cntQUERIES - OBJ.CTL.TIK;
		RTN.InfoRchn = "] delta(TIK="+ deltaTIK.toString() + ")";
	}
	// -----------------------------------------------------------------
	// If it's a QUERY
	//
	else if (RTN.TYP == "Q") {
		if (eltQUERY[typ] === undefined)
			alert(OBJ.CTL.TYP + " Type erreur");
		else {// clone !!
			query = Object.assign({}, eltQUERY[typ]);
			reply = Object.assign({}, eltREPLY[query.ACK]);
		}
		RTN.QJsonchn = CHN;
		RTN.InfoQchn = "]<br>";
		query.SNS =logHeader(SOK,OBJ)+"<"+query.SNS+" query "+OBJ.KMD.MID+" [";
		console.log(query.SNS + CHN);
		reply.SNS =logHeader(SOK,OBJ)+reply.SNS +"> reply "+OBJ.KMD.MID+" [";
		RTN.RJsonchn = SOKreply2query(SOK, OBJ, query.ACK);
		console.log(reply.SNS + RTN.RJsonchn);
		RTN.InfoRchn = "]<br>";
		RTN.Qsignals = query;
	}
	// -----------------------------------------------------------------
	// If it's unknown
	//
	else {
		console.log("<?" + CHN);
		$alert("CTL.TYPE error on\n" + CHN);
	}
	RTN.Rsignals = reply;
	return RTN;
};
// ====================================================================
/**
 * Search a TICK in FifoWait if found return a copy of object found else return
 * an empty object and push
 */
generateQUERY=                    function (QTYP,MODULEcfg) {
	cntQUERIES++;
	this.CTL = {
		"TYP" : QTYP,
		"TIK" : cntQUERIES,
	};
	this.KMD = {
		"MID" : MODULEcfg.Mid,
		"OP":"Nop",
		"CHN" : "Pilot "+QTYP,
	};
	fifoWait.push({
		TYP:this.CTL.TYP,
		TIK:this.CTL.TIK,
		TMS:(new Date()).getTime()
	});
}
// ====================================================================
/**
 * SLOT SOKCKET REPLY TO QUERY
 * 
 * @return {Cercle} Le nouvel objet Cercle
 */
function SOKreply2query(SOK, QUERY, ACK) {
	var slot=QUERY.KMD.MID;
	var KMD=MODULES[slot].reply2query(QUERY.KMD);
	var REPLY = Object.assign({}, QUERY);// clone !!
	REPLY.CTL.TYP = ACK;
	// REPLY.CTL.TMS = (new Date()).getTime();
	var CHN = JSON.stringify(REPLY);
	SOK.WS.send(CHN);
	return CHN;
}
// ====================================================================
/**
 * Search a TICK in FifoWait if found return a copy of object found else return
 * an empty object and push
 */
function lookForTIK(OBJ) {
	var Found = {};
	for (var i = fifoWait.length - 1; i >= 0; i--) {
		//
		// Extraire l'ACK du reçu
		//
		if (fifoWait[i].TIK === OBJ.TIK) {
			Object.assign(Found, fifoWait[i]);
			// fifoWait[i].TIK=-fifoWait[i].TIK;
			fifoWait.splice(i, 1);
			// break;
		}
		//
		// Supprimer les ACK non reçus
		//
		else if (fifoWait[i].TIK < OBJ.TIK) {
			fifoLost.push(fifoWait[i]);
			fifoWait.splice(i, 1);
		}

	}
	return Found;
}
//====================================================================
var intervalId = null;
function finish() {
	clearInterval(intervalId);
}
function test(TIMOUT, TEST, ARG, startTps, statusOBJ) {
	if (TEST(ARG)) {
		finish();
	} else {
		statusOBJ.WAIT = (new Date()).getTime() - startTps;
		if ((statusOBJ.WAIT) > TIMOUT) {
			statusOBJ.ERR = 10;
			skippedCnt++;
			finish();
		}
	}
}
// ====================================================================
function testWSBATO(WS) {
	return WS.bufferedAmount == 0;
}
function start(STEPTMS, TIMOUT, TEST, WS, statusOBJ) {
	statusOBJ.ERR = 0;
	statusOBJ.WAIT = 0;
	intervalId = setInterval(test, 3, TIMOUT, TEST, WS, (new Date()).getTime(),
			statusOBJ);// toutes// les 3 ms
}
/**
 * KEEP THIS ROUTINE IN LAST PLACE OF JAVASCRIPT TO BE SURE IT IS CALLED BY
 * UPPER LEVEL ONLY !!! Search a TICK in FifoWait if found return a copy of
 * object found else return return a status OBJ {ERROR CODE,STRING}
 */
sendJSON=function (WS, OBJ,BAMOUNT_TMS) {
	var statusOBJ = {};
	start(3, BAMOUNT_TMS, testWSBATO, WS, statusOBJ);
	if (statusOBJ.WAIT != 0)
		alert(statusOBJ.WAIT = 0);
	// console.log("=>WS ", WS);
	var sns=">>[";
	if (statusOBJ.ERR == 0) {
		if (typeof OBJ === 'object') {
			sns=SNS(OBJ.CTL.TYP)+">[";
			statusOBJ.MSG = JSON.stringify(OBJ);
			WS.send(statusOBJ.MSG);
			//console.log(sns, statusOBJ.MSG, "]");
		} else {
			statusOBJ.MSG = OBJ;
			WS.send(OBJ);
			console.log(sns, OBJ, "]as is...");
		}
	} else {
		console.log(sns, statusOBJ.MSG,
				"] !!!!!!!!!! error ", statusOBJ.ERR);
	}
	// console.log("~>WS ", WS);
	return statusOBJ;
}
// ===================================================================
/*
 * ========================================================= Send
 * =========================================================
 */
sendKMD=function (SOK,MID,KMD,BAMOUNT_TMS){
	cntQUERIES++;
	var CTL={};
	CTL.TYP=SOK.PROTOCOL;
	CTL.TIK=cntQUERIES;
	var OBJ={};
	OBJ.CTL=CTL;
	OBJ.KMD=Object.assign({MID:MID},KMD);
	addMsg(0,logHeader(SOK,OBJ)+">> QUERY "+MID+" [",JSON.stringify(OBJ),"]");
	sendJSON(SOK.WS, OBJ,BAMOUNT_TMS);
	fifoWait.push({
		TYP:CTL.TYP,
		TIK:CTL.TIK,
		TMS:(new Date()).getTime()
	});
}
/**
 * FIFO DUMP
 */
fifoDump=function (TITLE, FIFO) {
	var CHN = TITLE + "[" + FIFO.length + "]=|";
	for (var i = FIFO.length - 1; i >= 0; i--) {
		CHN += FIFO[i].TIK + "|";
	}
	return CHN;
}
/*
 * Cf. https://github.com/GoogleChrome/samples/blob/gh-pages/classes-es6/demo.js
 */
class Module{
// var THIS=this;
	constructor(MOD_CFG) {
		this._name = MOD_CFG.Name;
		this._Mid = MOD_CFG.Mid;
		this._protocol = MOD_CFG.Qtype;
		this._period = MOD_CFG.loop_TMS;
		this._lastTMS = (new Date()).getTime();
		console.log( "~", "\n-Creates module ", this._name, this._protocol,this._period);
	}
	QUERY(PROTOCOL) {
		this._lastTMS = (new Date()).getTime();
		cntQUERIES++;

	  	var query={
	  		"CTL" : {},
	  		"KMD" : {},
	  	}
	  	var prot;
	  	if (PROTOCOL === undefined) {
	    	prot=this._protocol;
		}else{
	    	prot=PROTOCOL;
		}

		query.CTL = {
			"TYP" : prot,
			"TIK" : cntQUERIES,
		};
		query.KMD = {
			"MID" : this._Mid,
		};
		return query;
	}
}



class mod_MOTORS extends Module{
  	constructor(MODULES) {
    	super(MOTORScfg.MOD);
    	MODULES[this._Mid]=this;
  	}
	QUERY(PROTOCOL) {
		var query=super.QUERY(PROTOCOL);
		Object.defineProperty(query.KMD, DEF_KF_LFT, {value: 100, writable: false,enumerable:true});
		Object.defineProperty(query.KMD, DEF_KF_RGT, {value: 123, writable: false,enumerable:true});
		fifoWait.push(query.CTL);
		return query;
	}
}
// /////////////////////////////////////////////////////////////////////
class mod_TIMEMS extends Module{
  	constructor(MODULES) {
    	super(TIMEMScfg.MOD);
    	MODULES[this._Mid]=this;
  	}
	QUERY(PROTOCOL) {
		var query=super.QUERY(PROTOCOL);
		Object.defineProperty(query.KMD, 'T', {value:(new Date()).getTime() , writable: false,enumerable:true});
		fifoWait.push(query.CTL);
		return query;
	}
}