import { describe, it, expect, afterEach, vi } from "vitest";
import { UpdateTag } from "../interfaces/updateTag";
import { SimulationUpdateTag } from "./simulationUpdateTag";
import { MachineGeometry } from "../meshGeometry/machineGeometry";
import { PartGeometry } from "../meshGeometry/partGeometry";
import { ConveyorGeometry } from "../meshGeometry/conveyorGeometry";

const mockUpdateTag: UpdateTag = {
  time: 0,
  instanceName: "[130] Treatment(1) - Main Icon",
  translate: {
    x: 40.1875,
    y: 5,
    z: 4.3125,
  },
  scale: {
    x: 5.461,
    y: 4.064,
    z: 2.666999,
  },
  rotate: {
    order: "zxy",
    x: 0,
    y: -90,
    z: 0,
  },
};

const mockUpdateTagPartPosition: UpdateTag = {
  time: 0,
  instanceName: "Box - Entity (67)",
  partPosition: {
    instanceName: "[115] Buffer1(1) - Part Queue",
    position: 0.3,
  },
};

const mockUpdateTagVisible: UpdateTag = {
  time: 0,
  instanceName: "[130] Treatment(1) - Main Icon",
  visible: false,
};

/**
 * Mock instance of mesh geometry class
 */
function mockMeshGeometry() {
  return {
    setPosition: vi.fn(),
    setScaling: vi.fn(),
    setRotation: vi.fn(),
    changeVisibility: vi.fn(),
  };
}

/**
 * Mock instance of part geometry class
 */
function mockPartGeometry() {
  // Purpose: ensure that TypeScript considers it an instanceof 'PartGeometry'
  const mockPartGeometry = Object.create(PartGeometry.prototype);

  Object.assign(mockPartGeometry, {
    setPosition: vi.fn(),
    setScaling: vi.fn(),
    setRotation: vi.fn(),
    changeVisibility: vi.fn(),
  });

  return mockPartGeometry;
}

/**
 * Mock instance of machine geometry class
 */
function mockMachineGeometry() {
  // Purpose: ensure that TypeScript considers it an instanceof 'MachineGeometry'
  const mockMachineGeometry = Object.create(MachineGeometry.prototype);

  Object.assign(mockMachineGeometry, {
    setPosition: vi.fn(),
    setScaling: vi.fn(),
    setRotation: vi.fn(),
    changeVisibility: vi.fn(),
    positionPart: vi.fn(),
  });

  return mockMachineGeometry;
}

/**
 * Mock instance of conveyor geometry class
 */
function mockConveyorGeometry() {
  // Purpose: ensure that TypeScript considers it an instanceof 'ConveyorGeometry'
  const mockConveyorGeometry = Object.create(ConveyorGeometry.prototype);

  Object.assign(mockConveyorGeometry, {
    setPosition: vi.fn(),
    setScaling: vi.fn(),
    setRotation: vi.fn(),
    changeVisibility: vi.fn(),
    positionPart: vi.fn(),
  });

  return mockConveyorGeometry;
}

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), async () => {
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z }));
  const Tools = vi.fn(() => ({}));

  // use 'undefined' to represent meshes
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (Tools as any).ToRadians = vi.fn((inputValue) => inputValue);

  const babylonJSImport = await import("@babylonjs/core");

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (babylonJSImport as any).Vector3 = Vector3;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (babylonJSImport as any).Tools = Tools;

  return { ...babylonJSImport };
});

describe("SimulationUpdateTag class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should translate, scale and rotate as necessary when invoking 'actOnTagLogic'", async () => {
    // Arrange

    // Add mocked instance of 'MeshGeometry' class to mocked map
    const fakeGeometriesMap = new Map();

    const mockedMeshGeometry = mockMeshGeometry();

    fakeGeometriesMap.set(mockUpdateTag.instanceName, mockedMeshGeometry);

    const simulationUpdateTag: SimulationUpdateTag = new SimulationUpdateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { geometriesMap: fakeGeometriesMap } as any,
      mockUpdateTag,
    );

    // Act
    await simulationUpdateTag.actOnTagLogic();

    // Assert 'setPosition' method has been invoked
    expect(mockedMeshGeometry.setPosition).toHaveBeenCalled();
    // Assert 'setScaling' method has been invoked
    expect(mockedMeshGeometry.setScaling).toHaveBeenCalled();
    // Assert 'setRotation' method has been invoked
    expect(mockedMeshGeometry.setRotation).toHaveBeenCalled();
  });

  it("should setVisibility as necessary when invoking 'actOnTagLogic'", async () => {
    // Arrange

    // Add mocked instance of 'MeshGeometry' class to mocked map
    const fakeGeometriesMap = new Map();

    // Create 'mocked' version of 'changeVisibility' method so it can be spied on
    const changeVisibility_mock = vi.fn();

    const mockedMeshGeometry = mockMeshGeometry();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (mockedMeshGeometry as any).changeVisibility = changeVisibility_mock;

    fakeGeometriesMap.set(
      mockUpdateTagVisible.instanceName,
      mockedMeshGeometry,
    );

    const simulationUpdateTag: SimulationUpdateTag = new SimulationUpdateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { geometriesMap: fakeGeometriesMap } as any,
      mockUpdateTagVisible,
    );

    // Act
    await simulationUpdateTag.actOnTagLogic();

    // Assert 'changeVisibility' method on mocked 'MeshGeometry' has been invoked
    expect(changeVisibility_mock).toHaveBeenCalledOnce();
  });

  it("should position part when <update> tag has <partPosition> subtag", async () => {
    // Arrange

    // Add mocked instance of 'MeshGeometry' class to mocked map
    const fakeGeometriesMap = new Map();

    // Mock part
    const mockedPartGeometry = mockPartGeometry();

    fakeGeometriesMap.set(
      mockUpdateTagPartPosition.instanceName,
      mockedPartGeometry,
    );

    // Mock machine geometry

    const mockedMachineGeometry = mockMachineGeometry();

    fakeGeometriesMap.set(
      mockUpdateTagPartPosition.partPosition?.instanceName,
      mockedMachineGeometry,
    );

    const simulationUpdateTag: SimulationUpdateTag = new SimulationUpdateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { geometriesMap: fakeGeometriesMap } as any,
      mockUpdateTagPartPosition,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const handlePartPositioning_spy = vi.spyOn<any, string>(
      simulationUpdateTag,
      "handlePartPositioning",
    );

    // Act
    await simulationUpdateTag.actOnTagLogic();

    // Assert that 'handlePartPositioning' method has been invoked
    expect(handlePartPositioning_spy).toHaveBeenCalled();

    // Assert that 'positionPart' method of mocked machine has been called
    expect(mockedMachineGeometry.positionPart).toHaveBeenCalled();
  });

  it("should call 'positionPart' method on conveyor when <update> tag has <partPosition> subtag", async () => {
    // Arrange

    // Add mocked instance of 'MeshGeometry' class to mocked map
    const fakeGeometriesMap = new Map();

    // Mock part
    const mockedPartGeometry = mockPartGeometry();

    fakeGeometriesMap.set(
      mockUpdateTagPartPosition.instanceName,
      mockedPartGeometry,
    );

    // Mock conveyor geometry

    const mockedConveyorGeometry = mockConveyorGeometry();

    fakeGeometriesMap.set(
      mockUpdateTagPartPosition.partPosition?.instanceName,
      mockedConveyorGeometry,
    );

    const simulationUpdateTag: SimulationUpdateTag = new SimulationUpdateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { geometriesMap: fakeGeometriesMap } as any,
      mockUpdateTagPartPosition,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const handlePartPositioning_spy = vi.spyOn<any, string>(
      simulationUpdateTag,
      "handlePartPositioning",
    );

    // Act
    await simulationUpdateTag.actOnTagLogic();

    // Assert that 'handlePartPositioning' method has been invoked
    expect(handlePartPositioning_spy).toHaveBeenCalled();

    // Assert that 'positionPart' method of mocked conveyor has been called
    expect(mockedConveyorGeometry.positionPart).toHaveBeenCalled();
  });
});
