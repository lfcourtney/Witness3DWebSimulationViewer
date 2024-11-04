import { describe, it, expect, beforeAll } from "vitest";
import { SimulationContentFormat } from "./simulationContentFormat";

let simulationContentFormat: SimulationContentFormat;

const validCreateTag = {
  create: {
    time: "0.000000",
    geometry:
      "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dg-vh-Agv1",
    instanceName: "[112] Weigh(1) - Main Icon",
  },
};

describe("SimulationContentFormat class", () => {
  beforeAll(() => {
    simulationContentFormat = new SimulationContentFormat();
  });

  it("should format valid create tag appropriately", () => {
    // Act
    const returnCreateTag =
      simulationContentFormat.formatCreateTag(validCreateTag);

    // Assert that return create tag is not undefined
    expect(returnCreateTag).toBeTruthy();

    // Assert that return create tag has its time parsed as a number
    expect(returnCreateTag?.create.time).toBe(0);
  });
});
