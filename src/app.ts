import '@babylonjs/core/Debug/debugLayer';
import '@babylonjs/inspector';
import {
  WebGPUEngine,
  Engine,
  Scene,
  ArcRotateCamera,
  Vector3,
  HemisphericLight,
  MeshBuilder,
  Tools,
  SceneLoader,
  AbstractMesh,
} from '@babylonjs/core';
import { blueMat } from './materials/colors';

const positionImportedMesh = (
  importedMesh: AbstractMesh,
  scaleFactor: number
) => {
  importedMesh.scaling = new Vector3(scaleFactor, scaleFactor, scaleFactor);
  const heightAdjustment =
    0 - importedMesh.getBoundingInfo().boundingBox.minimumWorld.y;
  importedMesh.position.y = heightAdjustment * scaleFactor;
};

class App {
  readonly canvas: HTMLCanvasElement;
  private engine: Engine | WebGPUEngine;

  constructor() {
    this.canvas = this.createCanvas();

    this.loadEngine().then((engine) => {
      this.engine = engine;

      const scene = this.createScene();

      SceneLoader.ImportMesh(
        '',
        'https://raw.githubusercontent.com/lfcourtney/Witness3DWebSimulationViewerModels/main/VRNATIVE/example_obj/',
        'teapot.obj',
        scene,
        function (newMeshes) {
          const teapot = newMeshes[0];
          positionImportedMesh(teapot, 0.5);
          teapot.material = blueMat();
        }
      );

      // var sphere: Mesh = MeshBuilder.CreateSphere(
      //   'sphere',
      //   { diameter: 2 },
      //   scene
      // );

      // More sphere up one half of its height so it stays on the ground
      // sphere.position.y = 1;

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

  private createCanvas(): HTMLCanvasElement {
    // create the canvas html element and attach it to the webpage
    var canvas = document.createElement('canvas');
    canvas.style.width = '100%';
    canvas.style.height = '100%';
    canvas.id = 'gameCanvas';
    document.body.appendChild(canvas);

    return canvas;
  }

  private loadEngine(): Promise<Engine | WebGPUEngine> {
    const returnEngine = new Promise<Engine | WebGPUEngine>(
      async (resolve, reject) => {
        try {
          if (navigator.gpu) {
            var engine = new WebGPUEngine(this.canvas);
            await engine.initAsync();
            resolve(engine);
          } else {
            resolve(new Engine(this.canvas, true));
          }
        } catch (exception) {
          reject(exception);
        }
      }
    );
    return returnEngine;
  }

  private addWindowResizeEventListener() {
    // Watch for browser/canvas resize events
    const engine = this.engine;
    const canvas = this.canvas;
    window.addEventListener('resize', function () {
      engine.resize();
      canvas.style.width = '100%';
      canvas.style.height = '100%';
    });
  }

  private createScene(): Scene {
    // This creates a basic Babylon Scene object (non-mesh)
    var scene = new Scene(this.engine);

    var camera = new ArcRotateCamera(
      'camera',
      Tools.ToRadians(90),
      Tools.ToRadians(65),
      10,
      Vector3.Zero(),
      scene
    );

    camera.upperRadiusLimit = 40;
    camera.lowerRadiusLimit = 5;
    camera.upperBetaLimit = Tools.ToRadians(85);

    // This attaches the camera to the canvas
    camera.attachControl(this.canvas, true);

    var light1: HemisphericLight = new HemisphericLight(
      'light1',
      new Vector3(0, 1, 0),
      scene
    );

    // Default intensity is 1. Let's dim the light a small amount
    light1.intensity = 0.7;

    return scene;
  }
}
new App();
