import { describe, it, expect, beforeEach, afterEach, vi } from "vitest";

import { Geometry } from "./geometry";

const exampleGeometryName = "exampleGeometry";

function initialiseAbstractMesh() {
  return {
    getDescendants: vi.fn().mockImplementation(() => [
      {
        getBoundingInfo: function () {
          return {
            boundingBox: {
              minimum: {
                y: -10,
              },
            },
            visibility: 1,
          };
        },
      },
    ]),
    position: {
      x: 0,
      y: 0,
      z: 0,
    },
  };
}

let abstractMesh = initialiseAbstractMesh();

let geometryObject: Geometry;

describe("Geometry class", () => {
  afterEach(() => {
    vi.clearAllMocks();
  });

  beforeEach(() => {
    if (geometryObject) {
      abstractMesh = initialiseAbstractMesh();
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      (geometryObject as any).transformMesh = abstractMesh;
    }
  });

  it("should automatically increase position to make geometry level with the floor upon geometry initialisation", () => {
    // Arrange

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const fixGeometryToTheGround_spy = vi.spyOn<any, string>(
      Geometry.prototype,
      "fixGeometryToGround",
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const isNodeAbstractMesh_spy = vi.spyOn<any, string>(
      Geometry.prototype,
      "isNodeAbstractMesh",
    );

    isNodeAbstractMesh_spy.mockReturnValue(true);

    // Act
    geometryObject = new Geometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleGeometryName,
    );

    // Assert that 'fixGeometryToGround' method has been called
    expect(fixGeometryToTheGround_spy).toHaveBeenCalledOnce();

    // Assert that 'abstractMesh' position has been moved
    expect(abstractMesh.position.y).toBe(10);
  });

  it("should change visibility to zero", () => {
    // Act
    geometryObject.changeVisibility(0);

    // Assert that 'getDescendants' has been called once
    expect(abstractMesh.getDescendants).toHaveBeenCalledOnce();
  });
});
