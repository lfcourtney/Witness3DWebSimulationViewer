import { Vector3 } from "@babylonjs/core";

/**
 * Stores the information for rendering the shape of a conveyor or path
 */
export interface ConveyorInformation {
  /**
   * The array of Vector3 points representing the conveyor or path mesh shape
   */
  conveyorPoints: Vector3[];
  /**
   * The tangent Vectors of the conveyor or path shape.
   */
  conveyorTangents: Vector3[];
  /**
   * The binormal Vectors of the conveyor or path shape.
   */
  conveyorBinormals: Vector3[];
}
