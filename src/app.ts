import "@babylonjs/core/Debug/debugLayer";
import "@babylonjs/inspector";
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
} from "@babylonjs/core";
import {
  StackPanel,
  AdvancedDynamicTexture,
  Button,
  Control,
} from "@babylonjs/gui";

/**
 * Increases height of .glb model so that the bottom touches the ground.
 * @param transformMesh First half of a .glb mesh that can have transformations applied to it to move the mesh in the global scene
 * @param geometryMesh Latter half of a .glb mesh that contains geometric information on the mesh
 */
const fixGlbModelToGround = (
  transformMesh: AbstractMesh,
  geometryMesh: AbstractMesh,
) => {
  const heightAdjustment =
    0 - geometryMesh.getBoundingInfo().boundingBox.minimumWorld.y;
  transformMesh.position.y = heightAdjustment;
};

export class App {
  private readonly canvas: HTMLCanvasElement;
  private engine?: Engine | WebGPUEngine;

  private readonly formContainer: HTMLElement;

  constructor(_formContainer: HTMLElement) {
    this.formContainer = _formContainer;

    this.canvas = this.createCanvas();

    this.loadEngine().then((engine) => {
      this.engine = engine;

      const scene = this.createScene();

      SceneLoader.ImportMesh(
        "",
        "https://raw.githubusercontent.com/lfcourtney/Witness3DWebSimulationViewerModels/main/WitnessGlbModels/",
        "dg-ic-Machine.glb",
        scene,
        function (newMeshes) {
          const transformMachine = newMeshes[0];
          const geometryMachine = newMeshes[1];

          fixGlbModelToGround(transformMachine, geometryMachine);
        },
      );

      // eslint-disable-next-line @typescript-eslint/no-unused-vars
      const ground = MeshBuilder.CreateGround(
        "ground",
        { width: 50, height: 50 },
        scene,
      );

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

  private createCanvas(): HTMLCanvasElement {
    // create the canvas html element and attach it to the webpage
    const canvas = document.createElement("canvas");
    canvas.style.width = "100%";
    canvas.style.height = "100%";
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
      10,
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
      this.formContainer.style.display = "";
      document.body.style.cursor = "";
    });

    this.canvas.style.cursor = "pointer";

    button.onPointerEnterObservable.add(() => {
      document.body.style.cursor = "pointer";
    });
    button.onPointerOutObservable.add(() => {
      document.body.style.cursor = "";
    });

    panel.addControl(button);
  }
}
