import { AbstractMesh, TransformNode, Vector3 } from "@babylonjs/core";
import { MeshGeometry } from "./meshGeometry";
import { QueueInfoTag } from "../interfaces/queueInfoTag";

/**
 * Subclass of the MeshGeometry class responsible for representing the status of an machine geometry in Babylon.js scene
 */
export class MachineGeometry extends MeshGeometry {
  private readonly queueInfoTag: QueueInfoTag;

  /**
   * Position of the queue. Should update in accordance with scaling and positioning of the machine.
   */
  private readonly queuePosition: TransformNode = new TransformNode(
    "queuePosition",
  );

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
    this.setPositionOfQueue();
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
    this.setPositionOfQueue();
  }

  /**
   * @override
   * Set the scaling of the machine. Updates queue position in
   * accordance with this new scaling.
   * @param newScaling New scaling of the machine
   */
  setScaling(newScaling: Vector3) {
    super.setScaling(newScaling);
    this.setPositionOfQueue();
  }

  /**
   * @override
   * Set the rotation of the mesh. Updates queue rotation in
   * accordance with this new rotation.
   * @param newRotation New rotation value of the mesh
   */
  setRotation(newRotation: Vector3) {
    super.setRotation(newRotation);
    this.queuePosition.rotation = newRotation;
  }

  /**
   * Position a part in the queue
   * @param part The part to position in the queue
   * @param position The amount to position the part in the queue by
   */
  public positionPart(part: MeshGeometry, partPosition: number): void {
    // Apply the queue rotation and scale to the parts
    part.setParent(this.queuePosition);

    part.setPosition(
      new Vector3(
        this.calculateQueueSizeLocation(
          this.queuePosition.position.x,
          this.queueInfoTag.direction.dx,
          partPosition,
        ),
        this.processPartPositioningAttribute(part),
        this.calculateQueueSizeLocation(
          this.queuePosition.position.z,
          this.queueInfoTag.direction.dz,
          partPosition,
        ),
      ),
    );
  }

  /**
   * For a given axis (eg, x, y, z), calculate part position based on queue size,
   * which is calculated as the difference between the queue position and direction
   * @param queuePosition The queue position of the given axis
   * @param queueDirection The queue direction of the given axis
   * @param partPosition The percentage in the queue the part should be positioned by relative to the overall queue size
   * @returns The part position for the given axis
   */
  private calculateQueueSizeLocation(
    queuePosition: number,
    queueDirection: number,
    partPosition: number,
  ): number {
    const startingPosition = queuePosition;
    const endingPosition = queuePosition + queueDirection;

    // Queue direction is 0 (no difference), so no change in position is necessary
    if (startingPosition === endingPosition) return 0;

    // With the 'partPosition' determining the percentage,
    // the percentage of the difference between the queue position and direction
    const percentageDifference =
      (endingPosition - startingPosition) * partPosition;

    return percentageDifference;
  }

  /**
   * Updates the position of the queue in relation to the machine mesh. So this method
   * should be called in conjunction with updating the position and scaling of the main machine.
   * @param part The machine part to update the position of
   */
  private setPositionOfQueue(): void {
    this.queuePosition.position.x =
      this._transformMesh.position.x + this.scaledQueueInfo.x;
    this.queuePosition.position.y =
      this._transformMesh.position.y + this.scaledQueueInfo.y;
    this.queuePosition.position.z =
      this._transformMesh.position.z + this.scaledQueueInfo.z;
  }

  /**
   * Calculates correct vertical position of part relative to 'partPositioning' attribute
   * @returns Vertical position of part based on 'partPositioning' attribute
   */
  private processPartPositioningAttribute(part: MeshGeometry): number {
    const partSizeHalved = part.getScaling().y / 2;
    if (this.queueInfoTag.behaviour.partPositioning === "partOver") {
      return partSizeHalved;
    }
    if (this.queueInfoTag.behaviour.partPositioning === "partUnder") {
      return -partSizeHalved;
    }

    // A value of 'partCentre' does not need modification: origin of the part, which is the middle,
    // will be aligned with the queue y-axis position anyway.
    return 0;
  }
}
