import {
  AbstractMesh,
  SceneLoader,
  MeshBuilder,
  Vector3,
} from "@babylonjs/core";
import { MeshGeometry } from "../meshGeometry/meshGeometry";
import { SimulationTag, SimulationTagData } from "./simulationTag";
import { CreateTag } from "../interfaces/createTag";
import { MachineGeometry } from "../meshGeometry/machineGeometry";
import { ConveyorBuilder } from "../meshBuilder/conveyorBuilder";
import { ConveyorGeometry } from "../meshGeometry/conveyorGeometry";

/**
 * Class responsible for holding specific functionality of <create> tag
 */
export class SimulationCreateTag extends SimulationTag {
  private readonly createTag: CreateTag;

  /**
   * Create class responsible for holding specific functionality of <create> tag
   * @param _simulationTagData Data from Babylon.js scene
   * @param _createTag Specific <create tag>
   */
  constructor(_simulationTagData: SimulationTagData, _createTag: CreateTag) {
    super(_simulationTagData);
    this.createTag = _createTag;
    this._time = _createTag.time;
  }

  /**
   * Invoke specific functionality related to <create> tag
   */
  async actOnTagLogic(): Promise<void> {
    /**************************************************
     *             HANDLE CREATION OF GEOMETRY            *
     **************************************************/
    if (this.createTag.geometry && this.createTag.time === 0) {
      const geometryName = this.extractGeometry(this.createTag.geometry);

      if (!geometryName) return;

      if (
        geometryName === "dgu-pa-Conveyor5" ||
        geometryName === "dgu-pa-Conveyor6" ||
        geometryName === "dgu-pa-Track"
      ) {
        this.renderPathOrConveyor(geometryName);
      } else {
        await this.renderPartOrMachine(geometryName);
      }

      return;
    }

    /**************************************************
     *             HANDLE CREATION OF FLOOR            *
     **************************************************/

    const floorRegex = /^Floor [0-9]+$/;

    if (
      this.createTag.surface &&
      floorRegex.test(this.createTag.instanceName)
    ) {
      this.renderGround();
    }
  }

  /**
   * Create instance of conveyor or path as declared within the given <create> tag
   * @param geometryName The name of the geometry model used to render the conveyor or path specified in the given <create> tag
   */
  private renderPathOrConveyor(geometryName: string): void {
    if (!this.createTag.path) {
      throw new Error(
        "Unable to create instance of path or conveyor: <path> sub tag was not defined",
      );
    }
    const conveyorBuilder = new ConveyorBuilder(
      this.createTag.path,
      geometryName,
    );

    const conveyorMesh = conveyorBuilder.buildConveyor();

    if (!conveyorMesh) return;

    this.simulationTagData.geometriesMap.set(
      this.createTag.instanceName,
      new ConveyorGeometry(
        conveyorMesh,
        this.createTag.instanceName,
        geometryName,
        conveyorBuilder.conveyorInformation,
      ),
    );
  }

  /**
   *  Asynchronous function to import the machine or part mesh as named in the
   * value of the 'geometry' attribute of the given <create> tag
   * @param geometryName The name of the geometry model used to render the mesh of the <create> tag
   */
  private async renderPartOrMachine(geometryName: string): Promise<void> {
    try {
      const importMeshResult = await SceneLoader.ImportMeshAsync(
        "",
        "https://raw.githubusercontent.com/lfcourtney/Witness3DWebSimulationViewerModels/main/WitnessGlbModels/",
        geometryName + ".glb",
        this.simulationTagData.scene,
      );
      this.importMeshSuccess(importMeshResult.meshes, geometryName);
    } catch (error) {
      console.error("Error loading mesh:", error);
    }
  }

  /**
   * Callback function to be used if Babylon.js mesh was successfully imported
   * @param newMeshes Array of meshes that have been imported
   * @param geometryName The name of the geometry model used to render the mesh of the <create> tag
   */
  private importMeshSuccess(
    newMeshes: AbstractMesh[],
    geometryName: string,
  ): void {
    // Must have the main mesh and at least one child mesh
    if (newMeshes.length < 2) return;

    const transformMesh = newMeshes[0];

    //TODO: Undo default shrinking of meshes to 3% of their original size as soon as correctly sized
    // models are uploaded
    if (geometryName === "dg-pt-ManWalking1") {
      // TODO: Labour model is differently scaled from the other models: it only needs to be shrunk to 10% of its original size.
      // If the correctly sized labour model is uploaded, this should be undone.
      transformMesh.scaling = new Vector3(0.01, 0.01, 0.01);
    } else {
      transformMesh.scaling = new Vector3(0.003, 0.003, 0.003);
    }

    if (geometryName === "dg-vh-Agv1") {
      // TODO: It is possible that the 'dg-vh-Agv1' is added to a path without first being scaled with an <update> tag.
      // Therefore, we should apply this default scaling by default to this kind of vehicle, so it is scaled properly if
      // it is immediately added to the shape of a path or conveyor.
      transformMesh.scaling = new Vector3(
        1.451429 * 0.03,
        1.029986 * 0.03,
        0.608542 * 0.03,
      );
    }

    // Setting the name of the meshes allows us to search for the meshes from the scene
    transformMesh.name = this.createTag.instanceName;

    if (this.createTag.queueInfo) {
      /**
       * Only machines will have a <queueInfo> tag as a child tag of the <create> tag responsible for creating them
       */
      this.simulationTagData.geometriesMap.set(
        this.createTag.instanceName,
        new MachineGeometry(
          transformMesh,
          this.createTag.instanceName,
          geometryName,
          this.createTag.queueInfo,
        ),
      );
    } else {
      this.simulationTagData.geometriesMap.set(
        this.createTag.instanceName,
        new MeshGeometry(
          transformMesh,
          this.createTag.instanceName,
          geometryName,
        ),
      );
    }
  }

  /**
   * Render and position floor because the given <create> tag specifies a <surface> tag that corresponds to a floor
   */
  private renderGround(): void {
    if (!this.createTag.surface) return;

    const surfaceTag = this.createTag.surface;

    // Calculate width and depth
    const width = Math.abs(surfaceTag.x2 - surfaceTag.x1);
    const depth = Math.abs(surfaceTag.z2 - surfaceTag.z1);

    // Calculate the center position for the floor
    const centerX = (surfaceTag.x1 + surfaceTag.x2) / 2;
    const centerZ = (surfaceTag.z1 + surfaceTag.z2) / 2;

    // Create the ground plane
    const ground = MeshBuilder.CreateGround(
      this.createTag.instanceName,
      {
        width: width,
        height: depth,
      },
      this.simulationTagData.scene,
    );

    // Position the ground at the center of the coordinates
    ground.position.x = centerX;
    ground.position.z = centerZ;
    ground.position.y = surfaceTag.y1;
  }

  /**
   * Extract the name of the geometry from the 'geometry' attribute of a create tag
   * @param geometry The 'geometry' attribute of a create tag
   * @returns The name of the geometry if there is a valid geometry contained in the string. Or undefined if a valid geometry was unable to be undefined.
   */
  private extractGeometry(geometry: string): string | undefined {
    const geometryRegex =
      /(dg-ic-Machine1|dg-ic-WaterTank|dg-ic-Workbench2|dg-lq-Machine|dg-pt-ManWalking1|dg-pt-Part1|dg-vh-Agv1|dgu-pa-Conveyor5|dgu-pa-Conveyor6|dgu-pa-Track|QueuePlaceholder)(\.glb)?$/;

    const matchGeometry = geometryRegex.exec(geometry);

    if (!matchGeometry || matchGeometry?.length < 2) return undefined;

    return matchGeometry[1];
  }
}
