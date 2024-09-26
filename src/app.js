var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import '@babylonjs/core/Debug/debugLayer';
import '@babylonjs/inspector';
import { WebGPUEngine, Engine, Scene, ArcRotateCamera, Vector3, HemisphericLight, MeshBuilder, Tools, SceneLoader, } from '@babylonjs/core';
import { blueMat } from './materials/colors';
const positionImportedMesh = (importedMesh, scaleFactor) => {
    importedMesh.scaling = new Vector3(scaleFactor, scaleFactor, scaleFactor);
    const heightAdjustment = 0 - importedMesh.getBoundingInfo().boundingBox.minimumWorld.y;
    importedMesh.position.y = heightAdjustment * scaleFactor;
};
class App {
    constructor() {
        Object.defineProperty(this, "canvas", {
            enumerable: true,
            configurable: true,
            writable: true,
            value: void 0
        });
        Object.defineProperty(this, "engine", {
            enumerable: true,
            configurable: true,
            writable: true,
            value: void 0
        });
        this.canvas = this.createCanvas();
        this.loadEngine().then((engine) => {
            this.engine = engine;
            const scene = this.createScene();
            SceneLoader.ImportMesh('', 'https://raw.githubusercontent.com/lfcourtney/Witness3DWebSimulationViewerModels/main/VRNATIVE/example_obj/', 'teapot.obj', scene, function (newMeshes) {
                const teapot = newMeshes[0];
                positionImportedMesh(teapot, 0.5);
                teapot.material = blueMat();
            });
            var ground = MeshBuilder.CreateGround('ground', { width: 6, height: 6 }, scene);
            window.addEventListener('keydown', (ev) => {
                if (ev.shiftKey && ev.ctrlKey && ev.altKey && ev.keyCode === 73) {
                    if (scene.debugLayer.isVisible()) {
                        scene.debugLayer.hide();
                    }
                    else {
                        scene.debugLayer.show();
                    }
                }
            });
            engine.runRenderLoop(() => {
                scene.render();
            });
        });
    }
    createCanvas() {
        var canvas = document.createElement('canvas');
        canvas.style.width = '100%';
        canvas.style.height = '100%';
        canvas.id = 'gameCanvas';
        document.body.appendChild(canvas);
        return canvas;
    }
    loadEngine() {
        const returnEngine = new Promise((resolve, reject) => __awaiter(this, void 0, void 0, function* () {
            try {
                if (navigator.gpu) {
                    var engine = new WebGPUEngine(this.canvas);
                    yield engine.initAsync();
                    resolve(engine);
                }
                else {
                    resolve(new Engine(this.canvas, true));
                }
            }
            catch (exception) {
                reject(exception);
            }
        }));
        return returnEngine;
    }
    addWindowResizeEventListener() {
        const engine = this.engine;
        const canvas = this.canvas;
        window.addEventListener('resize', function () {
            engine.resize();
            canvas.style.width = '100%';
            canvas.style.height = '100%';
        });
    }
    createScene() {
        var scene = new Scene(this.engine);
        var camera = new ArcRotateCamera('camera', Tools.ToRadians(90), Tools.ToRadians(65), 10, Vector3.Zero(), scene);
        camera.upperRadiusLimit = 40;
        camera.lowerRadiusLimit = 5;
        camera.upperBetaLimit = Tools.ToRadians(85);
        camera.attachControl(this.canvas, true);
        var light1 = new HemisphericLight('light1', new Vector3(0, 1, 0), scene);
        light1.intensity = 0.7;
        return scene;
    }
}
new App();
