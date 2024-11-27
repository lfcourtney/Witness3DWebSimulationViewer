import { describe, it, expect, afterEach, vi } from "vitest";
import { UpdateTag } from "../interfaces/updateTag";
import { SimulationUpdateTag } from "./simulationUpdateTag";

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

const mockUpdateTagVisible: UpdateTag = {
  time: 0,
  instanceName: "[130] Treatment(1) - Main Icon",
  visible: false,
};

function mockMeshGeometry() {
  return {
    setPosition: vi.fn(),
    setScaling: vi.fn(),
    setRotation: vi.fn(),
  };
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
});
