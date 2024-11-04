import { SimulationContentFormat } from "./simulationContentFormat";

/**
 * Class responsible for allowing access to simulation information from uploaded w3d file and formatting this information.
 */
export class SimulationContents {
  /**
   * The current tag in the tagStore array when looping through it
   * @type {number}
   */
  private tagIndex: number = 0;
  private readonly _tagStore: object[];

  private readonly _simulationContentFormat: SimulationContentFormat;

  /**
   * Create the object that will format and provide access for simulation contents of w3d file
   * @param _tagStore array of w3d tags formatted as JavaScript objects preserving the order of the tags in the original file
   */
  constructor(_tagStore: object[]) {
    if (!this.isTagStoreArray(_tagStore)) {
      throw new Error(
        "'_tagStore' argument is not an array of XML parsed JavaScript objects",
      );
    }

    this._tagStore = _tagStore;
    this._simulationContentFormat = new SimulationContentFormat();
  }

  public get tagStore() {
    return this._tagStore;
  }

  public get simulationContentFormat() {
    return this._simulationContentFormat;
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
      key.match(
        /create|delete|end|load|new|start|update|version|translate|rotate|scale|queueInfo|behaviour|position|direction|path|line|arc|extrude|surface|partPosition/,
      ),
    );

    return indexOfTagName;
  }

  /**
   * Extract the name of the geometry from the 'geometry' attribute of a create tag
   * @param geometry The 'geometry' attribute of a create tag
   * @returns The name of the geometry if there is a valid geometry contained in the string. Or undefined if a valid geometry was unable to be undefined.
   */
  extractGeometry(geometry: string): string | undefined {
    const geometryRegex =
      /(dg-ic-Machine1|dg-ic-Machine1|dg-ic-WaterTank|dg-ic-Workbench2|dg-lq-Machine|dg-pt-ManWalking1|dg-pt-Part1|dg-vh-Agv1|dgu-pa-Conveyor5|dgu-pa-Conveyor6|dgu-pa-Track)(\.glb)?$/;

    const matchGeometry = geometryRegex.exec(geometry);

    if (!matchGeometry || matchGeometry?.length < 2) return undefined;

    return matchGeometry[1];
  }

  /**
   * Takes a JavaScript object produced by 'fast-xml-parser' parsing XML tag, where attributes and tag contents are separate, 
   and reformats this object to make the attribute contents and tag contents part of the same object. For extra information,
   this is the same format that would be produced by the 'fast-xml-parser' if the 'preserveOrder' option was not enabled, 
   which it is for this application
   * @param tag Individual JavaScript object element from 'tagStore'
   * @returns The formatted JavaScript object, where attributes and tag contents are part of the same object.
   */
  formatTag(tag: object): object {
    const tagName = this.tagName(tag);

    if (tagName === undefined) {
      throw new Error(
        "inputted tag is not formatted correctly as per 'fast-xml-parser' npm dependency",
      );
    }

    const tagAttributes = tag[":@"];

    // We assume that the tag name field holds an array as its value.
    // For the reason why, this is the standard format when 'fast-xml-parser'
    // parses xml content with the 'preserveOrder' option enabled, which it is in this case
    const tagArray = tag[tagName];

    let tagObject = {};

    if (Array.isArray(tagArray)) {
      tagArray.forEach((tagIter) => {
        tagObject = { ...tagObject, ...this.formatTag(tagIter) };
      });
    }

    return {
      [tagName]: { ...tagAttributes, ...tagObject },
    };
  }

  private isTagStoreArray(_tagStore: object[]): boolean {
    return _tagStore.every((tag) => this.tagName(tag) !== undefined);
  }
}
