
// http://jsfiddle.net/Stallion33/sgcfu4tt/

// Variables
var cameraDistance = 500;
var cameraFOV = 45;
var lightD = 20;
var sphereRadius = 100;

//var SelectedCameraString = "camera" ;
var camera1, camera2, RenderCamera;
var controls, controls1, controls2;

// Scene
var scene = new THREE.Scene();

// Renderer
var renderer = new THREE.WebGLRenderer();
renderer.setClearColor(0x008888); //scene.fog.color ); 
renderer.setPixelRatio(window.devicePixelRatio);
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.sortObjects = false;

document.getElementById('container').appendChild(renderer.domElement);


scene.add(new THREE.AmbientLight(0xffffff));

///var light1 = new THREE.PointLight(0xffffff);
//light1.position.set(-5400, 1400, -1400);

//... camera1

camera1Fov = 75;
camera1Far = 1200;
camera1 = new THREE.PerspectiveCamera(camera1Fov, window.innerWidth / window.innerHeight, 1, camera1Far);
//camera1.position.z = camera1Far;
camera1.position.set(0, 0, 0);

/*
	// Near & Far Plane dimensions
	c1_hNear = 2 * Math.tan(camera1.fov * Math.PI / 180 / 2) * camera1.near; // CHANGED - deg to radians
	c1_wNear = c1_hNear * camera1.aspect; // width

	c1_hFar = 2 * Math.tan(camera1.fov * Math.PI / 180 / 2) * camera1.far; // CHANGED - deg to radians
	c1_wFar = c1_hFar * camera1.aspect; // width

	//... Frustrum Vertex Points
	//... far
	var camera1_farTopLeft 		= new THREE.Vector3(-c1_wFar / 2,  c1_hFar / 2, -camera1.far); // CHANGED - sign flips
	var camera1_farBottomRight 	= new THREE.Vector3( c1_wFar / 2, -c1_hFar / 2, -camera1.far);
	var camera1_farTopRight 	= new THREE.Vector3( c1_wFar / 2,  c1_hFar / 2, -camera1.far);
	var camera1_farBottomLeft 	= new THREE.Vector3(-c1_wFar / 2, -c1_hFar / 2, -camera1.far);
	//... near
	var camera1_nearTopLeft 	= new THREE.Vector3(-c1_wNear / 2,  c1_hNear / 2, -camera1.near); // CHANGED - sign flips
	var camera1_nearBottomRight = new THREE.Vector3( c1_wNear / 2, -c1_hNear / 2, -camera1.near);
	var camera1_nearTopRight 	= new THREE.Vector3( c1_wNear / 2,  c1_hNear / 2, -camera1.near);
	var camera1_nearBottomLeft 	work = new THREE.Vector3(-c1_wNear / 2, -c1_hNear / 2, -camera1.near);
	
	//... SW store vertices in array 
	
	Frustrum_Vertices = [];
	
	Frustrum_Vertices.push (camera1_farTopLeft);
	Frustrum_Vertices.push (camera1_farBottomRight);
	Frustrum_Vertices.push (camera1_farTopRight);
	Frustrum_Vertices.push (camera1_farBottomLeft);
	
	Frustrum_Vertices.push (camera1_nearTopLeft);
	Frustrum_Vertices.push (camera1_nearBottomRight);
	Frustrum_Vertices.push (camera1_nearTopRight);
	Frustrum_Vertices.push (camera1_nearBottomLeft);
	
	//... Frustrum_Marker_Spheres
	
	Frustrum_Marker_Spheres = [];
	
	var FV = Frustrum_Vertices;
	for (iii=0;iii<Frustrum_Vertices.length; iii++)
	{
var sphereRadius = 60;
		var FVsphereMaterial = new THREE.MeshPhongMaterial( { color: 0xffff00, side: THREE.DoubleSide  } );
		
		if(iii>3)
		{
			sphereRadius = 0.05;//90;
			var FVsphereMaterial = new THREE.MeshPhongMaterial( { color: 0x0000ff, side: THREE.DoubleSide } );				
		}
		var sphereGeo  = new THREE.SphereGeometry( sphereRadius, 8, 8);	
		var sphereMesh = new THREE.Mesh( sphereGeo, FVsphereMaterial);
		Frustrum_Marker_Spheres.push ( sphereMesh );			
		sphereMesh.position.set( FV[iii].x, FV[iii].y, FV[iii].z );	

	camera1.add( sphereMesh );				
	}
	*/

scene.add(camera1);
camera1.position.set(0, 0, 1);
camera1.name = "Camera_1";
RenderCamera = camera1;

var sGeo = new THREE.SphereGeometry(40, 8, 8);
var sMaterial = new THREE.MeshPhongMaterial({
  color: 0xff00ff
});
Cam1Target = new THREE.Mesh(sGeo, sMaterial);
Cam1Target.position.set(0, 0, -200);
scene.add(Cam1Target);

controls1 = new THREE.OrbitControls(camera1, renderer.domElement);
controls = controls1; //... initially.
camera1.lookAt(Cam1Target);

camera1_Helper = new THREE.CameraHelper(camera1);
camera1_Helper.update();
scene.add(camera1_Helper);

// camera2
camera2 = new THREE.PerspectiveCamera(cameraFOV, window.innerWidth / window.innerHeight, 1, 20000);
var c2PosX = 5500;
var c2PosY = 3500;
var c2PosZ = -10000;

camera2.position.set(c2PosX, c2PosY, c2PosZ);
scene.add(camera2);
camera2.name = "Camera_2";


//controls2 = new THREE.OrbitControls(camera2, renderer.domElement);
//camera2.lookAt(camera1);

/*
// Material
  var material = new THREE.MeshPhongMaterial({ 
       color: 0xff0000, 
       opacity: 1,
       visible: true,
       side: THREE.DoubleSide
  });

// Smaterial
  var Smaterial = new THREE.MeshLambertMaterial({ 
       color: 0xffff00, 
       opacity: 0.5,
       visible: true,
       transparent: true,
       side: THREE.DoubleSide
  });

	//... Fbox  
	//approximation to frustrum geometry
	  var FboxGeo      = new THREE.BoxGeometry( 200,100,500);
	  var FboxMesh     = new THREE.Mesh(FboxGeo, Smaterial);
	  FboxMesh.position.set(0, 0, -125 );
	  //camera1.add(FboxMesh);
 

//====================================================

function srnd(rng){return (Math.random()*rng*2)-rng;}

//====================================================

//... make balls
  var sphereRadius = 20;
  for(var i=0;i<100;i++)
  {
      // Ball
      var ballGeo      = new THREE.SphereGeometry( sphereRadius, sphereRadius, sphereRadius);
      var ballMaterial = new THREE.MeshLambertMaterial( { color: 0x00ff00 } );
      var ballMesh     = new THREE.Mesh(ballGeo, ballMaterial);
      ballMesh.position.set(swork rnd(500),srnd(500), -camera1Far + srnd(500));
      scene.add(ballMesh);
  }
*/
//var tick=0;
var raf;

//var frustum = new THREE.Frustum();

F_frame();

//... END OF MAIN
//====================================================


//----------------------------------------------------------------
function F_frame() {
  //... Render
  raf = requestAnimationFrame(F_frame);

  controls.update(); //...BAD in jsfiddle

  renderer.render(scene, RenderCamera);

  //tick+=0.001;

} //... EOF Frame().

//-------------------------------------------------------------
function F_Switch_Camera() {
  var SelectedCameraString = document.getElementById('myTextField').value;
  //...toggle
  //alert("Hello");
  if (SelectedCameraString == "camera1") {
    SelectedCameraString = "camera2";
    RenderCamera = camera2;
    //controls = new THREE.OrbitControls(camera2, container);//renderer.domElement);
    controls = new THREE.OrbitControls(camera2, renderer.domElement);
    //controls.object = camera2;
    //controls.update();
    //controls = controls2;
  } else {
    SelectedCameraString = "camera1";
    RenderCamera = camera1;
    //controls = new THREE.OrbitControls(camera1, container);//renderer.domElement);
    controls = new THREE.OrbitControls(camera1, renderer.domElement);
    //controls.object = camera1;
    //controls = controls1;		
    //controls.update();
  }
  document.getElementById('myTextField').value = SelectedCameraString;
}

//----------------------------------------------------------------------
function F_Reset_Camera1() {
  camera1.position.set(0, 0, 1);
  camera1.lookAt(Cam1Target);
}
//----------------------------------------------------------------------
function F_Reset_Camera2() {
  camera2.position.set(c2PosX, c2PosY, c2PosZ);
  camera2.lookAt(camera1);
}
