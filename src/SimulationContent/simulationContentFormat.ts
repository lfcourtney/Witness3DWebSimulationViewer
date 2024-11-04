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

    if (!hasEssentialFields) return undefined;

    const definiteCreateTag = possibleCreateTag as { create: CreateTag };

    // Cast time to be a number
    definiteCreateTag.create.time = +definiteCreateTag.create.time;

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

    if (!hasEssentialFields) return undefined;

    const definiteUpdateTag = possibleUpdateTag as { update: UpdateTag };

    // Cast time to be a number
    definiteUpdateTag.update.time = +definiteUpdateTag.update.time;

    return definiteUpdateTag;
  }
}
