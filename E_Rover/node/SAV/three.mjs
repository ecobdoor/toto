///////////////////////////////////////////////////////////////////////
/*
import {
  BoxBufferGeometry,
  Color,
  Mesh,
  MeshBasicMaterial,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
} from "https://github.com/mrdoob/three.js/tree/dev/src/core";      
*/
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
/*
////////////// KO ////////////////////////
import OrbitControls from 'threejs-orbit-controls';
import threejsOrbitControls from 'https://cdn.skypack.dev/threejs-orbit-controls';
import  { OrbitControls } from './jsm/controls/OrbitControls.js';
*/
    let camera, scene, renderer, cameraControls,worldORG;
//=====================================================================
/*
    function world(RADIUS,COLOR)
*/
function world(RADIUS,COLOR){

    const material = new THREE.LineBasicMaterial( { color: COLOR } );
    const points = [];
    points.push( new THREE.Vector3( 0,0,0 ) );
    points.push( new THREE.Vector3( RADIUS,RADIUS,RADIUS ) );
    const geometry = new THREE.BufferGeometry().setFromPoints( points );
    return new THREE.Line( geometry, material );
    //WORLD.add( world );
}
//=====================================================================
/*
    function line(WORLD,XA,YA,ZA,XB,YB,ZB,COLOR)
*/
function line(WORLD,XA,YA,ZA,XB,YB,ZB,COLOR){
    //create a blue LineBasicMaterial
    const material = new THREE.LineBasicMaterial( { color: COLOR } );

    //After material we will need a geometry with some vertices:
    const points = [];
    points.push( new THREE.Vector3( XA,YA,ZA ) );
    points.push( new THREE.Vector3( XB,YB,ZB ) );
    const geometry = new THREE.BufferGeometry().setFromPoints( points );

    //Note that lines are drawn between each consecutive pair of vertices, but not between the first and last (the line is not closed.)
    const line = new THREE.Line( geometry, material );

    //All that's left is to add it to the scene and call render.
    WORLD.add( line );
}
//=====================================================================
/*
    function frame(XA,YA,ZA,LEN)
*/
function frame(WORLD,X,Y,Z,LEN){
    line(WORLD,X,Y,Z,X+LEN,Y,Z,0xff0000);
    line(WORLD,X,Y,Z,X,Y+LEN,Z,0x00ff00);
    line(WORLD,X,Y,Z,X,Y,Z+LEN,0x0000ff);
}
//=====================================================================
function TEST(DIV,COLOR){
    init();
    animate();
    //-----------------------------------------------------------------
    function init() {
        const container = document.getElementById( DIV );
        var w = container.offsetWidth;
        var h = container.offsetHeight;

        // renderer
        renderer = new THREE.WebGLRenderer( { antialias: true } );
        renderer.setPixelRatio( window.devicePixelRatio );
        renderer.setSize( w, h );
        container.appendChild( renderer.domElement );
        renderer.localClippingEnabled = true;

        // scene
        scene = new THREE.Scene();

        // camera
        camera = new THREE.PerspectiveCamera( 45, w / h, 1, 5000 );
        camera.position.set( 0, 75, 160 );
        camera.position.set( 150, 150, 150 );

        cameraControls = new OrbitControls( camera, renderer.domElement );
        cameraControls.target.set( 0, 40, 0 );
        cameraControls.maxDistance = 400;
        cameraControls.minDistance = 10;
        cameraControls.update();

        // Origin world
        worldORG=world(100,0x000000)
        worldORG.rotation.set(THREE.MathUtils.degToRad(0), THREE.MathUtils.degToRad(0),THREE.MathUtils.degToRad(90));
        scene.add( worldORG)
        frame(worldORG,0,0,0,50);

       // walls
        const planeGeo = new THREE.PlaneGeometry( 100.1, 100.1 );

        const plane_Xmax = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0xff0000 } ) );
        plane_Xmax.position.x = 50;
        plane_Xmax.position.y = 50;
        plane_Xmax.rotateY( - Math.PI / 2 );
        worldORG.add( plane_Xmax );

        const plane_Xmin = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0xff7777 } ) );
        plane_Xmin.position.x = - 50;
        plane_Xmin.position.y = 50;
        plane_Xmin.rotateY( Math.PI / 2 );
        worldORG.add( plane_Xmin );

        const plane_Ymax = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x00ff00 } ) );
        plane_Ymax.position.y = 100;
        plane_Ymax.rotateX( Math.PI / 2 );
        worldORG.add( plane_Ymax );

        const plane_Ymin = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x77ff77 } ) );
        plane_Ymin.rotateX( - Math.PI / 2 );
        worldORG.add( plane_Ymin );

        const plane_Zmax = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x0000ff } ) );
        plane_Zmax.position.z = 50;
        plane_Zmax.position.y = 50;
        plane_Zmax.rotateY( Math.PI );
        worldORG.add( plane_Zmax );

        const plane_Zmin = new THREE.Mesh( planeGeo, new THREE.MeshPhongMaterial( { color: 0x7777ff } ) );
        plane_Zmin.position.z = - 50;
        plane_Zmin.position.y = 50;
        //plane_Zmin.rotateY( Math.PI );
        worldORG.add( plane_Zmin );

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

        window.addEventListener( 'resize', onWindowResize );

    }
    //-----------------------------------------------------------------
    function onWindowResize() {

        camera.aspect = w / h;
        camera.updateProjectionMatrix();

        renderer.setSize( w, h );

    }
    //-----------------------------------------------------------------
    function animate() {

        requestAnimationFrame( animate );


        // save the original camera properties
        const currentRenderTarget = renderer.getRenderTarget();
        const currentXrEnabled = renderer.xr.enabled;
        const currentShadowAutoUpdate = renderer.shadowMap.autoUpdate;
        renderer.xr.enabled = false; // Avoid camera modification
        renderer.shadowMap.autoUpdate = false; // Avoid re-computing shadows


        // restore the original rendering properties
        renderer.xr.enabled = currentXrEnabled;
        renderer.shadowMap.autoUpdate = currentShadowAutoUpdate;
        renderer.setRenderTarget( currentRenderTarget );

        // render the main scene
        renderer.render( scene, camera );

    }
}
//#####################################################################
TEST('M3D',0xffff00)
//TEST('MAP',0xff0000)

///////////////////////////////////////////////////////////////////////
