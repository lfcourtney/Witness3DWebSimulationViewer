import { XMLParser } from "fast-xml-parser";
import { describe, expect, it, beforeAll } from "vitest";
import { SimulationContents } from "./simulationContent";

// Example contents of w3d file
const exampleXMLStructure = `<start type="session" />
	<start time="0.000000" type="build" />
	<version number="1" />
	<new model="C:\\Dev\\Witness3DWebSimulationViewer\\WitnessModels\\Quick3D.mod" />
  <create time="0.000000" geometry="C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dg-vh-Agv1" instanceName="[112] Weigh(1) - Main Icon">
		<queueInfo queueParent="dg-vh-Agv1">
			<behaviour partPositioning="partOver" partRoll="0.000000" partPitch="0.000000" partYaw="0.000000" />
			<position x="0.000000" y="0.300000" z="-0.500000" />
			<direction dx="0.000000" dy="0.000000" dz="1.000000" />
		</queueInfo>
	</create>`;
let simulationContents: SimulationContents;

const instantiationErrorMessage =
  "'_tagStore' argument is not an array of XML parsed JavaScript objects";
const formatTagErrorMessage =
  "inputted tag is not formatted correctly as per 'fast-xml-parser' npm dependency";

describe("test SimulationContents class", () => {
  beforeAll(() => {
    const options = {
      ignoreAttributes: false, // To ensure attributes are parsed
      attributeNamePrefix: "", // To avoid prefixing attribute names
      preserveOrder: true,
    };
    const parser = new XMLParser(options);

    const arrayOfParsedTags = parser.parse(exampleXMLStructure);

    simulationContents = new SimulationContents(arrayOfParsedTags);
  });

  it(`throws appropriate exception if SimulationContents is attempted to be instantiated with an array that is not an object`, () => {
    const incorrectTagStore = [{}];

    // Assert exception is thrown during instantiation
    expect(() => {
      new SimulationContents(incorrectTagStore);
    }).toThrowError(instantiationErrorMessage);
  });

  it(`returns the correct tag name when 'tagName' method is called`, () => {
    const createTag = simulationContents.tagStore[4];

    // Assert tagName is 'create'
    expect(simulationContents.tagName(createTag)).toBe("create");
  });

  it(`returns undefined when 'tagName' method is called with incorrectly formatted tag`, () => {
    const incorrectlyFormattedObject = { test: null };

    // Assert tagName is undefined
    expect(simulationContents.tagName(incorrectlyFormattedObject)).toBe(
      undefined,
    );
  });

  it(`throws exception if 'formatTag' method is invoked with incorrectly formatted tag`, () => {
    const incorrectlyFormattedObject = { test: null };

    // Assert 'formatTag' method throws proper exception
    expect(() =>
      simulationContents.formatTag(incorrectlyFormattedObject),
    ).toThrowError(formatTagErrorMessage);
  });

  it(`formats tag if 'formatTag' method is supplied with a properly formatted tag`, () => {
    const createTag = simulationContents.tagStore[4];

    // Assert that original tag object has more than one parent field, one for contents and the attributes
    expect(Object.keys(createTag).length).toBe(2);

    // Act
    const createTagFormatted = simulationContents.formatTag(createTag);

    // Assert that new object only has one parent field named create
    expect(Object.keys(createTagFormatted).length).toBe(1);
    expect(Object.keys(createTagFormatted)[0]).toBe("create");
  });
});
