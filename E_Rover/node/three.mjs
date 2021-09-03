///////////////////////////////////////////////////////////////////////
//  https://medium.com/codex/global-variables-and-javascript-modules-ce674a869164
window.ONE = 1;
self.TWO = 2;
globalThis.TROIS = 3;


globalThis.tjsTSK = { error: 0 };


globalThis.tjsATT = {
	scalePos: 1, Min: 0, Def: 1, Max: 10, Step: 0.01
}




globalThis.liveKAM = 'K_1';
globalThis.moveKAM = 'K_1';
globalThis.moveKAM_flag = false;
globalThis.switchKAM = '';
globalThis.Kam_reset = false;
globalThis.tjsCam = new Array()
/*
*/
tjsCam['K_1'] = {
	attach: 'GND',
	target: 'ROV',
	xMin: -200, x: 150, xMax: 200, xStep: 1,
	yMin: -200, y: 150, yMax: 200, yStep: 1,
	zMin: -200, z: 150, zMax: 200, zStep: 1,
	rollMin: -180, roll: 0, rollMax: 180, rollStep: 1,
	pitchMin: -90, pitch: 0, pitchMax: 90, pitchStep: 1,
	yawMin: -180, yaw: 0, yawMax: 180, yawStep: 1,
	nearMin: 0.1, near: 1, nearMax: 10, nearStep: 0.1,
	farMin: 10, far: 1000, farMax: 2000, farStep: 0.1,
	fovMin: 5, fov: 75, fovMax: 200, fovStep: 0.1,
	color: 0xff0000
}
tjsCam['K_2'] = {
	attach: 'ROV',
	target: 'GND',
	xMin: -200, x: 150, xMax: 200, xStep: 1,
	yMin: -200, y: 150, yMax: 200, yStep: 1,
	zMin: -200, z: 150, zMax: 200, zStep: 1,
	rollMin: -180, roll: 0, rollMax: 180, rollStep: 1,
	pitchMin: -90, pitch: 0, pitchMax: 90, pitchStep: 1,
	yawMin: -180, yaw: 0, yawMax: 180, yawStep: 1,
	nearMin: 0.1, near: 1, nearMax: 10, nearStep: 0.1,
	farMin: 10, far: 1000, farMax: 2000, farStep: 0.1,
	fovMin: 5, fov: 75, fovMax: 200, fovStep: 0.1,
	color: 0x00ff00
}
tjsCam['K_3'] = {
	attach: 'ROV',
	target: '',
	xMin: -200, x: 100, xMax: 200, xStep: 1,
	yMin: -200, y: 0, yMax: 200, yStep: 1,
	zMin: -200, z: 0, zMax: 200, zStep: 1,
	rollMin: -180, roll: 90, rollMax: 180, rollStep: 1,
	pitchMin: -90, pitch: 0, pitchMax: 90, pitchStep: 1,
	yawMin: -180, yaw: -90, yawMax: 180, yawStep: 1,
	nearMin: 0.1, near: 1, nearMax: 10, nearStep: 0.1,
	farMin: 10, far: 1000, farMax: 2000, farStep: 0.1,
	fovMin: 5, fov: 50, fovMax: 200, fovStep: 0.1,
	color: 0x0000ff
}
tjsCam['K_4'] = {
	attach: 'GND',
	target: '',
	xMin: -200, x: 150, xMax: 200, xStep: 1,
	yMin: -200, y: 150, yMax: 200, yStep: 1,
	zMin: -200, z: 150, zMax: 200, zStep: 1,
	rollMin: -180, roll: 0, rollMax: 180, rollStep: 1,
	pitchMin: -90, pitch: 0, pitchMax: 90, pitchStep: 1,
	yawMin: -180, yaw: 0, yawMax: 180, yawStep: 1,
	nearMin: 0.1, near: 1, nearMax: 10, nearStep: 0.1,
	farMin: 10, far: 1000, farMax: 2000, farStep: 0.1,
	fovMin: 5, fov: 75, fovMax: 200, fovStep: 0.1,
	color: 0x000000
}
tjsCam['IMU'] = {
	attach: 'GND',
	target: '',
	xMin: -200, x: 1000, xMax: 200, xStep: 1,
	yMin: -200, y: 1000, yMax: 200, yStep: 1,
	zMin: -200, z: 1000, zMax: 200, zStep: 1,
	rollMin: -180, roll: 0, rollMax: 180, rollStep: 1,
	pitchMin: -90, pitch: 0, pitchMax: 90, pitchStep: 1,
	yawMin: -180, yaw: 0, yawMax: 180, yawStep: 1,
	nearMin: 0.1, near: 1, nearMax: 10, nearStep: 0.1,
	farMin: 10, far: 10000, farMax: 2000, farStep: 0.1,
	fovMin: 5, fov: 75, fovMax: 200, fovStep: 0.1,
	color: 0xffffff
}
/*
		this._camera = new THREE.PerspectiveCamera(50, this.aspectRatio, .1, 10000)
		this._camera.position.set(1000, 1000, 1000)
		this._camera.name = 'IMU'
*/
//
// IMU VIEW
//
globalThis.imu_Flag = false;
//
// MAGNETO RAW DATA
//
globalThis.calMag_START=false
globalThis.rawMag_FlagShift = false;
globalThis.rawMag_ptr = 0;
globalThis.rawMag_Flag = false;
globalThis.rawMag_MaxPts = 3600; //  !!!
globalThis.rawMag_Count = 0;
globalThis.rawMag_MaxLen = 3 * rawMag_MaxPts;
globalThis.rawMag_Data = new Float32Array(rawMag_MaxLen);
for (var i = 0; i < rawMag_MaxLen; i++)
	rawMag_Data[i] = 0;
//
// MAGNETO CAL DATA
//
let calMagLines=new Array();
globalThis.calMag_ptr = 0;
globalThis.calMag_Flag = false;
globalThis.calMag_MaxPts = 3600; //  !!!
globalThis.calMag_Count = 0;
globalThis.calMag_MaxLen = 3 * calMag_MaxPts;
globalThis.calMag_Data = new Float32Array(calMag_MaxLen);
for (var i = 0; i < calMag_MaxLen; i++)
	calMag_Data[i] = 0;
//
// TRAJECTORY POSITIONS
//
let roverTrajectory = {};
let roverRaycasting = {};
globalThis.traject_Flag = false;
globalThis.traject_MaxPts = 17 //36000; // one hour !!!
/**
 * 
 * 12 octets Float32 1 point 3D xyz * 36000 = 432000 vs 0,412 Mo
 * 
 */
globalThis.traject_Queue = new Array();
globalThis.traject_Count = 0;
globalThis.traject_Filled = false;
globalThis.traject_MaxLen = 3 * traject_MaxPts;
let Rover_positions = new Float32Array(traject_MaxLen);
for (var i = 0; i < traject_MaxLen; i += 3) {
	Rover_positions[i + 0] = 50 * Math.cos(3.14 * i / 180);
	Rover_positions[i + 1] = 50 * Math.sin(3.14 * i / 180);
	Rover_positions[i + 2] = i / 1000;
}

let Rover_groundpts = new Float32Array(traject_MaxLen);
for (var i = 0; i < traject_MaxLen; i += 3) {
	Rover_groundpts[i + 0] = 50 * Math.cos(3.14 * i / 180);
	Rover_groundpts[i + 1] = 50 * Math.sin(3.14 * i / 180);
	Rover_groundpts[i + 2] = i / 1000;
}
/**
 * Rover_index is twice size of trajectory points count to obtain a ring queue effet on long time motions
 */
let Rover_index = []
Rover_index[0]=0;
Rover_index[traject_MaxLen-1]=0;
for (var i = 0; i < traject_MaxLen; i ++) {
	Rover_index[i] = i;
}


let Rover_colors= []
Rover_colors[0]=0;
Rover_colors[traject_MaxLen-1]=0;
let j=0
for (var i = 0; i < traject_MaxPts; i ++) {
	//Rover_colors[i] = Math.random() * 0.5 + 0.5;
	switch((Math.trunc(i/1) % 3)){
		case 0:
			Rover_colors[j++] = 1;
			Rover_colors[j++] = 0;
			Rover_colors[j++] = 0;
			break;
		case 1:
			Rover_colors[j++] = 0;
			Rover_colors[j++] = 1;
			Rover_colors[j++] = 0;
			break;
		case 2:
			Rover_colors[j++] = 0;
			Rover_colors[j++] = 0;
			Rover_colors[j++] = 1;
			break;
	}
}
/*
Rover_colors[0] = 1;
Rover_colors[traject_MaxLen-2] = 1;
*/
let Rover_RPY = new Float32Array(traject_MaxLen);
let Rover_gravity = new Float32Array(traject_MaxLen);
let Rover_north = new Float32Array(traject_MaxLen);
let Rover_quat = new Array();
let RADAR = new Array();
/*
//  OK  ===============================================
import * as THREE from 'https://cdn.skypack.dev/three'
import threejsOrbitControls from 'https://cdn.skypack.dev/threejs-orbit-controls';
*/
//  OK  ===============================================
import * as THREE from './THREE/three.js/build/three.module.js'
import * as CameraUtils from './THREE/three.js/examples/jsm/utils/CameraUtils.js';
import { OrbitControls } from './THREE/three.js/examples/jsm/controls/OrbitControls.js';
import { ArcballControls } from './THREE/three.js/examples/jsm/controls/ArcballControls.js';
import { STLLoader } from './THREE/three.js/examples/jsm/loaders/STLLoader.js'
//import Stats from './THREE/three.js/examples/jsm/libs/stats.module'
//import { MathUtils } from     './THREE/three.js/src/math/MathUtils.js';
//import { myCameraHelper } from     './THREE/three.js/src/helpers/myCameraHelper.js';
import { myCameraHelper } from './myCameraHelper.js';
import { CSS2DRenderer, CSS2DObject } from './THREE/three.js/examples/jsm/renderers/CSS2DRenderer.js';
import { ParametricGeometry } from       './THREE/three.js/examples/jsm/geometries/ParametricGeometry.js'
let DEBUG = false;
let sph = {}
const _Xmin_Color = 0xff7777;
const _Xmax_Color = 0xff0000;
const _Ymin_Color = 0x77ff77;
const _Ymax_Color = 0x00ff00;
const _Zmin_Color = 0x7777ff;
const _Zmax_Color = 0x0000ff;
let cameraControls;
//#####################################################################
let erase=(function (OBJ){
	var lvl=-1;
	return function(OBJ){
/*
		if(typeof LVL === 'undefined')
			lvl=0
		else 
			lvl=LVL
*/
		lvl+=1
		let tab=lvl
		for (var i=0;i<=lvl;i++)
			tab+="    "
		//console.info(tab,'--------------------------------')
		//console.info(tab,OBJ)
		if(OBJ === null){
			console.info(tab,'??? OBJ NULL !!!');
		}
		else if(Array.isArray(OBJ)){
			for (var key in OBJ) {
	  			//console.info(tab,key,':',OBJ[key]);
	  			if(typeof OBJ[key] === 'object' && OBJ[key] !== null)
	  				erase(OBJ[key])
				else if (OBJ.hasOwnProperty(key)){
					let descriptor = Object.getOwnPropertyDescriptor(OBJ, key);
					if(descriptor.configurable && descriptor.writable)
						delete OBJ[key]
					else
						console.info(tab,' NOT configurable');
				}
				else 
					console.info(tab,' has Own Property FALSE');
			}
		}
		else if(typeof OBJ === 'object'){
			for (const [key, value] of Object.entries(OBJ)) {
	  			//console.info(tab,key,':',value);
	  			let desc = Object.getOwnPropertyDescriptor(OBJ, key);
				if(typeof value === 'object' && OBJ.hasOwnProperty(key) && desc.configurable && value !== null)
					erase(value)
				else if (OBJ.hasOwnProperty(key)){
					let descriptor = Object.getOwnPropertyDescriptor(OBJ, key);
					if(descriptor.configurable && descriptor.writable)
						delete OBJ[key]
					else
						console.info(tab,' NOT configurable');
				}
				else 
					console.info(tab,' has Own Property FALSE');
			}
		}
		lvl-=1;
		if(lvl == 0)
			console.info('TERMINATE')
	}
})();
/** ===================================================================
 * @fn function label(TEXT,OBJ,X=0,Y=0,Z=0)
 * @brief Adds a label to OBJ
 * 
 * https://sbcode.net/threejs/annotations/
 * 
 * @param TEXt of label
 * @param OBJ object3D where adding LABEL
 * @param X to place text (default 0)
 * @param Y to place text (default 0)
 * @param Z to place text (default 0)
 */
function label(TEXT,OBJ,X=0,Y=0,Z=0){
	const Div = document.createElement( 'div' );
	Div.className = 'label';
	Div.textContent = TEXT;
	Div.style.color = OBJ.material.color.getStyle ()
	Div.style.marginTop = '-1em';
	const Label = new CSS2DObject( Div );
	Label.position.set( X, Y, Z );
	OBJ.userData.label=Label //OBJ.children.length
	OBJ.add( Label );
	//earthLabel.layers.set( 0 );
}
/** ===================================================================
 * @fn function _segment(NAME,XA,YA,ZA,XB,YB,ZB,COLOR)
 */
function _segment(NAME,XA, YA, ZA, XB, YB, ZB, COLOR) {
	//create a blue LineBasicMaterial
	let material = new THREE.LineBasicMaterial({ color: COLOR}); //, linewidth :5.0 doesn't run

	//After material we will need a geometry with some vertices:
	let points = [];
	points.push(new THREE.Vector3(XA, YA, ZA));
	points.push(new THREE.Vector3(XB, YB, ZB));
	
	let geometry = new THREE.BufferGeometry().setFromPoints( points ) //dispose problem ????

	//Note that lines are drawn between each consecutive pair of vertices, but not between the first and last (the line is not closed.)
	let obj=new THREE.Line(geometry, material);
	obj.name=NAME
	return obj 
}
/** ===================================================================
 * @fn function _sphera(RADIUS,COLOR)
 */
function _sphera(NAME, RADIUS, COLOR) {
	const geometry = new THREE.SphereGeometry(RADIUS, 32, 16);
	const material = new THREE.MeshBasicMaterial({ color: COLOR });
	const sph = new THREE.Mesh(geometry, material);
	sph.name = NAME
	return sph
}
/** ===================================================================
 * @fn function _cloud(NAME,DATA,COLOR)
 * @param NAME of object3D
 * @param DATA raw data array
 * @param COLOR of object3D
 * @return object3D
 */
function _cloud(NAME, DATA, COLOR) {
	const material = new THREE.PointsMaterial({ color: COLOR, size: 5 });
	var geometry = new THREE.BufferGeometry();
	geometry.setAttribute('position', new THREE.BufferAttribute(DATA, 3));
	const obj = new THREE.Points(geometry, material);
	obj.name = NAME
	return obj
}
/** ===================================================================
 * @fn function _trajectory(NAME,DATA,COLOR)
 * @param NAME of object3D
 * @param DATA raw data array
 * @param COLOR of object3D
 * @return object3D
 */
 class _trajectory {
	constructor(NAME, DATA, COLOR) {
		this.start=0
		//create a blue LineBasicMaterial
		const material = new THREE.LineBasicMaterial( { vertexColors: true } );//{ color: COLOR });

		//After material we will need a geometry with some vertices:
		//    const geometry = new THREE.BufferGeometry().setFromPoints( DATA );

		var geometry = new THREE.BufferGeometry();
		//var positions = new Float32Array( MAX_POINTS * 3 ); 
		geometry.setIndex(Rover_index );
		geometry.setAttribute('position', new THREE.BufferAttribute(DATA, 3));
		geometry.setAttribute( 'color', new THREE.Float32BufferAttribute( Rover_colors, 3 ) );
		
		this.p3D = new THREE.Line(geometry, material);
		this.p3D.name = NAME
	}
	/** ---------------------------------------------------------------
	 * @fn countPoints()
	 * @brief 
	 * @return the count of point to draw 
	 */
	countPoints() {
		if(traject_Filled){
			var tmp=Rover_index.splice(0, 3);
			Rover_index.push(tmp[0], tmp[1],tmp[2]);
			 tmp=Rover_colors.splice(0, 3);
			Rover_colors.push(tmp[0], tmp[1],tmp[2]);
			return traject_MaxPts
		}else{
			return traject_Count;
		}
	}
}
/** ===================================================================
 * @fn function _tickLine(NAME,XA,YA,ZA,XB,YB,ZB,COLOR,RADIUS)
 */
function _tickLine(NAME, XA, YA, ZA, XB, YB, ZB, COLOR, RADIUS) {
	var Elts = new Array();
	// Material
	const material = new THREE.LineBasicMaterial({ color: COLOR });

	let startVector = new THREE.Vector3(XA, YA, ZA);
	let endVector = new THREE.Vector3(XB, YB, ZB);
	//After material we will need a geometry with some vertices:
	const points = [];
	points.push(startVector, endVector);
	// Geometry
	const tubeGeometry = new THREE.TubeGeometry(
		new THREE.CatmullRomCurve3(points),
		16,// path segments
		RADIUS,// THICKNESS
		8, //Roundness of Tube (number of points for circle)
		false //closed
	);
	const tube = new THREE.Line(tubeGeometry, material);
	tube.name = NAME
	return tube
}
/*

Parametric ruled surfaces box

                      3────────────2
                      │    ╭<─╮    │
                      │    │y │    │
                      │    ╰──╯    │
2────────────3────────7────────────6────────2
│    ╭<─╮    │  ╭<─╮  │    ╭<─╮    │  ╭<─╮  │
│    │z+│    │  │x+│  │    │z-│    │  │x-│  │
│    ╰──╯    │  ╰──╯  │    ╰──╯    │  ╰──╯  │
0────────────1────────5────────────4────────0 
                      │    ╭<─╮    │
                      │    │y │    │
                      │    ╰──╯    │
                      1────────────0




0,1,2,3 z+
4,5,6,7 z-
1,5,3,7 x+
4,0,6,2 x-
1,0,5,4 y
6,7,2,3 y     



*/
const prsb = new Array()
prsb.push({x:-120,y:-50,z:+50})
prsb.push({x:+80, y:-50,z:+20})
prsb.push({x:-120,y:+50,z:+50})
prsb.push({x:+80, y:+50,z:+20})
prsb.push({x:-120,y:-80,z:-50})
prsb.push({x:+80, y:-30,z:-50})
prsb.push({x:-120,y:+80,z:-50})
prsb.push({x:+80, y:+30,z:-50})

/**
 * 
 */
function _face(PA,PB,QA,QB,COLOR){
	/**
	 * @param s curvilign abcissa € [0,1]
	 * @param p0 start point (s=0)
	 * @param p1 end point (s=1)
	 * @return pt
	 */
	function pt(s,p0,p1){
		let x=p0.x+s*(p1.x-p0.x)
		let y=p0.y+s*(p1.y-p0.y)
		let z=p0.z+s*(p1.z-p0.z)
		return {x:x,y:y,z:z}
	}
	/**
	 * @param u € [0,1]
	 * @param v € [0,1]
	 */
	function param(u,v,target){
		let Pu=pt(u,PA,PB)
		let Qu=pt(u,QA,QB)
		let Rv=pt(v,Pu,Qu)
		target.set(Rv.x,Rv.y,Rv.z)
	}
	const faceMesh = new THREE.Mesh(
	    new ParametricGeometry( param, 5, 10),
	    new THREE.MeshLambertMaterial({color:COLOR,wireframe:false,side:THREE.DoubleSide,transparent:true,opacity:0.5})// ,emissive:0xff0000
	)
	return faceMesh
}
function _quadBox(){
	const cubeMesh = new THREE.Mesh(
	    new THREE.BoxGeometry(50, 100, 150),
	    new THREE.MeshLambertMaterial({color:0x123456,emissive:0xff0000,wireframe:true})
	)
	return cubeMesh
}

/** ===================================================================
 * @fn function Frame2(XNAME,YNAME,ZNAME,X,Y,Z,LEN)
 */
function Frame2(XNAME,YNAME,ZNAME,X, Y, Z, LEN) {
	var Elts = new Array();
	Elts.push(new _segment(XNAME,X, Y, Z, X + LEN, Y, Z, 0xff0000));
	Elts.push(new _segment(YNAME,X, Y, Z, X, Y + LEN, Z, 0x00ff00));
	Elts.push(new _segment(ZNAME,X, Y, Z, X, Y, Z + LEN, 0x0000ff));
	return Elts;
}
/** ===================================================================
 * @fn function wall(NAME,WORLD,PLANEGEO,DX,DY,DZ,A,B,C,COLOR)
 */
function wall(NAME, Xlen, Ylen, DX, DY, DZ, A, B, C, COLOR, SIDE) {
	var Elts = new Array();
	const planeGeo_XYo = new THREE.PlaneGeometry(0.1 + Xlen, 0.1 + Ylen, 5, 5);// centered rectangle (X,Y)

	if (DEBUG) {
		const material = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true, transparent: true });
		Elts.push(new THREE.Mesh(planeGeo_XYo, material));
	}

	const mesh = new THREE.Mesh(
		planeGeo_XYo,
		new THREE.MeshPhongMaterial({ color: COLOR, side: SIDE }));
	mesh.name = NAME;
	mesh.position.x = DX;
	mesh.position.y = DY;
	mesh.position.z = DZ;
	mesh.rotateX(A);// % mesh center
	mesh.rotateY(B);// % mesh center
	mesh.rotateZ(C);// % mesh center
	Elts.push(mesh);
	if (DEBUG) {
		const plane = new THREE.Mesh(
			planeGeo_XYo,
			new THREE.MeshBasicMaterial({ color: COLOR, wireframe: true, transparent: true }));
		plane.name = "dbg_" + NAME;
		plane.position.x = DX;
		plane.position.y = DY;
		plane.position.z = DZ;
		Elts.push(plane);
	}
	return Elts;
}
/** ===================================================================
 * @fn function Stl(NAME,FILENAME)
	https://github.com/mrdoob/three.js/blob/master/examples/webgl_loader_stl.html
 */
function loadStl(WORLD, NAME, FILENAME) {
	const loader = new STLLoader()
	loader.load(FILENAME, function(geometry) {
		const material = new THREE.MeshPhongMaterial(
			{
				color: 0x8f5902,
				specular: 0x342d91,
				shininess: 100,
				side: THREE.DoubleSide,
				flatShading: true,
				wireframe: false,
				reflectivity: 1,
				transparent: true,
				opacity: 0.5
			});
		const mesh = new THREE.Mesh(geometry, material);
		mesh.name = NAME
		mesh.rotation.set(0, 0, 0);
		mesh.scale.set(15, 40, 10);
		mesh.position.set(-300, - 150, -500);
		mesh.castShadow = true;
		mesh.receiveShadow = true;
		//			scene.add( mesh );
		WORLD.insert(mesh);
	}
	);
}
/** ===================================================================
 * @fn function Frame(NAME, LEN, Xlabel,Ylabel,Zlabel,Xcolor, Ycolor, Zcolor)
 * @param NAME of fram
 * @param LEN of axis
 * @param Xlabel label of axis (LEN,0,0)
 * @param Ylabel label of axis (0,LEN,0)
 * @param Zlabel label of axis (0,0,LEN)
 * @param Xcolor color of axis (LEN,0,0)
 * @param Ycolor color of axis (0,LEN,0)
 * @param Zcolor color of axis (0,0,LEN)
 * @return an array of 3 object3D (one for each axis)
 */
function Frame(NAME, LEN, Xlabel,Ylabel,Zlabel,Xcolor, Ycolor, Zcolor) {
	var Elts = new Array();
	/*
		_tickLine(WORLD,Xmin,0,0,Xmax,0,0,0xff0000,2);
		_tickLine(WORLD,0,Ymin,0,0,Ymax,0,0x00ff00,2);
		_tickLine(WORLD,0,0,Zmin,0,0,Zmax,0x0000ff,2);
	*/
	Elts.push(new _tickLine('iX+', 0, 0, 0, LEN, 0, 0, Xcolor, 1));
	label(Xlabel,Elts[Elts.length-1],LEN,0,0)
	Elts.push(new _tickLine('jY+', 0, 0, 0, 0, LEN, 0, Ycolor, 1));
	label(Ylabel,Elts[Elts.length-1],0,LEN,0)
	Elts.push(new _tickLine('kZ+', 0, 0, 0, 0, 0, LEN, Zcolor, 1));
	label(Zlabel,Elts[Elts.length-1],0,0,LEN)

	return Elts
}
/** ===================================================================
 * @fn function Box(NAME,Xmin,Ymin,Zmin,Xmax,Ymax,Zmax)
 */
function Box(NAME, Xmin, Ymin, Zmin, Xmax, Ymax, Zmax) {
	var Elts = new Array();
	/*
		_tickLine(WORLD,Xmin,0,0,Xmax,0,0,0xff0000,2);
		_tickLine(WORLD,0,Ymin,0,0,Ymax,0,0x00ff00,2);
		_tickLine(WORLD,0,0,Zmin,0,0,Zmax,0x0000ff,2);
	*/
	/*
	Elts.push(new _tickLine('iX+', 0, 0, 0, Xmax, 0, 0, _Xmax_Color, 2));
	Elts.push(new _tickLine('jY+', 0, 0, 0, 0, Ymax, 0, _Ymax_Color, 2));
	Elts.push(new _tickLine('kZ+', 0, 0, 0, 0, 0, Zmax, _Zmax_Color, 2));

	Elts.push(new _tickLine('iX-', Xmin, 0, 0, 0, 0, 0, _Xmin_Color, 2));
	Elts.push(new _tickLine('jY-', 0, Ymin, 0, 0, 0, 0, _Ymin_Color, 2));
	Elts.push(new _tickLine('kZ-', 0, 0, Zmin, 0, 0, 0, _Zmin_Color, 2));

*/
	// walls         

	Elts.push(new wall("wX+", 0.1 + Zmax - Zmin, 0.1 + Ymax - Ymin,
		Xmax,
		(Ymax + Ymin) / 2,
		(Zmax + Zmin) / 2,
		0,
		- Math.PI / 2,
		0,
		_Xmax_Color,
		THREE.FrontSide));
	Elts.push(new wall("wX-", 0.1 + Zmax - Zmin, 0.1 + Ymax - Ymin,
		Xmin,
		(Ymax + Ymin) / 2,
		(Zmax + Zmin) / 2,
		0,
		+ Math.PI / 2,
		0,
		_Xmin_Color,
		THREE.FrontSide));

	Elts.push(new wall("wY+", 0.1 + Xmax - Xmin, 0.1 + Zmax - Zmin,
		(Xmax + Xmin) / 2,
		Ymax,
		(Zmax + Zmin) / 2,
		- Math.PI / 2,
		0,
		0,
		_Ymax_Color,
		THREE.BackSide));
	Elts.push(new wall("wY-", 0.1 + Xmax - Xmin, 0.1 + Zmax - Zmin,
		(Xmax + Xmin) / 2,
		Ymin,
		(Zmax + Zmin) / 2,
		- Math.PI / 2,
		0,
		0,
		_Ymin_Color,
		THREE.FrontSide));

	//        DEBUG=true;
	Elts.push(new wall("wZ+", 0.1 + Xmax - Xmin, 0.1 + Ymax - Ymin,
		(Xmax + Xmin) / 2,
		(Ymax + Ymin) / 2,
		Zmax,
		0,
		Math.PI,
		0,
		_Zmax_Color,
		THREE.FrontSide));
	Elts.push(new wall("wZ-", 0.1 + Xmax - Xmin, 0.1 + Ymax - Ymin,
		(Xmax + Xmin) / 2,
		(Ymax + Ymin) / 2,
		Zmin,
		0,
		Math.PI,
		0,
		_Zmin_Color,
		THREE.BackSide));

	return Elts
}
/** ===================================================================
 * @class class Kamera
 */
class Kamera {
	constructor(VIEW, NAME, KAM, FRAME, TARGET) {
		this._KID = KAM
		this._cfg = tjsCam[KAM]
		this._cfg.attach = FRAME
		this._cfg.target = TARGET

		this._camera = new THREE.PerspectiveCamera(this._cfg.fov, VIEW.aspectRatio, this._cfg.near, this._cfg.far)
		this._camera.name = NAME
		this._helper = new myCameraHelper(this._camera, this._cfg.color)
		this._controlsStates = {
			object: this._camera,
			zoom0: 1,
			target0: new THREE.Vector3(0, 0, 50),
			position0: new THREE.Vector3(0, 0, 50)
		}

		//this._camera.removeFromParent()
		FRAME.add(this._camera)
		VIEW.scene.add(this._helper)
		this.place()
		VIEW.cameras[KAM] = this
		this._cameraControls = new OrbitControls(this._camera, VIEW.labelRenderer.domElement)
		this.saveControls()
	}
	/** ---------------------------------------------------------------
	 * @fn place()
	 * @brief Places the camera & aims the target (if any)
	 */
	place() {
		const cfg = tjsCam[this._KID]
		//
		//	Local OBJECT situation wined for the camera
		//
		const euler = new THREE.Euler(
			THREE.MathUtils.degToRad(this._cfg.roll),
			THREE.MathUtils.degToRad(this._cfg.pitch),
			THREE.MathUtils.degToRad(this._cfg.yaw),
			'ZYX'
		);
		const quaternion = new THREE.Quaternion();
		quaternion.setFromEuler(euler);

		this._camera.matrixWorldNeedsUpdate = true;
		this._camera.setRotationFromQuaternion(quaternion)
		this._camera.position.set(
			this._cfg.x,
			this._cfg.y,
			this._cfg.z
		)
		this._camera.updateMatrixWorld(true);// force children updating
		this._camera.near = this._cfg.near
		this._camera.far = this._cfg.far
		this._camera.fov = this._cfg.fov
		this.track()
	}
	/** ---------------------------------------------------------------
	 * @fn track()
	 * @brief Turns the camera to aim the target
	 * 
	 */
	track() {
		const xxx = new THREE.Vector3()
		if (typeof this._cfg.target !== 'undefined') {
			this._cfg.target.getWorldPosition(xxx)
		}
		else {
			this._cfg.attach.getWorldPosition(xxx) //camera frame origin
		}
		this._camera.lookAt(xxx)
		this._camera.updateProjectionMatrix();
		this._helper.update()
	}
	/** ---------------------------------------------------------------
	 * @fn function saveControls(KAM)
	 * @brief save the state of camera controls
	 */
	saveControls() {
		this._cameraControls.saveState()
		this._controlsStates.object = this._camera
		this._controlsStates.zoom0 = this._cameraControls.zoom0
		this._controlsStates.target0 = this._cameraControls.target0
		this._controlsStates.position0 = this._cameraControls.position0

	}
	/** ---------------------------------------------------------------
	 * @fn function restoreControls(KAM)
	 * @brief restore the state of camera controls
	 */
	restoreControls(KAM) {
		this._cameraControls.object = this._controlsStates.object
		this._cameraControls.zoom0 = this._controlsStates.zoom0
		this._cameraControls.target0 = this._controlsStates.target0
		this._cameraControls.position0 = this._controlsStates.position0
		this._cameraControls.reset() // include 	cameraControls.update()
	}
}

/**********************************************************************
 * position & angles (in degrees) IN LOCAL
 */
/*
function placeObject(OBJ,X, Y, Z, A, B, C){
	//
	//	Local OBJECT situation wined for the camera
	//
	const euler = new THREE.Euler(THREE.MathUtils.degToRad(A),THREE.MathUtils.degToRad(B),THREE.MathUtils.degToRad(C),'ZYX');
	const quaternion = new THREE.Quaternion().setFromEuler(euler);

	OBJ.matrixWorldNeedsUpdate=true;
	OBJ.setRotationFromQuaternion(quaternion)
	OBJ.position.set(X,Y,Z)
	OBJ.updateMatrixWorld( true );// force children updating
//	this.cameras[KAM][2].matrixWorldNeedsUpdate=true;

	//
	//	Local OBJECT situation wined for the camera
	//
	const world_Translation = FRAME.localToWorld(new THREE.Vector3(X,Y,Z))
	const euler = new THREE.Euler(THREE.MathUtils.degToRad(A),THREE.MathUtils.degToRad(B),THREE.MathUtils.degToRad(C),'ZYX');
	const quaternion = new THREE.Quaternion();
	quaternion.setFromEuler(euler);
	//
	//	World FRAME situation
	//
	const world_Position = new THREE.Vector3();
	const world_Quaternion = new THREE.Quaternion();
	const world_Scale = new THREE.Vector3();
	FRAME.matrixWorld.decompose( world_Position, world_Quaternion, world_Scale );
	//
	//	Compose local with world
	//
	world_Quaternion.multiply(quaternion)
	//
	// Just to verify....
	//
	{
		const world_Euler = new THREE.Euler();
		world_Euler.setFromQuaternion(world_Quaternion,'XYZ')
		const world_degrees={
			A:THREE.MathUtils.radToDeg(world_Euler.x),
			B:THREE.MathUtils.radToDeg(world_Euler.y),
			C:THREE.MathUtils.radToDeg(world_Euler.z)
		}
	}
	Camera.position.set(world_Translation.x,world_Translation.y,world_Translation.z)
	Camera.setRotationFromQuaternion(world_Quaternion)
	Camera.fov=F
	Camera.updateProjectionMatrix();
	}
*/
let dmplvl = 0
function dump(OBJ) {
	var tab = '.'
	for (var i = 0; i < dmplvl; i++)
		tab += '    '
	console.info(tab, dmplvl, OBJ.name, '	', OBJ.type)
	dmplvl++
	for (var i = 0; i < OBJ.children.length; i++)
		dump(OBJ.children[i])
	dmplvl--
}
/** ===================================================================
 * @fn function moveCamera(VIEW,KAM)
 * @brief Changes KAM camera parameters in VIEW renderer
 * @return false if KAM is in VIEW, else true (to reset the moveKAM_flag - or not)
 */
function moveCamera(VIEW,KAM){
	if (typeof VIEW.cameras[KAM]!='undefined') {
		VIEW.cameras[KAM].place()
		return false
	}
	return true
}

/** ===================================================================
 * @class World
 */
class World {
	/** ---------------------------------------------------------------
	 * @fn constructor(PARENT,NAME,X,Y,Z,A,B,C,LEN,RADIUS,COLOR)
	 */
	constructor(PARENT, NAME, X, Y, Z, A, B, C, LEN, RADIUS, COLOR) {
		this._frame = new _sphera(NAME, RADIUS, COLOR)
		this._frame.position.set(X, Y, Z);
		this._frame.rotation.set(THREE.MathUtils.degToRad(A), THREE.MathUtils.degToRad(B), THREE.MathUtils.degToRad(C));
		if (typeof PARENT._frame === 'undefined')
			PARENT.add(this._frame)
		else
			PARENT._frame.add(this._frame)
	}
	/** ---------------------------------------------------------------
	 * @fn insert(Object_s_3D)
	 * @brief While Object_s_3D is an array, calls recursivetly inserts on each array's element until Object_s_3D is an single object
	 */
	insert(mesh3D) {
		if (Array.isArray(mesh3D))
			for (var i = 0; i < mesh3D.length; i++)
				this.insert(mesh3D[i]);
		else
			this._frame.add(mesh3D)
	}
}
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
let actViews=new Array()
/** *******************************************************************
 * @class View_
 * @brief View_ defines a Three js viewer canevas with WebGL & CSS2 labels renderers
 * 
 * PROPERTIES:
 * 		cameras array to choose a particular pow (has to be populated after View_ constructor)
 * EVENTS:
 * 		Mouse double click event swaps beetween div  & full screen viewing
 */
class View_ {
	/** ===============================================================
	 * @fn constructor(DIV, COLOR, LABELS)
	 * @brief Initializes the renderer & the scene
	 * @param DIV a body absolute div element for rendering
	 * @param COLOR scene background color
	 * @param LABELS CSS2DRenderer (if defined)
	 */
	constructor(DIV, COLOR) {
		actViews[DIV]=this
		this.cameras=new Array()
		this.container = document.getElementById(DIV);
		this.saveClassName=this.container.className // save de current classes
		//this.container.innerHTML = '';
		this.renderer = new THREE.WebGLRenderer({ antialias: true });
		this.labelRenderer = new CSS2DRenderer();
		this.labelRenderer.domElement.style.position = 'absolute';
		this.labelRenderer.domElement.style.top = '0px';
		this.fct_Resize=this.full_Resize
		this.swapView()
		this.container.appendChild( this.renderer.domElement )
		this.renderer.localClippingEnabled = true;
		this.labelRenderer.domElement.addEventListener('dblclick', this.swapView.bind(this))

		this.container.appendChild( this.labelRenderer.domElement );

		/*
		 * SCENE
		 */
		this.scene = new THREE.Scene();
		this.scene.background = new THREE.Color(COLOR);
		//scene.add( new THREE.GridHelper( 400, 10 ) );
	}
	/** ===============================================================
	 * @fn swapView(e)
	 * @brief swap full screen <-> div view of scene
	 * @param e event
	 */
	swapView(e){
  		if(this.fct_Resize==this.div_Resize)
  			this.fct_Resize=this.full_Resize
  		else 
  			this.fct_Resize=this.div_Resize
  		this.fct_Resize()
		this.cow = this.container.offsetWidth;
		this.coh = this.container.offsetHeight;
		this.aspectRatio = this.cow / this.coh;
		this.update_cameras()
		this.renderer.setSize( this.cow, this.coh )
		this.labelRenderer.setSize(  this.cow, this.coh )
	}
	div_Resize(){
		this.container.className = this.saveClassName
	}
	full_Resize(){
		this.saveClassName=this.container.className // save de current classes
		this.container.className = 'fullWindow';
	}
	update_cameras(){
		for (var key in this.cameras){
 			this.cameras[key]._camera.aspect=this.aspectRatio 
 			this.cameras[key]._camera.updateProjectionMatrix();
			this.cameras[key]._helper.update()
		}
	}
}
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/** ===================================================================
 * @class View_IMU extends View_
 * @brief View_IMU defines an extended View_ class to display IMU data
 * 
 * PROPERTIES:
 * 		rawMagnetometer
 * 		calMagnetometer
 * 
 * METHODS:
 * 		IMU_init creates a parent View_ camera, Magnetometer raw & calibration cloud data
 * 		IMU_animate recieves & display magnetometer raw data then calibration data 
 * 
 * PSEUDO-EVENTS from rover interface:
 * 		rawMag_Flag 	:when recieving magnetometer raw data
 * 		calMag_START 	:when starting calibration data reception
 * 		calMag_Flag 	:when recieving magnetometer calibration data
 * 		rawMag_FlagShift:when translating raw data follwing iron correctors
 */
class View_IMU extends View_ {
	constructor(DIV, COLOR,LABELS) {
		super(DIV, COLOR,LABELS)
		this.IMU_init();
		this.IMU_animate();
	}
	/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * 
	 * @fn IMU_init()
	 * @brief Initializes the scene objects, frames & controls
	 */
	IMU_init() {
		/*
		 * GROUND FRAME -----------------------------------------------
		 */
		this._Ground = new World(this.scene, 'GROUND', 0, 0, 0, 0, 0, 0, 0, 4, 0x777777)
		label('O',this._Ground._frame)
		let vct = new THREE.Vector3(1, 1, 1);
		// ====	Isometric view
		this._Ground._frame.rotateOnAxis(vct.normalize(), THREE.MathUtils.degToRad(240))
		//
		//	objects ---------------------------------------------------
		//
		this.Connectors=new THREE.Group()
		this.Connectors.name='myLines'
		this.rawMagnetometer = new _cloud('rawMag', rawMag_Data, 0xff0000)
		this.calMagnetometer = new _cloud('calMag', calMag_Data, 0x00ff00)
		this._Ground.insert([
			new Frame('gndFrame', 100,'u','v','w' ,0xff0000, 0x00ff00, 0x0000ff),
			this.rawMagnetometer,
			this.calMagnetometer,
			this.Connectors
		]);
		new Kamera(this, 'KAL', 'IMU', this.scene);
	}
	/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * 
	 * @fn IMU_animate()
	 * @brief Recieves the rover Driver motion & animates the M3D scene
	 */
	IMU_animate() {
		const anim = this.IMU_animate;
		requestAnimationFrame(anim.bind(this));
		/*
		 * FLAG MOVE CAMERA ---------------------------------------------------
		 */
		if (moveKAM_flag)
			moveKAM_flag = moveCamera(this,moveKAM)
		/*
		 * POPULATE THE MAGNETOMETER RAW DATA ---------------------------------------------------
		 */
		if (rawMag_Flag) {
			const rawMag_simul = false;
			//
			//	magCalibration simulation ---------------------------------
			//
			if (rawMag_simul) { //random € [0,1[
				const latitude = Math.PI * (-90 + Math.random() * 180) / 180   //  -90° pole sud  0° equateur   +90° pole nord
				const longitude = Math.PI * (-180 + Math.random() * 360) / 180 // -180° ouest     0° Greenwich +180° est
				const radius = 100 + Math.random() * 10
				const radcos = radius * Math.cos(latitude)
				rawMag_Data[rawMag_ptr++] = radcos * Math.cos(longitude)
				rawMag_Data[rawMag_ptr++] = radcos * Math.sin(longitude)
				rawMag_Data[rawMag_ptr++] = radius * Math.sin(latitude)
				rawMag_Count++
			}
			//
			// SET THE DRAW RAW RANGE -----------------------------------------
			//
			this.rawMagnetometer.geometry.setDrawRange(0, rawMag_Count);
			this.rawMagnetometer.geometry.attributes.position.needsUpdate = true;
			if (rawMag_Count == rawMag_MaxPts) {
				rawMag_Count = 0;
				rawMag_ptr = 0;
			}
			rawMag_Flag = false;
		}
		/*
		 * INIT THE MAGNETOMETER CALIBRATION ---------------------------------------------------
		 */
		if (calMag_START) {
			this.connectColor=new THREE.Color(Math.random(),Math.random(),Math.random())
			rawMag_Count = 0;
			rawMag_ptr = 0;
			calMag_Count = 0;
			calMag_ptr = 0;
			let grp=this._Ground._frame.getObjectByName ('myLines')
			grp.children.forEach(child => {
				if(child.name == "CM"){
					child.geometry.dispose()
					child.material.dispose()
					grp.remove(child)
//					erase(child)
					var x=0
				}
			})
//			this._Ground._frame.remove(this._Ground._frame.getObjectByName ('myLines'))
			for(var i=0;i<calMagLines.length;i++){
//				calMagLines[i].geometry.dispose()
//				calMagLines[i].material.dispose()
				calMagLines[i]=null;
			}
			calMagLines.length=0;
			const x=this._Ground._frame.children.length;
			this._Ground._frame.geometry.attributes.position.needsUpdate = true;
			//
			// SET THE DRAW RANGE -------------------------------------
			//
			this.rawMagnetometer.material.color.setHex(0xff0000) // restore raw color
			this.rawMagnetometer.geometry.setDrawRange(0, rawMag_Count);
			this.rawMagnetometer.geometry.attributes.position.needsUpdate = true;
			this.calMagnetometer.geometry.setDrawRange(0, calMag_Count);
			this.calMagnetometer.geometry.attributes.position.needsUpdate = true;
			calMag_START = false;
		this.renderer.render(this.scene, this.cameras['IMU']._camera);
		this.labelRenderer.render(this.scene, this.cameras['IMU']._camera);
		return
		}
		/*
		 * SHIFT THE MAGNETOMETER RAW DATA ---------------------------------------------------
		 */
		if (rawMag_FlagShift) {
			const dX = rawMag_Data[rawMag_ptr]
			const dY = rawMag_Data[rawMag_ptr + 1]
			const dZ = rawMag_Data[rawMag_ptr + 2]
			for (var i = 0; i < rawMag_Count; i++) {
				rawMag_Data[3*i] += dX
				rawMag_Data[3*i + 1] += dY
				rawMag_Data[3*i + 2] += dZ
			}
			//
			// SET THE DRAW RAW RANGE -----------------------------------------
			//
			this.rawMagnetometer.material.color.setHex(0x000000)
			this.rawMagnetometer.geometry.setDrawRange(0, rawMag_Count);
			this.rawMagnetometer.geometry.attributes.position.needsUpdate = true;
			rawMag_FlagShift = false;
		}
		/*
		 * UPDATE THE MAGNETOMETER CAL DATA ---------------------------------------------------
		 */
		if (calMag_Flag) {
			const calMag_simul = false;
			/*
			//
			//	magCalibration simulation ---------------------------------
			//
			if (calMag_simul) { //random € [0,1[
				const latitude = Math.PI * (-90 + Math.random() * 180) / 180   //  -90° pole sud  0° equateur   +90° pole nord
				const longitude = Math.PI * (-180 + Math.random() * 360) / 180 // -180° ouest     0° Greenwich +180° est
				const radius = 100 + Math.random() * 10
				const radcos = radius * Math.cos(latitude)
				calMag_Data[calMag_ptr++] = radcos * Math.cos(longitude)
				calMag_Data[calMag_ptr++] = radcos * Math.sin(longitude)
				calMag_Data[calMag_ptr++] = radius * Math.sin(latitude)
				calMag_Count++
			}
			*/
			//
			// SET THE DRAW CAL RANGE -----------------------------------------
			//
			this.calMagnetometer.geometry.setDrawRange(0, calMag_Count);
			this.calMagnetometer.geometry.attributes.position.needsUpdate = true;
			const p = calMag_ptr - 3
			calMagLines.push(_segment('CM',
					rawMag_Data[p], rawMag_Data[p + 1], rawMag_Data[p + 2],
					calMag_Data[p], calMag_Data[p + 1], calMag_Data[p + 2],
					this.connectColor.getHex ()
					)
			)
			this.Connectors.add(calMagLines[calMagLines.length-1])
			if (calMag_Count == calMag_MaxPts) {
				calMag_Count = 0;
				calMag_ptr = 0;
			}
			calMag_Flag = false;
		}
		/*
		 * UPDATE THE IMU VALUES ---------------------------------------------------
		 */
		if (imu_Flag) {
			/*
						this.Magnetometer.position.set( );
						this.Magnetometer.geometry.attributes.position.needsUpdate = true;
						this.Accelerometer.position.set( );
						this.Accelerometer.geometry.attributes.position.needsUpdate = true;
						this.Gyrometer.position.set( );
						this.Gyrometer.geometry.attributes.position.needsUpdate = true;
			*/
			imu_Flag = false;
		}
		this.renderer.render(this.scene, this.cameras['IMU']._camera);
		this.labelRenderer.render(this.scene, this.cameras['IMU']._camera);
	}
}
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/** ===================================================================
 * @class View_M3D extends View_
 * @brief View_M3D defines an extended View_ class to display lanscape & rover
 * 
 * PROPERTIES:
 * 		STL landscape
 * 		roverTrajectory
 * 		roverRaycasting
 * 		_Rover frame & 6 _face  box shape
 * 
 * METHODS:
 * 		M3D_init creates 4 parent View_ cameras (fixed & followers),
 * 		M3D_animate recieves & display rover trajectory & attitude in the lanscape 
 * 
 * PSEUDO-EVENTS from rover interface:
 * 		traject_Flag 	:when recieving a trajectory point (ie position, quaternion, speed, temperature...)
 * 		moveKAM_flag 	:mov camera
 * 		switchKAM	 	:switch camera
 */
class View_M3D extends View_ {
	constructor(DIV, COLOR) {
		super(DIV, COLOR)
		this.M3D_init();
		var timeoutID = setTimeout(this.M3D_animate.bind(this), 1000);	// for stl landscape ???
		this.landscape = this._Ground._frame.getObjectByName( "landscape" );
		//this.M3D_animate()
		this.CountPoints=0
	}
	/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * 
	 * @fn M3D_init()
	 * @brief Initializes the objects, frames & controls
	 */
	M3D_init() {

		/*
		 * GROUND FRAME ---------------------------------------------------
		 */
		this._Ground = new World(this.scene, 'GROUND', 0, 0, 0, 0, 0, 0, 0, 4, 0x777777)
		let vct = new THREE.Vector3(1, 1, 1);
		// ====	Isometric view
		this._Ground._frame.rotateOnAxis(vct.normalize(), THREE.MathUtils.degToRad(240))
		//
		//	objects ---------------------------------------------------
		//
		this._RayCast = new World(this._Ground, 'RAYCAST', 0, 0, 0, 0, 0, 0, 0, 5, 0xffffff)
		loadStl(this._Ground, 'landscape', './Picchu2.ast')
		roverTrajectory = new _trajectory('Traject', Rover_positions, 0xff00ff)
		roverRaycasting = new _trajectory('Project', Rover_groundpts, 0x000000)
		this._Ground.insert([
			new Frame('gndFrame', 100,'X','Y','Z', 0xff0000, 0x00ff00, 0x0000ff),
			roverTrajectory.p3D,roverRaycasting.p3D
		]);
		/*
		 * ROVER FRAME ---------------------------------------------------
		 */
		this._Rover = new World(this._Ground, 'ROVER', 0, 0, 0, 0, 0, 0, 0, 5, 0x000000)
		/*
		 * ROVER GRAVITY ---------------------------------------------------
		 */


		// ROVER World objects
		this.grav=_tickLine( 'Grav', 0, 0, 0, 0, 0, -100, 0xffffff)
		label('G',this.grav,0,0,-100)
		this.grav.geometry.attributes.position.setUsage(THREE.DynamicDrawUsage)
		this.north_UV=_tickLine('N', 0, 0, 0, 0, 0, 100, 0xffff00, 1)
		label('N',this.north_UV,0,0,100)
		this.north_W=_segment( 'North', 0, 0, 0, 100, 100, 0, 0x000000)
		this.north_W.geometry.attributes.position.setUsage(THREE.DynamicDrawUsage)
		this._Rover.insert([
			new Frame('rovFrame', 100,'U','V','W', 0x880000, 0x008800, 0x000088),
			this.grav,
			this.north_W,this.north_UV,
			_face(prsb[0],prsb[1],prsb[2],prsb[3],0x0000ff),//ok z+
			_face(prsb[4],prsb[5],prsb[6],prsb[7],0x5555ff),
			_face(prsb[1],prsb[5],prsb[3],prsb[7],0xff0000),//ok x+
			_face(prsb[4],prsb[0],prsb[6],prsb[2],0xff5555),
			_face(prsb[1],prsb[0],prsb[5],prsb[4],0x55ff55),
			_face(prsb[6],prsb[7],prsb[2],prsb[3],0x00ff00) //ok y+


		]);

		/*
		 * CAMERAS ---------------------------------------------------
		 */
		new Kamera(this, 'camGND', 'K_1', this._Ground._frame, this._Rover._frame);
		new Kamera(this, 'camROV', 'K_2', this._Rover._frame, this._Ground._frame);
		new Kamera(this, 'camAAA', 'K_3', this._Rover._frame);
		new Kamera(this, 'camBBB', 'K_4', this._Ground._frame)

		this.sph = new _sphera('sph', 5, 0x000000)
		this._Rover._frame.add(this.sph)
		this.sph.position.set(-150, 0, 0)

		/*
		 * CONTROLS (after this.cameras...) ---------------------------------------------------
		 */

		this.cameras['K_1'].restoreControls()

		/*
		 * LIGHTING ---------------------------------------------------
		 */
		const ambientLight = new THREE.AmbientLight( 0xffffff, 1 );
		this.scene.add( ambientLight );
		/*
		
				const hemisphereLight = new THREE.HemisphereLight( 0xff00ff,0xff00ff, 5. );
				scene.add( hemisphereLight );
		
		*/
		/*
		const mainLight = new THREE.PointLight(0xcccccc, 2., 250);
		//mainLight.position.y = 600;
		//mainLight.position.set( 550, 550, 550 );
		mainLight.position.set(0, 550, 0);
		this.scene.add(mainLight);

		const redLight = new THREE.PointLight(0xcccccc, 10.25, 1000);
		redLight.position.set(550, 0, 0);
		this.scene.add(redLight);

		const greenLight = new THREE.PointLight(0xcccccc, 10.25, 1000);
		greenLight.position.set(0, 500, 0);
		this.scene.add(greenLight);

		const blueLight = new THREE.PointLight(0xcccccc, 10.25, 1000);
		blueLight.position.set(0, 0, 550);
		this.scene.add(blueLight);
		// lights
		const mainLight = new THREE.PointLight( 0xcccccc, 1.5, 250 );
		mainLight.position.y = 60;
		this._Ground.add( mainLight );

		const greenLight = new THREE.PointLight( 0x00ff00, 0.25, 1000 );
		greenLight.position.set( 550, 50, 0 );
		this._Ground.add( greenLight );

		const redLight = new THREE.PointLight( 0xff0000, 0.25, 1000 );
		redLight.position.set( - 550, 50, 0 );
		this._Ground.add( redLight );

		const blueLight = new THREE.PointLight( 0x7f7fff, 0.25, 1000 );
		blueLight.position.set( 0, 50, 550 );
		this._Ground.add( blueLight );
		*/


		//window.addEventListener( 'resize', onWindowResize );
		console.info('---------------------------------------')
		dump(this._Ground._frame)
		console.info('---------------------------------------')
		dump(this.scene)
		console.info('---------------------------------------')
	}
	/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * 
	 * @fn M3D_animate()
	 * @brief Recieves the rover Driver motion & animates the M3D scene
	 */
	M3D_animate() {
		let pointer=0

		/*
		dump(scene)
		console.info('---------------------------------------')
		*/
		const anim = this.M3D_animate;
		requestAnimationFrame(anim.bind(this));

		/*
				// save the original this.cameras[liveKAM] properties
				const currentRenderTarget = renderer.getRenderTarget();
				const currentXrEnabled = renderer.xr.enabled;
				const currentShadowAutoUpdate = renderer.shadowMap.autoUpdate;
				renderer.xr.enabled = false; // Avoid this.cameras[liveKAM] modification
				renderer.shadowMap.autoUpdate = false; // Avoid re-computing shadows
		
		
				// restore the original rendering properties
				renderer.xr.enabled = currentXrEnabled;
				renderer.shadowMap.autoUpdate = currentShadowAutoUpdate;
				renderer.setRenderTarget(currentRenderTarget);
		*/

		/*
		 * FLAG MOVE CAMERA ---------------------------------------------------
		 */
		if (moveKAM_flag)
			moveKAM_flag = moveCamera(this,moveKAM)
		/*
		 * SWITCH CAMERAS ---------------------------------------------------
		 */
		if (switchKAM.length) {
			if (Kam_reset) {
				Kam_reset = false;
			} else {
				this.cameras[liveKAM].saveControls()
				liveKAM = switchKAM
				this.cameras[liveKAM].restoreControls()
			}
			switchKAM = '';
		}
		/*
		 * UPDATE THE MOTION ---------------------------------------------------
		 */
		//if (traject_Flag) {
/*
		if (true) {
			const simul = false;
			//
			//	Trajectory simulation ---------------------------------
			//
			if (simul) {

				this._Rover._frame.rotateZ(3 * 3.14 / 180);
				this._Rover._frame.position.set(
					tjsATT.scalePos * 50 * Math.cos(traject_Count * 3.14 / 180),
					tjsATT.scalePos * 100 * Math.sin(traject_Count * 3.14 / 180),
					100
				);
				this._Rover._frame.rotateZ(+Rover_RPY[3*traject_Count-3]/180);
				this._Rover._frame.rotateY(+Rover_RPY[3*traject_Count-2]/180);
				this._Rover._frame.rotateX(-Rover_RPY[3*traject_Count-1]/180);
			}
			//
			//	True trajectory ---------------------------------------
			//
			else {
*/	
		let DRVkmd={}
		if((traject_Queue.length)&&(!traject_Queue[0]['_WGL_done'])){
			DRVkmd=Object.assign({}, traject_Queue[0])
			traject_Queue[0]['_WGL_done']=true
		document.getElementById("wglSemaphore").innerHTML = "wgl "+traject_Queue.length.toString()+ "  "+traject_Count.toString()+" "+(traject_Filled?"full":"...");
			if (traject_Count == traject_MaxPts) {
				traject_Filled = true
				traject_Count = 0;
			} 
			pointer=3 * traject_Count
			Rover_positions[pointer++] = DRVkmd[DEF_KF_XC0];
			Rover_positions[pointer++] = DRVkmd[DEF_KF_YC0];
			Rover_positions[pointer++] = DRVkmd[DEF_KF_ZC0];
			pointer -= 3;
			Rover_RPY[pointer++] = DRVkmd[DEF_KF_ROLL];
			Rover_RPY[pointer++] = DRVkmd[DEF_KF_PITCH];
			Rover_RPY[pointer++] = DRVkmd[DEF_KF_YAW];
			pointer -= 3;
			Rover_gravity[pointer++] = DRVkmd[DEF_KF_GRX];
			Rover_gravity[pointer++] = DRVkmd[DEF_KF_GRY];
			Rover_gravity[pointer++] = DRVkmd[DEF_KF_GRZ];
			pointer -= 3;
			Rover_north[pointer++] = DRVkmd[DEF_KF_Imx];
			Rover_north[pointer++] = DRVkmd[DEF_KF_Imy];
			Rover_north[pointer++] = DRVkmd[DEF_KF_Imz];

			RADAR.push(DRVkmd[DEF_KF_DIST]);
			//reordered Quaternion( x, y , z, w ) !!!
			Rover_quat.push(DRVkmd["Q1"]);
			Rover_quat.push(DRVkmd["Q2"]);
			Rover_quat.push(DRVkmd["Q3"]);
			Rover_quat.push(DRVkmd["Q0"]);

			traject_Count++;


			// Rover_quat is reordered Quaternion( x, y , z, w ) by updateAttitude !!!
			pointer=4 * (traject_Count-1)
			this._Rover._frame.setRotationFromQuaternion(
				new THREE.Quaternion(
					Rover_quat[pointer++],
					Rover_quat[pointer++],
					Rover_quat[pointer++],
					Rover_quat[pointer]
				)
			)
			pointer=3 * (traject_Count-1)
			this._Rover._frame.position.set(
				tjsATT.scalePos * Rover_positions[pointer++],
				tjsATT.scalePos * Rover_positions[pointer++],
				tjsATT.scalePos * Rover_positions[pointer]
			)
			//
			//	update screen from trajectory data --------------------
			//  ----------------------------------
			//
			this._Rover._frame.updateMatrixWorld(true);// force children updating
			/**
			 * Update Grav & North
			 */
			 /*
			const grav = this._Rover._frame.getObjectByName( "Grav" ).geometry.attributes.position;
			grav[3]=Rover_gravity[3 * traject_Count - 3]
			grav[4]=Rover_gravity[3 * traject_Count - 2]
			grav[5]=Rover_gravity[3 * traject_Count - 1]
			grav.needsUpdate = true;
			*/
			this.grav.lookAt(
				this._Rover._frame.localToWorld(
					new THREE.Vector3(
						-2*Rover_gravity[3 * traject_Count - 3],
						-2*Rover_gravity[3 * traject_Count - 2],
						-2*Rover_gravity[3 * traject_Count - 1]
						)
					)
				)
/*
			let normG=0.01*Math.sqrt(
				Rover_gravity[3 * traject_Count - 3]*Rover_gravity[3 * traject_Count - 3]+
				Rover_gravity[3 * traject_Count - 2]*Rover_gravity[3 * traject_Count - 2]+
				Rover_gravity[3 * traject_Count - 1]*Rover_gravity[3 * traject_Count - 1])
			this.grav.geometry.attributes.position.array[3]=Rover_gravity[3 * traject_Count - 3]/normG
			this.grav.geometry.attributes.position.array[4]=Rover_gravity[3 * traject_Count - 2]/normG
			this.grav.geometry.attributes.position.array[5]=Rover_gravity[3 * traject_Count - 1]/normG
			this.grav.geometry.attributes.position.needsUpdate = true;

			this.grav.userData.label.position.x=Rover_gravity[3 * traject_Count - 3]/normG
			this.grav.userData.label.position.y=Rover_gravity[3 * traject_Count - 2]/normG
			this.grav.userData.label.position.z=Rover_gravity[3 * traject_Count - 1]/normG
*/
			/*
			const north_W = this._Rover._frame.getObjectByName( "North" ).geometry.attributes.position;
			north_W[3]=Rover_north[3 * traject_Count - 3]
			north_W[4]=Rover_north[3 * traject_Count - 2]
			north_W[5]=Rover_north[3 * traject_Count - 1]
			north_W.needsUpdate = true;
			*/
			let normN=0.01*Math.sqrt(
				Rover_north[3 * traject_Count - 3]*Rover_north[3 * traject_Count - 3]+
				Rover_north[3 * traject_Count - 2]*Rover_north[3 * traject_Count - 2]+
				Rover_north[3 * traject_Count - 1]*Rover_north[3 * traject_Count - 1])
			this.north_W.geometry.attributes.position.array[3]=0//Rover_north[3 * traject_Count - 3]/normN
			this.north_W.geometry.attributes.position.array[4]=0//Rover_north[3 * traject_Count - 2]/normN
			this.north_W.geometry.attributes.position.array[5]=-Rover_north[3 * traject_Count - 1]/normN
			this.north_W.geometry.attributes.position.needsUpdate = true;

			this.north_UV.lookAt(
				this._Rover._frame.localToWorld(
					new THREE.Vector3(
						Rover_north[3 * traject_Count - 3],
						Rover_north[3 * traject_Count - 2],
						0//,Rover_north[3 * traject_Count - 1]
						)
					)
				)
			this.north_UV.geometry.attributes.position.needsUpdate = true;
/*
			this.north_UV.userData.label.position.x=Rover_north[3 * traject_Count - 3]/normN
			this.north_UV.userData.label.position.y=Rover_north[3 * traject_Count - 2]/normN
			this.north_UV.userData.label.position.z=Rover_north[3 * traject_Count - 1]/normN
*/
			//matrixWorldNeedsUpdate=true;
			/*
						this.cameras['K_3'][0].matrixWorldNeedsUpdate=true;
						this.cameras['K_3'][1].update();
			*/
			//
			//	RAYCASTER -------------------------------------------------
			//
			// when STL is loaded !!!!
			if (typeof this.landscape !== 'undefined'){


				const rovPos = new THREE.Vector3()
				this._Rover._frame.getWorldPosition(rovPos) 
				const rovDir=new THREE.Vector3(0,-1,0)

				const raycaster = new THREE.Raycaster(rovPos,rovDir)
				const target=new Array()
				raycaster.intersectObject (this.landscape, false,target)
				if(target.length){
					for(var i=0;i<target.length;i++){
						console.log(i,":",target[i])//,target[i].object)			
						console.log(i,": p=",target[i].point,", n=",target[i].face.normal)//,target[i].object)	
						this.scene.attach( this._RayCast._frame)		// detach from this._Ground._frame
						this._RayCast._frame.position.set( target[i].point.x,target[i].point.y, target[i].point.z );
						this._Ground._frame.attach( this._RayCast._frame );
						// to do angles orientation

						pointer=3 * (traject_Count-1)
						Rover_groundpts[pointer++]=this._RayCast._frame.position.x
						Rover_groundpts[pointer++]=this._RayCast._frame.position.y
						Rover_groundpts[pointer]=this._RayCast._frame.position.z
						


					}
					target.length=0
				}
				else{
					pointer=3 * (traject_Count-1)
					Rover_groundpts[pointer++]=Rover_groundpts[pointer-3]
					Rover_groundpts[pointer++]=Rover_groundpts[pointer-3]
					Rover_groundpts[pointer]=Rover_groundpts[pointer-3]
					console.log("no landscape intersection")
				}
			}
			// when STL is not loaded yet
			else{
				pointer=3 * (traject_Count-1)
				Rover_groundpts[pointer++]=0
				Rover_groundpts[pointer++]=0
				Rover_groundpts[pointer]=0
				console.log("this.landscape is undefined")			
				this.landscape = this._Ground._frame.getObjectByName( "landscape" );
			}
			//
			// SET THE DRAW RANGE -----------------------------------------
			//
			this._Rover._frame.geometry.attributes.position.needsUpdate = true;

			this.CountPoints=roverTrajectory.countPoints()
			console.log("CNT cnt ",traject_Count,this.CountPoints)

			roverTrajectory.p3D.geometry.setDrawRange(0,this.CountPoints );
			roverTrajectory.p3D.geometry.attributes.position.needsUpdate = true;

			roverTrajectory.p3D.geometry.attributes.color.needsUpdate = true;



			roverRaycasting.p3D.geometry.setDrawRange(0, this.CountPoints );
			roverRaycasting.p3D.geometry.attributes.position.needsUpdate = true;

			roverRaycasting.p3D.geometry.attributes.color.needsUpdate = true;


			//
			// TRACK CAMERAS TO THEIR MOVING TARGET -----------------------
			//
			for (var key in this.cameras)
				this.cameras[key].track()
			this.cameras[liveKAM].saveControls()		// if a switch camera happens...

			/*
			*/
			traject_Flag = false;
		}
		//this.cameras[liveKAM][1].update();//Updates the helper based on the projectionMatrix of the camera.
		this.renderer.render(this.scene, this.cameras[liveKAM]._camera);
		this.labelRenderer.render(this.scene, this.cameras[liveKAM]._camera);
	}
}
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/** ===================================================================
 * @class View_ATT extends View_
 * @brief View_ATT defines an extended View_ class to display ...
 * 
 * PROPERTIES:
 * 
 * METHODS:
 * 
 * PSEUDO-EVENTS from rover interface:
 * 
 */
class View_ATT extends View_ {
	constructor(DIV, COLOR) {
		super(DIV, COLOR)
		this.ATT_init();
		this.ATT_animate();
	}
	/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * 
	 * @fn ATT_init()
	 * @brief Initializes the scene objects, frames & controls
	 */
	ATT_init() {
		/*
		 * GROUND FRAME ---------------------------------------------------
		 */
		this._Ground = new World(this.scene, 'GROUND', 0, 0, 0, 0, 0, 0, 0, 4, 0x777777)
		let vct = new THREE.Vector3(1, 1, 1);
		// ====	Isometric view
		this._Ground._frame.rotateOnAxis(vct.normalize(), THREE.MathUtils.degToRad(240))
		//
		//	objects ---------------------------------------------------
		//
		this.Compass = _sphera('Compass', 100, 0xffffff)
		const loader = new THREE.TextureLoader()
		this.Compass.material.map = loader.load('text.png')
		this._Ground.insert([
			new Frame('gndFrame', 100,'a','b','c', 0xff0000, 0x00ff00, 0x0000ff),
			this.Compass
		]);
		this._camera = new THREE.PerspectiveCamera(50, this.aspectRatio, .1, 10000)
		this._camera.position.set(1000, 1000, 1000)
		this._camera.name = 'ATT'
		this._helper = new myCameraHelper(this._camera, 0xffffff)
		//		this._Ground._frame.add(this._camera)
		this.scene.add(this._camera)
		this.scene.add(this._helper)


		this._camera.updateMatrixWorld(true);// force children updating
		const xxx = new THREE.Vector3()
		this._Ground._frame.getWorldPosition(xxx)
		//		this._camera.lookAt(xxx)
		this._camera.lookAt(new THREE.Vector3(0, 0, 0))
		this._camera.updateProjectionMatrix();
		this._helper.update()



		this._cameraControls = new OrbitControls(this._camera, this.labelRenderer.domElement)
	}
	/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * 
	 * @fn ATT_animate()
	 * @brief Recieves the rover Driver motion & animates the M3D scene
	 */
	ATT_animate() {
		const anim = this.ATT_animate;
		requestAnimationFrame(anim.bind(this));
		/** ********************************************************
		 * UPDATE THE ATT VALUES
		 */
		if (imu_Flag) {
			/*
						this.Magnetometer.position.set( );
						this.Magnetometer.geometry.attributes.position.needsUpdate = true;
						this.Accelerometer.position.set( );
						this.Accelerometer.geometry.attributes.position.needsUpdate = true;
						this.Gyrometer.position.set( );
						this.Gyrometer.geometry.attributes.position.needsUpdate = true;
			*/
			imu_Flag = false;
		}
		this.renderer.render(this.scene, this._camera);
		this.labelRenderer.render(this.scene, this._camera);
	}
}
//#####################################################################
function window_Resize(e){
	for (var div in actViews){
		const SELF=actViews[div]
		if(SELF.fct_Resize==SELF.div_Resize)
			return
		SELF.cow = window.innerWidth;
		SELF.coh = window.innerHeight;
		SELF.aspectRatio = SELF.cow / SELF.coh;
		SELF.update_cameras()
		SELF.renderer.setSize( SELF.cow, SELF.coh )
		SELF.labelRenderer.setSize(  SELF.cow, SELF.coh )
	}
}
let M3Dview = new View_M3D('M3D', 0x999999)
let IMUview = new View_IMU('IMU_3js', 0x999999)
let ATTview = new View_ATT('MAP', 0x999999)
window.addEventListener('resize', window_Resize)

///////////////////////////////////////////////////////////////////////
