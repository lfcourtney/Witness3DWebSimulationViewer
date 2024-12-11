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
   * Return the starting position of the queue scaled along the parent geometry
   */
  private get scaledQueueInfo(): {
    x: number;
    y: number;
    z: number;
  } {
    let scaleFactor = 1;

    if (
      this._instanceName !== "[130] Treatment(1) - Main Icon" &&
      this._instanceName !== "[130] Treatment(2) - Main Icon"
    ) {
      //TODO: Undo 'scaleFactor' variable as soon as all the models are properly sized, and thus do not need their sizes scaled up by 97% to
      // reflect their true scalings as they were initially shrunk by 3%
      scaleFactor = 33.333333;
    }

    return {
      x:
        this.queueInfoTag.position.x *
        (this._transformMesh.scaling.x * scaleFactor),
      y:
        this.queueInfoTag.position.y *
        (this._transformMesh.scaling.y * scaleFactor),
      z:
        this.queueInfoTag.position.z *
        (this._transformMesh.scaling.z * scaleFactor),
    };
  }

  /**
   * @override
   * Set the position of the machine. Updates queue position in
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
   * @override
   * Set the scaling of the machine. Updates queue position in
   * accordance with this new scaling.
   * @param newScaling New scaling of the machine
   */
  setScaling(newScaling: Vector3) {
    super.setScaling(newScaling);
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
    part.position.x = this._transformMesh.position.x + this.scaledQueueInfo.x;
    part.position.y =
      this._transformMesh.position.y +
      this.applyPartPositioningToQueuePosition();
    part.position.z = this._transformMesh.position.z + this.scaledQueueInfo.z;
  }

  /**
   * Calculates correct vertical position of part relative to 'partPositioning' attribute
   * @returns Vertical position of part based on 'partPositioning' attribute
   */
  private applyPartPositioningToQueuePosition(): number {
    const partSizeHalved = this.PART_SIZE / 2;
    if (this.queueInfoTag.behaviour.partPositioning === "partOver") {
      return this.scaledQueueInfo.y + partSizeHalved;
    }
    if (this.queueInfoTag.behaviour.partPositioning === "partUnder") {
      return this.scaledQueueInfo.y - partSizeHalved;
    }

    // A value of 'partCentre' does not need modification: origin of the part, which is the middle,
    // will be aligned with the queue y-axis position anyway.
    return this.scaledQueueInfo.y;
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
