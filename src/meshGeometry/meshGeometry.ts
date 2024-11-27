import { AbstractMesh, Node, Vector3 } from "@babylonjs/core";

/**
 * Generic class responsible for representing the status of an imported geometry in Babylon.js scene
 */
export class MeshGeometry {
  protected readonly _transformMesh: AbstractMesh;

  protected readonly _instanceName: string;

  /**
   * Create an object representing a generic geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   */
  constructor(_transformMesh: AbstractMesh, _instanceName: string) {
    this._transformMesh = _transformMesh;

    this._instanceName = _instanceName;
  }

  public get instanceName() {
    return this._instanceName;
  }

  /**
   * Set the position of the mesh
   * @param newPosition New position of the mesh
   */
  setPosition(newPosition: Vector3) {
    this._transformMesh.position = newPosition;
  }

  /**
   * Set the rotation of the mesh
   * @param newRotation New rotation value of the mesh
   */
  setRotation(newRotation: Vector3) {
    this._transformMesh.rotation = newRotation;
  }

  /**
   * Set the scaling of the mesh
   * @param newScaling New scaling value of the mesh
   */
  setScaling(newScaling: Vector3) {
    this._transformMesh.scaling = newScaling;
  }

  /**
   * Adds to 'y' position of geometry the difference between the origin of the scene and the 'y' position of the
   * geometry to make the geometry level with the floor in the Babylon.js scene
   */
  fixGeometryToGround(): void {
    let minY = 0;

    this._transformMesh.getDescendants().forEach((descendant) => {
      if (this.isNodeAbstractMesh(descendant)) {
        const boundingInfo = descendant.getBoundingInfo();
        const minimum = boundingInfo.boundingBox.minimum;

        minY = Math.min(minY, minimum.y);
      }
    });
    this._transformMesh.position.y -= minY;
  }

  private isNodeAbstractMesh(node: Node): node is AbstractMesh {
    return node instanceof AbstractMesh;
  }

  /**
   * Change visibility for geometry
   * @param newVisibility new visibility number
   */
  changeVisibility(newVisibility: number): void {
    this._transformMesh.getDescendants().forEach((descendant) => {
      if (this.isNodeAbstractMesh(descendant)) {
        descendant.visibility = newVisibility;
      }
    });
  }
}
