import { describe, it, expect, afterEach, vi } from "vitest";
import { SimulationContentFormat } from "./simulationContentFormat";

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
    time: "0",
    instanceName: "[130] Treatment(2) - Main Icon",
    translate: {
      x: "40.1875",
      y: "5",
      z: "8.8125",
    },
    scale: {
      x: "5.461",
      y: "4.064",
      z: "2.666999",
    },
    rotate: {
      x: "5.461",
      y: "4.064",
      z: "2.666999",
    },
    visible: "false",
  },
};

// mock SimulationTagData class
vi.mock(import("../simulationTags/simulationTag"), () => {
  const SimulationTag = vi.fn();
  const SimulationTagData = vi.fn();
  return { SimulationTag, SimulationTagData };
});

describe("SimulationContentFormat class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should call 'formatCreateTag' if create tag is supplied to constructor", () => {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const formatCreateTag_spy = vi.spyOn<any, string>(
      SimulationContentFormat.prototype,
      "formatCreateTag",
    );

    // Act by calling constructor
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    new SimulationContentFormat(validCreateTag, {} as any);

    // Assert 'formatCreateTag' method has been called
    expect(formatCreateTag_spy).toHaveBeenCalledOnce();
  });

  it("should call 'formatUpdateTag' if create tag is supplied to constructor", () => {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const formatUpdateTag_spy = vi.spyOn<any, string>(
      SimulationContentFormat.prototype,
      "formatUpdateTag",
    );

    // Act by calling constructor
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    new SimulationContentFormat(validUpdateTag, {} as any);

    // Assert 'formatUpdateTag' method has been called
    expect(formatUpdateTag_spy).toHaveBeenCalledOnce();
  });
});
