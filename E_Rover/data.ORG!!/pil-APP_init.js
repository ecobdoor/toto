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
var addMsg = (function() {
	var cnt = 0;
	var keep = 10;
	return function(LVL, HEAD, CHN, FOOT) {
		var Tab = "";
		for (var i = 0; i < LVL; i++) {
			Tab += i + "   ";
		}
		var element = document.getElementById('log');
		// Verify if already more than keep lines in txt
		var txt = element.innerHTML;
		cnt++;
		if (cnt > keep)
			txt = txt.substring(txt.indexOf('</div>'));
		document.getElementById('log').innerHTML = txt
			+ '<div class="line"><b>' + HEAD + '</b>' + CHN + "<b>" + FOOT
			+ '</b></div>';
		// element.scrollTop = 1000000;//Scroll à la fin à améliorer
	}
})();
/*
 * Start une fois la page chargée
 */

function Start() {
//window.setTimeout(alert("Atente du chargement complet"), 2000);	
	var SELF = this;
	var xMin = -1000;
	var xMax = +1000;
	var yMin = -1000;
	var yMax = +1000;
	var CANVAS_MAP = new _Canvas2DUmi('MAPcanvas', xMin, xMax, yMin, yMax, 50, 10, 30, 25, 'MAP',
		[{
			LEGEND: 'AccXY',
			COLOR: "#0000ff"
		}, {
			LEGEND: 'motLF',
			COLOR: "#ff0000"
		}, {
			LEGEND: 'Gxy',
			COLOR: "#00ff00"
		}]);
	CANVAS_MAP.Line(0, 0, 50, 50, "#888888");
	CANVAS_MAP.Line(0, 0, 30, 50, "#aaaaaa");
	CANVAS_MAP.Point(100, 100, "#000000");
	CANVAS_MAP.placeLine(0, xMin, 0, xMax, 0);
	CANVAS_MAP.placeLine(0, 0, yMin, 0, yMax);
	CANVAS_MAP.placePoint(1, 0.8, -0.5);
	var CANVAS_PILOT = new _CanvasUmi('DIRcanvas', CFG.client.CANVAS.PILOT.MIN,
		CFG.client.CANVAS.PILOT.MAX, CFG.client.CANVAS.GAP, 'Driver', [{
			LEGEND: 'Speed',
			COLOR: "#ff0000"
		}, {
			LEGEND: 'Rotate',
			COLOR: "#0000ff"
		}]);
	var CANVAS_MOTORS = new _CanvasUmi('VITcanvas',
		CFG.client.CANVAS.MOTORS.MIN, CFG.client.CANVAS.MOTORS.MAX,
		CFG.client.CANVAS.GAP, 'Motors', [{
			LEGEND: 'Left',
			COLOR: "#ff0000"
		}, {
			LEGEND: 'Right',
			COLOR: "#0000ff"
		}]);
	var JOY_STICK = new _PilotUmi('mv2_pilotJS', 'mv2_pilotCursor',
		CFG.client.PILOT);
	/**
	 * 1) Create MMI Man Machine Interface modules
	 */
	var _WSROV = new Slot_socket(MODULES, "wsR",
		new __select("classDebug", 'divDbgLOC_WSROV', "IDdbg_locSOCKET", "wsR", CFG.client.__field_dlvl, "SOK_dbgLVL"),
		new __select("classDebug", 'divDbgROV_WSROV', "IDdbg_rovSOCKET", "wsR", CFG.client.__field_dlvl, "SOK_dbgLVL"),
		new __select("classExec", "divExe_WSROV", 'IDexe_WSROV', "wsR", CFG.client.__Socket_exec),
		new __select("classTimer", 'divTim_WSROV', "IDtim_WSROV", "wsR", CFG.client.__field_timer, "SOK_tmsOUT"),
		new __text("classT_i_o", 'divTio_WSROV', "IDtio_WSROV", CFG.client.__field_txt),
		new SocketCLI("rover", "ws://" + allCFG.roverURL + ":" + allCFG.roverPORT + "/ws", DEF_QUERY_PILOT_TO_ROVER),
		allCFG.BamountTMS);
	var _WSBAS = new Slot_socket(MODULES, "wsB",
		new __select("classDebug", 'divDbgLOC_WSBAS', "IDdbg_locSOCKET", "wsB", CFG.client.__field_dlvl, "SOK_dbgLVL"),
		new __select("classDebug", 'divDbgROV_WSBAS', "IDdbg_rovSOCKET", "wsB", CFG.client.__field_dlvl, "SOK_dbgLVL"),
		new __select("classExec", "divExe_WSBAS", 'IDexe_WSBAS', "wsB", CFG.client.__Socket_exec),
		new __select("classTimer", 'divTim_WSBAS', "IDtim_WSBAS", "wsB", CFG.client.__field_timer, "SOK_tmsOUT"),
		new __text("classT_i_o", 'divTio_WSBAS', "IDtio_WSBAS", CFG.client.__field_txt),
		new SocketCLI("basis", "ws://" + allCFG.basisURL + ":" + allCFG.basisPORT + "/ws", DEF_QUERY_PILOT_TO_BASIS),
		allCFG.BamountTMS);
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
	var _MOTORS = new Slot_pilot(MODULES, "MOT",
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
		new __textarea("classTA_i_o", 'divTio_DRIVER', "IDtio_DRIVER", CFG.client.__field_txtarea),
		JOY_STICK, CANVAS_PILOT, CANVAS_MOTORS, CANVAS_MAP, _WSROV, _WSBAS);
	var _JOBGOAL = new Slot_jobgoal(MODULES, "JOB",
		new __select("classDebug", 'divDbgLOC_JOBGOAL', "IDdbg_locJOBGOAL", "JOB", CFG.client.__field_dlvl),
		new __select("classDebug", 'divDbgROV_JOBGOAL', "IDdbg_rovJOBGOAL", "JOB", CFG.client.__field_dlvl),
		new __select("classExec", "divExe_JOBGOAL", 'IDexe_JOBGOAL', "JOB", CFG.client.__Jobgoal_exec),
		new __select("classTimer", 'divTim_JOBGOAL', "IDtim_JOBGOAL", "JOB", CFG.client.__field_timer),
		new __text("classT_i_o", 'divTio_JOBGOAL', "IDtio_JOBGOAL", CFG.client.__field_txt),
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
	// ///////////////////////////////////////////////////////
	/**
	 * 2) Create MMI Machine Machine Interface modules Node compatibility
	 */
	var MMI_TPDI_DIR = new _TpidMmi(CFG.client.TPIDDIR);
	var MMI_TPDI_VIT = new _TpidMmi(CFG.client.TPIDVIT);
	// ///////////////////////////////////////////////////////
	/**
	 * 3) Run application
	 */
	this.APP = new _runAPP(MODULES, _SELF, _WSROV, _WSBAS, _FILSYS, _DATIM,
		_LEDS, _MOTORS, _JOBGOAL, _DRIVER);
}
// pour réception formulaire (visible hors de start
/*
 * Cf. http://linuxcnc.org/docs/html/motion/pid_theory_fr.html
 * 
 * 
 * MOT$"OP":"Pid","ax":1,"KP":0.5,"DP":5,"KI":1,"DI":5,"KD":0,"DD":0 Oscillation
 * légère MOT$"OP":"Pid","ax":1,"KP":1,"DP":5,"KI":0,"DI":0,"KD":0,"DD":0
 * Oscillation MOT$"OP":"Pid","AX":1,"KP":2,"DP":5,"KI":0,"DI":0,"KD":0,"DD":0
 * MOT$"OP":"Pid","AX":1,"KP":2,"DP":5,"KI":0.5,"DI":4,"KD":0,"DD":0
 * MOT$"OP":"Pid","AX":1,"KP":2,"DP":5,"KI":0.5,"DI":4,"KD":0.1,"DD":3
 */
var envoiManualKommand = function(wsSLOT) {
	// On récupère le message
	var ModKmd = document.getElementById('ModKmd').value;
	var occ = ModKmd.split("$");
	if (ModKmd != '') {
		MODULES[wsSLOT].Send(occ[0], JSON.parse("{" + occ[1] + "}"));
		// On vide le formulaire ?
		// document.getElementById('kommand').value = '';
	}
	// On retourne false pour que le formulaire n'actualise la page
	return false;
}
// ////////////////////////////////////////////////////////////////////
