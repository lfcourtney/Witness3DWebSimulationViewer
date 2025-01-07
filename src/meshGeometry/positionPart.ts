import { MeshGeometry } from "./meshGeometry";

/**
 * Represents a MeshGeometry sub class that is able to position parts, whether that is because it is a machine, a conveyor or a path
 */
export interface PositionPart {
  /**
   * Position the part
   * @param part The part to position
   * @param partPosition As a percentage of the overall queue or path shape, the position of the part.
   */
  positionPart(part: MeshGeometry, partPosition: number): void;
}
