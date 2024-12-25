import { AbstractMesh, Vector3 } from "@babylonjs/core";
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
    // TODO: Implement logic for this method
    console.log(part, partPosition);
  }
}
