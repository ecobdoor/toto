///////////////////////////////////////////////////////////////////////
//  https://medium.com/codex/global-variables-and-javascript-modules-ce674a869164
window.ONE = 1;
self.TWO = 2;
globalThis.TROIS = 3;


globalThis.Kam_key = 'GND';
globalThis.Kam_flag = false;
globalThis.Kam_reset = false;
globalThis.Kam_move = '';
globalThis.webglCTRL = new Array()
webglCTRL['GND']={ 
	xCam:150,
	yCam:150,
	zCam:150,
	rollCam:0,
	pitchCam:0,
	yawCam:0 ,
	nearCam:0.1,
	farCam:1000,
	colorCam:0xff0000,
	focalCam:1,
}
webglCTRL['ROV']={ 
	xCam:100,
	yCam:0,
	zCam:0,
	rollCam:0,
	pitchCam:0,
	yawCam:0 ,
	nearCam:0.1,
	farCam:1000,
	colorCam:0x00ff00,
	focalCam:1,
}
webglCTRL['AUX']={ 
	xCam:100,
	yCam:0,
	zCam:0,
	rollCam:0,
	pitchCam:0,
	yawCam:0 ,
	nearCam:0.1,
	farCam:1000,
	colorCam:0x0000ff,
	focalCam:1,
	frame:{}
}
		 // M3D controls

globalThis.traject_MaxPts = 36000; // one hour !!!
globalThis.traject_Count = 0;
globalThis.traject_Flag = false;
globalThis.traject_MaxLen = 3 * traject_MaxPts;
globalThis.POSITION = new Float32Array(traject_MaxLen);
for (var i = 0; i < traject_MaxLen; i += 3) {
	POSITION[i] = 50 * Math.cos(3.14 * i / 180);
	POSITION[i + 1] = 50 * Math.sin(3.14 * i / 180);
	POSITION[i + 2] = i / 1000;
}
globalThis.ATTITUDE = new Float32Array(traject_MaxLen);
globalThis.GRAVITY = new Float32Array(traject_MaxLen);
globalThis.MAGNORTH = new Float32Array(traject_MaxLen);
globalThis.QUAT = new Array();
globalThis.RADAR = new Array();
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
import { myCameraHelper } from     './THREE/three.js/src/helpers/myCameraHelper.js';
let DEBUG = false;

const _Xmin_Color = 0xff7777;
const _Xmax_Color = 0xff0000;
const _Ymin_Color = 0x77ff77;
const _Ymax_Color = 0x00ff00;
const _Zmin_Color = 0x7777ff;
const _Zmax_Color = 0x0000ff;
let aspectRatio,  scene, renderer, cameraControls, _Ground, _Rover;
let traject_Three = {};
let cameras = new Array();
let cur_Kam=Kam_key
/*
let gnd_controls = {}
let rov_controls = {}
*/
/*
	var worldMatrix=new THREE.Matrix4(_Rover._frame.matrixWorld);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
	function _segment(XA,YA,ZA,XB,YB,ZB,COLOR)
*/
//#####################################################################
function _segment(XA, YA, ZA, XB, YB, ZB, COLOR) {
	//create a blue LineBasicMaterial
	const material = new THREE.LineBasicMaterial({ color: COLOR });

	//After material we will need a geometry with some vertices:
	const points = [];
	points.push(new THREE.Vector3(XA, YA, ZA));
	points.push(new THREE.Vector3(XB, YB, ZB));
	const geometry = new THREE.BufferGeometry().setFromPoints(points);

	//Note that lines are drawn between each consecutive pair of vertices, but not between the first and last (the line is not closed.)
	return new THREE.Line(geometry, material);
}
//=====================================================================
/*
	function _sphera(RADIUS,COLOR)
*/
function _sphera(RADIUS, COLOR) {
	const geometry = new THREE.SphereGeometry(RADIUS, 32, 16);
	const material = new THREE.MeshBasicMaterial({ color: COLOR });
	return new THREE.Mesh(geometry, material);
}
//=====================================================================
/*
	function _trajectory(WORLD,COLOR)
*/
function _trajectory(WORLD, COLOR) {
	//create a blue LineBasicMaterial
	const material = new THREE.LineBasicMaterial({ color: COLOR });

	//After material we will need a geometry with some vertices:
	//    const geometry = new THREE.BufferGeometry().setFromPoints( POSITION );

	var geometry = new THREE.BufferGeometry();
	//var positions = new Float32Array( MAX_POINTS * 3 ); 
	geometry.addAttribute('position', new THREE.BufferAttribute(POSITION, 3));
	return new THREE.Line(geometry, material);
}
//=====================================================================
/*
	function _tickLine(NAME,XA,YA,ZA,XB,YB,ZB,COLOR,RADIUS)
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
		512,// path segments
		RADIUS,// THICKNESS
		8, //Roundness of Tube (number of points for circle)
		false //closed
	);
	return new THREE.Line(tubeGeometry, material);
}
//=====================================================================
/*
	function Frame2(XA,YA,ZA,LEN)
*/
function Frame2(X, Y, Z, LEN) {
	var Elts = new Array();
	Elts.push(new _segment(X, Y, Z, X + LEN, Y, Z, 0xff0000));
	Elts.push(new _segment(X, Y, Z, X, Y + LEN, Z, 0x00ff00));
	Elts.push(new _segment(X, Y, Z, X, Y, Z + LEN, 0x0000ff));
	return Elts;
}
//=====================================================================
/*
	function wall(NAME,WORLD,PLANEGEO,DX,DY,DZ,A,B,C,COLOR)
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
//=====================================================================
/*
	function Stl(NAME,FILENAME)
	https://github.com/mrdoob/three.js/blob/master/examples/webgl_loader_stl.html
*/
function loadStl(WORLD,NAME,FILENAME) {
	const loader = new STLLoader()
	loader.load( FILENAME, function ( geometry ) {
			const material = new THREE.MeshPhongMaterial( 
				{ 
					color: 0x8f5902, 
					specular: 0x342d91, 
					shininess: 100 ,
					side:THREE.DoubleSide,
					flatShading:true,
					wireframe:false,
					reflectivity:1,
					transparent:true,
					opacity:0.5
				} );
			const mesh = new THREE.Mesh( geometry, material );
			mesh.position.set( -100, - 100, -50 );
			mesh.rotation.set( 0, 0, 0 );
			mesh.scale.set( 5, 20, 1 );
			mesh.castShadow = true;
			mesh.receiveShadow = true;
//			scene.add( mesh );
	    WORLD.insert( mesh );
		} 
	);
}
//=====================================================================
/*
	function Frame(NAME,Xmin,Ymin,Zmin,Xmax,Ymax,Zmax)
*/
function Frame(NAME, LEN, Xcolor, Ycolor, Zcolor) {
	var Elts = new Array();
	/*
		_tickLine(WORLD,Xmin,0,0,Xmax,0,0,0xff0000,2);
		_tickLine(WORLD,0,Ymin,0,0,Ymax,0,0x00ff00,2);
		_tickLine(WORLD,0,0,Zmin,0,0,Zmax,0x0000ff,2);
	*/
	Elts.push(new _tickLine('iX+', 0, 0, 0, LEN, 0, 0, Xcolor, 2));
	Elts.push(new _tickLine('jY+', 0, 0, 0, 0, LEN, 0, Ycolor, 2));
	Elts.push(new _tickLine('kZ+', 0, 0, 0, 0, 0, LEN, Zcolor, 2));

	return Elts
}
//=====================================================================
/*
	function Box(NAME,Xmin,Ymin,Zmin,Xmax,Ymax,Zmax)
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

var w = 0;
var h = 0;
/**********************************************************************
 * position & angles (in degrees) IN LOCAL
 */
function moveCamera(KAM,FRAME){
	const ctl=webglCTRL[KAM]

	moveCamera2(KAM,FRAME,
		ctl.xCam,
		ctl.yCam,
		ctl.zCam,
		ctl.rollCam,
		ctl.pitchCam,
		ctl.yawCam,
		ctl.focalCam
		)
}
function moveCamera2(KAM,FRAME,X, Y, Z, A, B, C,F){
	const Camera=cameras[KAM][0]
	webglCTRL[KAM].attach=FRAME
	//
	//	Local OBJECT situation wined for the camera
	//
	const euler = new THREE.Euler(THREE.MathUtils.degToRad(A),THREE.MathUtils.degToRad(B),THREE.MathUtils.degToRad(C),'ZYX');
	const quaternion = new THREE.Quaternion();
	quaternion.setFromEuler(euler);

	Camera.matrixWorldNeedsUpdate=true;
	Camera.setRotationFromQuaternion(quaternion)
	Camera.position.set(X,Y,Z)
		Camera.updateMatrixWorld( true );// force children updating
	Camera.fov=F
	Camera.updateProjectionMatrix();
/*
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
*/
	}
/**********************************************************************
 * @fn trackCamera(KAM,FRAME,TARGET)
 * @brief Turns the camera to the target
 * @param KAM implied camera
 * @param FRAME base axis for relative X Y Z A B C data
 * @param TARGET where the camera looks at
 */
function trackCamera(KAM,FRAME,TARGET){
	placeCamera(KAM,FRAME,TARGET)
	const camera=cameras[KAM][0]
	const helper=cameras[KAM][1]
	const xxx = new THREE.Vector3()
	TARGET.getWorldPosition(xxx)
	camera.lookAt(xxx)//(TARGET.getWorldPosition);
camera.updateMatrixWorld (true)// updateChildren( force : Boolean )
	camera.updateProjectionMatrix();
	helper.update()
	}
/**********************************************************************
 * @fn placeCamera(KAM,FRAME,TARGET)
 * @brief Turns the camera to the target
 * @param KAM implied camera
 * @param FRAME base axis for relative X Y Z A B C data
 * @param TARGET where the camera looks at
 */
function placeCamera(KAM,FRAME,TARGET){
	const camera=cameras[KAM][0]
	const helper=cameras[KAM][1]
	/*const xxx = new THREE.Vector3()
	TARGET.getWorldPosition(xxx)
	camera.lookAt(xxx)//(
	*/
	camera.lookAt(TARGET.getWorldPosition);
	camera.updateProjectionMatrix();
	helper.update()
	}
/***************************************************************
 *
 */

class Kamera {

	constructor(FOV,ASPECT,NEAR,FAR,COLOR) {
		this._camera=new THREE.PerspectiveCamera(FOV,ASPECT,NEAR,FAR)
		this._helper= new myCameraHelper(this._camera,COLOR)
	}

	setA_ttitude(X, Y, Z, A, B, C){
		const euler = new THREE.Euler(THREE.MathUtils.degToRad(A),THREE.MathUtils.degToRad(B),THREE.MathUtils.degToRad(C),'ZYX');
		const quaternion = new THREE.Quaternion();
		quaternion.setFromEuler(euler);
		const position = new THREE.Vector3( X,Y,Z );
	}

}

/**********************************************************************
/*
	function Camera(NAME,X,Y,Z,A,B,C,COLOR)
*/
function Camera(NAME, X, Y, Z, A, B, C,NEAR,FAR, COLOR) {
	var Elts = new Array();
	const camera = new THREE.PerspectiveCamera(50, aspectRatio, NEAR,FAR);
	camera.name=NAME
	//const camera = new THREE.OrthographicCamera( -w/2, w/2, h/2,-h/2,-1000,1000 );
	camera.rotateX(THREE.MathUtils.degToRad(A));
	camera.rotateY(THREE.MathUtils.degToRad(B));
	camera.rotateZ(THREE.MathUtils.degToRad(C));
	camera.position.set(X, Y, Z);
	Elts.push(camera)

	const camera_Helper = new myCameraHelper(camera,COLOR);
	camera_Helper.update();//Updates the helper based on the projectionMatrix of the camera.
	Elts.push(camera_Helper)
	/*
	*/
	return Elts;
}
//=====================================================================
/*
	constructor(PARENT,NAME,X,Y,Z,A,B,C,LEN,RADIUS,COLOR)
*/
class World {
	constructor(PARENT, NAME, X, Y, Z, A, B, C, LEN, RADIUS, COLOR) {
		this._frame = new _sphera(RADIUS, COLOR)
		this._frame.position.set(X, Y, Z);
		this._frame.rotation.set(THREE.MathUtils.degToRad(A), THREE.MathUtils.degToRad(B), THREE.MathUtils.degToRad(C));
		if (typeof PARENT._frame === 'undefined')
			PARENT.add(this._frame)
		else
			PARENT._frame.add(this._frame)
	}
	insert(mesh3D) {
		if (Array.isArray(mesh3D))
			for (var i = 0; i < mesh3D.length; i++)
				this.insert(mesh3D[i]);
		else
			this._frame.add(mesh3D)
	}
}
/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * 
 * @fn function init(DIV)
 * @brief Initializes the scene objects, frames, controls
 * @param DIV the page element for rendering
 */
function init(DIV) {
	const container = document.getElementById(DIV);
	w = container.offsetWidth;
	h = container.offsetHeight;
	aspectRatio = w / h;
	// renderer
	renderer = new THREE.WebGLRenderer({ antialias: true });
	renderer.setPixelRatio(window.devicePixelRatio);
	renderer.setSize(w, h);
	container.appendChild(renderer.domElement);
	renderer.localClippingEnabled = true;

	/*=============================================================
	 * SCENE
	 */
	scene = new THREE.Scene();
	scene.background = new THREE.Color(0x888888);
	//scene.add( new THREE.GridHelper( 400, 10 ) );

	/*=============================================================
	 * GROUND FRAME
	 */
	_Ground = new World(scene, 'GROUND', 0, 0, 0, 0, 0, 0, 0, 4, 0x777777)
	let vct = new THREE.Vector3(1, 1, 1);
	// ====	Isometric view
	_Ground._frame.rotateOnAxis(vct.normalize(), THREE.MathUtils.degToRad(240))
	//
	//	objects ---------------------------------------------------
	//
	loadStl(_Ground,'NAME','./Picchu2.ast')
	traject_Three = new _trajectory('Taject', 0xff00ff)
	_Ground.insert([
		new Frame('gndFrame', 100, 0xff0000, 0x00ff00, 0x0000ff),
		traject_Three
	]);

	/*=============================================================
	 * ROVER FRAME
	 */
	_Rover = new World(_Ground, 'ROVER', 0, 0, 0, 0, 0, 0, 0, 5, 0x000000)



	// ROVER World objects
	_Rover.insert([
		new Frame('rovFrame', 100, 0xff00ff, 0xffff00, 0x00ffff)
		//new Box('roverWalls', +10, +20, +30, +90, +100, +110)
	]);

	/** ============================================================
	 * CAMERAS (before controls...)
	 */
	cameras['GND'] = Camera('gnd', 150, 150, 150, 0, 0, 0, 0.1, 2000.0,0xff0000);
	_Ground._frame.add(cameras['GND'][0]);
	cameras['GND'][1].update();
	scene.add(cameras['GND'][1]);

	cameras['ROV'] = Camera('rov', 0, 0, 50, 180, 0, 0,0.1,2000,0x0000ff);
	_Rover._frame.add(cameras['ROV'][0]);
	cameras['ROV'][1].update();
	scene.add(cameras['ROV'][1]);

	cameras['AUX'] = Camera('aux', 0, 0, 50, 180, 0, 0,1,200,0x00ff00);
	webglCTRL['AUX'].attach=_Rover._frame;
	_Rover._frame.add(cameras['AUX'][0]);
	cameras['AUX'][1].update();
	scene.add(cameras['AUX'][1]) // Every time attach to scene !!!!!!!!!

	/** ============================================================
	 * CONTROLS (after cameras...)
	 */
	 /*
	 
	 */

	cameraControls = new OrbitControls(cameras['GND'][0], renderer.domElement);
					saveControls('GND')
	cameraControls = new OrbitControls(cameras['ROV'][0], renderer.domElement);
				saveControls('ROV')
	cameraControls = new OrbitControls(cameras['AUX'][0], renderer.domElement);
					saveControls('AUX')
	
	restoreControls('GND')
	cameraControls.update()


/*
	gnd_controls = new ArcballControls(cameras['GND'][0], renderer.domElement,scene);
	rov_controls = new ArcballControls(cameras['ROV'][0], renderer.domElement,scene);
*/
	/*
			cameraControls.target.set(0, 40, 0);
			cameraControls.maxDistance = 400;
			cameraControls.minDistance = 10;
			cameraControls.update();
	*/


	/** ============================================================
	 * LIGHTING
	 */
	/*
			const ambientLight = new THREE.AmbientLight( 0xcccccc, 0.1 );
			scene.add( ambientLight );
	
			const hemisphereLight = new THREE.HemisphereLight( 0xff00ff,0xff00ff, 5. );
			scene.add( hemisphereLight );
	
	*/
	const mainLight = new THREE.PointLight(0xcccccc, 2., 250);
	//mainLight.position.y = 600;
	//mainLight.position.set( 550, 550, 550 );
	mainLight.position.set(0, 550, 0);
	scene.add(mainLight);

	const redLight = new THREE.PointLight(0xcccccc, 10.25, 1000);
	redLight.position.set(550, 0, 0);
	scene.add(redLight);

	const greenLight = new THREE.PointLight(0xcccccc, 10.25, 1000);
	greenLight.position.set(0, 500, 0);
	scene.add(greenLight);

	const blueLight = new THREE.PointLight(0xcccccc, 10.25, 1000);
	blueLight.position.set(0, 0, 550);
	scene.add(blueLight);
	/*
	// lights
	const mainLight = new THREE.PointLight( 0xcccccc, 1.5, 250 );
	mainLight.position.y = 60;
	_Ground.add( mainLight );

	const greenLight = new THREE.PointLight( 0x00ff00, 0.25, 1000 );
	greenLight.position.set( 550, 50, 0 );
	_Ground.add( greenLight );

	const redLight = new THREE.PointLight( 0xff0000, 0.25, 1000 );
	redLight.position.set( - 550, 50, 0 );
	_Ground.add( redLight );

	const blueLight = new THREE.PointLight( 0x7f7fff, 0.25, 1000 );
	blueLight.position.set( 0, 50, 550 );
	_Ground.add( blueLight );
*/


	//window.addEventListener( 'resize', onWindowResize );

}
//-----------------------------------------------------------------
/*
function onWindowResize() {

	cameras[Kam_key].aspect = w / h;
	cameras[Kam_key].updateProjectionMatrix();

	renderer.setSize( w, h );

}
*/
let trackBall =function () {

renderer.render( scene, cameras[cur_Kam][0] );

}
let controlsStates=new Array()
controlsStates['GND']={
	object:cameras['GND'],
	zoom0:1,
	target0:new THREE.Vector3(0,0,50),
	position0:new THREE.Vector3(0,0,50)
}
controlsStates['ROV']={
	object:cameras['ROV'],
	zoom0:1,
	target0:new THREE.Vector3(0,0,50),
	position0:new THREE.Vector3(0,0,50)
}
controlsStates['AUX']={
	object:cameras['AUX'],
	zoom0:1,
	target0:new THREE.Vector3(0,0,50),
	position0:new THREE.Vector3(0,0,50)
}
/**********************************************************************
 * @fn function saveControls(KAM)
 * @brief save the state of camera controls
 * @param KAM label key of camera
 */
function saveControls(KAM){
	cameraControls.saveState()
	controlsStates[KAM].object=cameras[KAM][0]
	controlsStates[KAM].zoom0=cameraControls.zoom0
	controlsStates[KAM].target0=cameraControls.target0
	controlsStates[KAM].position0=cameraControls.position0
	
}
/**********************************************************************
 * @fn function restoreControls(KAM)
 * @brief restore the state of camera controls
 * @param KAM label key of camera
 */
function restoreControls(KAM){
	cameraControls.object=controlsStates[KAM].object
	cameraControls.zoom0=controlsStates[KAM].zoom0
	cameraControls.target0=controlsStates[KAM].target0
	cameraControls.position0=controlsStates[KAM].position0
	cameraControls.reset()
}
/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * 
 * @fn function animate()
 * @brief Animates the scene
 */
function animate() {

	requestAnimationFrame(animate);

	/*
			// save the original cameras[Kam_key] properties
			const currentRenderTarget = renderer.getRenderTarget();
			const currentXrEnabled = renderer.xr.enabled;
			const currentShadowAutoUpdate = renderer.shadowMap.autoUpdate;
			renderer.xr.enabled = false; // Avoid cameras[Kam_key] modification
			renderer.shadowMap.autoUpdate = false; // Avoid re-computing shadows
	
	
			// restore the original rendering properties
			renderer.xr.enabled = currentXrEnabled;
			renderer.shadowMap.autoUpdate = currentShadowAutoUpdate;
			renderer.setRenderTarget(currentRenderTarget);
	*/

	/**************************************************************
	 * FLAG MOVE CAMERA
	 */
	if (Kam_move.length){
		//gnd_controls.saveState ();//save old GND...
		//cameras['GND'][0].position.set(webglCTRL.myCamera.xCam,webglCTRL.myCamera.yCam,webglCTRL.myCamera.zCam)
		//gnd_controls.reset ();//restore old GND...
		/*
		*/
		moveCamera(Kam_move,_Rover._frame)
		cameras[Kam_move][1].update();

		Kam_move='';
	}
	/**************************************************************
	 * SWITCH CAMERAS
	 */
	if (Kam_flag) {
		if (Kam_reset) {
			Kam_reset = false;
		} else {
			saveControls(cur_Kam)
			restoreControls(Kam_key)
			cameraControls.update();// to see the helper in ground view ???
			cur_Kam=Kam_key
		}
		Kam_flag = false;
	}
	/**********************************************************
	 * UPDATE THE OBJECTS
	 */
	if (traject_Flag) {
//	if (false) {
		const simul=true;
		//
		//	Trajectory simulation ---------------------------------
		//
		if(simul){

			_Rover._frame.rotateZ(3*3.14/180);
			_Rover._frame.position.set(
				50 * Math.cos(traject_Count*3.14/180),
				100 * Math.sin(traject_Count*3.14/180),
				100
			);
			/*
			_Rover._frame.rotateZ(+ATTITUDE[3*traject_Count-3]/180);
			_Rover._frame.rotateY(+ATTITUDE[3*traject_Count-2]/180);
			_Rover._frame.rotateX(-ATTITUDE[3*traject_Count-1]/180);
			*/
		}
		//
		//	True trajectory ---------------------------------------
		//
		else{	
			_Rover._frame.setRotationFromQuaternion(
				new THREE.Quaternion(
					QUAT[4 * traject_Count - 4],
					QUAT[4 * traject_Count - 3],
					QUAT[4 * traject_Count - 2],
					QUAT[4 * traject_Count - 1]
				)
			);
			_Rover._frame.position.set(
				webglCTRL.posScale * POSITION[3 * traject_Count - 3],
				webglCTRL.posScale * POSITION[3 * traject_Count - 2],
				webglCTRL.posScale * POSITION[3 * traject_Count - 1]
			);
		}
		_Rover._frame.updateMatrixWorld( true );// force children updating
		//matrixWorldNeedsUpdate=true;
			cameras['AUX'][0].matrixWorldNeedsUpdate=true;
			cameras['AUX'][1].update();



		//
		// SET THE DRAW RANGE -----------------------------------------
		//
		_Rover._frame.geometry.attributes.position.needsUpdate = true;
		traject_Three.geometry.setDrawRange(0, traject_Count + 100);
		traject_Three.geometry.attributes.position.needsUpdate = true;

		//
		// PLACE CAMERAS ----------------------------------------------
		//
/*
		trackCamera('GND',_Ground._frame,10,10,250,0,0,0,_Rover._frame);
		placeCamera('ROV',_Rover._frame,200,0,0,0,70,0,_Ground._frame);
*/
		trackCamera('GND',_Ground._frame,_Rover._frame);
		placeCamera('ROV',_Rover._frame,_Ground._frame);
//		placeCamera('GND',_Ground._frame,0,0,700,webglCTRL.myCamera.rollCam,webglCTRL.myCamera.pitchCam,webglCTRL.myCamera.yawCam,_Rover._frame);

		if(Kam_key==='GND'){
			saveControls('GND')
		}

		if(Kam_key==='ROV'){
			saveControls('ROV')
			cameraControls.update();// to see the helper in ground view ???
		}
		if(Kam_key==='AUX'){
			saveControls('AUX')
			cameraControls.update();// to see the helper in ground view ???
		}
		traject_Flag = false;
	}
	//cameras[Kam_key][1].update();//Updates the helper based on the projectionMatrix of the camera.
	renderer.render(scene, cameras[cur_Kam][0]);
}
//=====================================================================
function TEST(DIV, COLOR) {
	init(DIV);
	animate();
}
//#####################################################################
TEST('M3D', 0xffff00)
//TEST('MAP',0xff0000)

///////////////////////////////////////////////////////////////////////
