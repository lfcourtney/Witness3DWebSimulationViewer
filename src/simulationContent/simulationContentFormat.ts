import { CreateTag } from "../interfaces/createTag";
import { UpdateTag } from "../interfaces/updateTag";
import { SimulationTag } from "../simulationTags/simulationTag";
import { SimulationTagData } from "../interfaces/simulationTagData";
import { SimulationCreateTag } from "../simulationTags/simulationCreateTag";
import { SimulationUpdateTag } from "../simulationTags/simulationUpdateTag";
import { DeleteTag } from "../interfaces/deleteTag";
import { SimulationDeleteTag } from "../simulationTags/SimulationDeleteTag";

/**
 * Class responsible for checking the format of a generic tag, converting this tag to its specific type (eg, \<create\>, \<update\>, etc.)
 * and invoking the specific functionality of the converted tag.
 * For instance, through this class, different functionality will be delegated to the different types of tags, including \<create\>, \<update\>, etc.
 */
export class SimulationContentFormat {
  private readonly simulationTagData: SimulationTagData;

  private readonly simulationTag: SimulationTag | undefined;

  /**
   * Create class responsible for working with specific functionality of a generic tag
   * @param tagObj Generic tag
   * @param simulationTagData Data from Babylon.js scene
   */
  constructor(tagObj: object, _simulationTagData: SimulationTagData) {
    this.simulationTagData = _simulationTagData;

    // possibly a <create> tag
    const createTag = this.formatCreateTag(tagObj);

    if (createTag) {
      this.simulationTag = new SimulationCreateTag(
        _simulationTagData,
        createTag.create,
      );
      this.populateTimeTagStore();
      return;
    }

    // possibly an <update> tag
    const updateTag = this.formatUpdateTag(tagObj);

    if (updateTag) {
      this.simulationTag = new SimulationUpdateTag(
        _simulationTagData,
        updateTag.update,
      );
      this.populateTimeTagStore();
      return;
    }

    // possibly a <delete> tag
    const deleteTag = this.formatDeleteTag(tagObj);

    if (deleteTag) {
      this.simulationTag = new SimulationDeleteTag(
        _simulationTagData,
        deleteTag.delete,
      );
      this.populateTimeTagStore();
      return;
    }
  }

  /**
   * Storing tags so that they can be accessed in animation loop, store tag in 'timeTagStore' Map,
   * using the 'time' attribute of tag as the key in the key value pair added to the Map.
   */
  private populateTimeTagStore(): void {
    // If time is 0 or undefined return
    if (!this.simulationTag?.time) return;
    const parseTime = this.simulationTag.time.toFixed(2);
    const timeTagArray = this.simulationTagData.timeTagStore.get(parseTime);

    // If array does not already exist, create array with simulation tag as the initial element
    if (!timeTagArray) {
      this.simulationTagData.timeTagStore.set(parseTime, [this.simulationTag]);
      return;
    }

    timeTagArray.push(this.simulationTag);
  }

  /**
   * Invoke asynchronous functionality related to specific tag
   */
  async actOnTagLogic(): Promise<void> {
    if (this.simulationTag) {
      await this.simulationTag.actOnTagLogic();
    }
  }

  /**
   * Invoke asynchronous functionality related to specific tag but only if the tag is at time zero
   */
  async actOnTagLogicAtTimeZero(): Promise<void> {
    if (this.simulationTag?.time !== 0) return;
    await this.simulationTag.actOnTagLogic();
  }

  /**
   * Assigns formatted tag to the 'CreateTag' TypeScript interface if object is formatted appropriately. Otherwise, returns undefined.
   * @param possibleCreateTag Object that should be formatted as a 'UpdateTag'
   * @returns Tag formatted as 'CreateTag'. Otherwise, undefined if tag was not formatted as a 'UpdateTag'.
   */
  private formatCreateTag(
    possibleCreateTag: object,
  ): { create: CreateTag } | undefined {
    const createObj = possibleCreateTag["create"];

    if (!createObj) return undefined;

    const possibleCreateTagKeys = Object.keys(createObj);

    const hasEssentialFields: boolean = ["time", "instanceName"].every(
      (essentialField) =>
        possibleCreateTagKeys.indexOf(essentialField) !== -1 &&
        typeof createObj[essentialField] === "string",
    );

    // Does not include necessary fields, so return undefined
    if (!hasEssentialFields) return undefined;

    const definiteCreateTag = possibleCreateTag as { create: CreateTag };

    // Ensure numbers and booleans are formatted correctly
    this.parseObjectNumbersAndBooleans(definiteCreateTag.create);

    return definiteCreateTag;
  }

  /**
   * Assigns formatted tag to the 'UpdateTag' TypeScript interface if object is formatted appropriately. Otherwise, returns undefined.
   * @param possibleUpdateTag Object that should be formatted as a 'UpdateTag'
   * @returns Tag formatted as 'UpdateTag'. Otherwise, undefined if tag was not formatted as a 'UpdateTag'.
   */
  private formatUpdateTag(
    possibleUpdateTag: object,
  ): { update: UpdateTag } | undefined {
    const updateObj = possibleUpdateTag["update"];

    if (!updateObj) return undefined;

    const possibleUpdateTagKeys = Object.keys(updateObj);

    const hasEssentialFields: boolean = ["time", "instanceName"].every(
      (essentialField) =>
        possibleUpdateTagKeys.indexOf(essentialField) !== -1 &&
        typeof updateObj[essentialField] === "string",
    );

    // Does not include necessary fields, so return undefined
    if (!hasEssentialFields) return undefined;

    const definiteUpdateTag = possibleUpdateTag as { update: UpdateTag };

    // Ensure numbers and booleans are formatted correctly
    this.parseObjectNumbersAndBooleans(definiteUpdateTag.update);

    return definiteUpdateTag;
  }

  /**
   * Assigns formatted tag to the 'DeleteTag' TypeScript interface if object is formatted appropriately. Otherwise, returns undefined.
   * @param possibleDeleteTag Object that should be formatted as a 'DeleteTag'
   * @returns Tag formatted as 'DeleteTag'. Otherwise, undefined if tag was not formatted as a 'DeleteTag'.
   */
  private formatDeleteTag(
    possibleDeleteTag: object,
  ): { delete: DeleteTag } | undefined {
    const deleteObj = possibleDeleteTag["delete"];

    if (!deleteObj) return undefined;

    const possibleDeleteTagKeys = Object.keys(deleteObj);

    const hasEssentialFields: boolean = ["time", "instanceName"].every(
      (essentialField) =>
        possibleDeleteTagKeys.indexOf(essentialField) !== -1 &&
        typeof deleteObj[essentialField] === "string",
    );

    // Does not include necessary fields, so return undefined
    if (!hasEssentialFields) return undefined;

    const definiteDeleteTag = possibleDeleteTag as { delete: DeleteTag };

    // Ensure numbers and booleans are formatted correctly
    this.parseObjectNumbersAndBooleans(definiteDeleteTag.delete);

    return definiteDeleteTag;
  }

  /**
   * Takes in a generic object and loops through its key value pairs. For each string value,
   * it will convert the value to a number or boolean providing that the given string value resembles a number or boolean
   * @param parseObject The generic object to work with
   * @returns The inputted object with the strings resembling numbers converted to numbers and the strings resembling booleans converted to booleans
   */
  private parseObjectNumbersAndBooleans(parseObject: object): object {
    if (typeof parseObject !== "object") return parseObject;

    for (const key in parseObject) {
      /**
       * Use recursion to search through and if necessary parse
       * all child objects.
       */
      if (typeof parseObject[key] === "object") {
        this.parseObjectNumbersAndBooleans(parseObject[key]);
        continue;
      }

      if (typeof parseObject[key] === "string") {
        /**************************************************
         *             Parse String to Number            *
         **************************************************/
        // Result will be 'NaN' if string is not actually a number
        const parseNumber = +parseObject[key];
        if (!isNaN(parseNumber)) {
          parseObject[key] = parseNumber;
          continue;
        }

        /**************************************************
         *             Parse String to Boolean            *
         **************************************************/
        if (parseObject[key].toLowerCase() === "true") {
          parseObject[key] = true;
          continue;
        }

        if (parseObject[key].toLowerCase() === "false") {
          parseObject[key] = false;
          continue;
        }
      }
    }
    return parseObject;
  }
}
