///////////////////////////////////////////////////////////////////////
//  https://medium.com/codex/global-variables-and-javascript-modules-ce674a869164
window.ONE = 1;
self.TWO = 2;
globalThis.THREE = 3;


globalThis.webglCTRL = { posScale: 0 } // M3D controls

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
globalThis.Kam_key = 'GND';
globalThis.Kam_flag = false;
globalThis.Kam_reset = false;
/*
//  OK  ===============================================
import * as THREE from 'https://cdn.skypack.dev/three'
import threejsOrbitControls from 'https://cdn.skypack.dev/threejs-orbit-controls';
*/
//  OK  ===============================================
import * as THREE from './THREE/three.js/build/three.module.js'
import * as CameraUtils from './THREE/three.js/examples/jsm/utils/CameraUtils.js';
import { OrbitControls } from './THREE/three.js/examples/jsm/controls/OrbitControls.js';
//import { MathUtils } from     './THREE/three.js/src/math/MathUtils.js';
let DEBUG = false;

const _Xmin_Color = 0xff7777;
const _Xmax_Color = 0xff0000;
const _Ymin_Color = 0x77ff77;
const _Ymax_Color = 0x00ff00;
const _Zmin_Color = 0x7777ff;
const _Zmax_Color = 0x0000ff;
let aspectRatio,  scene, renderer, cameraControls, _Ground, _Rover;
let local_Matrix, world_Matrix;
let traject_Three = {};
let cameras = new Array();
/*
	var worldMatrix=new THREE.Matrix4(_Rover._world.matrixWorld);

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
//=====================================================================
/*
	function Camera(NAME,X,Y,Z,A,B,C,COLOR)
*/
var w = 0;
var h = 0;

function Camera(NAME, X, Y, Z, A, B, C, COLOR) {
	var Elts = new Array();

	const camera = new THREE.PerspectiveCamera(45, aspectRatio, 1, 500);
	//const camera = new THREE.OrthographicCamera( -w/2, w/2, h/2,-h/2,-1000,1000 );
	camera.position.set(X, Y, Z);
	camera.rotateX(THREE.MathUtils.degToRad(A));
	camera.rotateY(THREE.MathUtils.degToRad(B));
	camera.rotateZ(THREE.MathUtils.degToRad(C));
	Elts.push(camera)

	const camera_Helper = new THREE.CameraHelper(camera);
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
		this._world = new _sphera(RADIUS, COLOR)
		this._world.position.set(X, Y, Z);
		this._world.rotation.set(THREE.MathUtils.degToRad(A), THREE.MathUtils.degToRad(B), THREE.MathUtils.degToRad(C));
		if (typeof PARENT._world === 'undefined')
			PARENT.add(this._world)
		else
			PARENT._world.add(this._world)
	}
	insert(mesh3D) {
		if (Array.isArray(mesh3D))
			for (var i = 0; i < mesh3D.length; i++)
				this.insert(mesh3D[i]);
		else
			this._world.add(mesh3D)
	}
}
//=====================================================================
function TEST(DIV, COLOR) {
	init();
	animate();
	//-----------------------------------------------------------------
	function init() {
local_Matrix=new THREE.Matrix4();
world_Matrix=new THREE.Matrix4();

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

		// scene
		scene = new THREE.Scene();
		scene.background = new THREE.Color(0x888888);
		//scene.add( new THREE.GridHelper( 400, 10 ) );

		// GROUND WORLD
		_Ground = new World(scene, 'GROUND', 0, 0, 0, 0, 0, 0, 0, 4, 0x777777)
		let vct = new THREE.Vector3(1, 1, 1);
		// Isometric view
		_Ground._world.rotateOnAxis(vct.normalize(), THREE.MathUtils.degToRad(240))

		// ROVER WORLD
		_Rover = new World(_Ground, 'ROVER', 0, 0, 0, 0, 0, 0, 0, 5, 0x000000)

		// cameras
		cameras['GND'] = Camera('gnd', 150, 150, 150, 0, 0, 0);

		//This runs !!!
		scene.add(cameras['GND'][0]);
		cameras['GND'][1].update();
		scene.add(cameras['GND'][1]);

		/* 
		//This doesn't run !!!
		_Ground.insert(cameras['GND'][0]);
		cameras['GND'][1].update();
		_Ground.insert(cameras['GND'][1]);
		*/

		cameras['ROV'] = Camera('rov', 0, 0, 100, 180, 0, 0);
		//_Rover.insert(cameras['ROV']);
//		_Rover._world.add(cameras['ROV'][0]);
		scene.add(cameras['ROV'][0]);
		cameras['ROV'][1].update();
//		_Rover._world.add(cameras['ROV'][1]);
		scene.add(cameras['ROV'][1]);
		/* 
		//This doesn't run !!!
		scene.add(cameras['ROV'][0]);
		cameras['ROV'][1].update();
		scene.add(cameras['ROV'][1]);
*/
		// controls
		cameraControls = new OrbitControls(cameras[Kam_key][0], renderer.domElement);
		/*
				cameraControls.target.set(0, 40, 0);
				cameraControls.maxDistance = 400;
				cameraControls.minDistance = 10;
				cameraControls.update();
		*/
		// GROUND World objects
		traject_Three = new _trajectory('Taject', 0xff00ff)
		_Ground.insert([
			new Frame('gndFrame', 100, 0xff0000, 0x00ff00, 0x0000ff),
			traject_Three
		]);


		// ROVER World objects
		_Rover.insert([
			new Frame('rovFrame', 100, 0xff00ff, 0xffff00, 0x00ffff)
			//new Box('roverWalls', +10, +20, +30, +90, +100, +110)
		]);



		// lights
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
	//-----------------------------------------------------------------
	/**
	 * @fn function animate()
	 * @brief Animates the scene
	 * 
	 * 
	 */
	const old_gndCtrl = new OrbitControls(cameras['GND'][0], renderer.domElement);
	const old_rovCtrl = new OrbitControls(cameras['ROV'][0], renderer.domElement);
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
		// cameras switch
		if (Kam_flag) {
			if (Kam_reset) {
				Kam_reset = false;
			} else {
				/*
				*/
				if (Kam_key === 'GND') {
//					old_rovCtrl.saveState ();//save old ROV...
					cameras[Kam_key][0].lookAt(_Rover._world);
					cameras[Kam_key][1].update();
					old_gndCtrl.reset ();//restore old GND...
				cameraControls = old_gndCtrl;
				cameraControls.update();

				}
				if (Kam_key === 'ROV') {
					old_gndCtrl.saveState ();//save old GND...
					//        cameras[Kam_key][0].position.copy( _Rover._world.position );
					//       cameras[Kam_key][0].quaternion.copy( _Rover._world.quaternion );
					cameras['ROV'][0].lookAt(_Ground._world);
					cameras['ROV'][1].update();
					old_rovCtrl.reset ();//restore old ROV...
//				cameraControls = old_rovCtrl;
				cameraControls.update();
				}
			}
			Kam_flag = false;
		}
		// render the main scene
		if (traject_Flag) {
			

			/**********************************************************
			 * UPDATE ROVER WORLD
			 */

			//_Rover._world.rotateZ(3.14/180);
			/*
			_Rover._world.rotateZ(+ATTITUDE[3*traject_Count-3]/180);
			_Rover._world.rotateY(+ATTITUDE[3*traject_Count-2]/180);
			_Rover._world.rotateX(-ATTITUDE[3*traject_Count-1]/180);
			*/
			_Rover._world.setRotationFromQuaternion(
				new THREE.Quaternion(
					QUAT[4 * traject_Count - 4],
					QUAT[4 * traject_Count - 3],
					QUAT[4 * traject_Count - 2],
					QUAT[4 * traject_Count - 1]
				)
			);
			_Rover._world.position.set(
				webglCTRL.posScale * POSITION[3 * traject_Count - 3],
				webglCTRL.posScale * POSITION[3 * traject_Count - 2],
				webglCTRL.posScale * POSITION[3 * traject_Count - 1]
			);
			_Rover._world.updateMatrixWorld( true );// force updating
			


			/**********************************************************
			 * SET THE DRAW RANGE
			 */
			_Rover._world.geometry.attributes.position.needsUpdate = true;
			traject_Three.geometry.setDrawRange(0, traject_Count + 100);
			traject_Three.geometry.attributes.position.needsUpdate = true;



			/**********************************************************
			 * SET THE ROV CAMERA
			 */
//			if(Kam_key==='ROV'){
			if(false){
				/*	
					world_Matrix.copy(_Rover._world.worldMatrix);
					cameras['ROV'][0].worldMatrix.copy(world_Matrix);
				*/	
				const cameraOffset = new THREE.Vector3(150, 150, 150);


				var position = new THREE.Vector3();
				var quaternion = new THREE.Quaternion();
				var scale = new THREE.Vector3();
				_Rover._world.matrixWorld.decompose( position, quaternion, scale );

				cameras['ROV'][0].quaternion.copy( quaternion );



				const objectPosition = new THREE.Vector3();
				_Rover._world.getWorldPosition(objectPosition);
				cameras['ROV'][0].position.copy(objectPosition).add(cameraOffset);
				//_Rover._world.getWorldQuaternion(cameras['ROV'][0].quaternion);
				/*
				local_Matrix.copy(_Rover._world.matrix);
				cameras['ROV'][0].matrix.copy(local_Matrix);
						cameras['ROV'][0].setRotationFromQuaternion(
							new THREE.Quaternion(
								QUAT[4 * traject_Count - 4],
								QUAT[4 * traject_Count - 3],
								QUAT[4 * traject_Count - 2],
								QUAT[4 * traject_Count - 1]
							)
						);
						cameras['ROV'][0].position.set(
							webglCTRL.posScale * POSITION[3 * traject_Count - 3],
							webglCTRL.posScale * POSITION[3 * traject_Count - 2],
							webglCTRL.posScale * POSITION[3 * traject_Count - 1]
						);
			*/
			/*
				cameras['ROV'][0].update();
				word_Matrix.copy(_Rover._world.worldMatrix);
				cameras['ROV'][0].worldMatrix= new THREE.Matrix4();
				cameras['ROV'][0].applyMatrix4(word_Matrix);
				world_Matrix.copy(_Rover._world.matrixWorld);
				cameras['ROV'][0].updateMatrix();
			*/
				cameras['ROV'][1].update();
				cameraControls.update();
			}


			/**********************************************************
			 * END OF NEW TRAJECTORY POINT
			 */
			traject_Flag = false;
		}
		//cameras[Kam_key][1].update();//Updates the helper based on the projectionMatrix of the camera.

		renderer.render(scene, cameras[Kam_key][0]);

	}
}
//#####################################################################
TEST('M3D', 0xffff00)
//TEST('MAP',0xff0000)

///////////////////////////////////////////////////////////////////////
