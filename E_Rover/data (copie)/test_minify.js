// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//     Config PILOT  Version:'000_WS3' du 29 juil. 2021 à 15:59:35
//     File /media/DEV/ROB_000/setup/SCRIPTS/../SET_000_WS3/pil-CfgJS(000_WS3).js
//
//     This file is generated automatically. Don't modify it !!!
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	======================================================
//	 GLOBAL WS & JSON PROTOCOLS
//	------------------------------------------------------
//	---- dialogs ROVER BASIS -----------------------------
const DEF_QUERY_ROVER_TO_BASIS = "rQb";
const DEF_REPLY_BASIS_TO_ROVER = "bRr";
const DEF_QUERY_BASIS_TO_ROVER = "bQr";
const DEF_REPLY_ROVER_TO_BASIS = "rRb";
//	---- dialogs PILOT ROVER -----------------------------
const DEF_QUERY_PILOT_TO_ROVER = "pQr";
const DEF_REPLY_ROVER_TO_PILOT = "rRp";
const DEF_QUERY_ROVER_TO_PILOT = "rQp";
const DEF_REPLY_PILOT_TO_ROVER = "pRr";
//	---- dialogs BASIS PILOT -----------------------------
const DEF_QUERY_BASIS_TO_PILOT = "bQp";
const DEF_REPLY_PILOT_TO_BASIS = "pRb";
const DEF_QUERY_PILOT_TO_BASIS = "pQb";
const DEF_REPLY_BASIS_TO_PILOT = "bRp";
//	======================================================
//	 GLOBAL COMMANDS DICTIONNARY
//	------------------------------------------------------
const DEF_KF_DIR = "D";		//	KMD field
const DEF_KF_SPD = "S";		//	KMD field
const DEF_KF_LFT = "L";		//	KMD field
const DEF_KF_RGT = "R";		//	KMD field
const DEF_KF_FIL = "F";		//	KMD field
const DEF_KF_GET = "@";		//	KMD field
const DEF_KF_EXE = "X";		//	KMD field
const DEF_KF_MTS = "MTS";		//	KMD field (µs)
const DEF_KF_PMTS = "MTS";		//	KMD field
const DEF_KF_IMUT = "DTK";		//	KMD field
const DEF_KF_XC0 = "XC0";		//	KMD field
const DEF_KF_YC0 = "YC0";		//	KMD field
const DEF_KF_ZC0 = "ZC0";		//	KMD field
const DEF_KF_ROLL = "DNR";		//	KMD field
const DEF_KF_PITCH = "DEP";		//	KMD field
const DEF_KF_YAW = "DDY";		//	KMD field
const DEF_KF_GRX = "DGX";		//	KMD field
const DEF_KF_GRY = "DGY";		//	KMD field
const DEF_KF_GRZ = "DGZ";		//	KMD field
const DEF_KF_DIST = "DST";		//	KMD field
const DEF_KF_LAX = "DAX";		//	KMD field
const DEF_KF_LAY = "DAY";		//	KMD field
const DEF_KF_LAZ = "DAZ";		//	KMD field
const DEF_KF_SPX = "SPX";		//	KMD field
const DEF_KF_SPY = "SPY";		//	KMD field
const DEF_KF_SPZ = "SPZ";		//	KMD field
const DEF_KF_Iax = "Iax";		//	KMD field
const DEF_KF_Iay = "Iay";		//	KMD field
const DEF_KF_Iaz = "Iaz";		//	KMD field
const DEF_KF_Igx = "Igx";		//	KMD field
const DEF_KF_Igy = "Igy";		//	KMD field
const DEF_KF_Igz = "Igz";		//	KMD field
const DEF_KF_Imx = "Imx";		//	KMD field
const DEF_KF_Imy = "Imy";		//	KMD field
const DEF_KF_Imz = "Imz";		//	KMD field
//	======================================================
//	 GLOBAL OPERATIONS DICTIONNARY
//	------------------------------------------------------
const DEF_OP_INFO = "Inf";		//	KMD operation
const DEF_OP_MOT = "Mot";		//	KMD operation
const DEF_OP_PID = "Pid";		//	KMD operation
const DEF_OP_STOP = "Stop";		//	KMD operation
const DEF_OP_OPEN = "Open";		//	KMD operation
const DEF_OP_CLOSE = "Close";		//	KMD operation
const DEF_OP_SYNC = "Syn";		//	KMD operation
const DEF_OP_LEARN = "Learn";		//	KMD operation
const DEF_OP_JOBGOAL = "Jobgoal";		//	KMD operation
const DEF_OP_PILOT = "Pilot";		//	KMD operation
const DEF_OP_TSTSQ = "TstSQ";		//	KMD operation
const DEF_OP_SETDLVL = "set_DLVL";		//	KMD operation
const DEF_OP_SETTOUT = "setTmsOut";		//	KMD operation
const DEF_OP_GETTOUT = "getTmsOut";		//	KMD operation
const DEF_OP_OK = "Ok?";		//	KMD operation
const DEF_OP_CHK = "Chk";		//	KMD operation
//	======================================================
//	 If browser client is in javascript debug (devtools...)
//	 Add a delay to period timing
const DEF_DBGBROWSER = 3;
//	------------------------------------------------------
//	 If rover is in debug (print delays the application bandwidth)
//	 Add delay to period timing
const DEF_DBGLVLMAX = 3;
//	======================================================
//	 Default values for timing
//	 Add delay to period timing
//	------------------------------------------------------
const DEF_SEND_TMS = 0;
const DEF_SEND_TMS_JOBGOAL = 333;
const DEF_WATCH_TMS = 150 + (50 * DEF_DBGLVLMAX);		//	for WS reconnection
//	======================================================
//	 URL & WS CONFIGURATION DATA
//	------------------------------------------------------
var allCFG = {
	roverURL: "192.168.4.1",//"10.42.0.191",//		//	ROVER server url
	roverPORT: 80,		//	ROVER server port
	basisURL: "localhost",		//	BASIS server url
	basisPORT: 1337,		//	BASIS server port
	depthFIFO: 16,		//	depth of all fifos
	BamountTMS: 500,		//	(ms) buffered amount timeout
};
//	======================================================
const DEF_MOTORS_NBAXIS = 2;
//	======================================================
//	******************************************************
//	 "MOTORS" MODULE CONFIGURATION DATA
//	------------------------------------------------------
var MOTORScfg = {		//	LALALA
	MOD: {		//	Module standard parent configuration
		loop_TMS: DEF_SEND_TMS,		//	(ms) for scheduler
		Name: "MOTORS",		//	module name
		Mid: "MOT",		//	module identifier
		Qtype: DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 DRIVER MODULE CONFIGURATION DATA
//	------------------------------------------------------
var DRIVERcfg = {
	MOD: {		//	Module standard parent configuration
		loop_TMS: 100,		//	(ms) !!!
		Name: "DRIVER",		//	module name
		Mid: "DRV",		//	module identifier
		Qtype: DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 WATCHDOG MODULE CONFIGURATION DATA
//	------------------------------------------------------
var WATCHDOGcfg = {
	MOD: {		//	Module standard parent configuration
		loop_TMS: 1000,		//	(ms) Emergency stop !!!
		Name: "WATCHDOG",		//	module name
		Mid: "WAT",		//	module identifier
		Qtype: DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 JOBGOAL MODULE CONFIGURATION DATA
//	------------------------------------------------------
var JOBGOALcfg = {
	MOD: {		//	Module standard parent configuration
		loop_TMS: 2000,		//	(ms) NO scheduling !!!
		Name: "JOBGOAL",		//	module name
		Mid: "JOB",		//	module identifier
		Qtype: DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 TIMEMS MODULE CONFIGURATION DATA
//	------------------------------------------------------
var TIMEMScfg = {
	MOD: {		//	Module standard parent configuration
		loop_TMS: 10 * DEF_SEND_TMS,		//	(ms) for scheduler
		Name: "TIMEms",		//	module name
		Mid: "TMS",		//	module identifier
		Qtype: DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 BASIS CONFIGURATION DATA
//	------------------------------------------------------
var basisCFG = {
	loop_TMS: DEF_SEND_TMS,		//	(ms) for scheduler
	watch_TMS: DEF_WATCH_TMS,		//	(ms) for client connect retry
	cnx_ROVER: true,		//	webSocket server on|off
	PAT2NOD: "/media/DEV/ROB_000/node.js/node_modules/",		//	path to node
};
//	======================================================
//	 PILOT CONFIGURATION DATA
//	------------------------------------------------------
var pilotCFG = {
	loop_TMS: DEF_SEND_TMS,		//	(ms) for scheduler
	watch_TMS: DEF_WATCH_TMS,		//	(ms) for clients connect retry
	cnx_ROVER: true,		//	connect to ROVER server on|off
	cnx_BASIS: true,		//	connect to BASIS server on|off
};
//	//////////////////////////////////////////////////////
console.log("util.js coming...");
/*
 * util.js implements some utilities to start application & exchanges
 * configurations beetween client & serve
 */
/*
 * ================================================================= TRACE for
 * debugging replace with function console.log(){} for beta versions or
 * substitute console.log by //console.log(... for production
 * =================================================================
 */
function waitFor(TMS) {
	console.log("waitFor ", TMS, " ms...");
	var x = setTimeout(function() {
		alert("Hello");
		console.log("++++++++++++++++++++++++++++++");
	}, TMS);
	return TMS;
}
function _NOP_() {
}
_TMS_ = function() {
	return (new Date().getTime() - 1537700000000);
}
function _WAITMS_(DTMS) {
	var oldTMS = _TMS_();
	var newTMS = _TMS_();
	while ((newTMS - oldTMS) < DTMS) {
		newTMS = _TMS_();
	}
}
/*--------------------------------------------------------
 FORMATER UN ENTIER POSITIF AVEC DES ZEROS DEVANT
 */
function int2FixString(NBR, LEN) {
	var chn = "000000" + NBR;
	return chn.slice(-LEN);
}
/*--------------------------------------------------------
 FORMATER UN FLOTTANT AVEC DEC DECIMALES
 */
function SF_float(FLT, WID, DEC) {
	var CHN = FLT.toFixed(DEC);
	while (CHN.length < WID)
		CHN = " " + CHN;
	return CHN;
}
/*--------------------------------------------------------
 REMPLACER TOUTES LES OCCURENCES
 */
function strReplace(CHN, OLD, NEW) {
	var re = new RegExp(OLD, 'gi');// 
	return CHN.replace(re, NEW);
}
/*
 * ================================================================= Crée un
 * select/option
 * Crée un
 * select/option avec object{key:{opt:{sel:...,entry:...}},key:{...}...} if
 * field timer is defined it is add in a the preselected option to launch a
 * timer at launch exe
 * =================================================================
 */
function filesToHtmlSelect(ID, ITEMS, KEY, SCLASS) {
	// var idx = (typeof IDX == 'undefined') ? -1:IDX;
	var sclass = (typeof SCLASS == 'undefined') ? '' : " " + SCLASS;

	var cnt = 0;
	var txt = "<select id='" + ID + "' class='mv2_select " + sclass + "'>";
	// txt+="<option value=-1>...</option>";
	for (var key in ITEMS) {
		// var tim = "";
		if (typeof ITEMS[key].opt === 'object') {
			if (key == KEY) {
				var sel = ' selected="selected"';
				// if (typeof ITEMS[key].timer !== 'undefined') {
				// var tim = ' timer="' + ITEMS[key].timer + '"';
				// }
			} else {
				var sel = "";
			}
			txt += "<option value=" + cnt + sel + /* tim + */">"
				+ ITEMS[key].opt.entry + "</option>";
			cnt++;
		}
	}
	txt += "</select>";
	console.log("filesToHtmlSelect OPTIONS.length", cnt);
	return txt;
}
/*
 * ================================================================= Surcharge
 * l'option sélectionnée
 * 
 * //au choix...
 * 
 * selectSocket[selectSocket.selected Index].text="Connecté";
 * selectSocket[selectSocket.selected Index].textContent="textContent";
 * selectSocket[selectSocket.selected Index].innerHTML="innerHTML";
 * selectSocket[selectSocket.selected Index].innerText="innerText";
 * selectSocket.style.color = '#0000ff'; selectSocket.style.background =
 * '#00ff00';
 * 
 * =================================================================
 */
/*
 * function tagOption(ELEMENT,HELPER){ ELEMENT[ELEMENT.selected
 * Index].text=HELPER.text; ELEMENT.style.background=HELPER.background; //
 * return oldHelper; }
 */
/*
 * ================================================================= Fonctions
 * typeof =================================================================
 */
isDefined = function(VAR) {
	return !(typeof VAR === 'undefined');
}
isObject = function(VAR) {
	return (typeof VAR === 'object');
}
/*
 * ================================================================= CLonage
 * d'objets =================================================================
 */
clone = function(obj) {
	try {
		var copy = JSON.parse(JSON.stringify(obj));
	} catch (ex) {
		alert("Vous utilisez un vieux navigateur bien pourri, qui n'est pas pris en charge par ce site");
	}
	return copy;
}
///////////////////////////////////////////////////////////////////////
/**
 * Array of modules indexed by MID (module ID)
 */
MODULES = new Array();
var eltQUERY = new Array();
var eltREPLY = new Array();
// PILOT => <= ROVER
eltQUERY["pQr"] = {
	SNS: "=",
	ACK: "rRp"
};
eltREPLY["rRp"] = {
	SNS: "="
};
// ROVER => <= PILOT
eltQUERY["rQp"] = {
	SNS: "≈",
	ACK: "pRr"
};
eltREPLY["pRr"] = {
	SNS: "≈"
};
// PILOT => <= BASIS
eltQUERY["pQb"] = {
	SNS: "-",
	ACK: "bRp"
};
eltREPLY["bRp"] = {
	SNS: "-"
};
// BASIS => <= PILOT
eltQUERY["bQp"] = {
	SNS: "~",
	ACK: "pRb"
};
eltREPLY["pRb"] = {
	SNS: "~"
};
// BASIS => <= ROVER
eltQUERY["bQr"] = {
	SNS: "≈",
	ACK: "rRb"
};
eltREPLY["rRb"] = {
	SNS: "≈"
};
// ROVER => <= BASIS
eltQUERY["rQb"] = {
	SNS: "≈",
	ACK: "bRr"
};
eltREPLY["bRr"] = {
	SNS: "≈"
};
// ====================================================================
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
// ====================================================================
/**
 * Parent "class" of all slots
 * 
 * Define scheduler of module START_loop STOP The _Function function must be
 * initialized/ overloaded by true module the name of which is __oneShot()
 * Generaly __oneShot calls __oneShot
 */
Timer = function() {
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
function S_tick(VALUE) {
	var CHN = VALUE.toString(10);
	while (CHN.length < 10)
		CHN = "." + CHN;
	return CHN;
}
function logHeader(SOK, OBJ) {
	return "(" + fifoWait.length + "/" + fifoLost.length + ") " + S_tick(OBJ.CTL.TIK) + " " + SOK._name + "(" + SOK.cntWScnx + ") ";
}
// ====================================================================
/**
 * @fn function lookForTIK(OBJ)
 * @brief Search a query TICK in FifoWait 
 *
 * Replies are supposed returning synchronously with sent queries. So all fifoWait[i].TIK < OBJ.TIK
 * are pushed in fifoLost for memory (but nothing is done with)
 * @return if TIK is found a copy of object found {TYP,TIK[,TMS]} else an empty object
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
ON_MESSAGE = function(SOK, TRC, CHN) {
	var RTN = {
		TYP: "?"
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
		reply.SNS = logHeader(SOK, OBJ) + "<" + reply.SNS + " REPLY " + OBJ.KMD.MID + " [";
		//console.log(reply.SNS + CHN);
		var check = lookForTIK(OBJ.CTL);// what to do ?
		var slot = OBJ.KMD.MID;
		MODULES[slot].update2reply(OBJ.KMD);
		var deltaTIK = cntQUERIES - OBJ.CTL.TIK;
		RTN.InfoRchn = "] delta(TIK=" + deltaTIK.toString() + ")";
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
		query.SNS = logHeader(SOK, OBJ) + "<" + query.SNS + " query " + OBJ.KMD.MID + " [";
		console.log(query.SNS + CHN);
		reply.SNS = logHeader(SOK, OBJ) + reply.SNS + "> reply " + OBJ.KMD.MID + " [";
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
generateQUERY = function(QTYP, MODULEcfg) {
	cntQUERIES++;
	this.CTL = {
		"TYP": QTYP,
		"TIK": cntQUERIES,
	};
	this.KMD = {
		"MID": MODULEcfg.Mid,
		"OP": "Nop",
		"CHN": "Pilot " + QTYP,
	};
	fifoWait.push({
		TYP: this.CTL.TYP,
		TIK: this.CTL.TIK,
		TMS: (new Date()).getTime()
	});
}
// ====================================================================
/**
 * SLOT SOKCKET REPLY TO QUERY
 * 
 * @return {Cercle} Le nouvel objet Cercle
 */
function SOKreply2query(SOKCLI, QUERY, ACK) {
	var slot = QUERY.KMD.MID;
	var KMD = MODULES[slot].reply2query(QUERY.KMD);
	var REPLY = Object.assign({}, QUERY);// clone !!
	REPLY.CTL.TYP = ACK;
	// REPLY.CTL.TMS = (new Date()).getTime();
	var CHN = JSON.stringify(REPLY);
	SOKCLI.sendCLI(CHN);
	return CHN;
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
sendJSON = function(SOKCLI, OBJ, BAMOUNT_TMS) {
	var statusOBJ = {};
	start(3, BAMOUNT_TMS, testWSBATO, SOKCLI.WS, statusOBJ);
	if (statusOBJ.WAIT != 0)
		alert(statusOBJ.WAIT = 0);
	// console.log("=>WS ", WS);
	var sns = ">>[";
	if (statusOBJ.ERR == 0) {
		if (typeof OBJ === 'object') {
			sns = SNS(OBJ.CTL.TYP) + ">[";
			statusOBJ.MSG = JSON.stringify(OBJ);
			console.log(sns, statusOBJ.MSG, "]");
		} else {
			statusOBJ.MSG = OBJ;
			console.log(sns, OBJ, "]as is...");
		}
		SOKCLI.sendCLI(statusOBJ.MSG);
	} else {
		console.log(sns, statusOBJ.MSG, "] !!!!!!!!!! error ", statusOBJ.ERR);
	}
	// console.log("~>WS ", WS);
	return statusOBJ;
}
// ===================================================================
/*
 * ========================================================= Send
 * =========================================================
 */
sendKMD = function(SOKCLI, MID, KMD, BAMOUNT_TMS) {
	cntQUERIES++;
	var CTL = {};
	CTL.TYP = SOKCLI.PROTOCOL;
	CTL.TIK = cntQUERIES;
	var OBJ = {};
	OBJ.CTL = CTL;
	OBJ.KMD = Object.assign({ MID: MID }, KMD);
	if (SOKCLI.WS.readyState === 3) {
		addMsg(0, SOKCLI.WS.url + " injoignable (pas de réseau)");
	}
	addMsg(0, logHeader(SOKCLI, OBJ) + ">> QUERY " + MID + " [", JSON.stringify(OBJ), "]");
	sendJSON(SOKCLI, OBJ, BAMOUNT_TMS);
	fifoWait.push({
		TYP: CTL.TYP,
		TIK: CTL.TIK,
		TMS: (new Date()).getTime()
	});
}
/**
 * FIFO DUMP
 */
fifoDump = function(TITLE, FIFO) {
	var CHN = TITLE + "[" + FIFO.length + "]=|";
	for (var i = FIFO.length - 1; i >= 0; i--) {
		CHN += FIFO[i].TIK + "|";
	}
	return CHN;
}
