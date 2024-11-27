import { AbstractMesh } from "@babylonjs/core";
import { MeshGeometry } from "./meshGeometry";
import { QueueInfoTag } from "../interfaces/queueInfoTag";

/**
 * Subclass of the MeshGeometry class responsible for representing the status of an machine geometry in Babylon.js scene
 */
export class MachineGeometry extends MeshGeometry {
  private readonly queueInfoTag: QueueInfoTag;

  /**
   * Create an object representing a machine geometry imported into the Babylon.js scene
   * @param _transformMesh The transformation mesh of the geometry: can have transformations applied to it to move the mesh in the global scene
   * @param _instanceName The instance name of the geometry
   * @param _queueInfoTag The <queueInfo> tag contained directly as the child tag of the <create> tag responsible for creating the machine
   */
  constructor(
    _transformMesh: AbstractMesh,
    _instanceName: string,
    _queueInfoTag: QueueInfoTag,
  ) {
    super(_transformMesh, _instanceName);
    this.queueInfoTag = _queueInfoTag;
  }
}
