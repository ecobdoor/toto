/**
 * These routines are used to complete the html file
 * 
 * @param SCLASS
 *            class to prettify the injected element(s)
 * @param DIV
 *            id of html <div> to complete (via innerHTLM)
 * @param ID
 *            id of injected element(s)
 * @param CONF
 *            configuration data for injection if opt is in CONF preselects the
 *            default element (optional)
 * @param KMD_OP
 *            operation codes associates with each element (optional)
 */
/*
 * =================================================================
 * 
 * CLASS TEXT
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
		+ input.value + "' class='mv2_select " + SCLASS + "'"
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
	}
	/*
	 * ================================================================= SELECT
	 * AN OPTION
	 * =================================================================
	 */
	this.selectOpt = function(IDX, SEND) {
		console.log("selectOpt(", SEND, ",", MODULE, ") [", IDX, "]:", SELF
			._cfgItemByIdx(IDX));
		SELF._Elt.selectedIndex = IDX;
		SELF._tagOption(SELF._cfgItemByIdx(IDX).opt, IDX);
		/*
		 * var op = SELF._cfgItemByIdx(IDX).kmd.OP; var KMD = {}; KMD[op] =
		 * parseInt(IDX, 10); if (SEND) SELF.App.Send( MODULE, KMD);// 0 ????
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
