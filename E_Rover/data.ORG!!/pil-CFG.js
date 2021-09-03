/*
  QUE DES OBJETS SIMPLES ET DE MOINS DE 128 CARACTERES
  Pas d'espace dans ":{" 
*/
//var x=_TMS_();
const _RED='#ff0000';
const _GREY='#00ff00';
var CFG = {};

CFG.client={
	  	'PILOT':{
	  		'DIR':{'MIN':-255,'CLR':0,'MAX':+255},
	  		'VIT':{'MIN':-255,'CLR':0,'MAX':+255},
	  	},

  	'TPIDDIR':{'MIN':-255,'MAX':+255,'KP':1,'KI':0,'KD':100,'PROF':10},
  	'TPIDVIT':{'MIN':-255,'MAX':+255,'KP':1,'KI':0,'KD': 100,'PROF':10},
  	'CANVAS':{'GAP':50,'ANIMATE':true,
	  	'MOTORS':{'MIN':-1023,'CLR':0,'MAX':+1023},
	  	'PILOT':{'MIN':-255,'CLR':0,'MAX':+255},
  		}, // ± sur MIN MAX
  	'IOSERIAL':{'HOST':'http://localhost:8888','SPEED':115200,'TIMEOUT':100},
  	'LOG':{'SCROLL':50},
    'JOBGOAL':{'NAME':'TOTO.kmd'},
    'SOKTIMOUT':5000,//Délai de connection
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
    '__field_dlvl':{DBG_0:  {opt:{sel:0,show:1,entry:"0",background:'#aaaaaa'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_1:  {opt:{sel:0,show:1,entry:"1",background:'#aaaaff'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_2:  {opt:{sel:0,show:1,entry:"2",background:'#8888ff'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_3:  {opt:{sel:0,show:1,entry:"3",background:'#4444ff'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_4:  {opt:{sel:0,show:1,entry:"4",background:'#00aa00'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_5:  {opt:{sel:0,show:1,entry:"5",background:'#00ff00'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_6:  {opt:{sel:0,show:1,entry:"6",background:'#aaaa00'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_7:  {opt:{sel:0,show:1,entry:"7",background:'#ffff00'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_8:  {opt:{sel:0,show:1,entry:"8",background:'#ff8888'}, kmd:{OP:DEF_OP_SETDLVL}},
                    DBG_9:  {opt:{sel:0,show:1,entry:"9",background:_RED},  	kmd:{OP:DEF_OP_SETDLVL}}
                   },
   '__field_timer':{LISTEN:		{opt:{sel:1,show:1,entry:"   Never",background:'#aaaaaa'},	kmd:{OP:DEF_OP_GETTOUT, codeTMS:-1}},
	   				SHOOT: 		{opt:{sel:0,show:1,entry:" 1 Shoot",background:'#888888'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:0}},
	   				L_hour:		{opt:{sel:0,show:1,entry:"1/(hour)",background:'#ffffff'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:3600000}},
	   				L_5mn:		{opt:{sel:0,show:1,entry:"1/(5 mn)",background:'#88ffff'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:300000}},
	   				L_1mn:		{opt:{sel:0,show:1,entry:"1/(1 mn)",background:'#00ffff'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:60000}},
	   				L_10s:		{opt:{sel:0,show:1,entry:"  0.1 Hz",background:'#00ff88'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:10000}},
	   				L_1s: 		{opt:{sel:0,show:1,entry:"    1 Hz",background:'#00ff00'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:1000}},
	   				L_500ms:	{opt:{sel:0,show:1,entry:"    2 Hz",background:'#88ff00'},	kmd:{OP:DEF_OP_SETTOUT, codeTMS:500}},
	   				L_200ms:	{opt:{sel:0,show:1,entry:"    5 Hz",background:'#ffff00'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:200}},
	   				L_100ms:	{opt:{sel:0,show:1,entry:"   10 Hz",background:'#ff8800'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:100}},
	   				L_50ms: 	{opt:{sel:0,show:1,entry:"   20 Hz",background:'#ff0000'}, 	kmd:{OP:DEF_OP_SETTOUT, codeTMS:50}}
					},
	'__field_txtarea':{  init:	{value:/*x*/+86400,  background_color:'#ff0000',	color:"#ffffff",maxlength:360,cols:100,rows:15},
						max:  	{value:"Ø",      background_color:_GREY,		color:"#000000",maxlength:60,size:62},
						error: 	{value:"???",    background_color:'#000000',	color:"#ffffff",maxlength:60,size:62},
						min:   	{value:_TMS_(),  background_color:'#0000ff',	color:"#ffffff",maxlength:60,size:62}
						},
	'__field_txt':{  max:	{value:/*x*/+86400,  background_color:'#ff0000',	color:"#ffffff",maxlength:60,size:62},
							init:  	{value:"Ø",      background_color:_GREY,		color:"#000000",maxlength:60,size:62},
							error: 	{value:"???",    background_color:'#000000',	color:"#ffffff",maxlength:60,size:62},
							min:   	{value:_TMS_(),  background_color:'#0000ff',	color:"#ffffff",maxlength:60,size:62}
							},
	'__Driver_exec':{	Stop:	 	{opt:{sel:1,show:1,entry:"STOP",  	background:'#ff0000'},	timer:0, kmd:{"OP":DEF_OP_CLOSE,	"soks":["wsR"]}}
					},
	'__Motors_exec':{	Stop:	 	{opt:{sel:1,show:1,entry:"STOP",  	background:'#ff0000'},	timer:0, kmd:{"OP":DEF_OP_CLOSE,	"soks":["wsR"]}},
						PILOT:   	{opt:{sel:0,show:1,entry:"Pilotage", background:'#aaaaaa'}, timer:6, kmd:{"OP":DEF_OP_PILOT}},
						TESTsquare:	{opt:{sel:0,show:1,entry:"TSTsquare",background:'#aaaaaa'}, timer:0, kmd:{"OP":DEF_OP_TSTSQ}},
						LEARN:   	{opt:{sel:0,show:1,entry:"Circle", background:'#ffff00'}, timer:7, kmd:{"OP":DEF_OP_OPEN,"soks":["wsR"]}}
					},
	'__Datim_exec':	{	SynRover: 	{opt:{sel:1,show:1,entry:"TMS Rover",  	background:'#00ff00'}, timer:0, kmd:{"OP":DEF_OP_SYNC,"soks":["wsR"]}},
						SynBasis: 	{opt:{sel:0,show:1,entry:"TMS Basis",	background:'#0000ff'}, timer:0, kmd:{"OP":DEF_OP_SYNC,"soks":["wsB"]}},
						SynAll:		{opt:{sel:0,show:1,entry:"TMS All",  	background:'#ff0000'}, timer:0, kmd:{"OP":DEF_OP_SYNC,"soks":["wsR","wsB"]}}
					},
	'__Jobgoal_exec':{	Stop: 	{opt:{sel:1,show:1,entry:"STOP",  	background:'#ff0000'},	timer:0, kmd:{"OP":DEF_OP_CLOSE,	"soks":["wsR"]}},
						Mis_1: 	{opt:{sel:0,show:1,entry:"Autotest",background:'#00ff00'}, 	timer:6, kmd:{"OP":DEF_OP_OPEN,"soks":["wsR"]}},
						Mis_2: 	{opt:{sel:0,show:1,entry:"Circle",	background:'#0088ff'}, 	timer:6, kmd:{"OP":DEF_OP_OPEN,"soks":["wsR"]}},
						Mis_3: 	{opt:{sel:0,show:1,entry:"Autotest",background:'#ff8800'}, 	timer:6, kmd:{"OP":DEF_OP_OPEN,"soks":["wsR"]}}
					},
    '__Log_pilot':{	LOG_0:  {opt:{sel:1,show:1,entry:"-",background:'#aaaaaa'},  kmd:{OP:"set_logLVL"}},
                    LOG_S: 	{opt:{sel:0,show:1,entry:">",background:'#0000aa'},  kmd:{OP:"set_logLVL"}},
                    LOG_R: 	{opt:{sel:0,show:1,entry:"<",background:'#0000aa'},  kmd:{OP:"set_logLVL"}},
                    LOG_2:  {opt:{sel:0,show:1,entry:"X",background:'#aaaa00'},  kmd:{OP:"set_logLVL"}}
                    },
    '__Log_rover':{ LOG_0:  {opt:{sel:1,show:1,entry:"-",background:'#aaaaaa'},  kmd:{OP:"set_logLVL"}},
                    LOG_S: 	{opt:{sel:0,show:1,entry:">",background:'#0000aa'},  kmd:{OP:"set_logLVL"}},
                    LOG_R: 	{opt:{sel:0,show:1,entry:"<",background:'#0000aa'},  kmd:{OP:"set_logLVL"}},
                    LOG_2:  {opt:{sel:0,show:1,entry:"X",background:'#aaaa00'},  kmd:{OP:"set_logLVL"}}
                    },
    '__Leds_exec':  {BLACK: {opt:{sel:1,show:1,entry:"- - -",  background:'#000000'},  kmd:{"codeRYG":0}},
                     G:     {opt:{sel:0,show:1,entry:"- - G",  background:'#00ff00'},  kmd:{"codeRYG":1}},
                     Y:     {opt:{sel:0,show:1,entry:"- Y -",  background:'#ffff00'},  kmd:{"codeRYG":2}},
                     YG:    {opt:{sel:0,show:1,entry:"- Y G",  background:'#ffff88'},  kmd:{"codeRYG":3}},
                     R:     {opt:{sel:0,show:1,entry:"R - -",  background:'#ff0000'},  kmd:{"codeRYG":4}},
                     RG:    {opt:{sel:0,show:1,entry:"R - G",  background:'#ffff00'},  kmd:{"codeRYG":5}},
                     RY:    {opt:{sel:0,show:1,entry:"R Y -",  background:'#aaaa00'},  kmd:{"codeRYG":6}},
                     RYG:   {opt:{sel:0,show:1,entry:"R Y G",  background:'#ffffff'},  kmd:{"codeRYG":7}}
                    },
    '__Socket_exec':{ CONNECT:    {opt:{sel:0,show:1,	entry:"Connecter",       background:'#00ff00'}},
                      CLOSE: 	  {opt:{sel:1,show:1,	entry:"Fermer",          background:'#ff0000'}},
                      //TOUJOURS LES HELPERS après les options (pour onchange!!)
                      OPEN:       {help:{sta:0,  entry:"Connecté",      background:'#ff0000'}},
                      OPENING:    {help:{sta:1,  entry:"Connecte...",   background:'#ffff00'}},
                      CLOSING:    {help:{sta:2,  entry:"Deconnecte...", background:'#ffff00'}},
                      CLOSED:     {help:{sta:3,  entry:"Fermé ou muet", background:'#888800'}},
                      ONMESSAGE:  {help:{sta:4,  entry:"on Message",    background:'#0000ff'}},// pas de jctStatus(), on laisse passer STA
                      NEW:        {help:{sta:5,  entry:"New",           background:'#ffff00'}},
                      ER_TIMEOUT: {help:{sta:14, entry:"Time OUT",      background:'#ffff00'}},
                      WHAT:       {help:{sta:17, entry:"état inconnu",  background:'#ff0000'}},
                      ERRPARSE:   {help:{sta:128,entry:"err Parser",    background:'#ff0000'}},
                      NOSERVER:   {help:{sta:267,entry:"Pas de serveur",background:'#ff0000'}}
                    }
  };
  /*
{"text":"Pas connecté","background":'#ffff00'}
,{"text":"Connecté","background":'#00ff00'});
*/