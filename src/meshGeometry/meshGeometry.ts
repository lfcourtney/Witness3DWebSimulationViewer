import { AbstractMesh, Node, Vector3 } from "@babylonjs/core";

/**
 * Generic class responsible for representing the status of an imported geometry in Babylon.js scene
 */
export class MeshGeometry {
  protected readonly _transformMesh: AbstractMesh;

  /**
   * Original transform mesh upon mesh instantiation. Useful for cloning such mesh without any of the transformations that
   * may have been subsequently applied to it.
   */
  private readonly _originalTransformMesh: AbstractMesh;

  protected readonly _instanceName: string;

  protected readonly _geometryName: string;

  /**
   * Rotation of the mesh before it was added to the shape of a path or conveyor
   */
  private _nonPathOrConveyorRotation: Vector3 | null = null;

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
    this._originalTransformMesh = _transformMesh;

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
   * Set the rotation of the mesh specifically for being positioned on the shape of a path or conveyor
   * @param newRotation New rotation value of the mesh
   */
  setPartOrConveyorRotation(newRotation: Vector3) {
    // Store original rotation before it was added to the shape of the path or conveyor
    if (!this._nonPathOrConveyorRotation) {
      this._nonPathOrConveyorRotation = this._transformMesh.rotation;
    }
    this._transformMesh.rotation = newRotation;
  }

  /**
   * Resets rotation of mesh to its standard rotation before it was positioned on the shape of a path or conveyor
   */
  resetPartOrConveyorRotation(): void {
    if (!this._nonPathOrConveyorRotation) return;
    this._transformMesh.rotation = this._nonPathOrConveyorRotation;
    this._nonPathOrConveyorRotation = null;
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
   * Copy the underling Abstract Mesh to create a new Abstract Mesh
   * @param clonedMeshName Name of the new Abstract Mesh to create
   * @returns The new Abstract Mesh created through copying or null if copy was unsuccessful
   */
  clone(clonedMeshName: string): AbstractMesh | null {
    return this._originalTransformMesh.clone(clonedMeshName, null);
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

  /**
   * Gets the height of a geometry when it is being positioned in a queue or on the shape of a path or conveyor.
   * @returns The returned height of the geometry specifically for being positioned in a queue or
   * on the shape of a path or conveyor.
   */
  public getEffectivePartHeight(): number | undefined {
    // TODO: If the geometry of 'dg-pt-ManWalking1' is updated, remove this if condition to
    // make instance of labour geometry match the height of all the other geometries.
    if (this._geometryName === "dg-pt-ManWalking1") {
      return 2;
    }

    return this.getScaling().y;
  }
}
