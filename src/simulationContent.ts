/**
 * Class responsible for allowing access to simulation information from uploaded w3d file and formatting this information.
 */
export class SimulationContents {
  /**
   * The current tag in the tagStore array when looping through it
   * @type {number}
   */
  private tagIndex: number = 0;
  private readonly tagStore: [object];

  /**
   * Create the object that will format and provide access for simulation contents of w3d file
   * @param _tagStore array of w3d tags formatted as JavaScript objects preserving the order of the tags in the original file
   */
  constructor(_tagStore: [object]) {
    if (!this.isTagStoreArray(_tagStore)) {
      throw new Error(
        "'_tagStore' argument is not an array of JavaScript objects",
      );
    }

    this.tagStore = _tagStore;
  }

  /**
   * Returns the tag name of the inputted JavaScript object, formatted
   * from the output of 'fast-xml-parser'.
   * @param tag Individual JavaScript object element from 'tagStore'
   * @returns The type of command that the inputted object represents. Or null
   * if the inputted JavaScript object is not formatted properly.
   */
  tagName(tag: object): string | undefined {
    const keys: string[] = Object.keys(tag);

    const indexOfAttributes = keys.indexOf(":@");

    if (indexOfAttributes == -1) return undefined;

    const indexOfTagName = keys.find((key) =>
      key.match(/create|delete|end|load|new|start|update|version/),
    );

    return indexOfTagName;
  }

  private isTagStoreArray(_tagStore: unknown): _tagStore is object[] {
    return (
      Array.isArray(_tagStore) &&
      _tagStore.every((item) => typeof item === "object" && item !== null)
    );
  }
}
