import "@babylonjs/core/Debug/debugLayer";
import "@babylonjs/inspector";
import {
  WebGPUEngine,
  Engine,
  Scene,
  ArcRotateCamera,
  Vector3,
  HemisphericLight,
  Tools,
  Camera,
  AbstractMesh,
} from "@babylonjs/core";
import {
  StackPanel,
  AdvancedDynamicTexture,
  Button,
  Control,
} from "@babylonjs/gui";
import { SimulationContents } from "./simulationContent/simulationContent";
import { MeshGeometry } from "./meshGeometry/meshGeometry";
import { SimulationContentFormat } from "./simulationContent/simulationContentFormat";

/**
 * Class responsible for creating Babylon.js engine and rendering 3d scene (environment) to the browser
 */
export class App {
  private readonly canvas: HTMLCanvasElement;
  private engine?: Engine | WebGPUEngine;

  private simulationContents: SimulationContents;

  private readonly formContainer: HTMLElement;

  public readonly geometriesMap = new Map<string, MeshGeometry>();

  /**
   * Create class that renders 3d environment to the browser
   * @param _formContainer HTML element that contains form to upload .w3d file
   * @param _simulationContents Simulation information from uploaded .w3d file
   */
  constructor(
    _formContainer: HTMLElement,
    _simulationContents: SimulationContents,
  ) {
    this.formContainer = _formContainer;
    this.simulationContents = _simulationContents;

    this.canvas = this.createCanvas();

    this.loadEngine().then(async (engine) => {
      this.engine = engine;

      const scene = this.createScene();

      // Witness uses the right hand rule to describe the 3d coordinate axis
      scene.useRightHandedSystem = true;

      /**************************************************
       *             MAIN RENDER LOOP OF APPLICATION        *
       **************************************************/
      for (const tagObj of this.simulationContents.tagStore) {
        const simulationContentFormat: SimulationContentFormat =
          new SimulationContentFormat(
            this.simulationContents.formatTag(tagObj),
            { scene: scene, geometriesMap: this.geometriesMap },
          );

        await simulationContentFormat.actOnTagLogic();
      }

      if (scene.cameras.length > 0) {
        this.positionCameraToFloor(scene.cameras[0], scene);
      }

      this.createBackButton();

      // hide/show the Inspector
      window.addEventListener("keydown", (ev) => {
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

      this.addWindowResizeEventListener();
    });
  }

  private positionCameraToFloor(camera: Camera, scene: Scene) {
    // First floor mesh in the scene. Or undefined if there are no floor meshes
    let floorMesh: AbstractMesh | undefined;

    scene.meshes.forEach((mesh) => {
      if (floorMesh) return;
      const floorRegex = /^Floor [0-9]+$/;
      if (floorRegex.test(mesh.name)) {
        floorMesh = mesh;
      }
    });

    if (camera instanceof ArcRotateCamera && floorMesh !== undefined) {
      camera.setTarget(floorMesh);
    }
  }

  /**
   * Create HTML <canvas> element that will be the target for rendering the 3d scene
   * @returns HTML <canvas> element that will be the target for rendering the 3d scene
   */
  private createCanvas(): HTMLCanvasElement {
    // create the canvas html element and attach it to the webpage
    const canvas = document.createElement("canvas");
    canvas.style.width = "100%";
    canvas.style.height = "100%";
    canvas.style.position = "absolute";
    canvas.style.left = "0";
    canvas.style.top = "0";
    canvas.id = "gameCanvas";
    document.body.appendChild(canvas);

    return canvas;
  }

  /**
   * Loads engine used to render Babylon js scene. Experimental WebGPU engine will be used if the client is using a compatible browser.
   * Otherwise, will fallback to using the standard WebGL 2.0 engine.
   * @returns The engine used to render the Babylon js scene
   */
  private loadEngine(): Promise<Engine | WebGPUEngine> {
    const returnEngine = new Promise<Engine | WebGPUEngine>(
      (resolve, reject) => {
        try {
          // If client is using a browser compatible with WebGPU
          if (navigator.gpu) {
            const engine = new WebGPUEngine(this.canvas);
            engine.initAsync().then(() => {
              resolve(engine);
            });
          } else {
            resolve(new Engine(this.canvas, true));
          }
        } catch (exception) {
          reject(exception);
        }
      },
    );
    return returnEngine;
  }

  /**
   * Respond to browser resize window even by resizing Babylon.js scene.
   * This ensures Babylon.js scene matches the dimensions it would conform to
   * upon initial render in the event that the browser window has been resized.
   */
  private addWindowResizeEventListener(): void {
    if (!this.engine) {
      throw new Error("Babylon.js engine was undefined");
    }

    // Watch for browser/canvas resize events
    const engine = this.engine;
    const canvas = this.canvas;
    window.addEventListener("resize", function () {
      engine.resize();
      canvas.style.width = "100%";
      canvas.style.height = "100%";
    });
  }

  /**
   * Create Babylon.js scene and setup camera and light for scene
   * @returns Babylon.js scene with camera and light
   */
  private createScene(): Scene {
    if (!this.engine) {
      throw new Error("Babylon.js engine was undefined");
    }

    // This creates a basic Babylon Scene object (non-mesh)
    const scene = new Scene(this.engine);

    const camera = new ArcRotateCamera(
      "camera",
      Tools.ToRadians(90),
      Tools.ToRadians(65),
      30,
      Vector3.Zero(),
      scene,
    );

    camera.upperRadiusLimit = 40;
    camera.lowerRadiusLimit = 5;
    camera.upperBetaLimit = Tools.ToRadians(83);

    // This attaches the camera to the canvas
    camera.attachControl(this.canvas, true);

    const light1: HemisphericLight = new HemisphericLight(
      "light1",
      new Vector3(0, 1, 0),
      scene,
    );

    // Default intensity is 1. Let's dim the light a small amount
    light1.intensity = 0.7;

    return scene;
  }

  /**
   * Render button responsible for taking the user back to the initial form, whereby the
   * user uploads a w3d file, and removes the currently rendered Babylon.js scene.
   */
  private createBackButton(): void {
    // GUI
    const adt = AdvancedDynamicTexture.CreateFullscreenUI("UI");

    const panel = new StackPanel();
    panel.width = "600px";
    panel.left = "210px";
    panel.top = "-25px";
    panel.horizontalAlignment = Control.HORIZONTAL_ALIGNMENT_RIGHT;
    panel.verticalAlignment = Control.VERTICAL_ALIGNMENT_BOTTOM;
    adt.addControl(panel);

    const button = Button.CreateSimpleButton("backButton", "New Simulation");
    button.width = 0.25;
    button.height = "40px";
    button.color = "white";
    button.background = "green";

    button.onPointerClickObservable.add(() => {
      this.canvas.remove();
      // Must set the display to 'flex' because, by default, the display is 'none'
      this.formContainer.style.display = "flex";
      document.body.style.cursor = "";
    });

    button.onPointerEnterObservable.add(() => {
      document.body.style.cursor = "pointer";
    });
    button.onPointerOutObservable.add(() => {
      document.body.style.cursor = "";
    });

    panel.addControl(button);
  }
}
