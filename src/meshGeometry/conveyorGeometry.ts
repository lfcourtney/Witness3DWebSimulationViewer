import { AbstractMesh, Vector3, GreasedLineTools } from "@babylonjs/core";
import { MeshGeometry } from "./meshGeometry";
import { PartGeometry } from "./partGeometry";
import { PositionPart } from "./positionPart";

/**
 * Subclass of the MeshGeometry class responsible for representing the status of a conveyor or path geometry in Babylon.js scene
 */
export class ConveyorGeometry extends MeshGeometry implements PositionPart {
  /**
   * The array of Vector3 points representing the conveyor or path mesh shape
   */
  private readonly _conveyorPoints: Vector3[] = [];

  /**
   * Create an object representing a conveyor or path geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   * @param _geometryName The name of the geometry model used to render the given mesh
   * @param _conveyorPoints The array of Vector3 points representing the conveyor or path mesh shape
   */
  constructor(
    _transformMesh: AbstractMesh,
    _instanceName: string,
    _geometryName: string,
    _conveyorPoints: Vector3[],
  ) {
    super(_transformMesh, _instanceName, _geometryName);
    this._conveyorPoints = _conveyorPoints;
  }

  /**
   * Position a part on a conveyor or path
   * @param part The part to position conveyor or path on
   * @param position The amount to position the part on the conveyor or path by
   */
  public positionPart(part: PartGeometry, partPosition: number): void {
    // Remove parent as this could be set to a queue position
    part.setParent(null);

    const conveyorPointPosition = this.gteConveyorPointPosition(partPosition);

    const effectivePartHeight = part.getEffectivePartHeight() || 0;

    part.setPosition(
      conveyorPointPosition.add(new Vector3(0, effectivePartHeight / 2, 0)),
    );
  }

  /**
   * Get a position that reflects a percentage along a conveyor. Used for positioning parts on the conveyor.
   * @param at The percentage along the conveyor.
   * @returns A position that reflects the given percentage along the path of a conveyor.
   * Defaults to conveyor starting position if a specific position could not be found.
   * @see {@link https://forum.babylonjs.com/t/get-vector3-point-from-curve3-by-percentage/6389/7|Get Vector3 Point from Curve3 by Percentage?} for Babylon.js forum post
   * that this function has been adapted from.
   */
  private gteConveyorPointPosition(at: number): Vector3 {
    if (!this._conveyorPoints.length)
      throw new Error(
        "Unable to get conveyor point position: conveyor point positions array is empty",
      );

    if (at <= 0) return this._conveyorPoints[0];

    const pathLength = GreasedLineTools.GetLineLength(this._conveyorPoints);
    let previousPoint = this._conveyorPoints[0];
    let currentPoint: Vector3;
    let currentLength = 0;
    const targetLength = at * pathLength;

    for (let i = 0; i < pathLength; i++) {
      currentPoint = this._conveyorPoints[i];

      const distance = Vector3.Distance(previousPoint, currentPoint);
      currentLength += distance;
      if (currentLength === targetLength) {
        return currentPoint.clone();
      } else if (currentLength > targetLength) {
        const toLength = currentLength - targetLength;
        const diff = toLength / distance;
        const dir = previousPoint.subtract(currentPoint);
        return currentPoint.add(dir.scale(diff));
      }
      previousPoint = currentPoint;
    }
    return this._conveyorPoints[0];
  }
}
