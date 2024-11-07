import { CreateTag } from "../interfaces/createTag";
import { UpdateTag } from "../interfaces/updateTag";
import {
  SimulationTag,
  SimulationTagData,
} from "../simulationTags/simulationTag";
import { SimulationCreateTag } from "../simulationTags/simulationCreateTag";
import { SimulationUpdateTag } from "../simulationTags/simulationUpdateTag";

/**
 * Class responsible for checking the format of a generic tag, converting this tag to its specific type (eg, <create>, <update>, etc.)
 * and invoking the specific functionality of the converted tag.
 * For instance, through this class, different functionality will be delegated to the different types of tags, including <create>, <update>, etc.
 */
export class SimulationContentFormat {
  private readonly simulationTag?: SimulationTag;

  /**
   * Create class responsible for working with specific functionality of a generic tag
   * @param tagObj Generic tag
   * @param simulationTagData Data from Babylon.js scene
   */
  constructor(tagObj: object, simulationTagData: SimulationTagData) {
    // possibly a <create> tag
    const createTag = this.formatCreateTag(tagObj);

    if (createTag) {
      this.simulationTag = new SimulationCreateTag(
        simulationTagData,
        createTag.create,
      );
      return;
    }

    // possibly an <update> tag
    const updateTag = this.formatUpdateTag(tagObj);

    if (updateTag) {
      this.simulationTag = new SimulationUpdateTag(
        simulationTagData,
        updateTag.update,
      );
      return;
    }
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

    // Ensure that translate tag is formatted correctly
    if (definiteUpdateTag.update.translate) {
      this.parseObjectNumbersAndBooleans(definiteUpdateTag.update.translate);
    }

    // Ensure that scale tag is formatted correctly
    if (definiteUpdateTag.update.scale) {
      this.parseObjectNumbersAndBooleans(definiteUpdateTag.update.scale);
    }

    // Ensure that rotate tag is formatted correctly
    if (definiteUpdateTag.update.rotate) {
      this.parseObjectNumbersAndBooleans(definiteUpdateTag.update.rotate);
    }

    // Ensure numbers and booleans are formatted correctly
    this.parseObjectNumbersAndBooleans(definiteUpdateTag.update);

    return definiteUpdateTag;
  }

  /**
   * Takes in a generic object and loops through its key value pairs. For each string value,
   * it will convert the value to a number or boolean providing that the given string value resembles a number or boolean
   * @param parseObject The generic object to work with
   * @returns The inputted object with the strings resembling numbers converted to numbers and the strings resembling booleans converted to booleans
   */
  private parseObjectNumbersAndBooleans(parseObject: object): object {
    for (const key in parseObject) {
      if (typeof parseObject[key] === "string") {
        //#region number check
        const parseNumber = +parseObject[key];
        if (!isNaN(parseNumber)) {
          parseObject[key] = parseNumber;
          continue;
        }
        //#endregion

        //#region boolean check
        if (parseObject[key].toLowerCase() === "true") {
          parseObject[key] = true;
          continue;
        }

        if (parseObject[key].toLowerCase() === "false") {
          parseObject[key] = false;
          continue;
        }
        //#endregion
      }
    }
    return parseObject;
  }
}
