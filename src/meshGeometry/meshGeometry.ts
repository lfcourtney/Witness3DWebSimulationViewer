import { AbstractMesh, Node, Vector3 } from "@babylonjs/core";

/**
 * Generic class responsible for representing the status of an imported geometry in Babylon.js scene
 */
export class MeshGeometry {
  protected readonly _transformMesh: AbstractMesh;

  protected readonly _instanceName: string;

  protected readonly _geometryName: string;

  // 1 if mesh is visible. 2 if mesh is invisible
  private _visibility: number = 1;

  /**
   * Create an object representing a generic geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   * @param _geometryName The name of the geometry model used to render the given mesh
   */
  constructor(
    _transformMesh: AbstractMesh,
    _instanceName: string,
    _geometryName: string,
  ) {
    this._transformMesh = _transformMesh;

    this._instanceName = _instanceName;

    this._geometryName = _geometryName;
  }

  public get instanceName() {
    return this._instanceName;
  }

  public get visibility() {
    return this._visibility;
  }

  public get geometryName() {
    return this._geometryName;
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
   * Apply a parent mesh to the given mesh
   * @param parent Parent mesh to apply
   */
  setParent(parent: Node | null) {
    this._transformMesh.parent = parent;
  }

  /**
   * Get the scaling value of the mesh
   * @returns Scaling value of the mesh
   */
  getScaling(): Vector3 {
    return this._transformMesh.scaling;
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
    this._visibility = newVisibility;
    this._transformMesh.getDescendants().forEach((descendant) => {
      if (this.isNodeAbstractMesh(descendant)) {
        descendant.visibility = newVisibility;
      }
    });
  }
}
