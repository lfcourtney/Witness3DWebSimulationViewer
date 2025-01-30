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

const validDeleteTag = {
  delete: {
    time: "0",
    instanceName: "Box - Entity (67)",
  },
};

/**
 * Mock instance of 'SimulationTag' super class
 */
function mockSimulationTag() {
  return {
    time: 0,
    actOnTagLogic: vi.fn(),
  };
}

// mock SimulationTag class
vi.mock(import("../simulationTags/simulationTag"), () => {
  const SimulationTag = vi.fn();
  return { SimulationTag };
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

  it("should call 'formatDeleteTag' if create tag is supplied to constructor", () => {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const formatDeleteTag_spy = vi.spyOn<any, string>(
      SimulationContentFormat.prototype,
      "formatDeleteTag",
    );

    // Act by calling constructor
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    new SimulationContentFormat(validDeleteTag, {} as any);

    // Assert 'formatDeleteTag' method has been called
    expect(formatDeleteTag_spy).toHaveBeenCalledOnce();
  });

  it(`should add array containing simulation tag to tag store if simulation tag has a time greater than
     0 when 'populateTimeTagStore' method is called`, () => {
    // Arrange

    // Fake tag store map
    const fakeTimeTagStore = new Map();

    const simulationContentFormat = new SimulationContentFormat(
      validUpdateTag,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { timeTagStore: fakeTimeTagStore } as any,
    );

    const simulationTag_mock = mockSimulationTag();
    simulationTag_mock.time = 0.927536;

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (simulationContentFormat as any).simulationTag = simulationTag_mock;

    // Act
    simulationContentFormat["populateTimeTagStore"]();

    // Assert that fake tag store map has the correct content added to it
    expect(fakeTimeTagStore.get(simulationTag_mock.time.toFixed(2))).toEqual([
      simulationTag_mock,
    ]);
  });

  it(`should add to array containing simulation tag from tag store if simulation tag has a time greater than
    0 when 'populateTimeTagStore' method is called and when array from tag store at the specified time already exists`, () => {
    // Arrange

    // Fake tag store map
    const fakeTimeTagStore = new Map();

    const simulationContentFormat = new SimulationContentFormat(
      validUpdateTag,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { timeTagStore: fakeTimeTagStore } as any,
    );

    const simulationTag_mock = mockSimulationTag();
    simulationTag_mock.time = 0.927536;

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (simulationContentFormat as any).simulationTag = simulationTag_mock;

    // Already add to tag store before 'populateTimeTagStore' method can be called
    fakeTimeTagStore.set(simulationTag_mock.time.toFixed(2), [
      simulationTag_mock,
    ]);

    // Act
    simulationContentFormat["populateTimeTagStore"]();

    // Assert that fake tag store map has the correct content added to it
    expect(fakeTimeTagStore.get(simulationTag_mock.time.toFixed(2))).toEqual([
      simulationTag_mock,
      simulationTag_mock,
    ]);
  });

  it(`should invoke 'actOnTagLogic' method from object assigned to 'simulationTag' field when 'actOnTagLogic' method is called`, () => {
    // Arrange

    const simulationContentFormat = new SimulationContentFormat(
      validUpdateTag,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      {} as any,
    );

    const simulationTag_mock = mockSimulationTag();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (simulationContentFormat as any).simulationTag = simulationTag_mock;

    // Act
    simulationContentFormat.actOnTagLogic();

    // Assert that 'actOnTagLogic' method mock has been called
    expect(simulationTag_mock.actOnTagLogic).toHaveBeenCalled();
  });

  it(`should invoke 'actOnTagLogicAtTimeZero' method from object assigned to 'simulationTag' field when 'actOnTagLogicAtTimeZero' method is called
     but only if the 'simulationTag' time is greater than 0`, async () => {
    // Arrange

    const simulationContentFormat = new SimulationContentFormat(
      validUpdateTag,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      {} as any,
    );

    const simulationTag_mock = mockSimulationTag();
    simulationTag_mock.time = 0.927536;

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (simulationContentFormat as any).simulationTag = simulationTag_mock;

    // Act
    await simulationContentFormat.actOnTagLogicAtTimeZero();

    // Assert that 'actOnTagLogic' method mock has not been called
    expect(simulationTag_mock.actOnTagLogic).not.toHaveBeenCalled();

    simulationTag_mock.time = 0;

    // Act
    await simulationContentFormat.actOnTagLogicAtTimeZero();

    // Assert that 'actOnTagLogic' method mock has not been called
    expect(simulationTag_mock.actOnTagLogic).toHaveBeenCalled();
  });
});
