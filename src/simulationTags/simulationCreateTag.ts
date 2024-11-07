import { AbstractMesh, SceneLoader } from "@babylonjs/core";
import { MeshGeometry } from "../meshGeometry/meshGeometry";
import { SimulationTag, SimulationTagData } from "./simulationTag";
import { CreateTag } from "../interfaces/createTag";

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
  }

  /**
   * Invoke specific functionality related to <create> tag
   */
  async actOnTagLogic(): Promise<void> {
    if (this.createTag.geometry && this.createTag.time === 0) {
      const geometryName = this.extractGeometry(this.createTag.geometry);

      if (!geometryName) return;

      try {
        const importMeshResult = await SceneLoader.ImportMeshAsync(
          "",
          "https://raw.githubusercontent.com/lfcourtney/Witness3DWebSimulationViewerModels/main/WitnessGlbModels/",
          geometryName + ".glb",
          this.simulationTagData.scene,
        );
        this.importMeshSuccess(importMeshResult.meshes);
      } catch (error) {
        console.error("Error loading mesh:", error);
      }
      return;
    }
  }

  /**
   * Callback function to be used if Babylon.js mesh was successfully imported
   * @param newMeshes Array of meshes that have been imported
   * @param createTag The 'instanceName' attribute of corresponding <create> tag
   */
  private importMeshSuccess(newMeshes: AbstractMesh[]): void {
    // Must have the main mesh and at least one child mesh
    if (newMeshes.length < 2) return;

    const transformMesh = newMeshes[0];

    // Setting the name of the meshes allows us to search for the meshes from the scene
    transformMesh.name = this.createTag.instanceName;

    this.simulationTagData.geometriesMap.set(
      this.createTag.instanceName,
      new MeshGeometry(transformMesh, this.createTag.instanceName),
    );
  }

  /**
   * Extract the name of the geometry from the 'geometry' attribute of a create tag
   * @param geometry The 'geometry' attribute of a create tag
   * @returns The name of the geometry if there is a valid geometry contained in the string. Or undefined if a valid geometry was unable to be undefined.
   */
  private extractGeometry(geometry: string): string | undefined {
    const geometryRegex =
      /(dg-ic-Machine1|dg-ic-Machine1|dg-ic-WaterTank|dg-ic-Workbench2|dg-lq-Machine|dg-pt-ManWalking1|dg-pt-Part1|dg-vh-Agv1|dgu-pa-Conveyor5|dgu-pa-Conveyor6|dgu-pa-Track)(\.glb)?$/;

    const matchGeometry = geometryRegex.exec(geometry);

    if (!matchGeometry || matchGeometry?.length < 2) return undefined;

    return matchGeometry[1];
  }
}
