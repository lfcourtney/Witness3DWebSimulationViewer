import { describe, it, expect, beforeEach, afterEach, vi } from "vitest";

import { MeshGeometry } from "./meshGeometry";

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

let abstractMesh = initialiseAbstractMesh();

describe("MeshGeometry class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  beforeEach(() => {
    abstractMesh = initialiseAbstractMesh();
  });

  it("should increase mesh on y axis to make it level with the floor when 'fixGeometryToGround' method is invoked", () => {
    // Arrange
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const fixGeometryToTheGround_spy = vi.spyOn<any, string>(
      MeshGeometry.prototype,
      "fixGeometryToGround",
    );
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const isNodeAbstractMesh_spy = vi.spyOn<any, string>(
      MeshGeometry.prototype,
      "isNodeAbstractMesh",
    );

    isNodeAbstractMesh_spy.mockReturnValue(true);
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleGeometryName,
    );

    // Act
    geometryObject.fixGeometryToGround();

    // Assert that 'fixGeometryToGround' method has been called
    expect(fixGeometryToTheGround_spy).toHaveBeenCalledOnce();
    // Assert that 'abstractMesh' position has been moved
    expect(abstractMesh.position.y).toBe(10);
  });

  it("should change visibility to zero", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleGeometryName,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const isNodeAbstractMesh_spy = vi.spyOn<any, string>(
      MeshGeometry.prototype,
      "isNodeAbstractMesh",
    );
    isNodeAbstractMesh_spy.mockReturnValue(true);

    // Act
    geometryObject.changeVisibility(0);

    // Assert that 'getDescendants' has been called once
    expect(abstractMesh.getDescendants).toHaveBeenCalledOnce();
  });

  it("should apply position to 'transformMesh' when 'setPosition' method is invoked", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleGeometryName,
    );

    const mockPosition = { x: 10, y: 10, z: 10 };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (geometryObject as any).setPosition(mockPosition);

    // Assert that position has been applied
    expect(abstractMesh.position).toEqual(mockPosition);
  });

  it("should apply rotation to 'transformMesh' when 'setRotation' method is invoked", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleGeometryName,
    );

    const mockRotation = { x: 10, y: 10, z: 10 };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (geometryObject as any).setRotation(mockRotation);

    // Assert that rotation has been applied
    expect(abstractMesh.rotation).toEqual(mockRotation);
  });

  it("should apply scaling to 'transformMesh' when 'setScaling' method is invoked", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleGeometryName,
    );

    const mockScaling = { x: 10, y: 10, z: 10 };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (geometryObject as any).setScaling(mockScaling);

    // Assert that scaling has been applied
    expect(abstractMesh.scaling).toEqual(mockScaling);
  });
});
