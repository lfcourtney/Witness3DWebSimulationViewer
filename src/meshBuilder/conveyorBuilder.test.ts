import { vi, describe, afterEach, it, expect } from "vitest";
import { PathTag } from "../interfaces/pathTag";
import { ConveyorBuilder } from "./conveyorBuilder";

const mockGeometryName = "dgu-pa-Conveyor6";

const mockPathTag: PathTag = {
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
};

// Mock object representation of <path> tag specifically for 'dgu-pa-Conveyor5'
const mockSecondPathTag: PathTag = {
  startX: 33,
  startY: 0,
  startZ: 4.5,
  width: 0.5,
  path: [
    {
      line: {
        startX: 33,
        startY: 0,
        startZ: 4.5,
        endX: 33,
        endY: 0,
        endZ: 17,
      },
    },
    {
      arc: {
        startX: 33,
        startY: 0,
        startZ: 17,
        endX: 37,
        endY: 0,
        endZ: 17,
        centreX: 35,
        centreY: 0,
        centreZ: 17,
        angle: 180,
        sweepDirection: "counterclockwise",
      },
    },
    {
      line: {
        startX: 37,
        startY: 0,
        startZ: 17,
        endX: 37,
        endY: 3,
        endZ: 12,
      },
    },
    {
      line: {
        startX: 37,
        startY: 3,
        startZ: 12,
        endX: 37,
        endY: 3,
        endZ: 7,
      },
    },
  ],
};

// mock necessary materials
vi.mock("../utilities/colors", () => ({
  // Return object has arbitrary 'blackMat' property assigned to true so that future unit tests know the material has been applied
  blackMat: vi.fn(() => ({ blackMat: true })),
}));

// Mock utility line2D function
vi.mock("../utilities/line2D.ts", () => {
  const lineMeshWithFixedWidth = {
    material: undefined,
    clone: vi.fn(() => ({
      position: {
        x: 0,
        y: 0,
        z: 0,
      },
    })),
  };

  const line2D = vi.fn(() => lineMeshWithFixedWidth);

  return { line2D };
});

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  const Vector3 = vi.fn((x, y, z) => ({
    x,
    y,
    z,
    add: (vector3: { x: number; y: number; z: number }) => {
      return { x: x + vector3.x, y: y + vector3.y, z: z + vector3.z };
    },
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));

  const Path3D = vi.fn(() => ({
    getBinormals: vi.fn(),
    getTangents: vi.fn(),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  const Mesh = {
    MergeMeshes: vi.fn(() => ({
      material: undefined,
    })),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  } as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const StandardMaterial = vi.fn() as any;

  const Texture = vi.fn(
    (
      url,
      sceneOrEngine,
      noMipmapOrOptions,
      invertY,
      samplingMode,
      onLoadArg,
      onErrorArg,
    ) => {
      url = undefined;
      sceneOrEngine = undefined;
      noMipmapOrOptions = undefined;
      invertY = undefined;
      samplingMode = undefined;
      onLoadArg = undefined;

      const exampleErrorMessage =
        "example error loading texture. URL: " +
        url +
        sceneOrEngine +
        noMipmapOrOptions +
        invertY +
        samplingMode +
        onLoadArg;

      if (typeof onErrorArg === "function") {
        onErrorArg(exampleErrorMessage);
      }
    },
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  ) as any;

  return { Vector3, Mesh, StandardMaterial, Texture, Path3D };
});

describe("ConveyorBuilder class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it(`should invoke 'increaseConveyorPathHeight' method 
     when 'buildConveyor' method is invoked to create conveyor or path mesh`, () => {
    // Arrange
    vi.spyOn(console, "error").mockImplementation(() => undefined);

    const conveyorBuilder = new ConveyorBuilder(mockPathTag, mockGeometryName);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const increaseConveyorPathHeight_spy = vi.spyOn<any, string>(
      conveyorBuilder,
      "increaseConveyorPathHeight",
    );

    // Act
    conveyorBuilder.buildConveyor();

    // Assert 'increaseConveyorPathHeight' method has been invoked
    expect(increaseConveyorPathHeight_spy).toHaveBeenCalled();
  });

  it(`should modify array of the '_conveyorPoints' field when required so that the line rendered on the X and the Z 
    axis is free from any odd twisted shapes when 'modifyConveyorPointsRemoveTwistedLines' method is invoked`, () => {
    // Arrange
    vi.spyOn(console, "error").mockImplementation(() => undefined);

    const conveyorBuilder = new ConveyorBuilder(
      mockSecondPathTag,
      mockGeometryName,
    );

    conveyorBuilder.buildConveyor();

    // Act
    const modifiedConveyorPointsRemoveTwistedLines =
      conveyorBuilder["modifyConveyorPointsRemoveTwistedLines"]();

    // Assert that return value of 'modifyConveyorPointsRemoveTwistedLines' method is not the same
    // as the original '_conveyorPoints' class attribute
    expect(modifiedConveyorPointsRemoveTwistedLines).not.toEqual(
      conveyorBuilder["_conveyorPoints"],
    );
  });
});
