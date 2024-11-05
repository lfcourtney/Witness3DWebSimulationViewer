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

const validUpdateTag = {
  update: {
    time: "0.000000",
    instanceName: "AGV(1) - Entity (88)",
    visible: "false",
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

  it("should format valid update tag appropriately", () => {
    // Act
    const returnUpdateTag =
      simulationContentFormat.formatUpdateTag(validUpdateTag);

    // Assert that return update tag is not undefined
    expect(returnUpdateTag).toBeTruthy();

    // Assert that update tag has its visible field parsed as a boolean
    expect(returnUpdateTag?.update.visible).toBe(false);

    // Assert that update update tag has its time parsed as a number
    expect(returnUpdateTag?.update.time).toBe(0);
  });
});
