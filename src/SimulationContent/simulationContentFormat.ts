import { CreateTag } from "../interfaces/createTag";
import { UpdateTag } from "../interfaces/updateTag";

/**
 * Class responsible for checking formatted content of tags from w3d file and returning these objects in their correct TypeScript interface
 */
export class SimulationContentFormat {
  /**
   * Assigns formatted tag to the 'CreateTag' TypeScript interface if object is formatted appropriately. Otherwise, returns undefined.
   * @param possibleCreateTag Object that should be formatted as a 'UpdateTag'
   * @returns Tag formatted as 'CreateTag'. Otherwise, undefined if tag was not formatted as a 'UpdateTag'.
   */
  formatCreateTag(
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
  formatUpdateTag(
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
