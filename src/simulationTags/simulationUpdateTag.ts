import { Vector3, Tools } from "@babylonjs/core";
import { SimulationTag, SimulationTagData } from "./simulationTag";
import { UpdateTag } from "../interfaces/updateTag";
import { MachineGeometry } from "../meshGeometry/machineGeometry";

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
    this._time = _updateTag.time;
  }

  /**
   * Invoke specific functionality related to <update> tag
   */
  async actOnTagLogic(): Promise<void> {
    /********************************************************************
     *     HANDLE TRANSLATE, ROTATE, SCALE AND VISIBILITY AT TIME 0      *
     ********************************************************************/
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
      // TODO: remove this so that all meshes are scaled the same once new models are uploaded
      if (
        this.updateTag.instanceName !== "[130] Treatment(1) - Main Icon" &&
        this.updateTag.instanceName !== "[130] Treatment(2) - Main Icon"
      ) {
        foundGeometry.setScaling(
          new Vector3(
            this.updateTag.scale.x * 0.03,
            this.updateTag.scale.y * 0.03,
            this.updateTag.scale.z * 0.03,
          ),
        );
      } else {
        foundGeometry.setScaling(
          new Vector3(
            this.updateTag.scale.x,
            this.updateTag.scale.y,
            this.updateTag.scale.z,
          ),
        );
      }
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
    } else {
      // Assume updated mesh should be visible unless explicitly stated otherwise.
      foundGeometry.changeVisibility(1);
    }

    this.handlePartPositioning();
  }

  /**
   * Process logic of <partPosition> subtag of <update> tag: position part in queue
   */
  private handlePartPositioning(): void {
    if (!this.updateTag.partPosition) return;

    // Part to add to machine
    const partGeometry = this.simulationTagData.geometriesMap.get(
      this.updateTag.instanceName,
    );

    // Machine to have part added to
    const machineGeometry = this.simulationTagData.geometriesMap.get(
      this.updateTag.partPosition.instanceName,
    );

    if (!partGeometry) return;

    if (!(machineGeometry instanceof MachineGeometry)) return;

    // Position part
    machineGeometry.positionPart(
      partGeometry,
      this.updateTag.partPosition.position,
    );
  }
}
