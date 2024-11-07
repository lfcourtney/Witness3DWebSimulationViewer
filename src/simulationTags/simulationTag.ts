import { Scene } from "@babylonjs/core";
import { MeshGeometry } from "../meshGeometry/meshGeometry";

export interface SimulationTagData {
  scene: Scene;
  geometriesMap: Map<string, MeshGeometry>;
}

/**
 * Abstract class responsible for describing the format through which the functionality of a specific tag type will be invoked.
 * That means each derived class of this abstract class will represent a specific type of tag (<create>, <update>, etc.),
 * with each type of class having their own unique functionality.
 */
export abstract class SimulationTag {
  protected readonly simulationTagData: SimulationTagData;

  /**
   * Create the generic abstract representation of the tag functionality.
   * @param _simulationTagData Data from Babylon.js scene
   */
  constructor(_simulationTagData: SimulationTagData) {
    this.simulationTagData = _simulationTagData;
  }

  /**
   * The generic abstract method that represents the unique functionality to be invoked of the derived class.
   */
  abstract actOnTagLogic(): Promise<void>;
}
