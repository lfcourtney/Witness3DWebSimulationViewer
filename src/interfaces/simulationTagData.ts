import { Scene } from "@babylonjs/core";
import { MeshGeometry } from "../meshGeometry/meshGeometry";
import { SimulationTag } from "../simulationTags/simulationTag";

/**
 * Interface specifically for formatting the data that every concrete class implementation of 'SimulationTagData' will have.
 * To be precise, it contains the data from the wider Babylon.js scene that the class might need for performing its operations.
 */
export interface SimulationTagData {
  scene: Scene;
  geometriesMap: Map<string, MeshGeometry>;
  timeTagStore: Map<string, SimulationTag[]>;
}
