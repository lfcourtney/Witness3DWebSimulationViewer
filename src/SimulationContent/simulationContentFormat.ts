import { CreateTag } from "../interfaces/create";

/**
 * Class responsible for checking formatted content of tags from w3d file and returning these objects in their correct TypeScript interface
 */
export class SimulationContentFormat {
  /**
   * Takes in a formatted tag. Returns this tag formatted as the CreateTag TypeScript interface providing that the object has the correct formatting
   * @param possibleCreateTag Object that should be formatted as a create tag
   * @returns Tag formatted as CreateTag. Otherwise, undefined if tag was not formatted as a create tag.
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
}
