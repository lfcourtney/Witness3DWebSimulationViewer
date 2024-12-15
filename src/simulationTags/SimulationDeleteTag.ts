import { SimulationTag, SimulationTagData } from "./simulationTag";
import { DeleteTag } from "../interfaces/deleteTag";

/**
 * Class responsible for holding specific functionality of <delete> tag
 */
export class SimulationDeleteTag extends SimulationTag {
  private readonly deleteTag: DeleteTag;

  /**
   * Create class responsible for holding specific functionality of <delete> tag
   * @param _simulationTagData Data from Babylon.js scene
   * @param _deleteTag Specific <delete> tag
   */
  constructor(_simulationTagData: SimulationTagData, _deleteTag: DeleteTag) {
    super(_simulationTagData);
    this.deleteTag = _deleteTag;
    this._time = _deleteTag.time;
  }

  /**
   * Invoke specific functionality related to <delete> tag
   */
  async actOnTagLogic(): Promise<void> {
    const geometryToDelete = this.simulationTagData.geometriesMap.get(
      this.deleteTag.instanceName,
    );

    if (!geometryToDelete) return;

    console.log(this.deleteTag);

    // Simulate deleting mesh by making it invisible
    geometryToDelete.changeVisibility(0);
  }
}
