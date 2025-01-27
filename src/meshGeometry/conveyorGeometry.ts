import {
  AbstractMesh,
  Vector3,
  GreasedLineTools,
  Quaternion,
  Tools,
} from "@babylonjs/core";
import { MeshGeometry } from "./meshGeometry";
import { PositionPart } from "./positionPart";
import { ConveyorInformation } from "../interfaces/conveyorInformation";

/**
 * Subclass of the MeshGeometry class responsible for representing the status of a conveyor or path geometry in Babylon.js scene
 */
export class ConveyorGeometry extends MeshGeometry implements PositionPart {
  /**
   * The information representing the shape of the rendered conveyor or path
   */
  private readonly _conveyorInformation: ConveyorInformation;

  /**
   * Create an object representing a conveyor or path geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   * @param _geometryName The name of the geometry model used to render the given mesh
   * @param _conveyorInformation The information representing the shape of the rendered conveyor or path
   */
  constructor(
    _transformMesh: AbstractMesh,
    _instanceName: string,
    _geometryName: string,
    _conveyorInformation: ConveyorInformation,
  ) {
    super(_transformMesh, _instanceName, _geometryName);
    this._conveyorInformation = _conveyorInformation;
  }

  /**
   * Position a part on a conveyor or path
   * @param part The part to position conveyor or path on
   * @param position The amount to position the part on the conveyor or path by
   */
  public positionPart(part: MeshGeometry, partPosition: number): void {
    // Remove parent as this could be set to a queue position
    part.setParent(null);

    const [conveyorPointPosition, partIndex] =
      this.gteConveyorPointPosition(partPosition);

    // Set rotation on shape of path or conveyor before position is set
    this.setPartRotation(part, partIndex);

    const effectivePartHeight = part.getEffectivePartHeight() || 0;

    part.setPosition(
      conveyorPointPosition.add(new Vector3(0, effectivePartHeight / 2, 0)),
    );
  }

  /**
   * Update rotation of part so that it is facing the direction of travel on the shape of the path or conveyor it is travelling along
   * @param part The part to update the rotation of
   * @param index From the array of Vectors making up the shape of the path or conveyor, the index of the Vector3 the shape is currently positioned along
   */
  private setPartRotation(part: MeshGeometry, index: number): void {
    // Convert rotation quaternion to regular euler angles
    const rotationQuaternion = Quaternion.FromLookDirectionRH(
      this._conveyorInformation.conveyorTangents[index],
      this._conveyorInformation.conveyorBinormals[index],
    ).toEulerAngles();

    // Starting rotation of 'dg-vh-Agv1' vehicle needs to be set manually
    if (part.geometryName === "dg-vh-Agv1") {
      rotationQuaternion.y += Tools.ToRadians(90);
    }

    part.setPartOrConveyorRotation(rotationQuaternion);
  }

  /**
   * Get a position that reflects a percentage along a conveyor. Used for positioning parts on the conveyor.
   * @param at The percentage along the conveyor.
   * @returns Tuple containing a position that reflects the given percentage along the path of a conveyor along with the corresponding
   * index from the conveyor Vector array.
   * Defaults to conveyor starting position if a specific position could not be found.
   * @see {@link https://forum.babylonjs.com/t/get-vector3-point-from-curve3-by-percentage/6389/7|Get Vector3 Point from Curve3 by Percentage?} for Babylon.js forum post
   * that this function has been adapted from.
   */
  private gteConveyorPointPosition(at: number): [Vector3, number] {
    if (!this._conveyorInformation.conveyorPoints.length)
      throw new Error(
        "Unable to get conveyor point position: conveyor point positions array is empty",
      );

    if (at <= 0) return [this._conveyorInformation.conveyorPoints[0], 0];

    const pathLength = GreasedLineTools.GetLineLength(
      this._conveyorInformation.conveyorPoints,
    );
    let previousPoint = this._conveyorInformation.conveyorPoints[0];
    let currentPoint: Vector3;
    let currentLength = 0;
    const targetLength = at * pathLength;

    for (let i = 0; i < pathLength; i++) {
      currentPoint = this._conveyorInformation.conveyorPoints[i];

      const distance = Vector3.Distance(previousPoint, currentPoint);
      currentLength += distance;
      if (currentLength === targetLength) {
        return [currentPoint.clone(), i];
      } else if (currentLength > targetLength) {
        const toLength = currentLength - targetLength;
        const diff = toLength / distance;
        const dir = previousPoint.subtract(currentPoint);
        return [currentPoint.add(dir.scale(diff)), i];
      }
      previousPoint = currentPoint;
    }
    return [this._conveyorInformation.conveyorPoints[0], 0];
  }
}
