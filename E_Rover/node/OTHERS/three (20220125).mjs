///////////////////////////////////////////////////////////////////////
//  https://medium.com/codex/global-variables-and-javascript-modules-ce674a869164
window.ONE = 1;
self.TWO = 2;
globalThis.THREE = 3;

globalThis.traject_Count = 0;
globalThis.traject_Flag = false;
globalThis.traject_Three = {};
globalThis.traject_MaxPts = 100000;
globalThis.traject_MaxLen = 3 * traject_MaxPts;
globalThis.TRAJECTORY = new Float32Array(traject_MaxLen);
for (var i = 0; i < traject_MaxLen; i += 3) {
	TRAJECTORY[i] = 50 * Math.cos(3.14 * i / 180);
	TRAJECTORY[i + 1] = 50 * Math.sin(3.14 * i / 180);
	TRAJECTORY[i + 2] = i / 1000;
}
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

const _Xmin_Color = 0xff7777;
const _Xmax_Color = 0xff0000;
const _Ymin_Color = 0x77ff77;
const _Ymax_Color = 0x00ff00;
const _Zmin_Color = 0x7777ff;
const _Zmax_Color = 0x0000ff;
let camera, scene, renderer, cameraControls, worldORG, rover;
//=====================================================================
/*
	function sphera(RADIUS,COLOR)
*/
function sphera(RADIUS, COLOR) {
	const geometry = new THREE.SphereGeometry(RADIUS, 32, 16);
	const material = new THREE.MeshBasicMaterial({ color: COLOR });
	return new THREE.Mesh(geometry, material);
	/*
		const material = new THREE.LineBasicMaterial( { color: COLOR } );
		const points = [];
		points.push( new THREE.Vector3( 0,0,0 ) );
		points.push( new THREE.Vector3( RADIUS,RADIUS,RADIUS ) );
		const geometry = new THREE.BufferGeometry().setFromPoints( points );
		THREE.Line( geometry, material );
		*/
	//WORLD.add( world );
}
//=====================================================================
/*
	function Trajectory(WORLD,COLOR)
*/
function Trajectory(WORLD, COLOR) {
	//create a blue LineBasicMaterial
	const material = new THREE.LineBasicMaterial({ color: COLOR });

	//After material we will need a geometry with some vertices:
	//    const geometry = new THREE.BufferGeometry().setFromPoints( TRAJECTORY );

	var geometry = new THREE.BufferGeometry();
	//var positions = new Float32Array( MAX_POINTS * 3 ); 
	geometry.addAttribute('position', new THREE.BufferAttribute(TRAJECTORY, 3));



	//Note that lines are drawn between each consecutive pair of vertices, but not between the first and last (the line is not closed.)
	traject_Three = new THREE.Line(geometry, material);
	//++++++++++++++++++++++++++traject_Three.geometry.setDrawRange( 20000, 30000 );  
	//All that's left is to add it to the scene and call render.
	WORLD.add(traject_Three);
}
//=====================================================================
/*
	function line(WORLD,XA,YA,ZA,XB,YB,ZB,COLOR)
*/
function line(WORLD, XA, YA, ZA, XB, YB, ZB, COLOR) {
	//create a blue LineBasicMaterial
	const material = new THREE.LineBasicMaterial({ color: COLOR });

	//After material we will need a geometry with some vertices:
	const points = [];
	points.push(new THREE.Vector3(XA, YA, ZA));
	points.push(new THREE.Vector3(XB, YB, ZB));
	const geometry = new THREE.BufferGeometry().setFromPoints(points);

	//Note that lines are drawn between each consecutive pair of vertices, but not between the first and last (the line is not closed.)
	const line = new THREE.Line(geometry, material);

	//All that's left is to add it to the scene and call render.
	WORLD.add(line);
}
//=====================================================================
/*
	function tickLine(WORLD,XA,YA,ZA,XB,YB,ZB,COLOR,RADIUS)
*/
function tickLine(WORLD, XA, YA, ZA, XB, YB, ZB, COLOR, RADIUS) {
	// Material
	const material = new THREE.LineBasicMaterial({ color: COLOR });

	let startVector = new THREE.Vector3(XA, YA, ZA);
	let endVector = new THREE.Vector3(XB, YB, ZB);
	//After material we will need a geometry with some vertices:
	const points = [];
	points.push(startVector, endVector);
	// Geometry
	var tubeGeometry = new THREE.TubeGeometry(
		new THREE.CatmullRomCurve3(points),
		512,// path segments
		RADIUS,// THICKNESS
		8, //Roundness of Tube (number of points for circle)
		false //closed
	);
	let line = new THREE.Line(tubeGeometry, material);

	WORLD.add(line);
}
//=====================================================================
/*
	function frame(XA,YA,ZA,LEN)
*/
function frame(WORLD, X, Y, Z, LEN) {
	line(WORLD, X, Y, Z, X + LEN, Y, Z, 0xff0000);
	line(WORLD, X, Y, Z, X, Y + LEN, Z, 0x00ff00);
	line(WORLD, X, Y, Z, X, Y, Z + LEN, 0x0000ff);
}
//=====================================================================
/*
	function wall(NAME,WORLD,PLANEGEO,DX,DY,DZ,A,B,C,COLOR)
*/
let DEBUG = false;
function wall(NAME, WORLD, Xlen, Ylen, DX, DY, DZ, A, B, C, COLOR, SIDE) {
	const planeGeo_XYo = new THREE.PlaneGeometry(0.1 + Xlen, 0.1 + Ylen, 5, 5);// centered rectangle (X,Y)

	if (DEBUG) {
		const material = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true, transparent: true });
		const mesh_planeGeo = new THREE.Mesh(planeGeo_XYo, material);
		WORLD.add(mesh_planeGeo);
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
	WORLD.add(mesh);
	if (DEBUG) {
		const plane = new THREE.Mesh(
			planeGeo_XYo,
			new THREE.MeshBasicMaterial({ color: COLOR, wireframe: true, transparent: true }));
		plane.name = "dbg_" + NAME;
		plane.position.x = DX;
		plane.position.y = DY;
		plane.position.z = DZ;
		WORLD.add(plane);
	}
}
//=====================================================================
/*
	function box(WORLD,Xmin,Ymin,Zmin,Xmax,Ymax,Zmax)
*/
function box(WORLD, Xmin, Ymin, Zmin, Xmax, Ymax, Zmax) {
	/*
		tickLine(WORLD,Xmin,0,0,Xmax,0,0,0xff0000,2);
		tickLine(WORLD,0,Ymin,0,0,Ymax,0,0x00ff00,2);
		tickLine(WORLD,0,0,Zmin,0,0,Zmax,0x0000ff,2);
	*/
	tickLine(WORLD, 0, 0, 0, Xmax, 0, 0, _Xmax_Color, 2);
	tickLine(WORLD, 0, 0, 0, 0, Ymax, 0, _Ymax_Color, 2);
	tickLine(WORLD, 0, 0, 0, 0, 0, Zmax, _Zmax_Color, 2);

	tickLine(WORLD, Xmin, 0, 0, 0, 0, 0, _Xmin_Color, 2);
	tickLine(WORLD, 0, Ymin, 0, 0, 0, 0, _Ymin_Color, 2);
	tickLine(WORLD, 0, 0, Zmin, 0, 0, 0, _Zmin_Color, 2);


	// walls         

	wall("Xmax", WORLD, 0.1 + Zmax - Zmin, 0.1 + Ymax - Ymin,
		Xmax,
		(Ymax + Ymin) / 2,
		(Zmax + Zmin) / 2,
		0,
		- Math.PI / 2,
		0,
		_Xmax_Color,
		THREE.FrontSide);
	wall("Xmin", WORLD, 0.1 + Zmax - Zmin, 0.1 + Ymax - Ymin,
		Xmin,
		(Ymax + Ymin) / 2,
		(Zmax + Zmin) / 2,
		0,
		+ Math.PI / 2,
		0,
		_Xmin_Color,
		THREE.FrontSide);
	/*
			const plane_Xmax = new THREE.Mesh( planeGeo_XYo, new THREE.MeshPhongMaterial( { color: 0xff0000 } ) );
			plane_Xmax.position.x = Xmax;
			plane_Xmax.position.y = (Ymax+Ymin)/2;
			plane_Xmax.position.z = (Zmax+Zmin)/2;
			plane_Xmax.rotateY( - Math.PI / 2 );// % PlaneGeometry center
			WORLD.add( plane_Xmax );
	
			const plane_Xmin = new THREE.Mesh( planeGeo_XYo, new THREE.MeshPhongMaterial( { color: 0xff7777 } ) );
			plane_Xmin.position.x = Xmin;
			plane_Xmin.position.y = (Ymax+Ymin)/2;
			plane_Xmin.position.z = (Zmax+Zmin)/2;
			plane_Xmin.rotateY( Math.PI / 2 );
			WORLD.add( plane_Xmin );
	*/
	wall("Ymax", WORLD, 0.1 + Xmax - Xmin, 0.1 + Zmax - Zmin,
		(Xmax + Xmin) / 2,
		Ymax,
		(Zmax + Zmin) / 2,
		- Math.PI / 2,
		0,
		0,
		_Ymax_Color,
		THREE.BackSide);
	wall("Ymin", WORLD, 0.1 + Xmax - Xmin, 0.1 + Zmax - Zmin,
		(Xmax + Xmin) / 2,
		Ymin,
		(Zmax + Zmin) / 2,
		- Math.PI / 2,
		0,
		0,
		_Ymin_Color,
		THREE.FrontSide);
	/*
	const plane_Ymax = new THREE.Mesh( planeGeo_XYo, new THREE.MeshPhongMaterial( { color: 0x00ff00 } ) );
	plane_Ymax.position.x = (Xmax+Xmin)/2;
	plane_Ymax.position.y = Ymax;
	plane_Ymax.position.z = Zmax;
	plane_Ymax.rotateX( Math.PI / 2 );
	WORLD.add( plane_Ymax );

	const plane_Ymin = new THREE.Mesh( planeGeo_XYo, new THREE.MeshPhongMaterial( { color: 0x77ff77 } ) );
	plane_Ymin.position.x = (Xmax+Xmin)/2;
	plane_Ymin.position.y = Ymin;
	plane_Ymin.position.z = Zmin;
	plane_Ymin.rotateX( - Math.PI / 2 );
	WORLD.add( plane_Ymin );
	*/
	//        DEBUG=true;
	wall("Zmax", WORLD, 0.1 + Xmax - Xmin, 0.1 + Ymax - Ymin,
		(Xmax + Xmin) / 2,
		(Ymax + Ymin) / 2,
		Zmax,
		0,
		Math.PI,
		0,
		_Zmax_Color,
		THREE.FrontSide);
	wall("Zmin", WORLD, 0.1 + Xmax - Xmin, 0.1 + Ymax - Ymin,
		(Xmax + Xmin) / 2,
		(Ymax + Ymin) / 2,
		Zmin,
		0,
		Math.PI,
		0,
		_Zmin_Color,
		THREE.BackSide);

	/*
			const plane_Zmax = new THREE.Mesh( planeGeo_XYo, new THREE.MeshPhongMaterial( { color: 0x0000ff } ) );
			plane_Zmax.position.z = 50;
			plane_Zmax.position.y = 50;
			plane_Zmax.rotateY( Math.PI );
			WORLD.add( plane_Zmax );
	
			const plane_Zmin = new THREE.Mesh( planeGeo_XYo, new THREE.MeshPhongMaterial( { color: 0x7777ff } ) );
			plane_Zmin.position.z = - 50;
			plane_Zmin.position.y = 50;
			//plane_Zmin.rotateY( Math.PI );
			WORLD.add( plane_Zmin );
	*/
	/*
		   // walls
	
			const planeGeo = new THREE.PlaneGeometry( 100.1, 100.1 );
	
			const plane_Xmax = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0xff0000 } ) );
			plane_Xmax.position.x = 50;
			plane_Xmax.position.y = 50;
			plane_Xmax.rotateY( - Math.PI / 2 );
			WORLD.add( plane_Xmax );
	
			const plane_Xmin = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0xff7777 } ) );
			plane_Xmin.position.x = - 50;
			plane_Xmin.position.y = 50;
			plane_Xmin.rotateY( Math.PI / 2 );
			WORLD.add( plane_Xmin );
	
			const plane_Ymax = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x00ff00 } ) );
			plane_Ymax.position.y = 100;
			plane_Ymax.rotateX( Math.PI / 2 );
			WORLD.add( plane_Ymax );
	
			const plane_Ymin = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x77ff77 } ) );
			plane_Ymin.rotateX( - Math.PI / 2 );
			WORLD.add( plane_Ymin );
	
			const plane_Zmax = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x0000ff } ) );
			plane_Zmax.position.z = 50;
			plane_Zmax.position.y = 50;
			plane_Zmax.rotateY( Math.PI );
			WORLD.add( plane_Zmax );
	
			const plane_Zmin = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x7777ff } ) );
			plane_Zmin.position.z = - 50;
			plane_Zmin.position.y = 50;
			//plane_Zmin.rotateY( Math.PI );
			WORLD.add( plane_Zmin );
	*/
}
//=====================================================================
function TEST(DIV, COLOR) {
	init();
	animate();
	//-----------------------------------------------------------------
	function init() {
		const container = document.getElementById(DIV);
		var w = container.offsetWidth;
		var h = container.offsetHeight;

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

		// camera
		camera = new THREE.PerspectiveCamera(45, w / h, 1, 5000);
		//camera = new THREE.OrthographicCamera( -w/2, w/2, h/2,-h/2,-1000,1000 );
		camera.position.set(0, 75, 160);
		camera.position.set(150, 150, 150);

		// controls
        cameraControls = new OrbitControls(camera, renderer.domElement);
		cameraControls.target.set(0, 40, 0);
		cameraControls.maxDistance = 400;
		cameraControls.minDistance = 10;
		cameraControls.update();

		// Origin world
        worldORG = sphera(4, 0x777777)
        worldORG.rotation.set(THREE.MathUtils.degToRad(45), THREE.MathUtils.degToRad(0), THREE.MathUtils.degToRad(90));
        scene.add(worldORG)

        // worldORG objects
        rover = sphera(8, 0x000000)
		worldORG.add(rover)
		//frame(worldORG,0,0,0,50);
		//        box(worldORG,-130,-50,-10,+80,+30,+100);
		box(worldORG, +10, +20, +30, +90, +100, +110);
		Trajectory(worldORG, 0xff00ff)

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
		worldORG.add( mainLight );

		const greenLight = new THREE.PointLight( 0x00ff00, 0.25, 1000 );
		greenLight.position.set( 550, 50, 0 );
		worldORG.add( greenLight );

		const redLight = new THREE.PointLight( 0xff0000, 0.25, 1000 );
		redLight.position.set( - 550, 50, 0 );
		worldORG.add( redLight );

		const blueLight = new THREE.PointLight( 0x7f7fff, 0.25, 1000 );
		blueLight.position.set( 0, 50, 550 );
		worldORG.add( blueLight );
*/


		//window.addEventListener( 'resize', onWindowResize );

	}
	//-----------------------------------------------------------------
	/*
	function onWindowResize() {

		camera.aspect = w / h;
		camera.updateProjectionMatrix();

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
	function animate() {

		requestAnimationFrame(animate);


		// save the original camera properties
		const currentRenderTarget = renderer.getRenderTarget();
		const currentXrEnabled = renderer.xr.enabled;
		const currentShadowAutoUpdate = renderer.shadowMap.autoUpdate;
		renderer.xr.enabled = false; // Avoid camera modification
		renderer.shadowMap.autoUpdate = false; // Avoid re-computing shadows


		// restore the original rendering properties
		renderer.xr.enabled = currentXrEnabled;
		renderer.shadowMap.autoUpdate = currentShadowAutoUpdate;
		renderer.setRenderTarget(currentRenderTarget);

		// render the main scene
		if (traject_Flag) {
            
            rover.position.set(TRAJECTORY[3*traject_Count-3], TRAJECTORY[3*traject_Count-2], TRAJECTORY[3*traject_Count-1]);
            rover.geometry.attributes.position.needsUpdate = true;
			traject_Three.geometry.setDrawRange(0, traject_Count + 100);
			traject_Three.geometry.attributes.position.needsUpdate = true;
			traject_Flag = false;
		}
		renderer.render(scene, camera);

	}
}
//#####################################################################
TEST('M3D', 0xffff00)
//TEST('MAP',0xff0000)

///////////////////////////////////////////////////////////////////////
