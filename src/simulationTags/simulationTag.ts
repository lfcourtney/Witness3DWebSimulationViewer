import { SimulationTagData } from "../interfaces/simulationTagData";

/**
 * Abstract class responsible for describing the format through which the functionality of a specific tag type will be invoked.
 * That means each derived class of this abstract class will represent a specific type of tag (<create>, <update>, etc.),
 * with each type of class having their own unique functionality.
 */
export abstract class SimulationTag {
  protected readonly simulationTagData: SimulationTagData;

  // Every parent tag should have a time attribute
  protected _time: number | undefined;

  /**
   * Create the generic abstract representation of the tag functionality.
   * @param _simulationTagData Data from Babylon.js scene
   */
  constructor(_simulationTagData: SimulationTagData) {
    this.simulationTagData = _simulationTagData;
  }

  /**
   * Returns the value of the 'time' attribute of the tag an instance of this class is encapsulating
   */
  public get time() {
    return this._time;
  }

  /**
   * The generic abstract method that represents the unique functionality to be invoked of the derived class.
   */
  abstract actOnTagLogic(): Promise<void>;
}
