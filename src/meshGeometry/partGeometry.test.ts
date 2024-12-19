import { describe, it, expect, afterEach, vi } from "vitest";

import { PartGeometry } from "./partGeometry";

const examplePartInstanceName = "examplePartInstanceName";
const examplePartGeometryName = "examplePartGeometryName";

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

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  const TransformNode = vi.fn(() => ({
    position: {
      x: undefined,
      y: undefined,
      z: undefined,
    },
    rotation: {
      x: undefined,
      y: undefined,
      z: undefined,
    },
    scale: {
      x: undefined,
      y: undefined,
      z: undefined,
    },
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));

  return { TransformNode, Vector3 };
});

describe("PartGeometry class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it(`should return the correct height value from the 'getEffectivePartHeight' method based on whether the PartGeometry
    is encapsulating a part or labour`, () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const partGeometry = new PartGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      examplePartInstanceName,
      examplePartGeometryName,
    );

    const mockPartYScaling = 1;

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(partGeometry, "getScaling").mockImplementation(
      () => ({
        x: 0,
        y: mockPartYScaling,
        z: 0,
      }),
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (partGeometry as any)._geometryName = "dg-pt-Part1";

    // Assert that part returns y-axis scaling
    expect(partGeometry.getEffectivePartHeight()).toBe(mockPartYScaling);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (partGeometry as any)._geometryName = "dg-pt-ManWalking1";

    // Assert that labour returns 2
    expect(partGeometry.getEffectivePartHeight()).toBe(2);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (partGeometry as any)._geometryName = "unknown";

    // Assert that unknown part geometry returns undefined
    expect(partGeometry.getEffectivePartHeight()).toBe(undefined);
  });
});
