//=====================================================================
/*
    constructor(PARENT,NAME,X,Y,Z,A,B,C,LEN,RADIUS,COLOR)
*/
class World {
	constructor(PARENT,NAME,X,Y,Z,A,B,C,LEN,RADIUS,COLOR) {
    	this.add(new _sphera(RADIUS,COLOR))
    	this.position.set(X,Y,Z);
    	this.rotation.set(THREE.MathUtils.degToRad(A), THREE.MathUtils.degToRad(B), THREE.MathUtils.degToRad(C));
    	PARENT.add(this)
  	}
    insert(mesh3D){
        if(Array.isArray(mesh3D))
            for(var i=0;i<mesh3D.length,i++)
                this.insert(mesh3D[i])
        else
            this.add(array_mesh3D)
    }
};
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

		// cameras
		cameras['GND'] = new THREE.PerspectiveCamera(45, w / h, 1, 5000);
		//cameras = new THREE.OrthographicCamera( -w/2, w/2, h/2,-h/2,-1000,1000 );
		cameras['GND'].position.set(0, 75, 160);
		cameras['GND'].position.set(150, 150, 150);

		// controls
        cameraControls = new OrbitControls(cameras[Kam_key], renderer.domElement);
		cameraControls.target.set(0, 40, 0);
		cameraControls.maxDistance = 400;
		cameraControls.minDistance = 10;
		cameraControls.update();

		// GROUND WORLD
        _Ground = new World(scene,'GROUND',0,0,0,45,0,90,50,4,0x777777)

        // GROUND World objects
        traject_Three=new _trajectory('Taject', 0xff00ff)
        _Ground.insert([
            new Box(_Ground,'gndWalls', +10, +20, +30, +90, +100, +110),
            traject_Three
            ]);
        
        // ROVER WORLD
        _Rover = new World(_Ground,'ROVER',0,0,0,0,0,0,50,5,0x000000)
		
        // ROVER World objects
        _Rover.insert([
            new Box('roverWalls', +10, +20, +30, +90, +100, +110)
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
	function animate() {

		requestAnimationFrame(animate);


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

		// render the main scene
		if (traject_Flag) {
            
            _Rover.position.set(TRAJECTORY[3*traject_Count-3], TRAJECTORY[3*traject_Count-2], TRAJECTORY[3*traject_Count-1]);
            _Rover.geometry.attributes.position.needsUpdate = true;
			traject_Three.geometry.setDrawRange(0, traject_Count + 100);
			traject_Three.geometry.attributes.position.needsUpdate = true;
			traject_Flag = false;
		}
		renderer.render(scene, currentCamera);

	}
}
//#####################################################################
TEST('M3D', 0xffff00)
//TEST('MAP',0xff0000)

///////////////////////////////////////////////////////////////////////
