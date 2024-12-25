import { vi, describe, afterEach, it, expect } from "vitest";
import { PathTag } from "../interfaces/pathTag";
import { ConveyorBuilder } from "./conveyorBuilder";

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
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));

  const Mesh = {
    MergeMeshes: vi.fn(() => ({
      material: undefined,
    })),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  } as any;

  return { Vector3, Mesh };
});

describe("ConveyorBuilder class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it(`should invoke 'increaseConveyorPathHeight' method 
     when 'buildConveyor' method is invoked to create conveyor or path mesh`, () => {
    // Arrange
    const conveyorBuilder = new ConveyorBuilder(mockPathTag);

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
});
