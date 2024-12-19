import { AbstractMesh } from "@babylonjs/core";
import { MeshGeometry } from "./meshGeometry";

/**
 * Subclass of the MeshGeometry class responsible for representing the status of a part or labour in Babylon.js scene
 */
export class PartGeometry extends MeshGeometry {
  /**
   * Create an object representing a part or labour imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the part or labour: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the part or labour
   * @param _geometryName The name of the part or labour geometry model used to render the given mesh
   */
  constructor(
    _transformMesh: AbstractMesh,
    _instanceName: string,
    _geometryName: string,
  ) {
    super(_transformMesh, _instanceName, _geometryName);
  }

  /**
   * Get the effective height of the given part or labour. This is used for positioning parts
   * within a queue. To do so, the height of the part, provided by this function, is required.
   * @returns The height of the given part. Otherwise, undefined if, somehow, the mesh this
   * object is encapsulating is not actually a part or labour.
   */
  public getEffectivePartHeight(): number | undefined {
    if (this._geometryName === "dg-pt-Part1") {
      return this.getScaling().y;
    }

    if (this._geometryName === "dg-pt-ManWalking1") {
      return 2;
    }

    return undefined;
  }
}
