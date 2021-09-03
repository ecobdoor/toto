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
 * =================================================================
 */
function oldfilesToHtmlSelect(ID, ITEMS, IDX, SCLASS) {
	var idx = (typeof IDX == 'undefined') ? -1 : IDX;
	var sclass = (typeof SCLASS == 'undefined') ? '' : " " + SCLASS;

	console.log("filesToHtmlSelect ITEMS.length", ITEMS.length);
	var cnt = 0;
	var txt = "<select id='" + ID + "' class='mv2_select " + sclass + "'>";
	// txt+="<option value=-1>...</option>";
	for (var i = 0; i < ITEMS.length; i++) {
		if (i == idx) {
			var sel = ' selected="selected"';
		} else {
			var sel = "";
		}
		txt += "<option value=" + cnt + sel + ">" + ITEMS[i].entry
				+ "</option>";
		cnt++;
	}
	txt += "</select>";
	return txt;
}
/*
 * ================================================================= Crée un
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
	for ( var key in ITEMS) {
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
// //////////////////////////////////////////////////////////////////////
