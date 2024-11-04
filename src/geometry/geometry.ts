import { AbstractMesh, Vector3 } from "@babylonjs/core";

/**
 * Generic class responsible for representing the status of an imported geometry in Babylon.js scene
 */
export class Geometry {
  private readonly transformMesh: AbstractMesh;

  private readonly _instanceName: string;

  /**
   * Create an object representing a generic geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   */
  constructor(_transformMesh: AbstractMesh, _instanceName: string) {
    this.transformMesh = _transformMesh;

    this._instanceName = _instanceName;

    // We work with the 'geometryMesh' when we want to make a geometry invisible
    this.transformMesh.visibility = 0;

    this.fixGeometryToGround();
  }

  public get instanceName() {
    return this._instanceName;
  }

  getPosition(): Vector3 {
    return this.transformMesh.position;
  }

  setPosition(newPosition: Vector3): void {
    this.transformMesh.position = newPosition;
  }

  /**
   * Adds to 'y' position of geometry the difference between the origin of the scene and the 'y' position of the
   * geometry to make the geometry level with the floor in the Babylon.js scene
   */
  private fixGeometryToGround(): void {
    let minY = 0;

    this.transformMesh.getDescendants().forEach((descendant) => {
      if (descendant instanceof AbstractMesh) {
        const boundingInfo = descendant.getBoundingInfo();
        const minimum = boundingInfo.boundingBox.minimum;

        minY = Math.min(minY, minimum.y);
      }
    });
    this.transformMesh.position.y -= minY;
  }
}
