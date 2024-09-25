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
      }
      resolve(new Engine(canvas, true));
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
      var scene = new Scene(engine);

      var camera: ArcRotateCamera = new ArcRotateCamera(
        'Camera',
        Math.PI / 2,
        Math.PI / 2,
        2,
        Vector3.Zero(),
        scene
      );
      camera.attachControl(canvas, true);
      var light1: HemisphericLight = new HemisphericLight(
        'light1',
        new Vector3(1, 1, 0),
        scene
      );
      var sphere: Mesh = MeshBuilder.CreateSphere(
        'sphere',
        { diameter: 1 },
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

    // initialize babylon scene and engine
    var engine = new WebGPUEngine(canvas);
    engine.initAsync().then(() => {});
  }
}
new App();
