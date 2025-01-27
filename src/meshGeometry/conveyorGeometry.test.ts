import { describe, it, expect, afterEach, vi } from "vitest";

import { ConveyorGeometry } from "./conveyorGeometry";

const exampleConveyorInstanceName = "exampleConveyorInstanceName";
const exampleConveyorGeometryName = "exampleConveyorGeometryName";

function mockTransformMesh() {
  return {
    position: {
      x: 0,
      y: 0,
      z: 0,
    },
    rotation: {
      x: 0,
      y: 0,
      z: 0,
    },
    scaling: {
      x: 0,
      y: 0,
      z: 0,
    },
  };
}

/**
 * Mock instance of 'PartGeometry' class
 */
function createMockPart() {
  return {
    setParent: vi.fn(),
    setPosition: vi.fn(),
    getEffectivePartHeight: vi.fn(),
    setPartOrConveyorRotation: vi.fn(),
  };
}

/**
 * Mock instance of 'Vector3' class from Babylon.js
 */
function createConveyorPointMock() {
  return {
    add: vi.fn(() => createConveyorPointMock()),
    scale: vi.fn(),
    subtract: vi.fn(() => createConveyorPointMock()),
    clone: vi.fn(),
  };
}

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));
  // Ensure that distance is greater than current length
  Vector3.Distance = vi.fn(() => 4);

  const GreasedLineTools = {
    GetLineLength: vi.fn(() => 6),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  } as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Tools = vi.fn(() => ({})) as any;

  // use 'undefined' to represent meshes
  Tools.ToRadians = vi.fn((inputValue) => inputValue);

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Quaternion = vi.fn(() => ({})) as any;

  Quaternion.FromLookDirectionRH = vi.fn(() => ({
    x: null,
    y: null,
    z: null,
    toEulerAngles: vi.fn(),
  }));

  return { Vector3, GreasedLineTools, Tools, Quaternion };
});

describe("ConveyorGeometry class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it(`should throw appropriate error if part is attempted to be positioned on conveyor when there are no conveyor points`, () => {
    // Arrange

    const unableToGetConveyorPointsError =
      "Unable to get conveyor point position: conveyor point positions array is empty";

    const transformMesh_mock = mockTransformMesh();

    const mockConveyorPoints = [];

    const conveyorGeometry = new ConveyorGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleConveyorInstanceName,
      exampleConveyorGeometryName,
      {
        conveyorPoints: mockConveyorPoints,
        conveyorTangents: [],
        conveyorBinormals: [],
      },
    );

    const mockPart = createMockPart();

    // Represents 60% of the way in the conveyor
    const mockPartPosition = 0.6;

    // Assert that Error is thrown
    expect(() => {
      // Act
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      conveyorGeometry.positionPart(mockPart as any, mockPartPosition);
    }).toThrowError(unableToGetConveyorPointsError);
  });

  it(`should invoke 'add' method of the conveyor point that is closest to the point where the part needs to be positioned on the conveyor.`, () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const createFirstConveyorPoint = createConveyorPointMock();

    const mockConveyorPoints = [
      createFirstConveyorPoint,
      createConveyorPointMock(),
      createConveyorPointMock(),
      createConveyorPointMock(),
      createConveyorPointMock(),
      createConveyorPointMock(),
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
    ] as any;

    const conveyorGeometry = new ConveyorGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleConveyorInstanceName,
      exampleConveyorGeometryName,
      {
        conveyorPoints: mockConveyorPoints,
        conveyorTangents: [],
        conveyorBinormals: [],
      },
    );

    const mockPart = createMockPart();

    // Represents 60% of the way in the conveyor
    const mockPartPosition = 0.6;

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    conveyorGeometry.positionPart(mockPart as any, mockPartPosition);

    // Assert that 'add' method has been invoked on the correct conveyor point
    expect(createFirstConveyorPoint.add).toHaveBeenCalled();
  });
});
