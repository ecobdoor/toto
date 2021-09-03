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
	//	roverURL: "192.168.4.1",//"10.42.0.191",//		//	ROVER server url
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
//	FORMATER UN ENTIER POSITIF AVEC DES ZEROS DEVANT
function int2FixString(NBR, LEN) {
	var chn = "000000" + NBR;
	return chn.slice(-LEN);
}

//	FORMATER UN FLOTTANT AVEC DEC DECIMALES
function SF_float(FLT, WID, DEC) {
	var CHN = FLT.toFixed(DEC);
	while (CHN.length < WID)
		CHN = " " + CHN;
	return CHN;
}
//	REMPLACER TOUTES LES OCCURENCES
function strReplace(CHN, OLD, NEW) {
	var re = new RegExp(OLD, 'gi');// 
	return CHN.replace(re, NEW);
}
function disableChildren(DIV){
		var nodes = document.getElementById(DIV).getElementsByTagName('*');
		for(var i = 0; i < nodes.length; i++){
		     nodes[i].disabled = true;
		}
	}
function enableChildren(DIV){
		var nodes = document.getElementById(DIV).getElementsByTagName('*');
		for(var i = 0; i < nodes.length; i++){
		     nodes[i].disabled = false;
		}
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
/**
 * - Null: Z
 * - No-op: N - no operation, to be ignored by the receiving end
 * - Boolean types: true (T) and false (F)
 * - Numeric types: int8 (i), uint8 (U), int16 (I), int32 (l), int64 (L), float32 (d), float64 (D), and high-precision (H)
 * - ASCII character: C
 * - UTF-8 string: S
 * 
 */
/*
 var schemaDRIVERinfos={
 	"SCHEMA":"DRVinfos",
 	CTL:{

 	},
 	KMD:{
	 	"MID":"S",
		DEF_KF_EXE:CTX.strRoverMode(),
		"OP":"S",

		DEF_KF_SPD:"I",
		DEF_KF_DIR:"I",
	//"MTS":54411073,
		DEF_KF_LFT:"I",
		DEF_KF_RGT:"I",
		if (_mustClearMAP)
			_mustClearMAP = false,
		else
			DEF_KF_PMTS:_drv_Kine.Fusensor.microTS,
		DEF_KF_DIST:"d",
		DEF_KF_IMUT:_drv_Kine.Attitude.imuTik,
		DEF_KF_XC0:"d",
		DEF_KF_YC0:"d",
		DEF_KF_ZC0:"d",
		DEF_KF_SPX:"d",
		DEF_KF_SPY:"d",
		DEF_KF_SPZ:"d",
		DEF_KF_LAX:"d",
		DEF_KF_LAY:"d",
		DEF_KF_LAZ:"d",
		DEF_KF_ROLL:"d",
		DEF_KF_PITCH:"d",
		DEF_KF_YAW:"d",
		DEF_KF_GRX:"d",
		DEF_KF_GRY:"d",
		DEF_KF_GRZ:"d",
		DEF_KF_Iax:"d",
		DEF_KF_Iay:"d",
		DEF_KF_Iaz:"d",
		DEF_KF_Igx:"d",
		DEF_KF_Igy:"d",
		DEF_KF_Igz:"d",
		DEF_KF_Imx:"d",
		DEF_KF_Imy:"d",
		DEF_KF_Imz:"d",
		"temp":"d",
		"Q0":"d",
		"Q1":"d",
		"Q2":"d",
		"Q3":"d",
	}
 };
*/
// ===================================================================
/**
 * 
 * https://askcodez.com/supprimer-vide-les-attributs-dun-objet-en-javascript.html
 * 
 * 
 * @fn jsonExpand = function(SHRINK,SCHEMA)
 * @brief Expands a shrinked object in a full json object
 * @param SHRINK shrinked json array
 * @param SCHEMA json model
 * @return full json object, otherwise {}
 */
var	jsonExpand = (function() {
	function cleanObject(obj) {
	  	Object.keys(obj).forEach(key => {
	    	if (obj[key] && typeof obj[key] === "object") cleanObject(obj[key]); //recurse
	    	else if (obj[key] == null) delete obj[key]; //delete
	  	});
	};
	var expand={};
	var idx=0;
	return function(SHRINK,SCHEMA,OBJ=false) {
		if(!OBJ){
			cleanObject(expand);
			idx=0;//SHRINK.length;
			jsonExpand(SHRINK,SCHEMA,expand);
		}
		Object.entries(SCHEMA).forEach(([key, value]) => {
			console.log(`${key}: ${value}`)
			if(typeof value !== 'Object'){
				OBJ.key=SHRINK[idx];
				idx++;
			}
			else{
				expand.key={};
				jsonExpand(SHRINK[key],SCHEMA[key],expand.key);
			}

		});
		return JSON.stringify(expand);
	}
})();
// ===================================================================
/**
 * @fn jsonShrink = function(EXPAND,MODEL)
 * @brief Shrinks a expanded object in a single json array
 * @param EXPAND expanded full json object
 * @param SCHEMA json model
 * @return Shrinked json array, otherwise []
 */
var jsonShrink =  (function() {
	function cleanArray(arr) {
		Array.splice(0,arr.length);//			arr.length = 0;
	};
	var shrink= new Array();
	return function(EXPAND,SCHEMA='') {
		if(SCHEMA.SCHEMA.length>0){
			clean(shrink);
			shrink.push(SCHEMA.SCHEMA);
		}
		Object.entries(EXPAND).forEach(([key, value]) => {
			console.log(`${key}: ${value}`)
			if(typeof value !== 'object'){
				shrink.push(value);
			}
			else
				jsonShrink(value);
		});
		return JSON.stringify(shrink);
	}
})();
// ===================================================================
/**
 * @fn ON_MESSAGE = function(SOK, TRC, CHN)
 * @brief PROCESS RECEIVED MESSAGES
 * 
 * Switch (OBJ.CTL.TYP[1])
 * - 'R'eply: calls MODULES[slot].update2reply(OBJ.KMD);
 * 
 * - 'Q'uery: calls SOKreply2query(SOK, OBJ, query.ACK);
 * - default: error
 * @param SOK
 * @param TRC
 * @param CHN
 * @return RTN object {TYP:R|Q|?,RJsonchn = CHN}
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
		//		console.log(CHN);
		OBJ = JSON.parse(CHN.substr(CHN.indexOf("{")));
	} catch (e) {
		console.error("Parsing error:", e);
		// //console.error("["+event.data+"]");
		// alert("Parsing error!");
		// process.exit(0);
		// var x=0/0;
		return RTN;
	}
	if(typeof OBJ.CTL==='undefined')
		return {TYP:"?"};
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
 * ========================================================= Query
 * =========================================================
 */
send_QUERY = function(SOKCLI, MID, KMD, BAMOUNT_TMS) {
	cntQUERIES++;
	var CTL = {};
	CTL.TYP = SOKCLI.PROTOCOL;
	CTL.TIK = cntQUERIES;
	var OBJ = {};
	OBJ.CTL = CTL;
	OBJ.KMD = Object.assign({ MID: MID }, KMD);
	if (SOKCLI.WS.readyState === 3) {
		addLog(0, SOKCLI.WS.url + " injoignable (pas de réseau)");
	}
	addLog(0, logHeader(SOKCLI, OBJ) + ">> QUERY " + MID + " [", JSON.stringify(OBJ), "]");
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
/*
 * Cf. https://github.com/GoogleChrome/samples/blob/gh-pages/classes-es6/demo.js
 *//*
class Module {
	// var THIS=this;
	constructor(MOD_CFG) {
		this._name = MOD_CFG.Name;
		this._Mid = MOD_CFG.Mid;
		this._protocol = MOD_CFG.Qtype;
		this._period = MOD_CFG.loop_TMS;
		this._lastTMS = (new Date()).getTime();
		console.log("~", "\n-Creates module ", this._name, this._protocol, this._period);
	}
	QUERY(PROTOCOL) {
		this._lastTMS = (new Date()).getTime();
		cntQUERIES++;

		var query = {
			"CTL": {},
			"KMD": {},
		}
		var prot;
		if (PROTOCOL === undefined) {
			prot = this._protocol;
		} else {
			prot = PROTOCOL;
		}

		query.CTL = {
			"TYP": prot,
			"TIK": cntQUERIES,
		};
		query.KMD = {
			"MID": this._Mid,
		};
		return query;
	}
}



class mod_MOTORS extends Module {
	constructor(MODULES) {
		super(MOTORScfg.MOD);
		MODULES[this._Mid] = this;
	}
	QUERY(PROTOCOL) {
		var query = super.QUERY(PROTOCOL);
		Object.defineProperty(query.KMD, DEF_KF_LFT, { value: 100, writable: false, enumerable: true });
		Object.defineProperty(query.KMD, DEF_KF_RGT, { value: 123, writable: false, enumerable: true });
		fifoWait.push(query.CTL);
		return query;
	}
}
// /////////////////////////////////////////////////////////////////////
class mod_TIMEMS extends Module {
	constructor(MODULES) {
		super(TIMEMScfg.MOD);
		MODULES[this._Mid] = this;
	}
	QUERY(PROTOCOL) {
		var query = super.QUERY(PROTOCOL);
		Object.defineProperty(query.KMD, 'T', { value: (new Date()).getTime(), writable: false, enumerable: true });
		fifoWait.push(query.CTL);
		return query;
	}
}
const pokeState = {
	poked: 0,
	fifoFull_: 1
};
const pikeState = {
	piked: 0,
	timeOut_: 1,
	notFound_: 2
};
var jctManager = function() {
	{
		slot: 2
	}
}

var FifoAck = function(FIFOMAX) {
	// private:var...
	var SELF = this;
	var _fifo = new Array(FIFOMAX);
	this._latence = 0;
	this.clear = function() {
		for (var i = 0; i < FIFOMAX; i++) {
			_fifo[i] = {
				TIK: 0,
				MSG: "",
				TMS: 0
			};
		}
	}
	this.clear();
	// public: via SELF... for events
	this.waitingACK = function() {
		var l = 0;
		for (var i = 0; i < FIFOMAX; i++)
			if (_fifo[i].TIK != 0)
				l++;
		return l;
	}
	this.pokeForACK = function(CHN) {
		do {
			for (var i = 0; i < FIFOMAX; i++)
				if (_fifo[i].TIK == 0) {
					_fifo[i].TMS = Date.now();
					_fifo[i].MSG = CHN;
					_fifo[i].TIK = parseInt(
						CHN.substring(1 + CHN.indexOf("}")), 10);
					;
					return pokeState.poked;
				}
		} while (true);
		return pokeState.fifoFull_;
	}
	this.pikeForACK = function(CHN) {
		var found = pikeState.notFound_;
		var indexOfapptik = 1 + CHN.indexOf("}");
		var tik = parseInt(CHN.substring(indexOfapptik), 10);
		for (var i = 0; i < FIFOMAX; i++) {
			if (_fifo[i].TIK != 0)
				console.log(`${i} ${_fifo[i].TIK}\t${_fifo[i].MSG}`);
			if (_fifo[i].TIK == Math.abs(tik)) {
				found = pikeState.piked;
				this._latence = Date.now() - _fifo[i].TMS;
				if (tik > 0) {// Case of multiple REPLY
					_fifo[i].MSG = "";
					_fifo[i].TIK = 0;
					_fifo[i].TMS = 0;
				}
			}
		}
		return found;
	}
}
*/

// ===================================================================
// On renvoie un entier aléatoire entre une valeur min (incluse)
// et une valeur max (incluse).
// Attention : si on utilisait Math.round(), on aurait une distribution
// non uniforme !
function getRandomIntInclusive(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(Math.random() * (max - min + 1)) + min;
}
/*
  QUE DES OBJETS SIMPLES ET DE MOINS DE 128 CARACTERES
  Pas d'espace dans ":{" 
*/
//var x=_TMS_();
const _RED = '#ff0000';
const _GREY = '#00ff00';
var CFG = {};

CFG.client = {
	'PILOT': {
		'DIR': { 'MIN': -255, 'CLR': 0, 'MAX': +255 },
		'VIT': { 'MIN': -255, 'CLR': 0, 'MAX': +255 },
	},

	'TPIDDIR': { 'MIN': -255, 'MAX': +255, 'KP': 1, 'KI': 0, 'KD': 100, 'PROF': 10 },
	'TPIDVIT': { 'MIN': -255, 'MAX': +255, 'KP': 1, 'KI': 0, 'KD': 100, 'PROF': 10 },
	'CANVAS': {
		'GAP': 50, 'ANIMATE': true,
		'MOTORS': { 'MIN': -1023, 'CLR': 0, 'MAX': +1023 },
		'PILOT': { 'MIN': -255, 'CLR': 0, 'MAX': +255 },
	}, // ± sur MIN MAX
	'IOSERIAL': { 'HOST': 'http://localhost:8888', 'SPEED': 115200, 'TIMEOUT': 100 },
	'LOG': { 'SCROLL': 50 },
	'JOBGOAL': { 'NAME': 'TOTO.kmd' },
	'SOKTIMOUT': 5000,//Délai de connection
	/*
		'__field_dbglvlloc':  { DBG_0:  {opt:{sel:1,show:1,entry:"0",background:'#aaaaaa'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_1:  {opt:{sel:0,show:1,entry:"1",background:'#0000aa'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_2:  {opt:{sel:0,show:1,entry:"2",background:'#aaaa00'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_3:  {opt:{sel:0,show:1,entry:"3",background:'#4444ff'},  kmd:{OP:DEF_OP_SETDLVL}}
					   },
		'__field_dbglvlrov':  { DBG_0:  {opt:{sel:1,show:1,entry:"0",background:'#aaaaaa'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_1:  {opt:{sel:0,show:1,entry:"1",background:'#0000aa'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_2:  {opt:{sel:0,show:1,entry:"2",background:'#aaaa00'},  kmd:{OP:DEF_OP_SETDLVL}}
					   },
		'__field_dbglvlesp':  { DBG_0:  {opt:{sel:1,show:1,entry:"0",background:'#aaaaaa'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_1:  {opt:{sel:0,show:1,entry:"1",background:'#aaaaff'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_2:  {opt:{sel:0,show:1,entry:"2",background:'#8888ff'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_3:  {opt:{sel:0,show:1,entry:"3",background:'#4444ff'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_4:  {opt:{sel:0,show:1,entry:"4",background:'#00aa00'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_5:  {opt:{sel:0,show:1,entry:"5",background:'#00ff00'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_6:  {opt:{sel:0,show:1,entry:"6",background:'#aaaa00'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_7:  {opt:{sel:0,show:1,entry:"7",background:'#ffff00'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_8:  {opt:{sel:0,show:1,entry:"8",background:'#ff8888'},  kmd:{OP:DEF_OP_SETDLVL}},
						DBG_9:  {opt:{sel:0,show:1,entry:"9",background:'#ff0000'},  kmd:{OP:DEF_OP_SETDLVL}}
					   },
	*/
	'__field_dlvl': {
		DBG_0: { opt: { sel: 0, show: 1, entry: "0", background: '#aaaaaa' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_1: { opt: { sel: 0, show: 1, entry: "1", background: '#aaaaff' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_2: { opt: { sel: 0, show: 1, entry: "2", background: '#8888ff' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_3: { opt: { sel: 0, show: 1, entry: "3", background: '#4444ff' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_4: { opt: { sel: 0, show: 1, entry: "4", background: '#00aa00' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_5: { opt: { sel: 0, show: 1, entry: "5", background: '#00ff00' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_6: { opt: { sel: 0, show: 1, entry: "6", background: '#aaaa00' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_7: { opt: { sel: 0, show: 1, entry: "7", background: '#ffff00' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_8: { opt: { sel: 0, show: 1, entry: "8", background: '#ff8888' }, kmd: { OP: DEF_OP_SETDLVL } },
		DBG_9: { opt: { sel: 0, show: 1, entry: "9", background: _RED }, kmd: { OP: DEF_OP_SETDLVL } }
	},
	'__field_timer': {
		LISTEN: { opt: { sel: 1, show: 1, entry: "   Never", background: '#aaaaaa' }, kmd: { OP: DEF_OP_GETTOUT, codeTMS: -1 } },
		SHOOT: { opt: { sel: 0, show: 1, entry: " 1 Shoot", background: '#888888' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 0 } },
		L_hour: { opt: { sel: 0, show: 1, entry: "1/(hour)", background: '#ffffff' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 3600000 } },
		L_5mn: { opt: { sel: 0, show: 1, entry: "1/(5 mn)", background: '#88ffff' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 300000 } },
		L_1mn: { opt: { sel: 0, show: 1, entry: "1/(1 mn)", background: '#00ffff' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 60000 } },
		L_10s: { opt: { sel: 0, show: 1, entry: "  0.1 Hz", background: '#00ff88' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 10000 } },
		L_1s: { opt: { sel: 0, show: 1, entry: "    1 Hz", background: '#00ff00' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 1000 } },
		L_500ms: { opt: { sel: 0, show: 1, entry: "    2 Hz", background: '#88ff00' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 500 } },
		L_200ms: { opt: { sel: 0, show: 1, entry: "    5 Hz", background: '#ffff00' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 200 } },
		L_100ms: { opt: { sel: 0, show: 1, entry: "   10 Hz", background: '#ff8800' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 100 } },
		L_50ms: { opt: { sel: 0, show: 1, entry: "   20 Hz", background: '#ff0000' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 50 } }
	},
	'__field_txtarea': {
		init: { value: "86400", background_color: '#002200', color: "#ffffff", maxlength: 360, cols: 100, rows: 15 },
		max: { value: "-1", background_color: _GREY, color: "#000000", maxlength: 48, size: 50 },
		error: { value: "???", background_color: '#000000', color: "#ffffff", maxlength: 48, size: 50 },
		min: { value: _TMS_(), background_color: '#0000ff', color: "#ffffff", maxlength: 48, size: 50 }
	},
	'__field_txt': {
		max: { value: "86400", background_color: '#ff0000', color: "#ffffff", maxlength: 48, size: 32 },
		init: { value: "-1", background_color: _GREY, color: "#000000", maxlength: 48, size: 32 },
		error: { value: "???", background_color: '#000000', color: "#ffffff", maxlength: 48, size: 32 },
		min: { value: _TMS_(), background_color: '#0000ff', color: "#ffffff", maxlength: 48, size: 32 }
	},
	'__Driver_exec': {
		Stop: { opt: { sel: 1, show: 1, entry: "STOP", background: '#ff0000' }, timer: 0, kmd: { "OP": DEF_OP_CLOSE, "soks": ["wsR"] } }
	},
	'__Motors_exec': {
		Stop: { opt: { sel: 1, show: 1, entry: "STOP", background: '#ff0000' }, timer: 0, kmd: { "OP": DEF_OP_CLOSE, "soks": ["wsR"] } },
		PILOT: { opt: { sel: 0, show: 1, entry: "Pilotage", background: '#ABCDEF' }, timer: 6, kmd: { "OP": DEF_OP_PILOT } },
		TESTsquare: { opt: { sel: 0, show: 1, entry: "TSTsquare", background: '#aaaaaa' }, timer: 0, kmd: { "OP": DEF_OP_TSTSQ } },
		LEARN: { opt: { sel: 0, show: 1, entry: "Circle", background: '#ffff00' }, timer: 7, kmd: { "OP": DEF_OP_OPEN, "soks": ["wsR"] } }
	},
	'__Datim_exec': {
		SynRover: { opt: { sel: 1, show: 1, entry: "TMS Rover", background: '#00ff00' }, timer: 0, kmd: { "OP": DEF_OP_SYNC, "soks": ["wsR"] } },
		SynBasis: { opt: { sel: 0, show: 1, entry: "TMS Basis", background: '#0000ff' }, timer: 0, kmd: { "OP": DEF_OP_SYNC, "soks": ["wsB"] } },
		SynAll: { opt: { sel: 0, show: 1, entry: "TMS All", background: '#ff0000' }, timer: 0, kmd: { "OP": DEF_OP_SYNC, "soks": ["wsR", "wsB"] } }
	},
	'__Jobgoal_exec': {
		Stop: { opt: { sel: 1, show: 1, entry: "STOP", background: '#ff0000' }, timer: 0, kmd: { "OP": DEF_OP_CLOSE, "soks": ["wsR"] } },
		Mis_1: { opt: { sel: 0, show: 1, entry: "Autotest", background: '#00ff00' }, timer: 6, kmd: { "OP": DEF_OP_OPEN, "soks": ["wsR"] } },
		Mis_2: { opt: { sel: 0, show: 1, entry: "Circle", background: '#0088ff' }, timer: 6, kmd: { "OP": DEF_OP_OPEN, "soks": ["wsR"] } },
		Mis_3: { opt: { sel: 0, show: 1, entry: "Autotest", background: '#ff8800' }, timer: 6, kmd: { "OP": DEF_OP_OPEN, "soks": ["wsR"] } }
	},
	'__Log_pilot': {
		LOG_0: { opt: { sel: 1, show: 1, entry: "-", background: '#aaaaaa' }, kmd: { OP: "set_logLVL" } },
		LOG_S: { opt: { sel: 0, show: 1, entry: ">", background: '#0000aa' }, kmd: { OP: "set_logLVL" } },
		LOG_R: { opt: { sel: 0, show: 1, entry: "<", background: '#0000aa' }, kmd: { OP: "set_logLVL" } },
		LOG_2: { opt: { sel: 0, show: 1, entry: "X", background: '#aaaa00' }, kmd: { OP: "set_logLVL" } }
	},
	'__Log_rover': {
		LOG_0: { opt: { sel: 1, show: 1, entry: "-", background: '#aaaaaa' }, kmd: { OP: "set_logLVL" } },
		LOG_S: { opt: { sel: 0, show: 1, entry: ">", background: '#0000aa' }, kmd: { OP: "set_logLVL" } },
		LOG_R: { opt: { sel: 0, show: 1, entry: "<", background: '#0000aa' }, kmd: { OP: "set_logLVL" } },
		LOG_2: { opt: { sel: 0, show: 1, entry: "X", background: '#aaaa00' }, kmd: { OP: "set_logLVL" } }
	},
	'__Leds_exec': {
		BLACK: { opt: { sel: 1, show: 1, entry: "- - -", background: '#000000' }, kmd: { "codeRYG": 0 } },
		G: { opt: { sel: 0, show: 1, entry: "- - G", background: '#00ff00' }, kmd: { "codeRYG": 1 } },
		Y: { opt: { sel: 0, show: 1, entry: "- Y -", background: '#ffff00' }, kmd: { "codeRYG": 2 } },
		YG: { opt: { sel: 0, show: 1, entry: "- Y G", background: '#ffff88' }, kmd: { "codeRYG": 3 } },
		R: { opt: { sel: 0, show: 1, entry: "R - -", background: '#ff0000' }, kmd: { "codeRYG": 4 } },
		RG: { opt: { sel: 0, show: 1, entry: "R - G", background: '#ffff00' }, kmd: { "codeRYG": 5 } },
		RY: { opt: { sel: 0, show: 1, entry: "R Y -", background: '#aaaa00' }, kmd: { "codeRYG": 6 } },
		RYG: { opt: { sel: 0, show: 1, entry: "R Y G", background: '#ffffff' }, kmd: { "codeRYG": 7 } }
	},
	'__Socket_exec': {
		CONNECT: { opt: { sel: 0, show: 1, entry: "Connecter", background: '#00ff00' } },
		CLOSE: { opt: { sel: 1, show: 1, entry: "Fermer", background: '#ff0000' } },
		//TOUJOURS LES HELPERS après les options (pour onchange!!)
		OPEN: { help: { sta: 0, entry: "Connecté", background: '#ff0000' } },
		OPENING: { help: { sta: 1, entry: "Connecte...", background: '#ffff00' } },
		CLOSING: { help: { sta: 2, entry: "Deconnecte...", background: '#ffff00' } },
		CLOSED: { help: { sta: 3, entry: "Fermé ou muet", background: '#888800' } },
		ONMESSAGE: { help: { sta: 4, entry: "on Message", background: '#0000ff' } },// pas de jctStatus(), on laisse passer STA
		NEW: { help: { sta: 5, entry: "New", background: '#ffff00' } },
		ER_TIMEOUT: { help: { sta: 14, entry: "Time OUT", background: '#ffff00' } },
		WHAT: { help: { sta: 17, entry: "état inconnu", background: '#ff0000' } },
		ERRPARSE: { help: { sta: 128, entry: "err Parser", background: '#ff0000' } },
		NOSERVER: { help: { sta: 267, entry: "Pas de serveur", background: '#ff0000' } }
	}
};
/*
{"text":"Pas connecté","background":'#ffff00'}
,{"text":"Connecté","background":'#00ff00'});
*//**
 * These routines are used to complete the html file
 * 
 * @param SCLASS class to prettify the injected element(s)
 * @param DIV id of html <div> to complete (via innerHTLM)
 * @param ID id of injected element(s)
 * @param CONF configuration data for injection if opt is in CONF preselects the default element (optional)
 * @param KMD_OP operation codes associates with each element (optional)
 */
/** ===================================================================
 * @fn __div(DIV)
 * @param DIV id of html <div> to complete (via innerHTLM)
 */
function __html(DIV) {
	// private:
	var SELF = this;
	this._Elt = document.getElementById(DIV);
	console.log("§§§ NEW __div", DIV);
	this._Elt.innerHTML = "";
	/*---------------------------------------------------------------
		SET html (replace all \n by <br>)
	 */
	this._setHtml = function(CHN) {
		var chn=strReplace(CHN, '\n', '<br>');
		// protect attributes
		chn=strReplace(chn, 'font color', 'font_color');
		chn=strReplace(chn, ' ', '&nbsp;');
		// restore attributes
		chn=strReplace(chn, 'font_color', 'font color');
		SELF._Elt.innerHTML = chn;
	}
};
/*
 * =================================================================
 * 
 * CLASS TEXT (EDITABLE)
 * 
 * =================================================================
 */
function __text(SCLASS, DIV, ID, CONF) {
	// private:
	var SELF = this;
	var oldOption = {};
	var oldIdx = -1;
	// public: car invoqués par les evenements
	this.MIN = +1000000000000;
	this.MAX = -1000000000000;
	this.App = {};
	this._Elt = document.getElementById(DIV);
	this.Conf = CONF;
	console.log("§§§ NEW __text", CONF);
	this.MIN = CONF.min.value;
	this.MAX = CONF.max.value;
	var input = CONF.init;
	this._Elt.innerHTML = "<input id='" + ID + "' type='text'" + " value='"
		+ input.value + "' class='" + SCLASS + "'"
		+ " STYLE='color: " + input.color + "; background-color: "
		+ input.background_color + ";' size='" + input.size + "'"
		+ " maxlength='" + input.maxlength + "'>";
	this._Elt = document.getElementById(ID); // point to element
	/*---------------------------------------------------------------
		SET text
	 */
	this._setText = function(CHN) {
		SELF._Elt.value = CHN;
		SELF._formText();
	}
	/*---------------------------------------------------------------
		FORMAT text
	 */
	this._formText = function() {
		var value = parseInt(SELF._Elt.value, 10);
		value = parseFloat(SELF._Elt.value, 10);
		if (Number.isNaN(value)) {
			SELF._Elt.style.background = SELF.Conf.error.background_color;
			SELF._Elt.style.color = SELF.Conf.error.color;
		} else if (value < SELF.MIN) {
			SELF._Elt.style.background = SELF.Conf.min.background_color;
			SELF._Elt.style.color = SELF.Conf.min.color;
		} else if (SELF.MAX < value) {
			SELF._Elt.style.background = SELF.Conf.max.background_color;
			SELF._Elt.style.color = SELF.Conf.max.color;
		} else {
			SELF._Elt.style.background = SELF.Conf.init.background_color;
			SELF._Elt.style.color = SELF.Conf.init.color;
		}
	}
	/*---------------------------------------------------------------
		DEFAULT events
	 */
	this.__textDefaultevents = function(APP) {

		SELF.App = APP;

		SELF._Elt.onchange = function() {
			SELF._formText("***");
		};

	}
};
/** ===================================================================
 * @fn CLASS function __radiobtns(DIALOGS, DIV, IDX,LABELS, VALUES, OP_KEY, FUNCTOR)
 * @param DIALOGS others dialogs containing the same control ({} if none)
 * @param DIV of dialog container
 * @param IDX of radio button to be chechecked at start
 * @param LABELS Array of labels of radio buttons
 * @param VALUES Array of values of radio buttons
 * @param OP_KEY object key of variable to be setted
 * @param FUNCTOR to be executed to set OP_KEY
 * ====================================================================
 */
function __radiobtns(DIALOGS, DIV, IDX, LABELS, VALUES, OP_KEY, FUNCTOR) {
	var SELF = this;
	this._Elt = document.getElementById(DIV);
	var chk;
	this._Elt.innerHTML = '';
	this.Elts_radio_id = new Array();
	for (var i = 0; i < LABELS.length; i++) {
		if (IDX == i)
			chk = ' checked="true"';
		else
			chk = ' checked="false"';
		this._Elt.innerHTML += '<label class="radiobtn">' + LABELS[i]
			+ '<input id="' + DIV + '.' + i + '" name="' + DIV + '_radio" type="radio"' + chk + ' value="' + VALUES[i] + '">'// onclick="handleRadio(this);"
			+ '<span class="radiomark"></span></label>'
			;
		this.Elts_radio_id.push(DIV + '.' + i);
	}
	/** ---------------------------------------------------------------
	 * @fn this._setRadio = function(IDX,VAL, UPDATE, DIFFUSE)
	 * @brief set the radio choice value with CHN. 
	 * @param IDX of radio entry to be checked
	 * @param VAL value to setup
	 * @param UPDATE if true|undefined, set the OP_KEY parameter with the CHN value
	 * @param DIFFUSE if true spreads to identical controls in other views
	 */
	this._setRadio = function(IDX, VAL, UPDATE, DIFFUSE) {
		if (UPDATE != false)
			if (typeof FUNCTOR !== 'undefined')
				FUNCTOR(OP_KEY, VAL);
		for (var i = 0; i < LABELS.length; i++) {
			document.getElementById(SELF.Elts_radio_id[i]).checked = false;
		}
		document.getElementById(SELF.Elts_radio_id[IDX]).checked = true;
		if (DIFFUSE == true)
			for (const [dlg, ctl] of Object.entries(DIALOGS))
				if (ctl.radiobtns != SELF)
					ctl.radiobtns._setRadio(IDX, VAL, false, false);
	}
	/*---------------------------------------------------------------
		DEFAULT events
	 */

	this.__radioDefaultevents = function() {
		document.getElementById(DIV).addEventListener('click', function(event) {
			if (event.target && event.target.matches("input[type='radio']")) {
				var idx = -1;
				for (var i = 0; i < LABELS.length; i++) {
					if (SELF.Elts_radio_id[i] === event.target.id) {
						idx = i;
					}
				}
				if (event.target.checked)
					SELF._setRadio(idx, event.target.value, true, true);
			}
		});
	}
	this.__radioDefaultevents();

}
/** ===================================================================
 * 
 * CLASS CHECK
 * DIV of container
 * ID of targeted value
 * =================================================================
 */
function __checkbox(DIALOGS, DIV, LABEL, VAL, OP_KEY, FUNCTOR) {
	var SELF = this;
	this._Elt = document.getElementById(DIV);
	var chk;
	if (VAL == true)
		chk = ' checked="checked"';
	else
		chk = '';
	this._Elt.innerHTML = '<label class="checkbox">' + LABEL
		+ '<input id="' + DIV + '_check" type="checkbox"' + chk + '>'
		+ '<span class="checkmark"></span></label>'
		;
	this.Elt_check = document.getElementById(DIV + '_check');
	/** ---------------------------------------------------------------
	 * @fn function(CHN, UPDATE_THREEJS)
	 * @brief set the slider value with CHN. 
	 * @param CHN value to setup
	 * @param UPDATE if true|undefined, set the OP_KEY parameter with the CHN value
	 * @param DIFFUSE if true spreads to identical controls in other views
	 */
	this._setCheck = function(BOOL, UPDATE, DIFFUSE) {
		SELF.Elt_check.checked = BOOL;
		if (UPDATE != false)
			if (typeof FUNCTOR !== 'undefined')
				FUNCTOR(OP_KEY, BOOL);
		if (DIFFUSE == true)
			for (const [dlg, ctl] of Object.entries(DIALOGS))
				if (ctl.checkbox != SELF)
					ctl.checkbox._setCheck(BOOL, false, false);
	}
	/*---------------------------------------------------------------
		DEFAULT events
	 */
	this.__checkDefaultevents = function() {
		SELF.Elt_check.onchange = function() {
			SELF._setCheck(SELF.Elt_check.checked, true, true);
		};
	}
	this.__checkDefaultevents();
}
/*
 * =================================================================
 * 
 * CLASS RANGE
 * DIV of container
 * ID of targeted value
 * =================================================================
 */
function __range(DIALOGS, DIV, LABEL, MIN, VAL, STEP, MAX, OP_KEY, FUNCTOR) {
	// private:
	var SELF = this;
	var ID_slider = DIV + "_slider";
	var ID_value = DIV + "_value";
	this._Elt = document.getElementById(DIV);

	this._Elt.innerHTML =
		"<table>"
		+ "<tr>"
		+ "<td><input id='" + ID_slider + "' type='range' min='" + MIN + "' max='" + MAX + "'  value='" + VAL + "' step='" + STEP + "' style='width: 128px;'></td>"
		+ "<td><input id='" + ID_value + "' type='text' value='" + VAL + "'"
		+ " size='" + 13 + "'"
		+ " maxlength='" + 13 + "'  style='width: 48px;'></td>"
		+ "<td>" + LABEL + "</td>"
		+ "</tr>"
		+ "</table>"
		;


	this.Elt_slider = document.getElementById(ID_slider);
	this.Elt_value = document.getElementById(ID_value);
	/** ---------------------------------------------------------------
	 * @fn function(CHN, UPDATE_THREEJS)
	 * @brief set the slider value with CHN. 
	 * @param CHN value to setup
	 * @param UPDATE if true|undefined, set the OP_KEY parameter with the CHN value
	 * @param DIFFUSE if true spreads to identical controls in other views
	 */
	this._setRange = function(CHN, UPDATE, DIFFUSE) {
		SELF.Elt_slider.value = CHN;
		SELF.Elt_value.value = SELF.Elt_slider.value;
		if (UPDATE != false)
			if (typeof FUNCTOR !== 'undefined')
				FUNCTOR(OP_KEY, SELF.Elt_slider.value);
		if (DIFFUSE == true)
			for (const [dlg, ctl] of Object.entries(DIALOGS))
				if (ctl.range != SELF)
					ctl.range._setRange(CHN, false, false);
	}
	/*---------------------------------------------------------------
		DEFAULT events
	 */
	this.__rangeDefaultevents = function() {
		SELF.Elt_slider.oninput = function() {
			SELF._setRange(SELF.Elt_slider.value, true, true);
		};
		SELF.Elt_value.onchange = function() {
			SELF._setRange(SELF.Elt_value.value, true, true);
		};
	}
	this.__rangeDefaultevents();
};
/*
 * =================================================================
 * 
 * CLASS TEXTAREA
 * 
 * =================================================================
 */
function __textarea(SCLASS, DIV, ID, CONF) {
	// private:
	var SELF = this;
	var oldOption = {};
	var oldIdx = -1;
	// public: car invoqués par les evenements
	this.MIN = +1000000000000;
	this.MAX = -1000000000000;
	this.App = {};
	this._Elt = document.getElementById(DIV);
	this.Conf = CONF;
	console.log("§§§ NEW __textarea", CONF);
	this.MIN = CONF.min.value;
	this.MAX = CONF.max.value;
	var input = CONF.init;
	var head = "<textarea id='" + ID + "' cols='" + input.cols + "' rows='"
		+ input.rows + "'" + " class='mv2_select " + SCLASS + "'"
		+ " STYLE='color: " + input.color + "; background-color: "
		+ input.background_color + "'" + " maxlength='" + input.maxlength
		+ "'>";
	var foot = "</textarea>";
	this._Elt.innerHTML = head + input.value + foot;
	this._Elt = document.getElementById(ID); // point to element
	/*---------------------------------------------------------------
	 SET text
	 */
	this._setText = function(CHN) {
		SELF._Elt.innerHTML = CHN;
	}
	/*---------------------------------------------------------------
	 FORMAT text
	 */
	this._formText = function() {
		var value = parseInt(SELF._Elt.value, 10);
		value = parseFloat(SELF._Elt.value, 10);
		if (Number.isNaN(value)) {
			SELF._Elt.style.background = SELF.Conf.error.background_color;
			SELF._Elt.style.color = SELF.Conf.error.color;
		} else if (value < SELF.MIN) {
			SELF._Elt.style.background = SELF.Conf.min.background_color;
			SELF._Elt.style.color = SELF.Conf.min.color;
		} else if (SELF.MAX < value) {
			SELF._Elt.style.background = SELF.Conf.max.background_color;
			SELF._Elt.style.color = SELF.Conf.max.color;
		} else {
			SELF._Elt.style.background = SELF.Conf.init.background_color;
			SELF._Elt.style.color = SELF.Conf.init.color;
		}
	}
	/*---------------------------------------------------------------
	 DEFAULT events
	 */
	this.__textDefaultevents = function(APP) {

		SELF.App = APP;

		SELF._Elt.onchange = function() {
			SELF._formText("***");
		};

	}
};
/*
 * =================================================================
 * 
 * CLASS SELECT Crée le select/options et surcharge kmd.OP avec KMD_OP si il est
 * défini (cas des dbgLVL socket ou global)
 * 
 * =================================================================
 */
function __select(SCLASS, DIV, ID, MODULE, CONF, KMD_OP) {
	var SELF = this;
	var oldOption = {};
	var oldIdx = -1;
	// public: car invoqués par les evenements
	this.App = {};
	this._Elt = document.getElementById(DIV);
	this.cfgItems = clone(CONF);// clôner la CONF !!!!!!!pour ne pas la
	// surcharger !!!!
	this.SELKEY = "";// default choosen option !!!
	this.keyIDX = new Array();
	this.idxKEY = new Array();
	this.sta = {};
	var idx = 0;
	for (var key in CONF) {
		this.keyIDX[idx] = key;
		this.idxKEY[key] = idx;
		if (isObject(CONF[key].kmd)) {
			if (isDefined(KMD_OP))
				this.cfgItems[key].kmd.OP = KMD_OP;
		}
		if (isObject(CONF[key].opt)) {
			if (CONF[key].opt.sel)// ==1 !!!
				this.SELKEY = key;
		}
		if (isObject(CONF[key].help)) {
			this.sta[CONF[key].help.sta] = key;
		}
		console.log("----> option   idx[", idx, "]=", key, CONF[key]);
		idx++;
	}
	if (this.SELKEY != "")
		console.log("====> Choosen    SELKEY=", this.SELKEY, CONF[this.SELKEY]);
	else
		console.log("====> No default SELKEY.");
	this._Elt.innerHTML = filesToHtmlSelect(ID, this.cfgItems, this.SELKEY, SCLASS);
	this._Elt = document.getElementById(ID); // point to list select ->
	// options

	this._cfgItemByIdx = function(IDX) {
		return SELF.cfgItems[SELF.keyIDX[IDX]];
	}
	this._itemBySta = function(STA) {
		return SELF.cfgItems[SELF.sta[STA]];
	}
	this._tagOption = function(OPTION, IDX) {
		var idx;
		if (isDefined(IDX))
			idx = IDX;
		else {
			idx = SELF._Elt.selectedIndex;// tag the first one idx=0 !!!
			alert("_tagOption must specify an index IDX");
		}
		SELF._Elt[idx].text = OPTION.entry;
		SELF._Elt.style.background = OPTION.background;
		return OPTION.entry;
	}
	/*
	 * ================================================================= 
	 * SELECT AN OPTION
	 * =================================================================
	 */
	this.selectOpt = function(IDX, SEND) {
		console.info("selectOpt(", SEND, ",", MODULE, ") [", IDX, "]:", SELF
			._cfgItemByIdx(IDX));
		SELF._Elt.selectedIndex = IDX;
		return SELF._tagOption(SELF._cfgItemByIdx(IDX).opt, IDX);
		/*
		 * var op = SELF._cfgItemByIdx(IDX).kmd.OP; var KMD = {}; KMD[op] =
		 * parseInt(IDX, 10); if (SEND) SELF.App.Query( MODULE, KMD);// 0 ????
		 * MODULE // Status!!!
		 */
	}
	/*
	 * =================================================================
	 * ACTIVATE EVENTS with SEND
	 * =================================================================
	 */
	this.__selectDefaultevents = function(APP) {
		SELF.App = APP;
		SELF._Elt.onchange = function() {
			SELF.selectOpt(SELF._Elt.value, true);// à l'appel par événement
			// toujours "" !!!
			// value est l'indice dans le tableau des options
		};
	}
	/*
	 * =================================================================
	 * ACTIVATE EVENTS without SEND
	 * =================================================================
	 */
	this.__selectNosendevents = function(APP) {
		SELF.App = APP;
		SELF._Elt.onchange = function() {
			SELF.selectOpt(SELF._Elt.value, false);// à l'appel par événement
			// toujours "" !!!
			// value est l'indice dans le tableau des options
		};
	}
};
///////////////////////////////////////////////////////////////////////
;
SocketCLI = function(NAME, WS_URL, Q_TYP, SLOT) {
	var SELF = this;
	this._name = NAME;
	this.cntWScnx = 0;
	this.cntErrors = 0;
	this.slot = SLOT;
	var WS;//CONNECT() to be passed by reference to the socket slot !!!
	this.PROTOCOL = Q_TYP;
	this.sendCLI = function(CHN) {
		try {
			SELF.WS.send(CHN);
		} catch (e) {
			addLog(0, '@@@ sendCLI to [<font color="red">', SELF.WS.url + ' injoignable (pas de réseau?)', '</font>]');
			addLog(0, 'DOMException(' + e.code + ':' + e.name + ') [<font color="red">', e.message, '</font>]');
		}
	}
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
		try {
			if (!navigator.onLine) {
			   alert("You are Offline");
			}
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
				//console.log("<-[ " + evt.data + " ]");
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
				SELF.Status("Close (" + event.code + ") " + event.reason);
				SELF.stop("CLOSE(" + event.code + ",'" + event.reason + "'," + event.wasClean + ")");
				setTimeout(function() {
					SELF.CONNECT();
				}, 1000);
			};
			//
			// Client ON_ERROR SOCKET
			//
			SELF.WS.onerror = function(event) {
				SELF.stop('ERROR');
				SELF.WS.close();
			};
		} catch (e) {
			console.error(e);
			addLog(0, '@@@ WS connet error [<font color="red">', e, '</font>]');

		}
	}
};
;
/*
var STATUX = Object.freeze({
	XOK: 0,
	KO: 1,
	ER_J_LOST: 2,
	RUNNING: 3,
	HALTED: 4,
	ER_UNKNOWN: 5,
	DEBUGON: 6,
	SW_RESET: 7,
	SW_UPLOAD: 8,
	NEW: 9,
	SW_NOSWITCHES: 10,
	ER_RESETING: 11,
	ER_PARSING: 12,
	ER_MODULE: 13,
	ER_TIMEOUT: 14,
	ER_KMD: 15,
	FS_ROPEN: 16,
	ER_FOPEN: 17,
	M__REMOTE: 18,
	M__AUTO: 19
});
*/
// -----------------------------------------------------------------
/**
 * MODULE timerChange
 */
function timerChange(THIS, IDX) {
	// console.log("[", IDX, "]:", THIS.EXE._cfgItemByIdx(IDX));
	// var IDX = THIS.TIM._Elt.value;
	console.log("[", IDX, "]:", THIS.TIM._cfgItemByIdx(IDX));
	THIS.TIM._tagOption(THIS.TIM._cfgItemByIdx(IDX).opt, IDX);
	THIS.TIM.oldIdx = IDX;
	var tmsOUT = parseInt(THIS.TIM._cfgItemByIdx(IDX).kmd.codeTMS, 10);
	if (tmsOUT > 0)
		THIS._Timer.START_loop(tmsOUT);
	else if (tmsOUT == 0) {
		THIS._Timer.START_oneShoot(tmsOUT);
		THIS.TIM.selectedIndex = 0;// marche pas???
		THIS.TIM.selectOpt(0, false);// marche ???
	} else
		THIS._Timer.STOP();
}
// -----------------------------------------------------------------
/**
 * exeEvents default launching
 */
function set_defaultEvents(THIS, APP) {
	THIS.App = APP;
	THIS.__OPchange(THIS.EXE.idxKEY[THIS.EXE.SELKEY]);
	if (isDefined(THIS.EXE.cfgItems[THIS.EXE.SELKEY].timer)) {
		var idx = THIS.EXE.cfgItems[THIS.EXE.SELKEY].timer;// parseInt(tim.nodeValue,
		// 10);
		THIS.TIM.selectedIndex = idx;// marche pas???
		THIS.TIM.selectOpt(idx, false);// marche ???
		timerChange(THIS, idx);
	}
}
// -----------------------------------------------------------------
/**
 * exeEvents events launching
 */
function exeEvents(THIS, APP) {
	// default options activate
	set_defaultEvents(THIS, APP);
	// event EXEC change
	THIS.EXE._Elt.onchange = function() {
		THIS.__OPchange(THIS.EXE._Elt.value);
	}
	// event TIMER change
	THIS.TIM._Elt.onchange = function() {
		timerChange(THIS, THIS.TIM._Elt.value);
	}
}
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT SOCKET
 */
Slot_socket = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO, SOCKET,
	BAMOUNT_TMS) {
	var SELF = this;
	var BAmount_TMS = BAMOUNT_TMS;
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.EXE = EXE;
	this.TIM = TIM;
	this.TIO = TIO;
	this.sokROVER = SOCKET;//SocketCLI
	this._Timer = new Timer();
	MODULES[MID] = this;
	// -----------------------------------------------------------------
	/**
	 * Slot socket.Query
	 */
	this.Query = function(MID, KMD) {
		var statusOBJ = {};
		statusOBJ = send_QUERY(this.sokROVER, MID, KMD, BAmount_TMS);
		// SELF.TIO._setText("+++++++");
	}
	// -----------------------------------------------------------------
	/**
	 * Slot socket ONESHOT function
	 */
	this.__oneShot = function() {
		SELF.TIO._setText("+++++++");
	}
	// -----------------------------------------------------------------
	/**
	 * Slot socket. EXEC events
	 */
	this.affStatus = function(CHN) {
		SELF.TIO._setText(CHN);
	}
	this.stopSender = function(CHN) {
		SELF.TIO._setText(CHN);
	}
	this.addMsg = function(HEAD, CHN, FOOT) {
		addLog(0, HEAD, CHN, FOOT);
	}
	this.node__exeEvents = function(APP) {
		SELF.App = APP;
		if (isDefined(this.sokROVER))
			this.sokROVER.CONNECT();
	}
	this.__exeEvents = function(APP) {//Slot_socket
		if (isDefined(this.sokROVER))
			this.sokROVER.slot = this;
		this.node__exeEvents(APP);
		// -----------------------------------------------------------------
		/**
		 * Slot socket. event Connecter Fermer
		 */
		SELF.EXE._Elt.onchange = function() {
			var idx = SELF.EXE._Elt.value;
			// SELF.EXE.select(idx);
			console.log("[", idx, "]:", SELF.EXE._cfgItemByIdx(idx));
			SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(idx).opt, idx);
			SELF.EXE.oldIdx = idx;

			switch (parseInt(idx, 10)) {
				case 0:
					if (SELF.App.Init()) {// attend déjà 5s
						// attendre la connexion effective (setTimeout)
						// setTimeout( loo p, 5000);// ATTENTION pas lo op() car ça
						// provoque un démarrage immédiat !!!
					} else {
						alert("Pas de connexion. Faire un reset du rover & recharger le pilote.");
					}
					break;
				case 1:
					// clearInterval(scheduler);
					SELF.App.Shutdown();
					SELF.EXE._tagOption(SELF.EXE.cfgItems.CLOSED.help);
					break;
				default:
					console.log('???????????');
					SLOTROV.Query({
						"set": parseInt(SELF.EXE._cfgItemByIdx(idx).kmd.codeRYG, 10)
					});

			}
		};
		// -----------------------------------------------------------------
		/**
		 * Slot socket. TIMER event & callback Fréquence du Ping
		 */
		SELF._Timer.CHANGE(SELF.__oneShot); // init callback !!!
		SELF.TIM._Elt.onchange = function() {
			var idx = SELF.TIM._Elt.value;
			console.log("[", idx, "]:", SELF.TIM._cfgItemByIdx(idx));
			SELF.TIM._tagOption(SELF.TIM._cfgItemByIdx(idx).opt, idx);
			var tmsOUT = parseInt(SELF.TIM._cfgItemByIdx(idx).kmd.codeTMS, 10);
			var KMD = {};
			KMD[SELF.TIM._cfgItemByIdx(idx).kmd.OP] = tmsOUT;
			SLOTROV.Query(KMD);
			if (tmsOUT >= 0) {// lancer une fois avant l'interval
				SELF.__oneShot();
			}
			if (tmsOUT > 0) {
				SELF._Timer.START_loop(tmsOUT);
			} else {
				SELF._Timer.STOP();
			}
		};
	}
	// -----------------------------------------------------------------
	/**
	 * Slot socket. Activation events
	 */
	this.launchEvents = function(APP) {//Slot_socket
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		this.__exeEvents(APP);//Slot_socket
	}
};
// /////////////////////////////////////////////////////////////////////
function ALERT(SLOTWS,MID,KMD,MSG){
	
}
/**
 * SLOT WATCHDOG
 */
Slot_watchdog = function(MODULES, MID, DBG_loc, DBG_rov, TIM, TIO, SLOTROV,
	SLOTBAS) {
	// register
	MODULES[MID] = this;
	// private:
	var SELF = this;
	// public::
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.TIO = TIO;
	this.TIM = TIM;
	this._Timer = new Timer();
	// -----------------------------------------------------------------
	/**
	 * WATCHDOG reply2query nothing to do yet
	 */
	this.reply2query = function(KMD) {
		SELF.TIO._setText("WATCHDOG > [" + JSON.stringify(KMD) + "]");
		var kmd = KMD;
		return kmd;
	}
	// -----------------------------------------------------------------
	/**
	 * WATCHDOG Activation events
	 */
	this.launchEvents = function(APP) {//Slot_watchdog
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * MY ALERT
 */
class my_ALERT{
 	constructor(DIVID,MSGID,BTNID) {
		this.divElt=document.getElementById(DIVID);
		this.msgElt=document.getElementById(MSGID);
		this.btnElt=document.getElementById(BTNID);
		this.btnElt.addEventListener("click",  this.clear.bind(this));
		this.callback=function(){}; // for test
	}
	set(MSG,CALLBACK){
		this.divElt.style.display = 'inline';
		this.msgElt.innerHTML = MSG;
		this.callback=CALLBACK;
	}
	clear(){
		this.divElt.style.display = 'none';
		this.callback();
	}
}
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT DRIVER
 */
Slot_driver = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO, JOYSTICK,
	CANVAS_PILOT, CANVAS_MOTORS, CANVAS_IMU, CANVAS_MAP, SLOTROV, SLOTBAS) {
	// register
	MODULES[MID] = this;
	// private:
	var SELF = this;
	//var infoTraj_ptr = 0;
	//var lostTraj_cnt = 0;
	// public::
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.TIO = TIO;
	this.TIM = TIM;
	this._Timer = new Timer();
	this.static_ALERT=false;
	this.static_MSG='jsglkfsql';
	this.static_CNT;
	this.sweetALERT=function(){
		this.static_ALERT=false;
		enableChildren('IMU_head');
	}
	// -----------------------------------------------------------------
	/**
	 * DRIVER reply2query nothing to do yet
	 */
	this.reply2query = function(KMD) {
		if (KMD['endCalPt'] !== undefined) {
			if((calMag_START)||(rawMag_Flag)||(rawMag_FlagShift)||(calMag_Flag)||(this.static_ALERT))// if threejs has not finish
				KMD['endCalPt']=false;
			else
				KMD['endCalPt']=true;
			return KMD;
		}
		else {
			if((calMag_START)||(rawMag_Flag)||(rawMag_FlagShift)||(calMag_Flag)||(this.static_ALERT))// if threejs has not finish
				alert('FATAL ERROR a flag is true... DEBUG THAT: '+calMag_START+' '+rawMag_Flag+' '+rawMag_FlagShift+' '+calMag_Flag+' '+this.static_ALERT+' !!!');
		}
		if (KMD["OP"] === "calMag_STOP") {
			IMU.message(KMD['MSG'],5000);
			this.static_ALERT=true;
			this.static_MSG=KMD['MSG'];
			_ALERT.set(	this.static_MSG,this.sweetALERT.bind(this));
		}
		else if (KMD["OP"] === "calMag_START") {
			this.static_CNT=KMD['CNT'];
			IMU.message(KMD['MSG']);
			calMag_START = true;
		}
		else if (KMD["OP"] === "calMag_CHECKIN") {
			IMU.message('Acquisition '+KMD['IDX']+ '/'+this.static_CNT);
			rawMag_Data[rawMag_ptr++] = KMD[DEF_KF_Imx];
			rawMag_Data[rawMag_ptr++] = KMD[DEF_KF_Imy];
			rawMag_Data[rawMag_ptr++] = KMD[DEF_KF_Imz];
			rawMag_Count++;
			rawMag_Flag = true;
		}
		else if (KMD["OP"] === "calMag_SHIFT") {
			rawMag_Data[rawMag_ptr++] = KMD[DEF_KF_Imx];
			rawMag_Data[rawMag_ptr++] = KMD[DEF_KF_Imy];
			rawMag_Data[rawMag_ptr++] = KMD[DEF_KF_Imz];
			rawMag_ptr-=3;
			rawMag_FlagShift = true;
			IMU.sound('_CM_AV.mp3');
		}
		else if (KMD["OP"] === "calMag_CHECKOUT") {
			IMU.message('Vérification '+KMD['IDX']+ '/'+this.static_CNT);
			calMag_Data[calMag_ptr++] = KMD[DEF_KF_Imx];
			calMag_Data[calMag_ptr++] = KMD[DEF_KF_Imy];
			calMag_Data[calMag_ptr++] = KMD[DEF_KF_Imz];
			calMag_Count++;
			calMag_Flag = true;
		}
		delete KMD[DEF_KF_Imx]
		delete KMD[DEF_KF_Imy]
		delete KMD[DEF_KF_Imz]
		KMD['endCalPt']=false;
		return KMD;
	}
	/** ---------------------------------------------------------------
	 * @fn this._OP_getCTXparam = function(PARAM)
	 * @brief Sends a ws QUERY to rover to get PARAM parameter value	sokROVER
	 * @param VALUE parameter value
	 */
	/*
		this._OP_getCTXparam = function(PARAM) {
			//		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
			var kmd = {
				OP: 'getPARAM',
				PARAM: PARAM
			};
			MODULES["wsR"].Query(MID, kmd);
		}
	*/
	/** ---------------------------------------------------------------
	 * @fn this._OP_setCTXparam = function(PARAM, VALUE)
	 * @brief Sends a ws QUERY to rover to set PARAM parameter	sokROVER
	 * @param PARAM parameter name
	 * @param VALUE parameter value
	 */
	this._OP_setCTXparam = function(PARAM, VALUE) {
		//		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: 'setPARAM',
			PARAM: PARAM,
			VALUE: VALUE
		};
		MODULES["wsR"].Query(MID, kmd);
		/*
				SELF.SOKs.forEach(function(sok) {
					MODULES[sok].Query(MID, kmd);
				});
		*/		// SELF.TIO._setText("TMS=" + TMS);
	}
	// -----------------------------------------------------------------
	/**
	 * DRIVER update2reply display driver intern parameters
	 */
	this.update2reply = function(KMD) {
		if (KMD['OP'] === 'setPARAM') {
			if (!KMD['X'])
				alert('ERROR setParam(\'' + KMD['PARAM'] + '\') ???');
			return;
		}
		else if (KMD[DEF_KF_PMTS] === undefined) {
			SELF.TIO._setHtml("STANDBY IMU DESACTIVEE...");
			CANVAS_IMU.Init2D();
			CANVAS_MAP.Init2D();
			return;
		}
		else
			this.updateAttitude(KMD);
		if ((KMD[DEF_KF_EXE] == "roaming") || (KMD[DEF_KF_EXE] == "working") || (KMD[DEF_KF_EXE] == "idle"))
			MODULES["MOT"].update2reply(KMD);
		MODULES["TMS"].update2reply(KMD);
	}
	// -----------------------------------------------------------------
	/**
	 * DRIVER Attitude updtae
	 * @fn updateAttitude
	 */
	this.updateAttitude = function(KMD) {
		//
		//	----	TEXT DIALOG ---------------------------------------
		//
		var DMP = "";
		DMP += "Ts=" + SF_float(KMD[DEF_KF_PMTS] / 1000000.0, 10, 3) + " sec";
		DMP += "\n----------------ATTITUDE--------------";
		DMP += "\nPos(" + SF_float(KMD[DEF_KF_XC0], 10, 3) + SF_float(KMD[DEF_KF_YC0], 10, 3) + SF_float(KMD[DEF_KF_ZC0], 10, 3) + ")m";
		DMP += "\nVel(" + SF_float(KMD[DEF_KF_SPX], 10, 3) + SF_float(KMD[DEF_KF_SPY], 10, 3) + SF_float(KMD[DEF_KF_SPZ], 10, 3) + ")mm/s";
		DMP += "\n<font color='red'>Acc(" + SF_float(KMD[DEF_KF_LAX], 10, 3) + SF_float(KMD[DEF_KF_LAY], 10, 3) + SF_float(KMD[DEF_KF_LAZ], 10, 3) + ")mm/s²</font>";
		//		DMP += "\nAcc(" + SF_float(KMD[DEF_KF_LAX], 10, 3) + SF_float(KMD[DEF_KF_LAY], 10, 3) + SF_float(KMD[DEF_KF_LAZ], 10, 3) + ")mm/s²";
		DMP += "\nRPY(" + SF_float(KMD[DEF_KF_ROLL], 10, 3) + SF_float(KMD[DEF_KF_PITCH], 10, 3) + SF_float(KMD[DEF_KF_YAW], 10, 3) + ")°";
		DMP += "\nGrv(" + SF_float(KMD[DEF_KF_GRX], 10, 3) + SF_float(KMD[DEF_KF_GRY], 10, 3) + SF_float(KMD[DEF_KF_GRZ], 10, 3) + ")mg";
		DMP += "\n-----------------SENSORS--------------";
		DMP += "\nRad=" + SF_float(KMD[DEF_KF_DIST], 10, 3) + " m, Temp="+SF_float(KMD['temp'] , 8, 2)+" °C";
		//		DMP += "\nAcc(" + SF_float(KMD[DEF_KF_Iax], 10, 3) + SF_float(KMD[DEF_KF_Iay], 10, 3) + SF_float(KMD[DEF_KF_Iaz], 10, 3) + ")g";
		DMP += "\n<font color='red'>Acc(" + SF_float(KMD[DEF_KF_Iax], 10, 3) + SF_float(KMD[DEF_KF_Iay], 10, 3) + SF_float(KMD[DEF_KF_Iaz], 10, 3) + ")g</font>";
		DMP += "\nGyr(" + SF_float(KMD[DEF_KF_Igx], 10, 3) + SF_float(KMD[DEF_KF_Igy], 10, 3) + SF_float(KMD[DEF_KF_Igz], 10, 3) + ")°/s";
		DMP += "\nMag(" + SF_float(KMD[DEF_KF_Imx], 10, 3) + SF_float(KMD[DEF_KF_Imy], 10, 3) + SF_float(KMD[DEF_KF_Imz], 10, 3) + ")mG";
		DMP += "\nQuat{" + SF_float(KMD["Q0"], 7, 3) + SF_float(KMD["Q1"], 7, 3) + SF_float(KMD["Q2"], 7, 3) + SF_float(KMD["Q3"], 7, 3) + "}";
		DMP += "\n-----------------MOTORS---------------";
		DMP += "\n  Left-Right(" + SF_float(KMD[DEF_KF_LFT], 10, 3) + SF_float(KMD[DEF_KF_RGT], 10, 3) + ")";
		DMP += "\n  Speed-Dir (" + SF_float(KMD[DEF_KF_SPD], 10, 3) + SF_float(KMD[DEF_KF_DIR], 10, 3) + ")";

		SELF.TIO._setHtml(DMP);
		//
		//	----	IMU 2D DIALOG -------------------------------------
		//

		CANVAS_IMU.moveMarkXY(0, KMD[DEF_KF_LAX], KMD[DEF_KF_LAY]);
		CANVAS_IMU.moveMarkXY(1, KMD[DEF_KF_GRX], KMD[DEF_KF_GRY]);
		CANVAS_IMU.moveMarkXY(2, KMD[DEF_KF_ROLL], KMD[DEF_KF_PITCH]);

		//
		//	----	MAP 2D DIALOG ---------------------------------------
		//
		var xx = KMD[DEF_KF_XC0];
		var yy = KMD[DEF_KF_YC0];
		var zz = KMD[DEF_KF_ZC0];
		var roll = KMD[DEF_KF_ROLL];
		var pitch = KMD[DEF_KF_PITCH];
		var yaw = KMD[DEF_KF_YAW];
		var dist = KMD[DEF_KF_DIST];
		var DistXY = 10 * dist * Math.cos(pitch * Math.PI / 180);
		var YawRD = yaw * Math.PI / 180;
		var xr = xx + DistXY * Math.cos(YawRD);
		var yr = yy + DistXY * Math.sin(YawRD);
		CANVAS_MAP.lineXY(2, xx, yy, xx + KMD[DEF_KF_Imx] / 10, yy + KMD[DEF_KF_Imy] / 10);
		CANVAS_MAP.ArcXY(1, xr, yr, .5, 0, 360);
		CANVAS_MAP.moveMarkXY(0, xx, yy);

		//
		//	----	WEBGL DIALOG ---------------------------------------
		//
		//		Injects all data for webgl visualization
		//
		while((traject_Queue.length)&&(traject_Queue[0]['_WGL_done']))
			traject_Queue.shift()

		KMD['_WGL_done']=false
		traject_Queue.push(Object.assign({}, KMD))
		document.getElementById("drvSemaphore").innerHTML = "drv "+traject_Queue.length.toString()+ "  "+traject_Count.toString()+" "+(traject_Filled?"full":"...");
		//traject_Queue[]=Object.assign({}, KMD)
/*
		if(!traject_Flag){
			if (traject_Count == traject_MaxPts) {
				traject_Filled = true
				traject_Count = 0;
				infoTraj_ptr = 0;
			} 
			Rover_positions[infoTraj_ptr++] = KMD[DEF_KF_XC0];
			Rover_positions[infoTraj_ptr++] = KMD[DEF_KF_YC0];
			Rover_positions[infoTraj_ptr++] = KMD[DEF_KF_ZC0];
			infoTraj_ptr -= 3;
			Rover_RPY[infoTraj_ptr++] = KMD[DEF_KF_ROLL];
			Rover_RPY[infoTraj_ptr++] = KMD[DEF_KF_PITCH];
			Rover_RPY[infoTraj_ptr++] = KMD[DEF_KF_YAW];
			infoTraj_ptr -= 3;
			Rover_gravity[infoTraj_ptr++] = KMD[DEF_KF_GRX];
			Rover_gravity[infoTraj_ptr++] = KMD[DEF_KF_GRY];
			Rover_gravity[infoTraj_ptr++] = KMD[DEF_KF_GRZ];
			infoTraj_ptr -= 3;
			Rover_north[infoTraj_ptr++] = KMD[DEF_KF_Imx];
			Rover_north[infoTraj_ptr++] = KMD[DEF_KF_Imy];
			Rover_north[infoTraj_ptr++] = KMD[DEF_KF_Imz];

			RADAR.push(KMD[DEF_KF_DIST]);
			//reordered Quaternion( x, y , z, w ) !!!
			Rover_quat.push(KMD["Q1"]);
			Rover_quat.push(KMD["Q2"]);
			Rover_quat.push(KMD["Q3"]);
			Rover_quat.push(KMD["Q0"]);

			traject_Count++;
			traject_Flag = true;
		}
		else{
			lostTraj_cnt++;
		}
		document.getElementById("drvSemaphore").innerHTML = traject_Queue.length.toString()+ "  "+traject_Count.toString()+ "("+lostTraj_cnt.toString()+" lost) "+(traject_Filled?"full":"...");
*/		
	}
	// -----------------------------------------------------------------
	/**
	 * DRIVER Activation events
	 */
	this.launchEvents = function(APP) {//Slot_driver
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT PILOT receives (DIR,VIT) from joy-stick signals and transforms in
 * (LEFT,RIGHT) motors values.
 */
Slot_pilot = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO, JOYSTICK,
	CANVAS_PILOT, CANVAS_MOTORS, SLOTROV, SLOTBAS) {
	// register
	MODULES[MID] = this;
	// private:
	var SELF = this;
	var _learnFile = "";
	var _qryIDX = 0;
	// public::
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.EXE = EXE;
	this.TIM = TIM;
	this.TIO = TIO;
	this._Timer = new Timer();// "parent"
	this.cntSQR = 0;
	var pulse = 20;
	var haut = 200.0;
	var scale = 50;
	this.LIN = new Array(0, scale, 0, -scale, 0);
	this.ROT = new Array(scale, 0, -scale, 0, scale);
	this.CNT = new Array(1, pulse, pulse, pulse, pulse);
	/*
	 * this.FCT= new Array(_square,_square); this.LIN = new Array(0, 50);
	 * this.ROT = new Array(50, 00); this.CNT = new Array(150, 150);
	 */
	this.cycleIDX = 0;
	// -----------------------------------------------------------------
	/**
	 * PILOT update2reply display motors speeds returned by rover
	 */
	this.update2reply = function(KMD) {
		switch (KMD.OP) {
			case DEF_OP_OPEN:// Learning
				if (_learnFile != KMD[DEF_KF_FIL]) {
					alert("Learning '" + _learnFile + "' error '" + KMD[DEF_KF_FIL] + "'?");
					_learnFile = "";
					_qryIDX = 0;
				} else {
					SELF._Timer.CHANGE(SELF._OP_driving);
					var idx = SELF.EXE._cfgItemByIdx(_qryIDX).timer;
					SELF.TIM.selectedIndex = idx;// marche pas???
					SELF.TIM.selectOpt(idx, false);// marche ???
					timerChange(SELF, idx);
				}
				break;
			case DEF_OP_CLOSE:// Learning
				_learnFile = "";
				SELF.TIO._setText(" < Learn STOPPED");
				break;
			default:
				SELF.TIO._setText("..." + JSON.stringify(KMD));
		}
		SELF.TIO._setText("Joystick(Run,Dir)=(" + (KMD[DEF_KF_SPD]) + "," + (KMD[DEF_KF_DIR]) +
			")" + " <> (Left,Right)=(" + (KMD[DEF_KF_LFT]) + "," + (KMD[DEF_KF_RGT]) + ")");
		CANVAS_PILOT.update([KMD[DEF_KF_SPD], KMD[DEF_KF_DIR]]);
		CANVAS_MOTORS.update([Math.round(KMD[DEF_KF_LFT]), Math
			.round(KMD[DEF_KF_RGT])]);
		// In driver or jobgoal mode&place the joystick
		if (KMD.OP == DEF_OP_CHK || KMD.OP == DEF_OP_INFO) {
			JOYSTICK._VAL = {
				'DIR': KMD[DEF_KF_DIR],
				'VIT': KMD[DEF_KF_SPD]
			};
			JOYSTICK.placeCursor();
		}
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT OP_close_Learning function asks rover to end learning
	 */
	this.OP_close_Learning = function() {
		if (_learnFile == "")
			alert("No learning in progress. Force stopping !!!");
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_CLOSE,
			TMS: TMS
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Query(MID, kmd);
		});
		SELF.TIO._setText("Ending learning TMS=" + TMS);
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT OP_open_Learning function asks rover to begin learning
	 */
	this.OP_open_Learning = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_OPEN,
			[DEF_KF_FIL]: _learnFile,
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Query(MID, kmd);
		});
		SELF.TIO._setText("Beginning learning on " + _learnFile);
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT _OP_driving steering or learning
	 */
	this._OP_driving = function() {
		var joystick = JOYSTICK.getVal();
		console.log("joystick", joystick);
		SLOTROV.Query("MOT", {
			OP: DEF_OP_MOT,
			[DEF_KF_SPD]: Math.round(joystick.VIT),
			[DEF_KF_DIR]: Math.round(joystick.DIR) // Sens trigo à vérifier !!!
		});
		SELF.TIO._setText("Joystick(Run,Dir)=(" + Math.round(joystick.VIT) + ","
			+ Math.round(joystick.DIR) + ") ");
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT _OP_waveTest driving
	 * 
	 */
	this._OP_waveTest = function() {
		if (SELF.cntSQR == 0) {
			oldLIN = SELF.LIN[SELF.cycleIDX];
			oldROT = SELF.ROT[SELF.cycleIDX];
			// same size for all arrays !!!
			SELF.cycleIDX = (SELF.cycleIDX + 1) % SELF.LIN.length;
		}
		var idx = SELF.cycleIDX;

		var lin = SELF.LIN[idx] + (oldLIN - SELF.LIN[idx])
			* (SELF.CNT[idx] - SELF.cntSQR) / SELF.CNT[idx];
		var rot = SELF.ROT[idx] + (oldROT - SELF.ROT[idx])
			* (SELF.CNT[idx] - SELF.cntSQR) / SELF.CNT[idx];

		var kmd = {
			[DEF_KF_SPD]: lin,
			[DEF_KF_DIR]: rot
		};
		JOYSTICK.placeCursor();
		SLOTROV.Query("MOT", Object.assign({ OP: DEF_OP_MOT }, kmd));
		SELF.TIO._setText("Square(Run,Dir)=(" + kmd[DEF_KF_SPD] + ","
			+ kmd[DEF_KF_DIR] + ") ");
		SELF.cntSQR = (SELF.cntSQR + 1) % SELF.CNT[idx];
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT reply2query nothing to do yet
	 */
	this.reply2query = function(KMD) {
		var kmd = KMD;
		return kmd;
	}
	// -----------------------------------------------------------------
	/**
	 * Slot pilot __OPchange
	 */
	this.__OPchange = function(IDX) {
		console.log("[", IDX, "]:", SELF.EXE._cfgItemByIdx(IDX));
		SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(IDX).opt, IDX);
		var kmd = SELF.EXE._cfgItemByIdx(IDX).kmd;
		switch (kmd.OP) {
			case DEF_OP_TSTSQ:
				SELF._Timer.CHANGE(SELF._OP_waveTest);// generate square wave
				break;
			case DEF_OP_OPEN:// Learning
				if (_learnFile != "") {
					SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(_qryIDX).opt,
						_qryIDX);
					alert("Stop previous learning '" + _learnFile + "' please...");
				} else {
					_learnFile = SELF.EXE._cfgItemByIdx(IDX).opt.entry;
					_qryIDX = IDX;
					SELF.SOKs = kmd.soks;
					SELF._Timer.CHANGE(SELF.OP_open_Learning);
					SELF._Timer.START_oneShoot(10);
				}
				break;
			case DEF_OP_CLOSE:// Learning
				SELF._Timer.CHANGE(SELF.OP_close_Learning);
				SELF.SOKs = kmd.soks;
				SELF._Timer.START_oneShoot(10);
				break;
			case DEF_OP_PILOT:
				SELF._Timer.CHANGE(SELF._OP_driving); // drive via joy-stick
				break;
			default:
				alert("Sorry, `" + kmd.OP + "` unkwnon !!");
		}
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT Activation events
	 */
	this.launchEvents = function(APP) {//Slot_pilot
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		exeEvents(SELF, APP);//Slot_pilot
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT DATIM
 */
Slot_datim = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO, SLOTROV,
	SLOTBAS) {
	// register
	MODULES[MID] = this;
	// private:
	var SELF = this;
	// public:
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.EXE = EXE;
	this.TIM = TIM;
	this.TIO = TIO;
	this.SOKs;
	this._Timer = new Timer();
	// -----------------------------------------------------------------
	/**
	 * Slot datim update2reply display dTMS latency returned by rover
	 */
	this.update2reply = function(KMD) {
		SELF.TIO._setText("DATIM < dTMS='" + KMD.DT + "' ms. MTS=" + KMD.MTS);
	}
	// -----------------------------------------------------------------
	/**
	 * Slot datim ONESHOT function Synchronize rover clock with pilot client \brief
	 * add scheduler TMSOUT to TMS to keep latency coherence
	 */
	this._OP_synchronize = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_SYNC,
			TMS: TMS
		};
		SELF.TIO._setText("DATIM > TMS=" + TMS);
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Query("TMS", kmd);
		});
	}
	// -----------------------------------------------------------------
	/**
	 * Slot datim reply2query OP is Syn only
	 */
	this.reply2query = function(KMD) {
		var kmd = KMD;
		kmd.DT = (new Date()).getTime() - kmd.TMS;
		return kmd;
	}
	// -----------------------------------------------------------------
	/**
	 * Slot datim __OPchange
	 */
	this.__OPchange = function(IDX) {
		console.log("[", IDX, "]:", SELF.EXE._cfgItemByIdx(IDX));
		SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(IDX).opt, IDX);
		var kmd = SELF.EXE._cfgItemByIdx(IDX).kmd;
		switch (kmd.OP) {
			case DEF_OP_SYNC:
				SELF._Timer.CHANGE(SELF._OP_synchronize);
				SELF.SOKs = kmd.soks;
				break;
			default:
				alert("Sorry, `" + kmd.OP + "` unkwnon !!");
		}
	}
	// -----------------------------------------------------------------
	/**
	 * Slot datim Activation events
	 */
	this.launchEvents = function(APP) {//Slot_datim
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		exeEvents(SELF, APP);//Slot_datim
		//##		this._OP_synchronize();
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT JOBGOAL
 */
Slot_jobgoal = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO,
	JOYSTICK, CANVAS_PILOT, CANVAS_MOTORS, SLOTROV, SLOTBAS) {
	// register
	MODULES[MID] = this;
	// private:
	var SELF = this;
	var _workFile = "";
	var _qryIDX = 0;
	var _qryLine = -1;
	// public:
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.EXE = EXE;
	this.TIM = TIM;
	this.TIO = TIO;
	this.SOKs;
	this._Timer = new Timer();
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL reply2query OP is ack to log jobgoal echo only
	 */
	this.reply2query = function(KMD) {
		SELF.TIO._setText("JOBGOAL > [" + JSON.stringify(KMD) + "]");
		// Check jobgoal file reading progression
		if ((KMD[DEF_KF_GET] - _qryLine) == 1) {
			_qryLine++;
		} else {

		}
		var kmd = KMD;
		kmd[DEF_KF_EXE] = true;
		return kmd;
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL update2reply display jobgoal echo from rover inutile jobgoal
	 * renverra le n° de ligne DEF_KF_GET
	 */
	this.update2reply = function(KMD) {
		var info = document.getElementById("divInfo_WORKJOB");
		switch (KMD.OP) {
			case "Report":
				info.innerHTML += '\n<br>' + KMD[DEF_KF_GET] + ":" + strReplace(KMD["Report"], " ", "&nbsp;");
				break;
			case DEF_OP_OPEN:
				info.innerHTML = "\nOPEN " + KMD[DEF_KF_FIL] + ".";
				if (_workFile != KMD[DEF_KF_FIL]) {
					_workFile = "";
					_qryIDX = 0;
				} else {
					_qryLine = 1;
					SELF._Timer.CHANGE(SELF._OP_scanRover);
					timerChange(SELF, _qryIDX);// faux yl y a une indirection en
					// +!!
				}
				break;
			case DEF_OP_CLOSE:
				_workFile = "";
				info.innerHTML += '\n<br>' + "CLOSE " + KMD[DEF_KF_FIL] + ".";
				break;
			case DEF_KF_GET:
				info.innerHTML += '\n<br>' + KMD[DEF_KF_GET] + " WORK file " + KMD[DEF_KF_FIL];
				break;
			default:
				break;
		}
		info.scrollTop = info.scrollHeight - info.clientHeight;
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL OP_close_Jobgoal function QUERY to rover to end job
	 */
	this.OP_close_Jobgoal = function() {
		if (_workFile == "")
			return true;
		if (window.confirm("Stop the working job '" + _workFile + "' ???")) {
			var kmd = { OP: DEF_OP_CLOSE };
			SELF.SOKs.forEach(function(sok) {
				MODULES[sok].Query(MID, kmd);
			});
			SELF.TIO._setText("Query to stop job '" + _workFile + "' near line " + _qryLine);
			return true;
		}
		return false;
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL OP_open_Jobgoal function QUERY to rover to begin job
	 */
	this.OP_open_Jobgoal = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_OPEN,
			[DEF_KF_FIL]: _workFile,
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Query(MID, kmd);
		});
		SELF.TIO._setText("Query to start job '" + _workFile + "'");
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL _OP_scanRover function QUERY to rover get status during job
	 */
	this._OP_scanRover = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_OK,
			TMS: TMS
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Query(MID, kmd);
		});
		// SELF.TIO._setText("TMS=" + TMS);
	}
	// -----------------------------------------------------------------
	/**
	 * Slot jobgoal __OPchange when selecting a work file
	 */
	this.__OPchange = function(IDX) {
		console.log("[", IDX, "]:", SELF.EXE._cfgItemByIdx(IDX));
		SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(IDX).opt, IDX);
		var kmd = SELF.EXE._cfgItemByIdx(IDX).kmd;
		switch (kmd.OP) {
			case DEF_OP_OPEN:
				if (_workFile != "") {
					SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(_qryIDX).opt, _qryIDX);
					alert("Stop first the previous job '" + _workFile + "'...");
				} else {
					_workFile = SELF.EXE._cfgItemByIdx(IDX).opt.entry;
					_qryIDX = IDX;
					SELF._Timer.CHANGE(SELF.OP_open_Jobgoal);
					SELF._Timer.START_oneShoot(10);
					SELF._Timer.CHANGE(SELF._OP_scanRover);
					SELF.SOKs = kmd.soks;
				}
				break;
			/*
						case DEF_OP_CLOSE:
							SELF._Timer.CHANGE(SELF.OP_close_Jobgoal);
							SELF.SOKs = kmd.soks;
							SELF._Timer.START_oneShoot(10);
							_workFile = "";
							break;
						default:
							alert("Sorry, `" + kmd.OP + "` unkwnon !!");
			*/
		}
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL Activation events
	 */
	this.launchEvents = function(APP) {//Slot_jobgoal
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		exeEvents(SELF, APP);//Slot_jobgoal
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT FILSYS
 */
Slot_filsys = function(MODULES, MID, DBG_rov, TIM, TIO, SLOTROV, SLOTBAS) {
	var SELF = this;
	this.TIM = TIM;
	this.DBG_rov = DBG_rov;
	this.TIO = TIO;
	this._Timer = new Timer();
	MODULES[MID] = this;
	// -----------------------------------------------------------------
	/**
	 * FILSYS Activation events
	 */
	this.launchEvents = function(APP) {//Slot_filsys
		this.DBG_rov.__selectDefaultevents(APP);
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT LEDS
 */
Slot_leds = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO, SLOTROV,
	SLOTBAS) {
	var SELF = this;
	this.DBG_loc = DBG_loc;
	this.DBG_rov = DBG_rov;
	this.EXE = EXE;
	this.TIO = TIO;
	this._Timer = new Timer();
	MODULES[MID] = this;
	// -----------------------------------------------------------------
	/**
	 * LEDS Activation events
	 */
	this.launchEvents = function(APP) {//Slot_leds
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		// exeEvents(SELF,APP);
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT SWITCHES
 */
Switches = function(_JUNCTION) {
	var SELF = this;
	this._Timer = new Timer();
	MODULES[MID] = this;
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT POWER
 */
Power = function(_JUNCTION) {
	var SELF = this;
	this._Timer = new Timer();
	MODULES[MID] = this;
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT ERRORS
 */
Errors = function(_JUNCTION) {
	var SELF = this;
	this._Timer = new Timer();
	MODULES[MID] = this;
};
// /////////////////////////////////////////////////////////////////////
;
/*/////////////////////////////////////////////////////////////////
	CONTROLEUR D'ETATS
=================================================================*/
function _runAPP(MODULES, PILOTE, SOKROV, SOKBAS, FILSYS, DATIM, LEDS, MOTORS, JOBGOAL, DRIVER) {
	var SELF = this;
	var cntQUERIES = 0;
	var _Pctrl = 256;
	//var _Pdtms	=-1;
	var _oldTms = _TMS_();
	var _Open = false;
	/*
		Appel des fonctions après initialisation des éléments d'interface homme machine (App)
		habillage 
		__selectNosendevents
		__selectDefaultevents
		__textDefaultevents
	*/
	PILOTE.launchEvents(SELF);


	if (cfgServer.rover_ON)
		SOKROV.launchEvents(SELF);
	if (SOKBAS !== undefined)
		SOKBAS.launchEvents(SELF);
	FILSYS.launchEvents(SELF);
	DATIM.launchEvents(SELF);
	LEDS.launchEvents(SELF);
	MOTORS.launchEvents(SELF);
	JOBGOAL.launchEvents(SELF);
	DRIVER.launchEvents(SELF);
}
//////////////////////////////////////////////////////////////
;
function _TpidMmi(CFG) {
	//*******************************************************
	//  CONSTRUCTEUR
	//
	var SELF = this;
	SELF.PROF = CFG.PROF;
	this._MIN = CFG.MIN;
	this._MAX = CFG.MAX;
	this._KP = CFG.KP;
	this._KI = CFG.KI;
	this._KD = CFG.KD;
	this.ERRORS = [];
	this.TPS = [];
	for (var i = 0; i < SELF.PROF; i++) {
		this.ERRORS.push(0);
		this.TPS.push(0);
	}
	this.PTR = -1;
	this.CNT = 0;
	//-------------------------------------------------------
	//  ACCUMULATION DES ERREURS
	//
	function pushError(ERROR, TPS) {
		SELF.PTR = (SELF.PTR + 1) % SELF.PROF;
		SELF.CNT++;
		if (SELF.CNT > SELF.PROF) SELF.CNT = SELF.PROF;
		SELF.ERRORS[SELF.PTR] = ERROR;
		SELF.TPS[SELF.PTR] = TPS;
	}
	//-------------------------------------------------------
	//  NEXT
	//
	function next(I, MOD) {
		var i = I + 1;
		if (i == MOD) return 0; else return i;
	}
	//-------------------------------------------------------
	//  PREV
	//
	function prev(I, MOD) {
		var i = I - 1;
		if (i < 0) return MOD - 1; else return i;
	}
	//-------------------------------------------------------
	//  INTEGRATION
	//
	function integrale() {
		var integ = 0;
		var cur = SELF.PTR;
		for (var i = 1; i < SELF.CNT; i++) {
			var prv = prev(cur, SELF.PROF);
			integ += SELF.ERRORS[cur] * (SELF.TPS[cur] - SELF.TPS[prv]);
			cur = prev(cur, SELF.PROF);
		}
		return integ;
	}
	//-------------------------------------------------------
	//  DERIVATION
	//
	function derivee() {
		var deriv = 0;
		var cur = SELF.PTR;
		for (var i = 1; i < SELF.CNT; i++) {
			var prv = prev(cur, SELF.PROF);
			deriv += (SELF.ERRORS[cur] - SELF.ERRORS[prv]) / (SELF.TPS[cur] - SELF.TPS[prv]);
			cur = prev(cur, SELF.PROF);
		}
		return deriv;
	}
	//-------------------------------------------------------
	//  asservissement
	//
	this.feedback = function(ERROR, TPS) {
		pushError(ERROR, TPS);
		var consigne = SELF._KP * ERROR + SELF._KI * integrale() + SELF._KD * derivee();
		if (this._MAX < consigne) consigne = this._MAX;
		if (consigne < this._MIN) consigne = this._MIN;
		return consigne;
	};
}
function _PilotUmi(CTRL, CURSOR, CFG) {
	//*******************************************************
	//  CONSTRUCTEUR
	//
	//=======================================================
	var SELF = this;
	this.CTRL = CTRL;
	this.CURSOR = CURSOR;
	this._CtrL = document.getElementById(CTRL);
	this._CtrLbounds = SELF._CtrL.getBoundingClientRect();
	this._Cursor = document.getElementById(CURSOR);
	this.CtrL_Height = this._CtrL.offsetHeight - this._Cursor.offsetHeight;
	this.CtrL_Width = this._CtrL.offsetWidth - this._Cursor.offsetWidth;
	this.Cd2_Height = this._Cursor.offsetHeight / 2;
	this.Cd2_Width = this._Cursor.offsetWidth / 2;
	this._Listening = false;
	this._MIN = { 'DIR': CFG.DIR.MIN, 'VIT': CFG.VIT.MIN };
	this._VAL = { 'DIR': CFG.DIR.CLR, 'VIT': CFG.VIT.CLR };
	this._MAX = { 'DIR': CFG.DIR.MAX, 'VIT': CFG.VIT.MAX };;
	//-------------------------------------------------------
	//  PLACER LE CURSEUR
	//
	this.placeCursor = function() {
		SELF._Cursor.style.marginTop = SELF.CtrL_Height * (1 - (SELF._VAL.VIT - SELF._MIN.VIT) / (SELF._MAX.VIT - SELF._MIN.VIT)) + "px";
		SELF._Cursor.style.marginLeft = SELF.CtrL_Width * ((SELF._VAL.DIR - SELF._MIN.DIR) / (SELF._MAX.DIR - SELF._MIN.DIR)) + "px";
	}
	this.placeCursor();
	events();// Anonyme, lancé une seule fois sans variable !!!
	//-------------------------------------------------------
	//  LIRE LES VALEURS
	//
	this.getVal = function() {
		return SELF._VAL;
	}
	this.setFunctions = function(MIN, VAL, MAX, SET) {
		SELF._MIN = MIN;
		SELF._VAL = VAL;
		SELF._MAX = MAX;
		SELF._SET = SET;
		SELF.placeCursor();
	}
	//-------------------------------------------------------
	//  GERER LE MOUVEMENT
	//
	function moveCursor(XX, YY) {
		// suivi de la position de la souris dans la console
		console.log("Position de la souris :  X=" + XX + "   Y=" + YY, joystickBounds);
		if (SELF._Listening) {
			var newMargLeft = XX - SELF._CtrLbounds.left - SELF.Cd2_Width;
			if (newMargLeft < 0) newMargLeft = 0
			else if (SELF.CtrL_Width < newMargLeft) newMargLeft = SELF.CtrL_Width;
			SELF._Cursor.style.marginLeft = newMargLeft + "px";
			SELF._VAL.DIR = SELF._MAX.DIR - (SELF._MAX.DIR - SELF._MIN.DIR) * (1 - (newMargLeft) / SELF.CtrL_Width);

			var newMargTop = YY - SELF._CtrLbounds.top - SELF.Cd2_Height;
			if (newMargTop < 0) newMargTop = 0
			else if (SELF.CtrL_Height < newMargTop) newMargTop = SELF.CtrL_Height;
			SELF._Cursor.style.marginTop = newMargTop + "px";
			SELF._VAL.VIT = SELF._MIN.VIT + (SELF._MAX.VIT - SELF._MIN.VIT) * (1 - (newMargTop) / SELF.CtrL_Height);

			//SELF._VAL.VIT=SELF._SET(SELF._VAL.VIT);   .... CB
			//placeCursor();
			console.log(SELF._VAL, newMargLeft, newMargTop);
		}
	}
	//-------------------------------------------------------
	//  events
	//
	function events() {

		SELF._CtrL.onmousemove = function(e) {
			e.preventDefault();
			moveCursor(e.clientX, e.clientY);
		};
		SELF._CtrL.ontouchmove = function(e) {
			e.preventDefault();
			//            var touch = e.originalEvent.touches[0] || e.originalEvent.changedTouches [0];
			var touch = e.touches[0] || e.changedTouches[0];
			moveCursor(touch.pageX, touch.pageY);
		};
		SELF._Cursor.onmousedown = function() {
			console.log("SELF._Cursor.onmousedown : SELF._Listening ", SELF._Listening);
			SELF._Listening = true;
			console.log("SELF._Cursor.onmousedown : SELF._Listening ", SELF._Listening);
		};
		SELF._Cursor.ontouchstart = function() {
			console.log("SELF._Cursor.ontouchstart : SELF._Listening ", SELF._Listening);
			SELF._Listening = true;
			console.log("SELF._Cursor.ontouchstart : SELF._Listening ", SELF._Listening);
		};
		SELF._Cursor.onmouseup = function() {
			console.log("SELF._Cursor.onmouseup : SELF._Listening ", SELF._Listening);
			SELF._Listening = false;
			console.log("SELF._Cursor.onmouseup : SELF._Listening ", SELF._Listening);
			SELF.stop();
		};
		SELF._Cursor.ontouchend = function() {
			console.log("SELF._Cursor.ontouchend : SELF._Listening ", SELF._Listening);
			SELF._Listening = false;
			console.log("SELF._Cursor.ontouchend : SELF._Listening ", SELF._Listening);
			SELF.stop();
		};
		return;
		/*
				$( "#"+SELF.CTRL).click(function(event){
					console.log(SELF._Listening,"click ",SELF.CTRL,MV2.pageY);
					SELF._Listening=true;
					SELF.move(event);
					SELF._Listening=false;
				});
				*/
	}
	//-------------------------------------------------------
	//  stop à affiner, sécurité dans le main: onmouseup over etc...
	//
	this.stop = function() {
		SELF._Listening = false;
		SELF._VAL = { 'DIR': CFG.DIR.CLR, 'VIT': CFG.VIT.CLR };
		SELF.placeCursor();
		console.log(".stop", SELF._VAL);
	}
};
//////////////////////////////////////////////////////////////////////////////////////
;
	// Force dimensions !!!
function	forceDim(CANVAS){
	CANVAS.height=CANVAS.clientHeight;
	CANVAS.width=CANVAS.clientWidth;
}

function _CanvasUmi(CANVAS, YMIN, YMAX, GAP, TITLE, canvasCFG/* LEGEND_A,COLOR_A,LEGEND_B,COLOR_B */) {
	// *******************************************************
	// CONSTRUCTEUR
	//
	// =======================================================

	var SELF = this;
	var Gtop = 30;
	var Gbot = 5;
	var Gleft = 70;
	var Gright = 5;
	var Ymoins = YMIN - GAP;
	var Yplus = YMAX + GAP;
	this._CANVAS = document.getElementById(CANVAS);
	if (!this._CANVAS) {
		alert("Impossible de récupérer le canvas");
		return;
	}
	forceDim(SELF._CANVAS);// Force dimensions !!!
	var _Gwidth = SELF._CANVAS.width - Gleft - Gright;
	var _Gheight = SELF._CANVAS.height - Gtop - Gbot;
	var _CTX = this._CANVAS.getContext('2d');
	if (!_CTX) {
		alert("Impossible de récupérer le contexte du canvas");
		return;
	}
	// -------------------------------------------------------
	// INIT
	//
	this.Init = function() {
		_CTX.fillStyle = "#888888";
		_CTX.fillRect(Gleft, Gtop, _Gwidth, _Gheight);

		_CTX.strokeStyle = '#ff0000';
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.moveTo(0 , 0 );// On se déplace au coin inférieur gauche
		_CTX.lineTo(0 , SELF._CANVAS.height );
		_CTX.lineTo(SELF._CANVAS.width, SELF._CANVAS.height);
		_CTX.lineTo(SELF._CANVAS.width, 0);
		_CTX.lineTo(0 , 0);
		_CTX.stroke();// On trace seulement les lignes.

		var s = 2;
		_CTX.strokeStyle = '#880000';
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.moveTo(Gleft - s, Gtop - s);// On se déplace au coin inférieur gauche
		_CTX.lineTo(Gleft - s, Gtop + _Gheight + s);
		_CTX.lineTo(Gleft + _Gwidth + s, Gtop + _Gheight + s);
		_CTX.lineTo(Gleft + _Gwidth + s, Gtop - s);
		_CTX.lineTo(Gleft - s, Gtop - s);
		_CTX.stroke();// On trace seulement les lignes.


		_CTX.font = "15px Arial";
		_CTX.fillText(TITLE, 10, 20);
		for (var i = 0; i < canvasCFG.length; i++) {
			_CTX.fillStyle = canvasCFG[i].COLOR;
			_CTX.fillText(canvasCFG[i].LABEL, 100 + 50 * i, 20);
		};
		/*
		 * _CTX.fillStyle=COLOR_A; _CTX.fillText(LEGEND_A,100,20);
		 * _CTX.fillStyle=COLOR_B; _CTX.fillText(LEGEND_B,200,20);
		 */
		_CTX.fillStyle = "#ffffff";
		_CTX.font = "10px Arial";
		_CTX.fillText(SF_float(YMIN, 7, 1), 10 + s, col(YMIN) - s);
		_CTX.fillText('+' + SF_float(YMAX, 7, 1), 10 + s, col(YMAX) + s);

		// _CTX.fill(); //On utilise la méthode fill(); si l'on veut une forme
		// pleine
		_CTX.closePath();
		_CTX.arc(10, 100, 50, 0, Math.PI); // On trace la courbe délimitant
		// notre forme
		_CTX.stroke();// On trace seulement les lignes.
	}
	SELF.Init();
	// -------------------------------------------------------
	// CERCLE
	//
	this.Circle = function() {
		// _CTX.beginPath(); //On démarre un nouveau tracé.
		_CTX.fillStyle = "#0000ff";
		_CTX.arc(10, 100, 20, 0, 2 * Math.PI); // On trace la courbe
		// délimitant notre forme
		_CTX.stroke();// On trace seulement les lignes.
		// _CTX.fill(); //On utilise la méthode fill(); si l'on veut une
		// forme pleine
		_CTX.closePath();
	}
	// -------------------------------------------------------
	// TEXTE
	//
	/*
		this.Text = function(TXT, X, Y) {
			_CTX.font = "30px Arial";
			_CTX.fillText(TXT, X, Y);
		}
	*/
	// -------------------------------------------------------
	// POINT
	//
	this.PointCR = function(X, Y) {
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.moveTo(X, Y);// On se déplace au coin inférieur gauche
		_CTX.lineTo(X - 1, Y - 1);
		_CTX.stroke();// On trace seulement les lignes.
		_CTX.closePath();
	}
	// -------------------------------------------------------
	// col
	//
	function col(Y) {
		var abs = SELF._CANVAS.height - _Gheight * (Y - Ymoins) / (Yplus - Ymoins);
		return abs;
	}
	// -------------------------------------------------------
	// addValueY
	//
	this.addValueY = function(Y, COLOR) {
		_CTX.strokeStyle = COLOR;
		this.PointCR(Gleft + _Gwidth, col(Y));
	}
	// -------------------------------------------------------
	// SHIFT LEFT DU GRAPHIQUE
	//
	this.animate = function() {
		// SELF.PointCR(SELF._CANVAS.width,20);
		var myImageData = _CTX.getImageData(Gleft + 1, Gtop, _Gwidth, _Gheight);
		_CTX.putImageData(myImageData, Gleft, Gtop);
		_CTX.strokeStyle = '#ffffff';
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.moveTo(Gleft + _Gwidth, Gtop);
		_CTX.lineTo(Gleft + _Gwidth, Gtop + _Gheight);
		_CTX.stroke();// On trace seulement les lignes.
		_CTX.closePath();
	}
	this.update = function(VALARRAY) {
		if (CFG.client.CANVAS.ANIMATE) {
			this.addValueY(YMAX, "#888888");
			this.addValueY(0, "#88ff88");
			this.addValueY(YMIN, "#888888");
			for (var i = 0; i < VALARRAY.length; i++)
				this.addValueY(VALARRAY[i], canvasCFG[i].COLOR);
			this.animate();
		}
	}
}
///////////////////////////////////////////////////////////////////////
;
function _Canvas2DUmi(CANVAS, XMIN, XORG, XMAX, YMIN, YORG, YMAX, LFT, RGT, TOP, BOT,
	TITLE, canvasCFG/* LEGEND_A,COLOR_A,LEGEND_B,COLOR_B */) {
	// *******************************************************
	// CONSTRUCTEUR
	//
	var SELF = this;
	var _Csiz = 5;
	this._CANVAS = document.getElementById(CANVAS);
	if (!this._CANVAS) {
		alert("Impossible de récupérer le canvas");
		return;
	}
	forceDim(SELF._CANVAS);// Force dimensions !!!
	var _Gwidth = SELF._CANVAS.width - LFT - RGT;
	var _Gheight = SELF._CANVAS.height - TOP - BOT;
	var _CTX = this._CANVAS.getContext('2d');
	if (!_CTX) {
		alert("Impossible de récupérer le contexte du canvas");
		return;
	}
	_CTX.translate(LFT, TOP); // !!!!!!!!!!!!!!
	// -------------------------------------------------------
	// INIT 2D
	//
	this.Init2D = function() {
		//		_CTX = this._CANVAS.getContext('2d');
		_CTX.clearRect(- LFT, - TOP, LFT + RGT + this._CANVAS.width, TOP + BOT + this._CANVAS.height);
		_CTX.fillStyle = "#008800";
		_CTX.fillRect(- LFT, - TOP, LFT + RGT + this._CANVAS.width, TOP + BOT + this._CANVAS.height);
		_CTX.fillStyle = "#ffffff";
		_CTX.fillRect(0, 0, _Gwidth, _Gheight);
		// DRAW THE FRAME

		_CTX.beginPath(); // Start new draw
		// Draw dark red borders
		{
			_CTX.strokeStyle = '#880000';
			var s = 2;
			_CTX.moveTo(- s, - s);// On se déplace au coin inférieur gauche
			_CTX.lineTo(- s, + _Gheight + s);
			_CTX.lineTo(+ _Gwidth + s, + _Gheight + s);
			_CTX.lineTo(+ _Gwidth + s, - s);
			_CTX.lineTo(- s, - s);
			_CTX.stroke();// On trace seulement les lignes.
		}
		// Draw labels & untag the old crosses
		{
			_CTX.font = "15px Arial";
			_CTX.fillText(TITLE, 10, -6);
			for (var i = 0; i < canvasCFG.length; i++) {
				_CTX.fillStyle = canvasCFG[i].COLOR;
				_CTX.fillText(canvasCFG[i].LABEL, 100 + 50 * i, -6);
				canvasCFG[i].oldCross = false;
			};
		}
		// draw origin axis
		{
			this.LineCR(col(XMIN), row(YORG), col(XMAX), row(YORG), "#333333");
			this.LineCR(col(XORG), row(YMIN), col(XORG), row(YMAX), "#333333");
		}
		// text axis min max values
		{
			_CTX.fillStyle = "#000000";
			_CTX.font = "10px Arial";
			_CTX.fillText(SF_float(XMIN, 7, 1), col(XMIN) - 10, row(YMIN) + 15);
			_CTX.fillText(SF_float(XORG, 7, 1), col(XORG) - 20, row(YMIN) + 15);
			_CTX.fillText(SF_float(XMAX, 7, 1), col(XMAX) - 30, row(YMIN) + 15);
			// preserve the transformation matrix!
			_CTX.rotate(-Math.PI / 2);
			_CTX.fillText(SF_float(YMIN, 7, 1), -row(YMIN) - 10, -5);
			_CTX.fillText(SF_float(YORG, 7, 1), -row(YORG) - 20, -5);
			_CTX.fillText(SF_float(YMAX, 7, 1), -row(YMAX) - 30, -5);
			_CTX.rotate(Math.PI / 2);
		}
		// Just to play...
		{
			_CTX.moveTo(-LFT + 10, -TOP);
			_CTX.arc(-LFT, -TOP, 10, 0, Math.PI / 2); // On trace la courbe délimitant notre forme
			_CTX.stroke();// On trace seulement les lignes.
		}
		_CTX.closePath();
		this.ArcXY(0, 15, 25, 10, 45, 90);// Single arc
		this.ArcXY(0, 25, 30, 10, 45, 90, 12345, "#00ff00");// fill camembert
		this.ArcXY(0, -25, -30, 15, -150, -30, false);// empty camembert
	}
	// -------------------------------------------------------
	// TEXTE
	//
	this.Text = function(TXT, X, Y) {
		_CTX.font = "30px Arial";
		_CTX.fillText(TXT, X, Y);
	}
	// -------------------------------------------------------
	// POINT
	//
	this.PointCR = function(C, R, COLOR) {
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.strokeStyle = COLOR;
		_CTX.moveTo(C, R);// On se déplace au coin inférieur gauche
		_CTX.lineTo(C - 1, R - 1);
		_CTX.stroke();// On trace seulement les lignes.
		_CTX.closePath();
	}
	// -------------------------------------------------------
	// XcrossCR
	//
	this.XcrossCR = function(C, R, COLOR) {
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.strokeStyle = COLOR;
		_CTX.moveTo(C - _Csiz, R - _Csiz);
		_CTX.lineTo(C + _Csiz, R + _Csiz);
		_CTX.moveTo(C - _Csiz, R + _Csiz);
		_CTX.lineTo(C + _Csiz, R - _Csiz);
		_CTX.stroke();// On trace seulement les lignes.
		_CTX.closePath();
	}
	// -------------------------------------------------------
	// LineCR pixels
	//
	this.LineCR = function(Ca, Ra, Cb, Rb, COLOR) {
		_CTX.beginPath();
		_CTX.strokeStyle = COLOR;
		_CTX.moveTo(Ca, Ra);
		_CTX.lineTo(Cb, Rb);
		_CTX.stroke();
		_CTX.closePath();
	}
	// -------------------------------------------------------
	// Arc pixels sens trigo (CAMEMBERT has just to be defined)
	//
	this.ArcCR = function(COL, ROW, RAD, A0, A1, COLOR, CAMEMBERT, FILLCOLOR) {
		_CTX.beginPath(); // On démarre un nouveau tracé.
		_CTX.strokeStyle = COLOR;
		if (typeof CAMEMBERT !== 'undefined')
			_CTX.moveTo(COL, ROW);
		_CTX.arc(COL, ROW, RAD, -A0, -A1, true); // On trace la courbe
		if (typeof CAMEMBERT !== 'undefined')
			_CTX.lineTo(COL, ROW);
		_CTX.stroke();// On trace seulement les lignes.
		if (typeof FILLCOLOR !== 'undefined') {
			_CTX.fillStyle = FILLCOLOR;
			_CTX.fill(); // si l'on veut une forme pleine
		}
		_CTX.closePath();
	}
	// -------------------------------------------------------
	// CERCLE (Angles in degrees)
	//
	this.ArcXY = function(IDX, XC, YC, RAD, A1, A2, CAMEMBERT, FILLCOLOR) {
		var Rad = col(RAD) - col(0);
		this.ArcCR(col(XC), row(YC), Rad, A1 * Math.PI / 180, A2 * Math.PI / 180,
			canvasCFG[IDX].COLOR, CAMEMBERT, FILLCOLOR);
	}
	// -------------------------------------------------------
	// lineXY
	//
	this.lineXY = function(ID, X1, Y1, X2, Y2) {
		this.LineCR(col(X1), row(Y1), col(X2), row(Y2), canvasCFG[ID].COLOR);
	}
	// -------------------------------------------------------
	// Abscissa
	//
	function col(X) {
		var abs = _Gwidth - _Gwidth * (XMAX - X) / (XMAX - XMIN);
		return abs;
	}
	// -------------------------------------------------------
	// Ordinate
	//
	function row(Y) {
		var ord = _Gheight - _Gheight * (Y - YMIN) / (YMAX - YMIN);
		return ord;
	}
	// -------------------------------------------------------
	// moveMarkXY
	//
	this.moveMarkXY = function(ID, X, Y) {
		// _CTX.globalCompositeOperation = 'xor';
		var Col = Math.round(col(X));
		var Row = Math.round(row(Y));

		this.PointCR(Col, Row, canvasCFG[ID].COLOR);
		if (canvasCFG[ID].oldCross) {
			_CTX.putImageData(canvasCFG[ID].savedCross,
				canvasCFG[ID].prvCol + LFT - _Csiz, canvasCFG[ID].prvRow + TOP - _Csiz);
			this.LineCR(Col, Row, canvasCFG[ID].prvCol, canvasCFG[ID].prvRow,
				canvasCFG[ID].COLOR);
		}
		canvasCFG[ID].savedCross = _CTX.getImageData(Col + LFT - _Csiz, Row + TOP - _Csiz,
			2 * _Csiz + 1, 2 * _Csiz + 1);
		canvasCFG[ID].oldCross = true;
		canvasCFG[ID].prvRow = Row;
		canvasCFG[ID].prvCol = Col;
		this.XcrossCR(Col, Row, canvasCFG[ID].COLOR);
	}
	SELF.Init2D();
}
///////////////////////////////////////////////////////////////////////
var Tick = {
	n: -1
};
var pseudo = '=>';
var joystickBounds;
var scheduler;
/*
 * http://tassedecafe.org/fr/variables-statiques-javascript-532 cnt & keep are
 * static like
 */
/**
 * @fn var addLog = (function() { return function(LVL, HEAD, CHN, FOOT) })();
 * @brief Adds a log query | reply CHN (static cnt & keep)
 * @param LVL to indent
 * @param HEAD prefix of message content: some text often ending with an opening bracket (bold)
 * @param CHN content of log message
 * @param FOOT suffix of message content: often ending with an closing bracket (bold)
 * 
 */
var addLog = (function() {
	var cnt = 0;
	var keep = 10;
	return function(LVL, HEAD, CHN, FOOT) {
		var Tab = "";
		for (var i = 0; i < LVL; i++) {
			Tab += i + "   ";
		}
		var element = document.getElementById('logMSG');
		// Verify if already more than keep lines in txt
		var txt = element.innerHTML;
		cnt++;
		if (cnt > keep)
			txt = txt.substring(txt.indexOf('</div>'));
		document.getElementById('logMSG').innerHTML = txt
			+ '<div class="line"><b>' + HEAD + '</b>' + CHN + "<b>" + FOOT
			+ '</b></div>';
		// element.scrollTop = 1000000;//Scroll à la fin à améliorer
	}
})();
// ////////////////////////////////////////////////////////////////////
/** *****************************************************************
 * @fn showView(areaID, viewID, HIDEALL)
 * @brief Display in areaID the viewID
 * @param areaID zone where to display
 * @param viewID to display in the zone
 * @param HIDEALL if true hid all before displaying, otherwise leave as it is 
 * @return void
 */
function showView(areaID, viewID, HIDEALL=true) {
	/** ---------------------------------------------------------------
	 * @fn clearArea(AREA)
	 * @brief hids all from AREA depending of LAYOUT items & places them in default position
	 */
	function clearArea(AREA) {
		for (var key of Object.keys(LAYOUT[AREA])) {
			var entry = Object.assign(LAYOUT[AREA][key]);
			entry.ELT.className = entry.ELT.className.replace(" " + AREA, "");
			if (entry.ELT.className.indexOf(" " + key + "_default") < 0)// keep other classes !!!
				entry.ELT.className += " " + key + "_default";
		}
	}
	/** ------------------------------------------------------------ */
	if (HIDEALL){
		clearArea(areaID);
		if(areaID==='CENTER_ur')
			clearArea('HEAD');
	}
	if (viewID === undefined)
		return;
	var view = Object.assign(LAYOUT[areaID][viewID]);
	view.ELT.className = view.ELT.className.replace(" " + viewID + "_default", "");
	view.ELT.className += " " + areaID;
	if (areaID === 'CENTER_ur') {
		clearArea('HEAD');
		if (typeof LAYOUT['HEAD'][viewID + '_head'] !== 'undefined') {
			view = Object.assign(LAYOUT['HEAD'][viewID + '_head']);
			showView('HEAD', viewID + '_head', false);
			/*
			if (LAY OUT['CENTER_ul'][viewID + '_ctrl'] !== 'undefined')
				if (LAY OUT['CENTER_ul'][viewID + '_ctrl'].ON)
					showView('CENTER_ul', viewID + '_ctrl', false);
			*/
		}
	}
}
//----------------------------------------------------------------------
function swapVC(THIS) {
	if (THIS.className === "checkBtn little") {
		THIS.className += " checked";
		THIS.textContent = 'CTRL';
		document.getElementById('ctrlsMenu').style.removeProperty('display');
		document.getElementById('viewsMenu').style.display = 'none';
		/*
				showView('CENTER_ul', THIS.name + '_ctrl', false);
				LAY OUT['CENTER_ul'][THIS.name + '_ctrl'].ON = true;
		*/
	} else {
		THIS.className = THIS.className.replace(" checked", "");
		THIS.textContent = 'VIEW';
		document.getElementById('ctrlsMenu').style.display = 'none';
		document.getElementById('viewsMenu').style.removeProperty('display');
		/*
				elt.className = elt.className + " " + THIS.name + "_ctrl_default"
				LAY OUT['CENTER_ul'][THIS.name + '_ctrl'].ON = false;
		*/
	}
}
//----------------------------------------------------------------------
function viewClear() {
	showView('CENTER_ul');
	showView('CENTER_ur');
	showView('HEAD');
}
//----------------------------------------------------------------------
/*
*/
	var urlWSrover;
	var urlXHRrover;

function cfgFile() {
	var json = {};
	function cfgListener() {
		console.info(this);
		json = JSON.parse(this.responseText);
	};
	var xhttp = new XMLHttpRequest();
	xhttp.onload = cfgListener;
	xhttp.open("GET", "/rover.json", false);// false is synchrone !!!
	xhttp.send();
	return json;
};
//----------------------------------------------------------------------
/*
https://www.w3schools.com/howto/howto_js_active_element.asp
verticalMenu
*/
function jsonXHR(REQUEST) {
	var json = {};
	function chooseListener() {
		console.info(this);
		json = JSON.parse(this.responseText);
	};
	var xhttp = new XMLHttpRequest();
	xhttp.onload = chooseListener;
	xhttp.open("GET", urlXHRrover + REQUEST, false);// false is synchrone !!!
	//xhttp.withCredentials = true;//false;//
	/*
		xhttp.timeout = 500; // durée en millisecondes
		xhttp.ontimeout = function(e) {
			alert("jsonXHR xhttp time out");
		};
	*/
	xhttp.send();
	return json;
};
//----------------------------------------------------------------------
//	WORK MODE
function WORK(THIS) {
	if (THIS.className === "bigBtn") {
		THIS.className += " Vactive";
		stopJOYSTICK();
		/*
				var json = jsonXHR('workFiles');
				var SELECT_FILE = new __select('classTimer', 'WORKJOB', 'choose', 'modToto', json);
				var filename = SELECT_FILE.selectOpt(0, "xxx");
				console.info(filename);
		*/
		showView('CENTER_ur', 'WORKJOB');
		showView('HEAD', 'WORKJOB_head');
		//		_WSROV.sokROVER.sendCLI("RW");
	} else {
		if (_JOBGOAL.OP_close_Jobgoal()) {
			// reset to choose a file !!!
			_JOBGOAL.EXE._Elt.value = 0;
			//    select.dispatchEvent(new Event('change'));

			THIS.className = "bigBtn";
			showView('HEAD');
			_WSROV.sokROVER.sendCLI("RI");
		}
	}
}
//----------------------------------------------------------------------
//	STOP JOYSTICK
function stopJOYSTICK() {
	_MOTORS.TIM.selectOpt(0);
	_MOTORS.EXE.selectOpt(0);
	_MOTORS.__OPchange(0);
	timerChange(_MOTORS, 0);
	//	JOY_STICK.stop();
}
//----------------------------------------------------------------------
//	START JOYSTICK
function startJOYSTICK() {
	_MOTORS.TIM.selectOpt(9);
	_MOTORS.EXE.selectOpt(1);
	_MOTORS.__OPchange(1);
	timerChange(_MOTORS, 9);
}
//----------------------------------------------------------------------
//	ROAM MODE
function ROAM(THIS) {
	if (THIS.className === "bigBtn") {
		THIS.className += " Vactive";
		_WSROV.sokROVER.sendCLI("RR");
	} else {
		THIS.className = "bigBtn";
		_WSROV.sokROVER.sendCLI("RI");
	}
}
//----------------------------------------------------------------------
//	DRIVE MODE
function DRIVE(THIS) {
	if (THIS.className === "bigBtn") {
		THIS.className += " Vactive";
		startJOYSTICK();
		_WSROV.sokROVER.sendCLI("RD");
	} else {
		THIS.className = "bigBtn";
		_WSROV.sokROVER.sendCLI("RI");
		stopJOYSTICK();
	}
}
//----------------------------------------------------------------------
//	LEARN MODE
function LEARN(THIS) {
	if (THIS.className === "bigBtn") {
		THIS.className += " Vactive";
		startJOYSTICK();
		_WSROV.sokROVER.sendCLI("RL");
	} else {
		THIS.className = "bigBtn";
		stopJOYSTICK();
		_WSROV.sokROVER.sendCLI("RI");
	}
}
//----------------------------------------------------------------------
//	PILOT RELOAD
function RELOAD() {
	location.reload();
}
//----------------------------------------------------------------------
//	ADMIN PAGE
function ADMIN(THIS) {
	location.replace("/");
}
//----------------------------------------------------------------------
//	RESET ROVER
function RESET() {
	_WSROV.sokROVER.sendCLI("@");
	setTimeout(RELOAD, 30000);
	//alert("RESET");
}
///////////////////////////////////////////////////////////////////////
/*
 * Start une fois la page chargée
 */
///////////////////////////////////////////////////////////////////////
/*
 * Start une fois la page chargée
 */
var _WSROV;
var _WSBAS;
var _MOTORS;
var _JOBGOAL;
var jsonWorkFiles;
var jsonCONTEXT;
var cfgServer = cfgFile();
var JOY_STICK;
var LAYOUT = {};
/** *******************************************************************
 * @fn var setTJS_KamKey = function(KEY, VALUE)
 * @brief modify a THREEjs camera parameter value & set the moveKAM flag
 */
var setTJS_KamKey = function(KEY, VALUE) {
	tjsCam[moveKAM][KEY] = Number(VALUE);
	moveKAM_flag = true;
}
/** *******************************************************************
 * @fn var set_tjsATT = function(KEY, VALUE)
 * @brief modify a THREEjs camera parameter value
 */
var set_tjsATT = function(KEY, VALUE) {
	tjsATT[KEY] = Number(VALUE);
}
//=====================================================================
/**
 * @fn class class_DLG
 * @brief Set the view, controls & dialogs. Manages Hactive radio buttons (if any)
 * 
 * constructor(BASE) where BASE is the root name of the view
 * Includes message(TEXT,DURATION) to display in _status vs @ref _text  
 * 
 */
class class_DLG {
	constructor(BASE){
		// IO status 
		this._status = new __text('IOstatus', BASE+'_io', BASE+'_status', CFG.client.__field_txt);
		this._status.__textDefaultevents();
		this._status._setText('blabla');
		//	SWITCHEES buttons events
		var ELT_head = document.getElementById(BASE+'_head');
		var eltBtns = ELT_head.getElementsByClassName("radioBtn");
		for (var i = 0; i < eltBtns.length; i++) {
			eltBtns[i].addEventListener("click", function() {
				var current = ELT_head.getElementsByClassName("Hactive");
				current[0].className = current[0].className.replace(" Hactive", "");
				this.className += " Hactive";
			});
		}
	}
	message(TEXT,DURATION){
		this._status._setText(TEXT);
		if(typeof DURATION !== 'undefined')
			setTimeout(this._status._setText, DURATION,'');
	}
	sound(SOUND_URL){
		var audio = new Audio(SOUND_URL);
		audio.play();
	}
}
//=====================================================================
/**
 * @fn class class_ATT
 * @brief Set the view, controls & dialogs. Manages attitude interactions
 */
class class_ATT extends class_DLG{
	constructor(BASE){
		super(BASE);
	}
	XYZo(){
		_WSROV.sokROVER.sendCLI("R");
		this.message('Return to origin (0,0,0)', 2000);
	}
}
let ATT={};
//=====================================================================
let CANVAS_IMU={}; // before setTimeout(CANVAS_IMU.Init2D,1000); !!!!
let IMU={};
/**
 * @fn class class_IMU
 * @brief Set the view, controls & dialogs. Manages attitude interactions
 */
class class_IMU extends class_DLG{
	constructor(BASE){
		super(BASE);
	}
	calIMU(){
		_WSROV.sokROVER.sendCLI("II");
		this.message('Calibrating accelerometer & gyroscop', 5000);
		//setTimeout(CANVAS_IMU.Init2D,1000);
		CANVAS_IMU.Init2D();
	}
	calMAG(SAMPLES){
		disableChildren('IMU_head');
		_WSROV.sokROVER.sendCLI("IM:"+SAMPLES);
		this.message('Calibrating magnetometer', 5000);
		//setTimeout(CANVAS_IMU.Init2D,1000);
		CANVAS_IMU.Init2D();
	}
}
//=====================================================================
//var xCam, yCam, zCam, rollCam, pithCam, yawCam, focalCam, nearCam, farCam;
//=====================================================================
/**
 * @fn class class_M3D
 * @brief Set the view, controls & dialogs. Manages attitude interactions
 */
class class_M3D extends class_DLG{
	constructor(BASE){
		super(BASE);

		this.xCam = new __range({}, "x_Cam", "Xo",
			tjsCam[liveKAM].xMin, tjsCam[liveKAM].x, tjsCam[liveKAM].xStep, tjsCam[liveKAM].xMax,
			'x', setTJS_KamKey);
		this.yCam = new __range({}, "y_Cam", "Yo",
			tjsCam[liveKAM].yMin, tjsCam[liveKAM].y, tjsCam[liveKAM].yStep, tjsCam[liveKAM].yMax,
			'y', setTJS_KamKey);
		this.zCam = new __range({}, "z_Cam", "Zo",
			tjsCam[liveKAM].zMin, tjsCam[liveKAM].z, tjsCam[liveKAM].zStep, tjsCam[liveKAM].zMax,
			'z', setTJS_KamKey);
		this.rollCam = new __range({}, "roll_Cam", "Roll",
			tjsCam[liveKAM].rollMin, tjsCam[liveKAM].roll, tjsCam[liveKAM].rollStep, tjsCam[liveKAM].rollMax,
			'roll', setTJS_KamKey);
		this.pithCam = new __range({}, "pitch_Cam", "Pitch",
			tjsCam[liveKAM].pitchMin, tjsCam[liveKAM].pitch, tjsCam[liveKAM].pitchStep, tjsCam[liveKAM].pitchMax,
			'pitch', setTJS_KamKey);
		this.yawCam = new __range({}, "yaw_Cam", "Yaw",
			tjsCam[liveKAM].yawMin, tjsCam[liveKAM].yaw, tjsCam[liveKAM].yawStep, tjsCam[liveKAM].yawMax,
			'yaw', setTJS_KamKey);
		this.focalCam = new __range({}, "focal_Cam", "Focale",
			tjsCam[liveKAM].fovMin, tjsCam[liveKAM].fov, tjsCam[liveKAM].fovStep, tjsCam[liveKAM].fovMax,
			'fov', setTJS_KamKey);
		this.nearCam = new __range({}, "near_Cam", "Proche",
			tjsCam[liveKAM].nearMin, tjsCam[liveKAM].near, tjsCam[liveKAM].nearStep, tjsCam[liveKAM].nearMax,
			'near', setTJS_KamKey);
		this.farCam = new __range({}, "far_Cam", "Lointain",
			tjsCam[liveKAM].farMin, tjsCam[liveKAM].far, tjsCam[liveKAM].farStep, tjsCam[liveKAM].farMax,
			'far', setTJS_KamKey);


		this.camera(liveKAM, false);

	}
	/**********************************************************************
	 * @fn camera(KAM, SWITCH)
	 * @brief Switches camera to KAM
	 * @param KAM key (if undefined reset the current camera)
	 * @param SWITCH if true or undefined ask a true THREEJS switch
	 */
	camera(KAM, SWITCH) {
		if (KAM === undefined)
			Kam_reset = true
		else {
			var chn = 'Camera<select onchange="M3D.camera(this.value,false);">';
			for (var key in tjsCam) {
				var selected = '';
				if (key === KAM)
					selected = ' selected="selected"'
				chn += '<option value="' + key + '"' + selected + '>';
				chn += key + '(' + tjsCam[key].attach.name + ') ';
				if (typeof tjsCam[key].target !== 'undefined')
					chn += '=> (' + tjsCam[key].target.name + ')';
				else
					chn += 'pas de cible';
				chn += '</option>';
			}
			chn += '</select>';

			document.getElementById("kamMove").innerHTML = chn;
			// Gets the parameters values of the concerned camera
			this.xCam._setRange(tjsCam[KAM].x, false)
			this.yCam._setRange(tjsCam[KAM].y, false)
			this.zCam._setRange(tjsCam[KAM].z, false)
			this.rollCam._setRange(tjsCam[KAM].roll, false)
			this.pithCam._setRange(tjsCam[KAM].pitch, false)
			this.yawCam._setRange(tjsCam[KAM].yaw, false)
			this.focalCam._setRange(tjsCam[KAM].fov, false)
			this.nearCam._setRange(tjsCam[KAM].near, false)
			this.farCam._setRange(tjsCam[KAM].far, false)
		}
		if (SWITCH === false)
			switchKAM = '';
		else
			switchKAM = KAM;// always switches
		moveKAM = KAM;
	}
}
let M3D={};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
var ctlRover = {
	trajectory: {
		scaleMin: 0,
		scaleMax: 10,
		scaleStep: 0.1,
		scaleValue: 1
	}
};
let _ALERT={};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/**
 * @fn Start()
 * @brief Launches the pilot application
 */
function Start() {
	/**
	 * Create head dialogs
	 */
	ATT=new class_ATT('ATT');
	IMU=new class_IMU('IMU');
	M3D=new class_M3D('M3D');
	//
	//  1) Check if TREE js here
	//

	if (typeof tjsTSK === 'undefined') {
		alert('THREE not loaded !!!');
	}
	else if (tjsTSK.error != 0) {
		alert('THREE error: ' + tjsTSK.error);

	}
	console.log('ONE=' + ONE);
	console.log('TWO=' + TWO);
	console.log('THREE=' + TROIS);
	ONE++;
	TWO++;
	TROIS++;

	console.log('ONE=' + ONE);
	console.log('TWO=' + TWO);
	console.log('THREE=' + TROIS);

	/*
	alert(
		"Screen size: " + screen.width + "x" + screen.height + "\n" +
		"Window size: " + window.outerWidth + "x" + window.outerHeight 
		);
	*/
	cfgServer = cfgFile();
	console.log("cfgServer=", cfgServer);
	//window.setTimeout(alert("Atente du chargement complet"), 2000);	
	var SELF = this;
	/*
		var xMin = -100;
		var xMax = +100;
		var yMin = -100;
		var yMax = +100;
		*/
	// Must be square to be correct
	var IMUxMin = -50;
	var IMUxOrg = 0;
	var IMUxMax = +50;
	var IMUyMin = -50;
	var IMUyOrg = 0;
	var IMUyMax = +50;
	CANVAS_IMU = new _Canvas2DUmi('IMUcanvas', IMUxMin, IMUxOrg, IMUxMax, IMUyMin, IMUyOrg, IMUyMax,
		20, 2, 25, 20, 'IMU',
		[
			{
				LABEL: 'Acc',
				COLOR: "#0000ff"
			},
			{
				LABEL: 'Grv',
				COLOR: "#00ff00"
			},
			{
				LABEL: 'RPY',
				COLOR: "#ff0000"
			}
		]);

	// Must be square to be correct
	var MAPxMin = -50;
	var MAPxOrg = 0;
	var MAPxMax = +50;
	var MAPyMin = -50;
	var MAPyOrg = 0;
	var MAPyMax = +50;
	var CANVAS_MAP = new _Canvas2DUmi('MAPcanvas', MAPxMin, MAPxOrg, MAPxMax, MAPyMin, MAPyOrg, MAPyMax,
		20, 2, 25, 20, 'MAP',
		[
			{
				LABEL: 'Posit',
				COLOR: "#ff0000"
			},
			{
				LABEL: 'Radar',
				COLOR: "#00ff00"
			},
			{
				LABEL: 'Nord',
				COLOR: "#0000ff"
			}
		]);
	var CANVAS_PILOT = new _CanvasUmi('DRVcanvas', CFG.client.CANVAS.PILOT.MIN,
		CFG.client.CANVAS.PILOT.MAX, CFG.client.CANVAS.GAP, 'Driver', [{
			LABEL: 'Speed',
			COLOR: "#ff0000"
		}, {
			LABEL: 'Rotate',
			COLOR: "#0000ff"
		}]);
	var CANVAS_MOTORS = new _CanvasUmi('MOTcanvas',
		CFG.client.CANVAS.MOTORS.MIN, CFG.client.CANVAS.MOTORS.MAX,
		CFG.client.CANVAS.GAP, 'Motors', [{
			LABEL: 'Left',
			COLOR: "#ff0000"
		}, {
			LABEL: 'Right',
			COLOR: "#0000ff"
		}]);
	JOY_STICK = new _PilotUmi('mv2_pilotJS', 'mv2_pilotCursor', CFG.client.PILOT);
	//-----------------------------------------------------------------
	if (cfgServer.rover_ON) {
		urlXHRrover="http://"+cfgServer.rover_URL+":"+cfgServer.rover_PORT+"/" 
		urlWSrover = "ws://"+cfgServer.rover_URL+":"+cfgServer.rover_PORT+"/ws"  
		console.log("urlXHRrover='"+urlXHRrover+"'")
		console.log("urlWSrover= '"+urlWSrover+"'")
		jsonWorkFiles = jsonXHR('workFiles');
		jsonCONTEXT = jsonXHR('jsonCONTEXT');
	}
	else {
		jsonWorkFiles = {
			CHOOSE: {
				opt: { sel: 1, show: 1, entry: "Choose a file (rover OFF)", background: "#ff0000" },
				timer: 0,
				kmd: { OP: DEF_OP_CLOSE, soks: ["wsR"] }
			}
		};
		jsonCONTEXT = {};
		urlWSrover = "ws://" + window.location.host + ":" + allCFG.roverPORT + "/ws";
		urlWSrover = "";
	}
	/**
	 * 1) Create MMI Man Machine Interface modules
	 */
	_ALERT=new my_ALERT("ALERT","alertMSG",'alertOK');
	_WSROV = new Slot_socket(MODULES, "wsR",
		new __select("classDebug", 'divDbgLOC_WSROV', "IDdbg_locSOCKET", "wsR", CFG.client.__field_dlvl, "SOK_dbgLVL"),
		new __select("classDebug", 'divDbgROV_WSROV', "IDdbg_rovSOCKET", "wsR", CFG.client.__field_dlvl, "SOK_dbgLVL"),
		new __select("classExec", "divExe_WSROV", 'IDexe_WSROV', "wsR", CFG.client.__Socket_exec),
		new __select("classTimer", 'divTim_WSROV', "IDtim_WSROV", "wsR", CFG.client.__field_timer, "SOK_tmsOUT"),
		new __text("classT_i_o", 'divTio_WSROV', "IDtio_WSROV", CFG.client.__field_txt),
		new SocketCLI("rover", urlWSrover, DEF_QUERY_PILOT_TO_ROVER),
		allCFG.BamountTMS);


	/*
		 _WSBAS = new Slot_socket(MODULES, "wsB",
			new __select("classDebug", 'divDbgLOC_WSBAS', "IDdbg_locSOCKET", "wsB", CFG.client.__field_dlvl, "SOK_dbgLVL"),
			new __select("classDebug", 'divDbgROV_WSBAS', "IDdbg_rovSOCKET", "wsB", CFG.client.__field_dlvl, "SOK_dbgLVL"),
			new __select("classExec", "divExe_WSBAS", 'IDexe_WSBAS', "wsB", CFG.client.__Socket_exec),
			new __select("classTimer", 'divTim_WSBAS', "IDtim_WSBAS", "wsB", CFG.client.__field_timer, "SOK_tmsOUT"),
			new __text("classT_i_o", 'divTio_WSBAS', "IDtio_WSBAS", CFG.client.__field_txt),
			new SocketCLI("basis", "ws://" + allCFG.basisURL + ":" + allCFG.basisPORT + "/ws", DEF_QUERY_PILOT_TO_BASIS),
			allCFG.BamountTMS);
*/
	var _SELF = new Slot_watchdog(MODULES, "WAT",
		new __select("classDebug", 'divDbgLOC_WATCH', "IDdbg_locGLO", "WAT", CFG.client.__field_dlvl, "SELF_dbgLVL"),
		new __select("classDebug", 'divDbgROV_WATCH', "IDdbg_rovGLO", "WAT", CFG.client.__field_dlvl, "SELF_dbgLVL"),
		new __select("classTimer", 'divTim_WATCH', "IDtim_SELF", "WAT", CFG.client.__field_timer, "SELF_tmsOUT"),
		new __text("classT_i_o", 'divTio_WATCH', "IDtio_SELF", CFG.client.__field_txt),
		_WSROV, _WSBAS);
	var _FILSYS = new Slot_filsys(MODULES, "F_S",
		new __select("classDebug", 'divDbgROV_FILSYS', "IDdbg_rovFILSYS", "F_S", CFG.client.__field_dlvl, "FIL_dbgLVL"),
		new __select("classTimer", 'divTim_FILSYS', "IDtim_FILSYS", "F_S", CFG.client.__field_timer, "FIL_tmsOUT"),
		new __text("classT_i_o", 'divTio_FILSYS', "IDtio_SILSYS", CFG.client.__field_txt),
		_WSROV, _WSBAS);
	var _DATIM = new Slot_datim(MODULES, "TMS",
		new __select("classDebug", 'divDbgLOC_DATIM', "IDdbg_locDATIM", "TMS", CFG.client.__field_dlvl),
		new __select("classDebug", 'divDbgROV_DATIM', "IDdbg_rovDATIM", "TMS", CFG.client.__field_dlvl),
		new __select("classExec", "divExe_DATIM", 'IDexe_DATIM', "TMS", CFG.client.__Datim_exec),
		new __select("classTimer", 'divTim_DATIM', "IDtim_DATIM", "TMS", CFG.client.__field_timer),
		new __text("classT_i_o", 'divTio_DATIM', "IDtio_DATIM", CFG.client.__field_txt),
		_WSROV, _WSBAS);
	_MOTORS = new Slot_pilot(MODULES, "MOT",
		new __select("classDebug", 'divDbgLOC_MOTORS', "IDdbg_locMOTORS", "MOT", CFG.client.__field_dlvl),
		new __select("classDebug", 'divDbgROV_MOTORS', "IDdbg_rovMOTORS", "MOT", CFG.client.__field_dlvl),
		new __select("classExec", "divExe_MOTORS", 'IDexe_MOTORS', "MOT", CFG.client.__Motors_exec),
		new __select("classTimer", 'divTim_MOTORS', "IDtim_MOTORS", "MOT", CFG.client.__field_timer),
		new __text("classT_i_o", 'divTio_MOTORS', "IDtio_MOTORS", CFG.client.__field_txt),
		JOY_STICK, CANVAS_PILOT, CANVAS_MOTORS, _WSROV, _WSBAS);
	var _DRIVER = new Slot_driver(MODULES, "DRV",
		new __select("classDebug", 'divDbgLOC_DRIVER', "IDdbg_locDRIVER", "DRV", CFG.client.__field_dlvl),
		new __select("classDebug", 'divDbgROV_DRIVER', "IDdbg_rovDRIVER", "DRV", CFG.client.__field_dlvl),
		new __select("classExec", "divExe_DRIVER", 'IDexe_DRIVER', "DRV", CFG.client.__Driver_exec),
		new __select("classTimer", 'divTim_DRIVER', "IDtim_DRIVER", "DRV", CFG.client.__field_timer),
		new __html('ATT'),
		JOY_STICK, CANVAS_PILOT, CANVAS_MOTORS, CANVAS_IMU, CANVAS_MAP, _WSROV, _WSBAS);

	_JOBGOAL = new Slot_jobgoal(MODULES, "JOB",
		new __select("classDebug", 'divDbgLOC_JOBGOAL', "IDdbg_locJOBGOAL", "JOB", CFG.client.__field_dlvl),
		new __select("classDebug", 'divDbgROV_JOBGOAL', "IDdbg_rovJOBGOAL", "JOB", CFG.client.__field_dlvl),
		new __select("sel_WORKJOB", "divExe_WORKJOB", 'IDchooseWorkjob', "JOB", jsonWorkFiles), // Choix fichier.work
		new __select("classTimer", 'divTim_JOBGOAL', "IDtim_JOBGOAL", "JOB", CFG.client.__field_timer), // TODO: Timer iutile !!!
		new __text("IOworkjob", 'WORKJOB_io', "IOworkjob", CFG.client.__field_txt), // Log
		JOY_STICK, CANVAS_PILOT, CANVAS_MOTORS, _WSROV, _WSBAS);
	var _LEDS = new Slot_leds(MODULES, "LED",
		new __select("classDebug", 'divDbgLOC_LEDS', "IDdbg_locLEDS", "LED", CFG.client.__field_dlvl),
		new __select("classDebug", 'divDbgROV_LEDS', "IDdbg_rovLEDS", "LED", CFG.client.__field_dlvl),
		new __select("classExec", 'divExe_LEDS', "IDexe_LEDS", "LED", CFG.client.__Leds_exec),
		new __select("classTimer", 'divTim_LEDS', "IDtim_LEDS", "LED", CFG.client.__field_timer),
		new __text("classT_i_o", 'divTio_LEDS', "IDtio_LEDS", CFG.client.__field_txt),
		_WSROV, _WSBAS);
	var joystick = document.getElementById('mv2_pilotJS');
	joystickBounds = joystick.getBoundingClientRect();
	console.log(joystickBounds);
	//-----------------------------------------------------------------
	/**
	 * 2) Create MMI Machine Machine Interface modules Node compatibility
	 */
	var MMI_TPDI_DIR = new _TpidMmi(CFG.client.TPIDDIR);
	var MMI_TPDI_VIT = new _TpidMmi(CFG.client.TPIDVIT);
	//-----------------------------------------------------------------
	/**
	 * 4) Run application
	 */
	this.APP = new _runAPP(MODULES, _SELF, _WSROV, _WSBAS, _FILSYS, _DATIM,
		_LEDS, _MOTORS, _JOBGOAL, _DRIVER);
	/** ***************************************************************
	 *
	 *	2) Define controls
	 */
	/** ---------------------------------------------------------------
	 * @fn function initRanges(CONTROL)
	 * @brief Initializes the dialog control CONTROL
	 * @param CONTROL object to initialize
	 */
	function initRanges(CONTROL) {
		var Params = Object.assign(CONTROL.Params);
		var Values = Object.assign(CONTROL.Values);
		for (const [Dlg, Ctl] of Object.entries(CONTROL.Dlgs))
			Ctl.range = new __range(CONTROL.Dlgs, Ctl.div, Params.label,
				Values.Min, Values.Def, Values.Step, Values.Max,
				Params.varKey, Params.setter);
	}
	/** ---------------------------------------------------------------
	 * @fn function initRadiobtns(CONTROL)
	 * @brief Initializes the dialog control CONTROL
	 * @param CONTROL object to initialize
	 */
	function initRadiobtns(CONTROL) {
		var Params = Object.assign(CONTROL.Params);
		for (const [Dlg, Ctl] of Object.entries(CONTROL.Dlgs))
			Ctl.radiobtns = new __radiobtns(CONTROL.Dlgs, Ctl.div, CONTROL.Idx,
				Params.label, CONTROL.Values,
				Params.varKey, Params.setter);
	}
	//	_CHOOSE_Dbg ------------------------------
	initRadiobtns({
		Dlgs: {
			DBG: { radiobtns: {}, div: '_CHOOSE_Dbg' },
			ATT: { radiobtns: {}, div: '_CHOOSE_Att' }
		},
		Params: { label: ['requêtes', 'titi'], varKey: 'MAX_DBG_DRV_XIMU', setter: _DRIVER._OP_setCTXparam },
		Values: ['val_requêtes', 'val_titi'],
		Idx: 1
	});
	//	_CHOOSE2_Dbg ------------------------------
	initRadiobtns({
		Dlgs: {
			DBG: { radiobtns: {}, div: '_CHOOSE2_Dbg' }
		},
		Params: { label: ['tata', 'toto', 'tutu'], varKey: 'MAX_DBG_DRV_XIMU', setter: _DRIVER._OP_setCTXparam },
		Values: ['val_tata', 'val_toto', 'val_tutu'],
		Idx: 0
	});
	/** ---------------------------------------------------------------
	 * @fn function initCheckboxes(CONTROL)
	 * @brief Initializes the dialog control CONTROL
	 * @param CONTROL object to initialize
	 */
	function initCheckboxes(CONTROL) {
		var Params = Object.assign(CONTROL.Params);
		for (const [Dlg, Ctl] of Object.entries(CONTROL.Dlgs))
			Ctl.checkbox = new __checkbox(CONTROL.Dlgs, Ctl.div, Params.label,
				CONTROL.Value, Params.varKey, Params.setter);
	}
	//	show_drvInfos_Dbg ------------------------------
	initCheckboxes({
		Dlgs: {
			DBG: { checkbox: {}, div: 'show_drvInfos_Dbg' },
			ATT: { checkbox: {}, div: 'show_drvInfos_Att' }
		},
		Params: { label: 'Show driver infos', varKey: 'show_drvInfos', setter: _DRIVER._OP_setCTXparam },
		Value: jsonCONTEXT.show_drvInfos
	});
	//	MAX_DBG_DRV_XIMU ------------------------------
	initRanges({
		Dlgs: {
			DBG: { range: {}, div: 'MAX_DBG_DRV_XIMU_Dbg' }
		},
		Params: { label: 'DRV_XIMU', varKey: 'MAX_DBG_DRV_XIMU', setter: _DRIVER._OP_setCTXparam },
		Values: { Min: 0, Def: jsonCONTEXT.MAX_DBG_DRV_XIMU, Max: jsonCONTEXT._MAX_DBG_DRV_XIMU, Step: 1 }
	});
	//	ALL_DBGMAX_TRUNC ------------------------------
	initRanges({
		Dlgs: {
			DBG: { range: {}, div: 'ALL_DBGMAX_TRUNC_Dbg' }
		},
		Params: { label: 'DBG_TRUNC', varKey: 'ALL_DBGMAX_TRUNC', setter: _DRIVER._OP_setCTXparam },
		Values: { Min: 0, Def: jsonCONTEXT.ALL_DBGMAX_TRUNC, Max: 9, Step: 1 }
	});
	//	Acceleration threshold ------------------------------
	initRanges({
		Dlgs: {
			ATT: { range: {}, div: 'thr_linAcc_Att' }
		},
		Params: { label: '&gt;Accel', varKey: 'thr_linAcc', setter: _DRIVER._OP_setCTXparam },
		Values: { Min: 0, Def: 3, Max: 20, Step: 0.1 }
	});
	//	Position scale --------------------------------------
	initRanges({
		Dlgs: {
			ATT: { range: {}, div: 'scalePos_Att' },
			TJS: { range: {}, div: 'scalePos_Tjs' }
		},
		Params: { label: 'Echelle', varKey: 'scalePos', setter: set_tjsATT },
		Values: tjsATT
	});
	/** ***************************************************************
	 * 
	 *	IMU CONTROLS
	 */
	//	Bêta coefficient ------------------------------------
	initRanges({
		Dlgs: {
			IMU: { range: {}, div: 'Beta_Imu' },
		},
		Params: { label: 'Bêta', varKey: 'beta', setter: _DRIVER._OP_setCTXparam },
		Values: { Min: 0, Def: jsonCONTEXT.beta, Max: 10, Step: 0.0001 }
	});

	//	Bêta coefficient ------------------------------------
	initRanges({
		Dlgs: {
			IMU: { range: {}, div: 'cntInt_Imu' },
		},
		Params: { label: 'Nb int', varKey: 'cntInt', setter: _DRIVER._OP_setCTXparam },
		Values: { Min: 1, Def: jsonCONTEXT.cntInt, Max: 100, Step: 1 }
	});


	/** ***************************************************************
	 * 
	 *	3) Adds global AREAS, VIEWS, horBtns event listeners
	 */
	LAYOUT = {
		'CENTER_ur': {
			ATT: { ELT: document.getElementById("ATT") },
			IMU: { ELT: document.getElementById("IMU") },
			M3D: { ELT: document.getElementById("M3D") },
			SYS: { ELT: document.getElementById("SYS") },
			MAP: { ELT: document.getElementById("MAP") },
			MOTORS: { ELT: document.getElementById("MOTORS") },
			WORKJOB: { ELT: document.getElementById("WORKJOB") }
		},
		'CENTER_ul': {
			ATT_ctrl: { ELT: document.getElementById("ATT_ctrl") },
			IMU_ctrl: { ELT: document.getElementById("IMU_ctrl") },
			M3D_ctrl: { ELT: document.getElementById("M3D_ctrl") },
			SYS_ctrl: { ELT: document.getElementById("SYS_ctrl") }
		},
		"HEAD": {
			ATT_head: { ELT: document.getElementById("ATT_head") },
			IMU_head: { ELT: document.getElementById("IMU_head") },
			M3D_head: { ELT: document.getElementById("M3D_head") },
			SYS_head: { ELT: document.getElementById("SYS_head") },
			WORKJOB_head: { ELT: document.getElementById("WORKJOB_head") }
		},
		"FOOT": {
		},
		"LEFT": {
		},
		"RIGHT": {
		}
	};
	//
	//	Add listeners to swap horizontal buttons Hactive class
	// Dont't forget adding the Hactive class to the default horizontal container button"
	// 
	var viewsMenu = document.getElementById("viewsMenu");
	var viewBtns = viewsMenu.getElementsByClassName("radioBtn");
	for (var i = 0; i < viewBtns.length; i++) {
		viewBtns[i].addEventListener("click", function() {
			var current = viewsMenu.getElementsByClassName("Hactive");
			current[0].className = current[0].className.replace(" Hactive", "");
			this.className += " Hactive";
		});
	}
	var ctrlsMenu = document.getElementById("ctrlsMenu");
	var ctrlBtns = ctrlsMenu.getElementsByClassName("radioBtn");
	for (var i = 0; i < ctrlBtns.length; i++) {
		ctrlBtns[i].addEventListener("click", function() {
			var current = ctrlsMenu.getElementsByClassName("Hactive");
			current[0].className = current[0].className.replace(" Hactive", "");
			this.className += " Hactive";
		});
	}
	var M3D_head = document.getElementById("M3D_head");
	var m3dBtns = M3D_head.getElementsByClassName("radioBtn");
	for (var i = 0; i < m3dBtns.length; i++) {
		m3dBtns[i].addEventListener("click", function() {
			var current = M3D_head.getElementsByClassName("Hactive");
			current[0].className = current[0].className.replace(" Hactive", "");
			this.className += " Hactive";
		});
	}
	/** ***************************************************************
	 *	5) Customize views
	 */
	showView('CENTER_ur', 'ATT');
}
///////////////////////////////////////////////////////////////////////
// pour réception formulaire (visible hors de start
/*
 * Cf.http://linuxcnc.org/docs/html/motion/pid_theory_fr.html
 * 
*/
var envoiManualKommand = function(wsSLOT) {
	// On récupère le message
	var ModKmd = document.getElementById('ModKmd').value;
	var occ = ModKmd.split("$");
	if (ModKmd != '') {
		MODULES[wsSLOT].Query(occ[0], JSON.parse("{" + occ[1] + "}"));
		// On vide le formulaire ?
		// document.getElementById('kommand').value = '';
	}
	// On retourne false pour que le formulaire n'actualise la page
	return false;
}
///////////////////////////////////////////////////////////////////////
