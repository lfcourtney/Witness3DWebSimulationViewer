import '@babylonjs/core/Debug/debugLayer';
import '@babylonjs/inspector';
import {
  WebGPUEngine,
  Engine,
  Scene,
  ArcRotateCamera,
  Vector3,
  HemisphericLight,
  Mesh,
  MeshBuilder,
  Tools,
} from '@babylonjs/core';

const loadEngine = (
  canvas: HTMLCanvasElement
): Promise<Engine | WebGPUEngine> => {
  const returnEngine = new Promise<Engine | WebGPUEngine>(
    async (resolve, reject) => {
      if (navigator.gpu) {
        var engine = new WebGPUEngine(canvas);
        await engine.initAsync();
        resolve(engine);
      } else {
        resolve(new Engine(canvas, true));
      }
    }
  );
  return returnEngine;
};

class App {
  constructor() {
    // create the canvas html element and attach it to the webpage
    var canvas = document.createElement('canvas');
    canvas.style.width = '100%';
    canvas.style.height = '100%';
    canvas.id = 'gameCanvas';
    document.body.appendChild(canvas);

    loadEngine(canvas).then((engine) => {
      // This creates a basic Babylon Scene object (non-mesh)
      var scene = new Scene(engine);

      var camera = new ArcRotateCamera(
        'camera',
        Tools.ToRadians(90),
        Tools.ToRadians(65),
        10,
        Vector3.Zero(),
        scene
      );

      // This attaches the camera to the canvas
      camera.attachControl(canvas, true);

      var light1: HemisphericLight = new HemisphericLight(
        'light1',
        new Vector3(0, 1, 0),
        scene
      );

      // Default intensity is 1. Let's dim the light a small amount
      light1.intensity = 0.7;

      var sphere: Mesh = MeshBuilder.CreateSphere(
        'sphere',
        { diameter: 2 },
        scene
      );

      // More sphere up one half of its height so it stays on the ground
      sphere.position.y = 1;

      var ground = MeshBuilder.CreateGround(
        'ground',
        { width: 6, height: 6 },
        scene
      );

      // hide/show the Inspector
      window.addEventListener('keydown', (ev) => {
        // Shift+Ctrl+Alt+I
        if (ev.shiftKey && ev.ctrlKey && ev.altKey && ev.keyCode === 73) {
          if (scene.debugLayer.isVisible()) {
            scene.debugLayer.hide();
          } else {
            scene.debugLayer.show();
          }
        }
      });

      // run the main render loop
      engine.runRenderLoop(() => {
        scene.render();
      });
    });
  }
}
new App();
