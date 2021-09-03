;
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
	this.sokROVER = SOCKET;
	this._Timer = new Timer();
	MODULES[MID] = this;
	// -----------------------------------------------------------------
	/**
	 * SOCKET SEND
	 */
	this.Send = function(MID, KMD) {
		var statusOBJ = {};
		statusOBJ = sendKMD(this.sokROVER, MID, KMD, BAmount_TMS);
		// SELF.TIO._setText("+++++++");
	}
	// -----------------------------------------------------------------
	/**
	 * SOCKET ONESHOT function
	 */
	this.__oneShot = function() {
		SELF.TIO._setText("+++++++");
	}
	// -----------------------------------------------------------------
	/**
	 * SOCKET EXEC events
	 */
	this.affStatus = function(CHN) {
		// addMsg(0,"wsCreate","**********");
		SELF.TIO._setText(CHN);
	}
	this.stopSender = function(CHN) {
		// addMsg(0,"wsCreate","**********");
		SELF.TIO._setText(CHN);
	}
	this.addMsg = function(HEAD, CHN, FOOT) {
		// addMsg(0,"wsCreate","**********");
		addMsg(0, HEAD, CHN, FOOT);
	}
	this.node__exeEvents = function(APP) {
		SELF.App = APP;
		if (isDefined(this.sokROVER))
			this.sokROVER.CONNECT();
	}
	this.__exeEvents = function(APP) {
		if (isDefined(this.sokROVER))
			this.sokROVER.slot = this;
		this.node__exeEvents(APP);
		// -----------------------------------------------------------------
		/**
		 * SOCKET event Connecter Fermer
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
					SLOTROV.Send({
						"set": parseInt(SELF.EXE._cfgItemByIdx(idx).kmd.codeRYG,
							10)
					});

			}
		};
		// -----------------------------------------------------------------
		/**
		 * SOCKET TIMER event & callback Fréquence du Ping
		 */
		SELF._Timer.CHANGE(SELF.__oneShot); // init callback !!!
		SELF.TIM._Elt.onchange = function() {
			var idx = SELF.TIM._Elt.value;
			console.log("[", idx, "]:", SELF.TIM._cfgItemByIdx(idx));
			SELF.TIM._tagOption(SELF.TIM._cfgItemByIdx(idx).opt, idx);
			var tmsOUT = parseInt(SELF.TIM._cfgItemByIdx(idx).kmd.codeTMS, 10);
			var KMD = {};
			KMD[SELF.TIM._cfgItemByIdx(idx).kmd.OP] = tmsOUT;
			SLOTROV.Send(KMD);
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
	 * SOCKETActivation events
	 */
	this.launchEvents = function(APP) {
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		this.__exeEvents(APP);
	}
};
// /////////////////////////////////////////////////////////////////////
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
	this.launchEvents = function(APP) {
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
	}
};
// /////////////////////////////////////////////////////////////////////
/**
 * SLOT DRIVER
 */
Slot_driver = function(MODULES, MID, DBG_loc, DBG_rov, EXE, TIM, TIO, JOYSTICK,
	CANVAS_PILOT, CANVAS_MOTORS, CANVAS_MAP, SLOTROV, SLOTBAS) {
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
	 * DRIVER reply2query nothing to do yet
	 */
	this.reply2query = function(KMD) {
		SELF.TIO._setText("DRIVER > [" + JSON.stringify(KMD) + "]\nsdlkjfwdfljk");
		var kmd = KMD;
		return kmd;
	}
	// -----------------------------------------------------------------
	/**
	 * DRIVER update2reply display driver intern parameters
	 */
	this.update2reply = function(KMD) {
		var DMP = "";
		DMP += "Position Ts=" + SF_float(KMD[DEF_KF_PMTS] / 1000000.0, 12, 3) + " sec";
		DMP += ",  radar=" + SF_float(KMD[DEF_KF_DIST], 12, 3) + " m\n";
		DMP += "P xyz (" + SF_float(KMD[DEF_KF_XC0], 12, 3) + " " + SF_float(KMD[DEF_KF_YC0], 12, 3) + " " + SF_float(KMD[DEF_KF_ZC0], 12, 3) + ")m\n";
		DMP += "S xyz'(" + SF_float(KMD[DEF_KF_SPX], 12, 3) + " " + SF_float(KMD[DEF_KF_SPY], 12, 3) + " " + SF_float(KMD[DEF_KF_SPZ], 12, 3) + ")mm/s\n";
		DMP += "A xyz\"(" + SF_float(KMD[DEF_KF_LAX], 12, 3) + " " + SF_float(KMD[DEF_KF_LAY], 12, 3) + " " + SF_float(KMD[DEF_KF_LAZ], 12, 3) + ")mm/s²\n";
		DMP += "⨁ rpy(" + SF_float(KMD[DEF_KF_ROLL], 12, 3) + " " + SF_float(KMD[DEF_KF_PITCH], 12, 3) + " " + SF_float(KMD[DEF_KF_YAW], 12, 3) + ")°\n";
		DMP += "Grav  (" + SF_float(KMD[DEF_KF_GRX], 12, 3) + " " + SF_float(KMD[DEF_KF_GRY], 12, 3) + " " + SF_float(KMD[DEF_KF_GRZ], 12, 3) + ")mg\n";
		DMP += "IMU------------------ tik=" + KMD[DEF_KF_IMUT] + "------\n";
		DMP += "Acc(" + SF_float(KMD[DEF_KF_Iax], 12, 3) + " " + SF_float(KMD[DEF_KF_Iay], 12, 3) + " " + SF_float(KMD[DEF_KF_Iaz], 12, 3) + ")\n";
		DMP += "Gyr(" + SF_float(KMD[DEF_KF_Igx], 12, 3) + " " + SF_float(KMD[DEF_KF_Igy], 12, 3) + " " + SF_float(KMD[DEF_KF_Igz], 12, 3) + ")\n";
		DMP += "Mag(" + SF_float(KMD[DEF_KF_Imx], 12, 3) + " " + SF_float(KMD[DEF_KF_Imy], 12, 3) + " " + SF_float(KMD[DEF_KF_Imz], 12, 3) + ")\n";
		DMP += "\n";
		DMP += "MOT L-R(" + SF_float(KMD[DEF_KF_LFT], 12, 3) + " " + SF_float(KMD[DEF_KF_RGT], 12, 3) + ")";

		SELF.TIO._setText(DMP);
		CANVAS_MAP.placePoint(1, KMD[DEF_KF_LFT], KMD[DEF_KF_RGT]);
		CANVAS_MAP.placePoint(0, KMD[DEF_KF_LAX], KMD[DEF_KF_LAY]);
		CANVAS_MAP.placePoint(2, KMD[DEF_KF_GRX], KMD[DEF_KF_GRY]);

		MODULES["MOT"].update2reply(KMD);
		// SELF.TIO._setText("DRIVER < [" + JSON.stringify(KMD) + "]\nsdlkjfwdfljk");
	}
	// -----------------------------------------------------------------
	/**
	 * DRIVER Activation events
	 */
	this.launchEvents = function(APP) {
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		// SELF.TIO._setText("DRIVER > []\nsdlkjfwdfljk");
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
	var _qryFile = "";
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
				if (_qryFile != KMD[DEF_KF_FIL]) {
					alert("Learning '" + _qryFile + "' error '" + KMD[DEF_KF_FIL] + "'?");
					_qryFile = "";
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
				_qryFile = "";
				SELF.TIO._setText(" < Learn STOPPED");
				break;
			default:
				SELF.TIO._setText("..." + JSON.stringify(KMD));
		}
		SELF.TIO._setText("(Speed,Dir)=(" + (KMD[DEF_KF_SPD]) + ","
			+ (KMD[DEF_KF_DIR]) + ")" + " > (Left,Right)=("
			+ (KMD[DEF_KF_LFT]) + ","
			+ (KMD[DEF_KF_RGT]) + ")");
		CANVAS_PILOT.update([KMD[DEF_KF_SPD], KMD[DEF_KF_DIR]]);
		CANVAS_MOTORS.update([Math.round(KMD[DEF_KF_LFT]), Math
			.round(KMD[DEF_KF_RGT])]);
		// In driver or jobgoal mode place the joystick
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
		if (_qryFile == "")
			alert("No learning in progress. Force stopping !!!");
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_CLOSE,
			TMS: TMS
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Send(MID, kmd);
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
			[DEF_KF_FIL]: _qryFile,
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Send(MID, kmd);
		});
		SELF.TIO._setText("Beginning learning on " + _qryFile);
	}
	// -----------------------------------------------------------------
	/**
	 * PILOT _OP_driving steering or learning
	 */
	this._OP_driving = function() {
		var joystick = JOYSTICK.getVal();
		console.log("joystick", joystick);
		SLOTROV.Send("MOT", {
			OP: DEF_OP_MOT,
			[DEF_KF_SPD]: Math.round(joystick.VIT),
			[DEF_KF_DIR]: -Math.round(joystick.DIR) // Sens trigo !!!
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
		SLOTROV.Send("MOT", Object.assign({
			OP: DEF_OP_MOT
		}, kmd));
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
	 * PILOT __OPchange
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
				if (_qryFile != "") {
					SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(_qryIDX).opt,
						_qryIDX);
					alert("Stop previous learning '" + _qryFile + "' please...");
				} else {
					_qryFile = SELF.EXE._cfgItemByIdx(IDX).opt.entry;
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
	this.launchEvents = function(APP) {
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		exeEvents(SELF, APP);
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
	 * DATIM update2reply display dTMS latency returned by rover
	 */
	this.update2reply = function(KMD) {
		SELF.TIO._setText("DATIM < dTMS='" + KMD.DT + "' ms.");
	}
	// -----------------------------------------------------------------
	/**
	 * DATIM ONESHOT function Synchronize rover clock with pilot client \brief
	 * add scheduler TMSOUT to TMS to keep latency coherence
	 */
	this._OP_synchronize = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_SYNC,
			TMS: TMS
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Send("TMS", kmd);
		});
		SELF.TIO._setText("DATIM > TMS=" + TMS);
	}
	// -----------------------------------------------------------------
	/**
	 * DATIM reply2query OP is Syn only
	 */
	this.reply2query = function(KMD) {
		var kmd = KMD;
		kmd.DT = (new Date()).getTime() - kmd.TMS;
		return kmd;
	}
	// -----------------------------------------------------------------
	/**
	 * DATIM __OPchange
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
	 * DATIM Activation events
	 */
	this.launchEvents = function(APP) {
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		exeEvents(SELF, APP);
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
	var _qryFile = "";
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
		switch (KMD.OP) {
			case DEF_OP_OPEN:
				if (_qryFile != KMD[DEF_KF_FIL]) {
					alert("Jobgoal '" + _qryFile + "' error '" + KMD[DEF_KF_FIL] + "'?");
					_qryFile = "";
					_qryIDX = 0;
				} else {
					_qryLine = 1;
					SELF._Timer.CHANGE(SELF._OP_scanRover);
					timerChange(SELF, _qryIDX);// faux yl y a une indirection en
					// +!!
				}
				break;
			case DEF_OP_CLOSE:
				_qryFile = "";
				SELF.TIO._setText("Jobgoal < STOPPED (Run=" + KMD[DEF_KF_EXE]
					+ ").");
				break;
			default:
				SELF.TIO._setText(" < JOB run(" + KMD[DEF_KF_EXE]
					+ ").");
		}
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL OP_close_Jobgoal function asks rover to end jobgoal
	 */
	this.OP_close_Jobgoal = function() {
		if (_qryFile == "")
			alert("No jobgoal in progress. Force stopping !!!");
		var kmd = { OP: DEF_OP_CLOSE };
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Send(MID, kmd);
		});
		SELF.TIO._setText("Jobgoal ending...");
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL OP_open_Jobgoal function asks rover to begin jobgoal
	 */
	this.OP_open_Jobgoal = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_OPEN,
			[DEF_KF_FIL]: _qryFile,
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Send(MID, kmd);
		});
		SELF.TIO._setText("Beginning jobgoal TMS=" + TMS);
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL _OP_scanRover function asks rover status during jobgoal
	 */
	this._OP_scanRover = function() {
		var TMS = (new Date()).getTime();// +SELF._Timer._Timeout;
		var kmd = {
			OP: DEF_OP_OK,
			TMS: TMS
		};
		SELF.SOKs.forEach(function(sok) {
			MODULES[sok].Send(MID, kmd);
		});
		// SELF.TIO._setText("TMS=" + TMS);
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL __OPchange
	 */
	this.__OPchange = function(IDX) {
		console.log("[", IDX, "]:", SELF.EXE._cfgItemByIdx(IDX));
		SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(IDX).opt, IDX);
		var kmd = SELF.EXE._cfgItemByIdx(IDX).kmd;
		switch (kmd.OP) {
			case DEF_OP_OPEN:
				if (_qryFile != "") {
					SELF.EXE._tagOption(SELF.EXE._cfgItemByIdx(_qryIDX).opt,
						_qryIDX);
					alert("Stop previous jobgoal '" + _qryFile + "' please...");
				} else {
					_qryFile = SELF.EXE._cfgItemByIdx(IDX).opt.entry;
					_qryIDX = IDX;
					SELF._Timer.CHANGE(SELF.OP_open_Jobgoal);
					SELF._Timer.START_oneShoot(10);

					SELF._Timer.CHANGE(SELF._OP_scanRover);
					SELF.SOKs = kmd.soks;
				}
				break;
			case DEF_OP_CLOSE:
				SELF._Timer.CHANGE(SELF.OP_close_Jobgoal);
				SELF.SOKs = kmd.soks;
				SELF._Timer.START_oneShoot(10);
				break;
			default:
				alert("Sorry, `" + kmd.OP + "` unkwnon !!");
		}
	}
	// -----------------------------------------------------------------
	/**
	 * JOBGOAL Activation events
	 */
	this.launchEvents = function(APP) {
		this.DBG_loc.__selectNosendevents(APP);
		this.DBG_rov.__selectDefaultevents(APP);
		this.TIO.__textDefaultevents(APP);
		exeEvents(SELF, APP);
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
	this.launchEvents = function(APP) {
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
	this.launchEvents = function(APP) {
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
