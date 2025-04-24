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
import { SimulationTag } from "./simulationTags/simulationTag";

/**
 * Class responsible for creating Babylon.js engine and rendering 3d scene (environment) to the browser
 */
export class App {
  private readonly canvas: HTMLCanvasElement;
  private engine?: Engine | WebGPUEngine;

  /**
   * Animation render loop. Should be cleared whenever canvas is removed.
   */
  private renderLoop?: NodeJS.Timeout;

  /**
   * Babylon.js GUI element. Should be updated on each frame of the simulation to reflect the current frame number.
   */
  private frameGUIButton?: Button;

  /**
   * Assigned on initialisation. Contains the tag information and the ability to format the tags from the uploaded w3d file.
   */
  private simulationContents: SimulationContents;

  private readonly formContainer: HTMLElement;

  /**
   * Map where each key is a name of a mesh that exists in the Babylon.js scene and where each value is that mesh
   */
  private readonly geometriesMap = new Map<string, MeshGeometry>();

  /**
   * Map where each key is a specific time and where each value is the parent tags from the uploaded simulation trace output file that correspond to that given time
   */
  private readonly timeTagStore = new Map<string, SimulationTag[]>();

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

    this.loadEngine().then(
      async (engine) => await this.runMainAppSetup(engine),
    );
  }

  /**
   * Finish the set up of Babylon.js scene.
   * Functions as the rest of the App constructor once the Babylon.js engine has now been initialised.
   * @param engine The Babylon.js engine responsible for rendering the 3d environment to the browser
   */
  private async runMainAppSetup(engine: Engine | WebGPUEngine): Promise<void> {
    this.engine = engine;

    const scene = this.createScene();

    // Witness uses the right hand rule to describe the 3d coordinate axis
    scene.useRightHandedSystem = true;

    /**************************************************
     *  PARSE APPLICATION LOGIC (<create>, <update> and <delete> tags)   *
     **************************************************/
    for (const tagObj of this.simulationContents.tagStore) {
      const simulationContentFormat: SimulationContentFormat =
        new SimulationContentFormat(this.simulationContents.formatTag(tagObj), {
          scene: scene,
          geometriesMap: this.geometriesMap,
          timeTagStore: this.timeTagStore,
        });

      // Only act at time zero
      await simulationContentFormat.actOnTagLogicAtTimeZero();
    }

    // Must create camera and, if floor exists, position camera to floor
    this.createCameraAndPositionToFloor(scene);

    this.renderSimulationGUI();

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

    this.runApplicationRenderLoop(scene);

    this.addWindowResizeEventListener();
  }

  /**
   * Create render loop to render Babylon.js scene continuously to browser
   * @param scene Babylon.js scene to render frames of
   */
  private runApplicationRenderLoop(scene: Scene): void {
    if (!this.engine) {
      throw new Error("Unable to create render loop: engine was undefined");
    }

    const engine = this.engine;

    const customFPS = 120; //120 or 250

    let frameNumber = 0;

    // run the main render loop
    this.renderLoop = setInterval(() => {
      // Parse frame number to string to resolve inconsistencies with floating point numbers
      const formattedFrameNumberString: string =
        this.formatFrameNumber(frameNumber);

      const timeTagStoreArray = this.timeTagStore.get(
        formattedFrameNumberString,
      );

      timeTagStoreArray?.forEach((tag) => {
        tag.actOnTagLogic();
      });

      /**
       * Note: WebGPU engine requires 'beginFrame' and 'endFrame' methods to be called
       * before rendering each frame
       */
      engine.beginFrame();
      scene.render();
      engine.endFrame();
      frameNumber += 0.01;

      this.updateGUIFrameNumber(formattedFrameNumberString);
    }, 1000 / customFPS);
  }

  /**
   * Round a number down to 2 decimal places, returning this number as a string
   * @param frameNumber The frame number to round down to 2 decimal places
   * @returns The number rounded down to 2 decimal places as a string
   */
  private formatFrameNumber(frameNumber: number): string {
    const roundFrameNumber = Math.floor(frameNumber * 100) / 100;
    return roundFrameNumber.toFixed(2);
  }

  /**
   * Create camera. If floor exists in scene, the target of camera will be set to the floor.
   * Otherwise, the target of the camera is the scene origin.
   * @param scene Scene to create camera for
   */
  private createCameraAndPositionToFloor(scene: Scene) {
    // First floor mesh in the scene. Or undefined if there are no floor meshes
    let floorMesh: AbstractMesh | undefined;

    scene.meshes.forEach((mesh) => {
      if (floorMesh) return;
      const floorRegex = /^Floor [0-9]+$/;
      if (floorRegex.test(mesh.name)) {
        floorMesh = mesh;
      }
    });

    // Floor if floor mesh exists. Scene origin otherwise.
    let cameraTarget: Vector3 = Vector3.Zero();

    if (floorMesh !== undefined) {
      cameraTarget = new Vector3(
        floorMesh.position.x,
        floorMesh.position.y,
        floorMesh.position.z,
      );
    }

    const camera = new ArcRotateCamera(
      "camera",
      Tools.ToRadians(90),
      Tools.ToRadians(65),
      30,
      cameraTarget,
      scene,
    );

    camera.upperRadiusLimit = 40;
    camera.lowerRadiusLimit = 5;
    camera.upperBetaLimit = Tools.ToRadians(83);

    // This attaches the camera to the canvas
    camera.attachControl(this.canvas, true);
  }

  /**
   * Create HTML \<canvas\> element that will be the target for rendering the 3d scene
   * @returns HTML \<canvas\> element that will be the target for rendering the 3d scene
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
        WebGPUEngine.IsSupportedAsync.then((isSupported) => {
          if (isSupported) {
            const engine = new WebGPUEngine(this.canvas);
            engine
              .initAsync()
              .then(() => {
                resolve(engine);
              })
              .catch((reason) => {
                reject(reason);
              });
          } else {
            //Instantiate regular WebGL 2.0 Babylon.js if the client is using a browser not compatible with WebGPU
            resolve(new Engine(this.canvas, true));
          }
        });
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
   * Render simulation GUI using Babylon.js GUI library: button responsible for taking the user back to the initial form,
   * removing the currently rendered Babylon.js scene and a frame counter element that will be updated to reflect the current
   * frame number of the simulation.
   */
  private renderSimulationGUI(): void {
    // GUI
    const adt = AdvancedDynamicTexture.CreateFullscreenUI("UI");

    const panel = new StackPanel();
    panel.isVertical = false;
    panel.height = "40px";
    panel.top = "-25px";
    panel.horizontalAlignment = Control.HORIZONTAL_ALIGNMENT_RIGHT;
    panel.verticalAlignment = Control.VERTICAL_ALIGNMENT_BOTTOM;
    adt.addControl(panel);

    const frameGUIButton = Button.CreateSimpleButton(
      "backButton",
      "Time: 0 mins",
    );

    frameGUIButton.width = "300px";
    frameGUIButton.color = "white";
    frameGUIButton.background = "green";
    frameGUIButton.paddingRight = "10px";

    this.frameGUIButton = frameGUIButton;

    panel.addControl(frameGUIButton);

    const button = Button.CreateSimpleButton("backButton", "New Simulation");
    button.width = "300px";
    button.color = "white";
    button.background = "green";
    button.paddingRight = "10px";

    panel.addControl(button);

    button.onPointerClickObservable.add(() => {
      this.clearCanvas();
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
  }

  /**
   * Updates the frame GUI button to reflect the current frame number of the simulation.
   * To reduce computation overhead, will only update on new full numbers of the frame.
   * @param currentFrameNumber The current frame number of the simulation
   */
  private updateGUIFrameNumber(currentFrameNumber: string): void {
    const splitFrameNumber = currentFrameNumber.split(".");

    if (splitFrameNumber.length !== 2) return;

    if (splitFrameNumber[1] !== "00") return;

    if (this.frameGUIButton?.textBlock) {
      this.frameGUIButton.textBlock.text =
        "Time: " + splitFrameNumber[0] + " mins";
    }
  }

  /**
   * Delete HTML \<canvas\> element, thus un-rendering the current Babylon.js 3D environment.
   */
  private clearCanvas(): void {
    clearInterval(this.renderLoop);
    this.canvas.remove();
  }
}
