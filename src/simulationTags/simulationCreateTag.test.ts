import { describe, it, expect, afterEach, vi } from "vitest";
import { CreateTag } from "../interfaces/createTag";
import { SimulationCreateTag } from "./simulationCreateTag";

const mockCreateTag: CreateTag = {
  time: 0,
  geometry:
    "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dg-vh-Agv1",
  instanceName: "AGV(1) - Entity (88)",
};

const mockCreateTagQueueInfo: CreateTag = {
  time: 0,
  geometry:
    "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Imported Assets\\Shapes\\QueuePlaceholder",
  instanceName: "[115] Buffer1(1) - Part Queue",
  queueInfo: {
    queueParent: "QueuePlaceholder",
    behaviour: {
      partPositioning: "partOver",
      partRoll: 0,
      partPitch: 0,
      partYaw: 0,
    },
    position: {
      x: 0,
      y: 0,
      z: 0,
    },
    direction: {
      dx: 0,
      dy: 0,
      dz: 1,
    },
  },
};

const mockCreateTagPart: CreateTag = {
  time: 0,
  geometry:
    "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dg-pt-Part1",
  instanceName: "Box - Entity (67)",
  queueInfo: {
    queueParent: "dg-pt-Part1",
    behaviour: {
      partPositioning: "partOver",
      partRoll: 0,
      partPitch: 0,
      partYaw: 0,
    },
    position: {
      x: 0,
      y: 0,
      z: 0,
    },
    direction: {
      dx: 0,
      dy: 0,
      dz: 1,
    },
  },
};

// Example create tag that is designed for the rendering of floors
const mockFloorCreateTag: CreateTag = {
  time: 0,
  instanceName: "Floor 1",
  surface: {
    x1: 14.135,
    y1: 0,
    z1: 1.76,
    x2: 48.365,
    y2: 0,
    z2: 30.74,
    normalX: 0,
    normalY: 1,
    normalZ: 0,
    color1: "#e8e8e8ff",
    color2: "#f3f3f3ff",
    singleSided: true,
  },
};

const mockGeometryName = "dgu-pa-Conveyor6";

// Example create tag for the creation of a conveyor
const mockCreateTagConveyor: CreateTag = {
  time: 0,
  geometry:
    "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dgu-pa-Conveyor6",
  instanceName: "[116] Conv2(1) - Path",
  queueInfo: {
    queueParent: "dgu-pa-Conveyor6",
    behaviour: {
      partPositioning: "partOver",
      partRoll: 0,
      partPitch: 0,
      partYaw: 0,
    },
    position: {
      x: 0,
      y: 0.25,
      z: -0.79,
    },
    direction: {
      dx: 0,
      dy: 0,
      dz: 1,
    },
  },
  path: {
    startX: 18.25,
    startY: 0,
    startZ: 27.3125,
    width: 0.75,
    path: [
      {
        line: {
          startX: 18.25,
          startY: 0,
          startZ: 27.3125,
          endX: 18.25,
          endY: 0,
          endZ: 20.4375,
        },
      },
      {
        arc: {
          startX: 18.25,
          startY: 0,
          startZ: 20.4375,
          endX: 23.25,
          endY: 0,
          endZ: 15.4375,
          centreX: 23.25,
          centreY: 0,
          centreZ: 20.4375,
          angle: 90,
          sweepDirection: "clockwise",
        },
      },
      {
        line: {
          startX: 23.25,
          startY: 0,
          startZ: 15.4375,
          endX: 28.25,
          endY: 0,
          endZ: 15.4375,
        },
      },
    ],
  },
};

/**
 * Mock instance of 'MeshGeometry' class
 */
function createMockMeshGeometry() {
  return {
    setParent: vi.fn(),
    // Mock the cloning of a mesh
    clone: vi.fn(() => createMockMeshGeometry()),
    setPosition: vi.fn(),
    getEffectivePartHeight: vi.fn(),
    setPartOrConveyorRotation: vi.fn(),
    changeVisibility: vi.fn(),
  };
}

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  const mockAbstractMesh = { name: null };

  // use 'undefined' to represent meshes
  const ImportMeshAsync = vi.fn(() => ({
    meshes: [mockAbstractMesh, mockAbstractMesh],
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const MeshBuilder = vi.fn(() => ({})) as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (MeshBuilder as any).CreateGround = vi.fn(() => ({
    position: {
      x: undefined,
      y: undefined,
      z: undefined,
    },
  }));

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));

  return { ImportMeshAsync, MeshBuilder, Vector3 };
});

// mock MachineGeometry
vi.mock(import("../meshGeometry/machineGeometry"), () => {
  // Mocked implementation has 'machineGeometry' property so that we know a 'machineGeometry' specifically
  // has been initialised and not some other type of mesh.

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const MachineGeometry = vi.fn(() => ({ machineGeometry: true })) as any;

  return { MachineGeometry };
});

// mock ConveyorGeometry
vi.mock(import("../meshGeometry/conveyorGeometry"), () => {
  // Mocked implementation has 'conveyorGeometry' property so that we know a 'ConveyorGeometry' specifically
  // has been initialised and not some other type of mesh.

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const ConveyorGeometry = vi.fn(() => ({ conveyorGeometry: true })) as any;

  return { ConveyorGeometry };
});

// mock ConveyorBuilder
vi.mock(import("../meshBuilder/conveyorBuilder"), () => {
  const ConveyorBuilder = vi.fn(() => ({
    buildConveyor: vi.fn(() => true),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  return { ConveyorBuilder };
});

describe("SimulationCreateTag class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should successfully 'extractGeometry' if supplied with a correctly formatted 'geometry' attribute of a <create> tag", () => {
    // Arrange
    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      {} as any,
      mockCreateTag,
    );

    // Act
    const returnedGeometry = simulationCreateTag["extractGeometry"](
      mockCreateTag.geometry!,
    );

    // Assert geometry was successfully returned
    expect(returnedGeometry).toBe("dg-vh-Agv1");
  });

  it("should call 'importMeshSuccess' when imvoking 'actOnTagLogic' and it is necessary to handle creation of geometry", async () => {
    // Arrange

    const fakeGeometriesMap = new Map();

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTag,
    );

    // Mock this method to return false so that 'renderPartOrMachine' can be invoked
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      simulationCreateTag,
      "actOnExistingMeshType",
    ).mockImplementationOnce(() => false);

    const importMeshSuccess_spy = vi
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      .spyOn<any, string>(simulationCreateTag, "importMeshSuccess");

    // Act
    await simulationCreateTag.actOnTagLogic();

    // Assert 'importMeshSuccess' has been invoked
    expect(importMeshSuccess_spy).toHaveBeenCalledOnce();
  });

  it(`Should set the visibility of a mesh geometry to visible if model has already been created in the simulation. 
    Then no more execution of the source code should occur.`, async () => {
    // Arrange

    const fakeGeometriesMap = new Map();

    const mockMeshGeometry = createMockMeshGeometry();

    // 'instanceName' key value should align with the 'instanceName' of the current create tag; otherwise it will not be able to be fetched from the map
    fakeGeometriesMap.set(mockCreateTag.instanceName, mockMeshGeometry);

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTag,
    );

    // Spy on 'renderPartOrMachine' method because this method will be invoked if the 'actOnTagLogic'
    // method did not return as soon as it changed the visibility of the found mesh as it should
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const actOnExistingMeshType_spy = vi.spyOn<any, string>(
      simulationCreateTag,
      "actOnExistingMeshType",
    );

    // Act
    await simulationCreateTag.actOnTagLogic();

    // Assert 'changeVisibility' has not been invoked to set the mesh geometry to be visible
    expect(mockMeshGeometry.changeVisibility).toHaveBeenCalledWith(1);
    // Assert 'actOnExistingMeshType' has not been invoked
    expect(actOnExistingMeshType_spy).not.toHaveBeenCalledOnce();
  });

  it("should log error if contents of 'renderPartOrMachine' method throws error", async () => {
    // Arrange
    const consoleErrorMock = vi
      .spyOn(console, "error")
      .mockImplementation(() => undefined);

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: {
          set: vi.fn(),
        },
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTag,
    );

    const mockImportMeshError = new Error("Mock Import Mesh Error");

    /**
     * Mock 'importMeshSuccess' so that it throws error
     */
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      simulationCreateTag,
      "importMeshSuccess",
    ).mockImplementationOnce(() => {
      throw mockImportMeshError;
    });

    const mockGeometryName = mockCreateTag.geometry || "";

    // Act
    await simulationCreateTag["renderPartOrMachine"](mockGeometryName);

    // Assert 'console.error' has been invoked
    expect(consoleErrorMock).toHaveBeenCalledWith(
      "Error loading mesh:",
      mockImportMeshError,
    );
  });

  it("should create 'MachineGeometry' 'SimulationTag' object if <create> tag contains <queueInfo> sub tag", async () => {
    // Arrange

    // Mock geometries map
    const fakeGeometriesMap = new Map();

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTagQueueInfo,
    );

    // Act
    await simulationCreateTag.actOnTagLogic();

    // Assert that instance of 'MachineGeometry' has been added to simulation tag data
    expect(
      fakeGeometriesMap.get(mockCreateTagQueueInfo.instanceName)
        .machineGeometry,
    ).toBe(true);
  });

  it(`should throw appropriate error if 'renderPathOrConveyor' method is invoked with a create tag that does not have
    <path> sub tag`, () => {
    // Arrange

    // Mock geometries map
    const fakeGeometriesMap = new Map();

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTagPart,
    );

    const unableToCreatePathOrConveyorError =
      "Unable to create instance of path or conveyor: <path> sub tag was not defined";

    // Assert that appropriate error is thrown
    expect(() => {
      // Act
      simulationCreateTag["renderPathOrConveyor"](mockGeometryName);
    }).toThrowError(unableToCreatePathOrConveyorError);
  });

  it(`should throw appropriate error if 'importMeshSuccess' method is invoked with an empty array as the first argument`, () => {
    // Arrange

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: {
          set: vi.fn(),
        },
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTagPart,
    );

    const unableToImportMeshError =
      "Unable to import mesh: array of meshes was empty";

    // Assert that appropriate error is thrown given an empty array as the first argument
    expect(() => {
      // Act
      simulationCreateTag["importMeshSuccess"]([], mockGeometryName);
    }).toThrowError(unableToImportMeshError);
  });

  it(`should create 'ConveyorGeometry' 'SimulationTag' object if <create> tag contains a <path> sub tag for drawing shape of conveyor or path`, () => {
    // Arrange

    // Mock geometries map
    const fakeGeometriesMap = new Map();

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTagConveyor,
    );

    // Act
    simulationCreateTag["renderPathOrConveyor"](mockGeometryName);

    // Assert that instance of 'ConveyorGeometry' has been added to simulation tag data
    expect(
      fakeGeometriesMap.get(mockCreateTagConveyor.instanceName)
        .conveyorGeometry,
    ).toBe(true);
  });

  it("should call 'renderGround' when imvoking 'actOnTagLogic' and it is necessary to render ground", async () => {
    // Arrange
    // Mock geometries map
    const fakeGeometriesMap = new Map();

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockFloorCreateTag,
    );

    const renderGround_spy = vi
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      .spyOn<any, string>(simulationCreateTag, "renderGround");

    // Act
    await simulationCreateTag.actOnTagLogic();

    // Assert 'renderGround' has been invoked
    expect(renderGround_spy).toHaveBeenCalledOnce();
  });

  it(`should return true to indicate that a new mesh has been created via cloning an existing abstract mesh when 'actOnExistingMeshType' 
    method is invoked and a mesh using the corresponding model has already been imported`, () => {
    // Arrange

    // Mock geometries map
    const fakeGeometriesMap = new Map();

    const mockMeshGeometry = createMockMeshGeometry();

    //Requires matching model type (as identified via this 'geometryName' property) via the new mesh that needs to be created.
    // Otherwise, this mocked mesh will never be used to clone the new mesh
    mockMeshGeometry["geometryName"] = mockGeometryName;

    // Add mesh that already exists
    fakeGeometriesMap.set(mockGeometryName, mockMeshGeometry);

    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: fakeGeometriesMap,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTagQueueInfo,
    );

    // Act
    const didCloneMesh: boolean =
      simulationCreateTag["actOnExistingMeshType"](mockGeometryName);

    // Assert that return value of 'actOnExistingMeshType' method is true
    expect(didCloneMesh).toBe(true);
  });
});
