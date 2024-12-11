import { blueMat } from "../materials/colors";
import { AbstractMesh, MeshBuilder, Mesh, Vector3 } from "@babylonjs/core";
import { MeshGeometry } from "./meshGeometry";
import { QueueInfoTag } from "../interfaces/queueInfoTag";

/**
 * Subclass of the MeshGeometry class responsible for representing the status of an machine geometry in Babylon.js scene
 */
export class MachineGeometry extends MeshGeometry {
  private readonly PART_SIZE: number = 0.6;

  private readonly queueInfoTag: QueueInfoTag;

  private readonly part: Mesh | undefined;

  /**
   * Create an object representing a machine geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   * @param _queueInfoTag The <queueInfo> tag contained directly as the child tag of the <create> tag responsible for creating the machine
   */
  constructor(
    _transformMesh: AbstractMesh,
    _instanceName: string,
    _queueInfoTag: QueueInfoTag,
  ) {
    super(_transformMesh, _instanceName);
    this.queueInfoTag = _queueInfoTag;

    /*********************************************
     *                  Create part
     *********************************************/
    if (this.hasQueuePosition()) {
      const part = MeshBuilder.CreateBox("part", { size: this.PART_SIZE });
      part.material = blueMat();
      this.part = part;
      this.setPositionOfPart(part);
    }
  }

  /**
   * @override
   * Set the position of the machine. Updates machine part positions in
   * accordance with this new position.
   * @param newPosition New position of the machine
   */
  setPosition(newPosition: Vector3) {
    super.setPosition(newPosition);
    if (this.part) {
      this.setPositionOfPart(this.part);
    }
  }

  /**
   * Updates the position of the part in relation to the machine mesh. So this method
   * should be called in conjunction with updating the position of the main machine.
   * @param part The machine part to update the position of
   */
  private setPositionOfPart(part: Mesh): void {
    part.position.x =
      this._transformMesh.position.x + this.queueInfoTag.position.x;
    part.position.y =
      this._transformMesh.position.y +
      this.applyPartPositioningToQueuePosition();
    part.position.z =
      this._transformMesh.position.z + this.queueInfoTag.position.z;
  }

  /**
   * Calculates correct vertical position of part relative to 'partPositioning' attribute
   * @returns Vertical position of part based on 'partPositioning' attribute
   */
  private applyPartPositioningToQueuePosition(): number {
    const partSizeHalved = this.PART_SIZE / 2;
    if (this.queueInfoTag.behaviour.partPositioning === "partOver") {
      return this.queueInfoTag.position.y + partSizeHalved;
    }
    if (this.queueInfoTag.behaviour.partPositioning === "partUnder") {
      return this.queueInfoTag.position.y - partSizeHalved;
    }

    // A value of 'partCentre' does not need modification: origin of the part, which is the middle,
    // will be aligned with the queue y-axis position anyway.
    return this.queueInfoTag.position.y;
  }

  /**
   * Checks whether queue position is not 0, 0, 0 on all 3 axes.
   * @returns True if position is not at origin. False otherwise.
   */
  private hasQueuePosition(): boolean {
    let hasQueuePosition: boolean = false;
    for (const position in this.queueInfoTag.position) {
      hasQueuePosition = this.queueInfoTag.position[position] !== 0;
    }
    return hasQueuePosition;
  }
}
