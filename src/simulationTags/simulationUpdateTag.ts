import { Vector3, Tools } from "@babylonjs/core";
import { SimulationTag, SimulationTagData } from "./simulationTag";
import { UpdateTag } from "../interfaces/updateTag";

/**
 * Class responsible for holding specific functionality of <update> tag
 */
export class SimulationUpdateTag extends SimulationTag {
  private readonly updateTag: UpdateTag;

  /**
   * Create class responsible for holding specific functionality of <update> tag
   * @param _simulationTagData Data from Babylon.js scene
   * @param _updateTag Specific <update tag>
   */
  constructor(_simulationTagData: SimulationTagData, _updateTag: UpdateTag) {
    super(_simulationTagData);
    this.updateTag = _updateTag;
  }

  /**
   * Invoke specific functionality related to <update> tag
   */
  async actOnTagLogic(): Promise<void> {
    if (this.updateTag.time === 0) {
      const foundGeometry = this.simulationTagData.geometriesMap.get(
        this.updateTag.instanceName,
      );

      if (!foundGeometry) return;

      if (this.updateTag.translate) {
        foundGeometry.transformMesh.position = new Vector3(
          this.updateTag.translate.x,
          this.updateTag.translate.y,
          this.updateTag.translate.z,
        );
      }

      if (this.updateTag.scale) {
        foundGeometry.transformMesh.scaling = new Vector3(
          this.updateTag.scale.x * 0.03,
          this.updateTag.scale.y * 0.03,
          this.updateTag.scale.z * 0.03,
        );
      }

      if (this.updateTag.rotate) {
        foundGeometry.transformMesh.rotation = new Vector3(
          Tools.ToRadians(this.updateTag.rotate.x),
          Tools.ToRadians(this.updateTag.rotate.y),
          Tools.ToRadians(this.updateTag.rotate.z),
        );
      }

      if (this.updateTag.visible === false) {
        foundGeometry.changeVisibility(0);
      }

      return;
    }
  }
}
