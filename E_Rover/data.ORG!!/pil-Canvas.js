;
function _CanvasUmi(CANVAS, YMIN, YMAX, GAP, TITLE, LEGENDS_COLORS/* LEGEND_A,COLOR_A,LEGEND_B,COLOR_B */) {
	// *******************************************************
	// CONSTRUCTEUR
	//
	// =======================================================

	var SELF = this;
	var Gtop = 30;
	var Gbot = 5;
	var Gleft = 70;
	var Gright = 5;
	this.Ymoins = YMIN - GAP;
	this.Yplus = YMAX + GAP;
	this._CANVAS = document.getElementById(CANVAS);
	this.Gwidth = SELF._CANVAS.width - Gleft - Gright;
	this.Gheight = SELF._CANVAS.height - Gtop - Gbot;
	if (!this._CANVAS) {
		alert("Impossible de récupérer le canvas");
		return;
	}
	this._CTX = this._CANVAS.getContext('2d');
	if (!this._CTX) {
		alert("Impossible de récupérer le contexte du canvas");
		return;
	}
	this._CTX.fillStyle = "#ffffff";
	this._CTX.fillRect(Gleft, Gtop, this.Gwidth, this.Gheight);

	this._CTX.strokeStyle = '#880000';
	var s = 2;
	this._CTX.beginPath(); // On démarre un nouveau tracé.
	this._CTX.moveTo(Gleft - s, Gtop - s);// On se déplace au coin inférieur
	// gauche
	this._CTX.lineTo(Gleft - s, Gtop + this.Gheight + s);
	this._CTX.lineTo(Gleft + this.Gwidth + s, Gtop + this.Gheight + s);
	this._CTX.lineTo(Gleft + this.Gwidth + s, Gtop - s);
	this._CTX.lineTo(Gleft - s, Gtop - s);
	this._CTX.stroke();// On trace seulement les lignes.
	this._CTX.font = "15px Arial";
	this._CTX.fillText(TITLE, 10, 20);
	for (var i = 0; i < LEGENDS_COLORS.length; i++) {
		this._CTX.fillStyle = LEGENDS_COLORS[i].COLOR;
		this._CTX.fillText(LEGENDS_COLORS[i].LEGEND, 100 + 50 * i, 20);
	};
	/*
	 * this._CTX.fillStyle=COLOR_A; this._CTX.fillText(LEGEND_A,100,20);
	 * this._CTX.fillStyle=COLOR_B; this._CTX.fillText(LEGEND_B,200,20);
	 */
	this._CTX.fillStyle = "#ffffff";
	this._CTX.font = "10px Arial";
	this._CTX.fillText(SF_float(YMIN, 7, 1), 10 + s, Abscisse(YMIN) - s);
	this._CTX.fillText('+' + SF_float(YMAX, 7, 1), 10 + s, Abscisse(YMAX) + s);

	// this._CTX.fill(); //On utilise la méthode fill(); si l'on veut une forme
	// pleine
	this._CTX.closePath();
	this._CTX.arc(10, 100, 50, 0, Math.PI); // On trace la courbe délimitant
	// notre forme
	this._CTX.stroke();// On trace seulement les lignes.
	// -------------------------------------------------------
	// CERCLE
	//
	this.Circle = function() {
		// this._CTX.beginPath(); //On démarre un nouveau tracé.
		this._CTX.fillStyle = "#0000ff";
		this._CTX.arc(10, 100, 20, 0, 2 * Math.PI); // On trace la courbe
		// délimitant notre forme
		this._CTX.stroke();// On trace seulement les lignes.
		// this._CTX.fill(); //On utilise la méthode fill(); si l'on veut une
		// forme pleine
		this._CTX.closePath();
	}
	// -------------------------------------------------------
	// TEXTE
	//
	this.Text = function(TXT, X, Y) {
		this._CTX.font = "30px Arial";
		this._CTX.fillText(TXT, X, Y);
	}
	// -------------------------------------------------------
	// POINT
	//
	this.Point = function(X, Y) {
		this._CTX.beginPath(); // On démarre un nouveau tracé.
		this._CTX.moveTo(X, Y);// On se déplace au coin inférieur gauche
		this._CTX.lineTo(X - 1, Y - 1);
		this._CTX.stroke();// On trace seulement les lignes.
		this._CTX.closePath();
	}
	// -------------------------------------------------------
	// Abscisse
	//
	function Abscisse(Y) {
		var abs = SELF._CANVAS.height - SELF.Gheight * (Y - SELF.Ymoins)
			/ (SELF.Yplus - SELF.Ymoins);
		return abs;
	}
	this.addValue = function(Y, COLOR) {
		this._CTX.strokeStyle = COLOR;
		this.Point(Gleft + SELF.Gwidth, Abscisse(Y));
	}
	// -------------------------------------------------------
	// SHIFT LEFT DU GRAPHIQUE
	//
	this.animate = function() {
		// SELF.Point(SELF._CANVAS.width,20);
		var myImageData = SELF._CTX.getImageData(Gleft + 1, Gtop, SELF.Gwidth,
			SELF.Gheight);
		SELF._CTX.putImageData(myImageData, Gleft, Gtop);
		this._CTX.strokeStyle = '#ffffff';
		this._CTX.beginPath(); // On démarre un nouveau tracé.
		this._CTX.moveTo(Gleft + this.Gwidth, Gtop);
		this._CTX.lineTo(Gleft + this.Gwidth, Gtop + this.Gheight);
		this._CTX.stroke();// On trace seulement les lignes.
		this._CTX.closePath();
	}
	this.update = function(VALARRAY) {
		if (CFG.client.CANVAS.ANIMATE) {
			this.addValue(YMAX, "#888888");
			this.addValue(0, "#88ff88");
			this.addValue(YMIN, "#888888");
			for (var i = 0; i < VALARRAY.length; i++) {
				this.addValue(VALARRAY[i], LEGENDS_COLORS[i].COLOR);
			};
			/*
						this.addValue(XA, COLOR_A);
						this.addValue(XB, COLOR_B);
						*/
			this.animate();
		}
	}
}
