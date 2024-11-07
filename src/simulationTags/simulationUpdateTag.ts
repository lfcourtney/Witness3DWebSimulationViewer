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
        foundGeometry.setPosition(
          new Vector3(
            this.updateTag.translate.x,
            this.updateTag.translate.y,
            this.updateTag.translate.z,
          ),
        );
      }

      if (this.updateTag.scale) {
        foundGeometry.setScale(
          new Vector3(
            this.updateTag.scale.x,
            this.updateTag.scale.y,
            this.updateTag.scale.z,
          ),
        );
      }

      if (this.updateTag.rotate) {
        foundGeometry.setRotation(
          new Vector3(
            Tools.ToRadians(this.updateTag.rotate.x),
            Tools.ToRadians(this.updateTag.rotate.y),
            Tools.ToRadians(this.updateTag.rotate.z),
          ),
        );
      }

      if (this.updateTag.visible === false) {
        foundGeometry.changeVisibility(0);
      }

      return;
    }
  }
}
