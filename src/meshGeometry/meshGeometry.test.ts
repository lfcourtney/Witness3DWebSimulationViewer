import { describe, it, expect, beforeEach, afterEach, vi } from "vitest";

import { MeshGeometry } from "./meshGeometry";
import { AbstractMesh } from "@babylonjs/core";

const exampleInstanceName = "exampleInstance";
const exampleGeometryName = "exampleGeometry";

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const AbstractMesh = vi.fn(() => ({})) as any;
  return { AbstractMesh };
});

function initialiseAbstractMesh() {
  const mockNode = Object.create(AbstractMesh.prototype);

  // Purpose: ensure that TypeScript considers this object an instanceof 'AbstractMesh'
  Object.assign(mockNode, {
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
  });

  return {
    getDescendants: vi.fn().mockImplementation(() => [mockNode]),
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
    parent: undefined,
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
      exampleInstanceName,
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
      exampleInstanceName,
      exampleGeometryName,
    );

    const newVisibility: number = 0;

    // Act
    geometryObject.changeVisibility(newVisibility);

    // Assert that 'getDescendants' has been called once
    expect(abstractMesh.getDescendants).toHaveBeenCalledOnce();

    // Assert that new visibility has been applied
    expect(geometryObject.visibility).toBe(newVisibility);
    expect(abstractMesh.getDescendants()[0].visibility).toBe(newVisibility);
  });

  it("should apply position to 'transformMesh' when 'setPosition' method is invoked", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleInstanceName,
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
      exampleInstanceName,
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
      exampleInstanceName,
      exampleGeometryName,
    );

    const mockScaling = { x: 10, y: 10, z: 10 };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (geometryObject as any).setScaling(mockScaling);

    // Assert that scaling has been applied
    expect(abstractMesh.scaling).toEqual(mockScaling);
  });

  it("should return the correct value for 'instanceName' getter", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleInstanceName,
      exampleGeometryName,
    );

    // Assert that 'instanceName' getter returns the correct value
    expect(geometryObject.instanceName).toBe(exampleInstanceName);
  });

  it("should apply parent when 'setParent' method is called", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleInstanceName,
      exampleGeometryName,
    );

    const mockParent = vi.fn();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    geometryObject.setParent(mockParent as any);

    // Assert that parent has been applied
    expect(abstractMesh.parent).toEqual(mockParent);
  });

  it("should return scaling when 'getScaling' is called", () => {
    // Arrange
    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      abstractMesh as any,
      exampleInstanceName,
      exampleGeometryName,
    );

    // Act
    const scalingValue = geometryObject.getScaling();

    // Assert that scaling has been returned
    expect(scalingValue).toEqual(abstractMesh.scaling);
  });

  it("should return the appropriate geometry height based on the 'geometry name' when the 'getEffectivePartHeight' method is invoked", () => {
    // Arrange

    const newAbstractMesh = initialiseAbstractMesh();

    // Set unique 'y' scaling value
    newAbstractMesh.scaling.y = 10;

    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      newAbstractMesh as any,
      exampleInstanceName,
      exampleGeometryName,
    );

    // Act
    const effectivePartHeight = geometryObject.getEffectivePartHeight();

    // Assert that 'y' value from abstract mesh scaling object has been returned
    expect(effectivePartHeight).toBe(newAbstractMesh.scaling.y);
  });

  it(`Should store non path or conveyor rotation correctly and subsequently reset it when 
    'setPartOrConveyorRotation' and 'resetPartOrConveyorRotation' methods are called respectively`, () => {
    // Arrange

    const newAbstractMesh = initialiseAbstractMesh();

    const geometryObject = new MeshGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      newAbstractMesh as any,
      exampleInstanceName,
      exampleGeometryName,
    );

    const mockRotationOne = { x: 10, y: 10, z: 10 };
    const mockRotationTwo = { x: 0, y: 0, z: 0 };

    // Set original rotation
    newAbstractMesh.rotation = mockRotationOne;

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (geometryObject as any).setPartOrConveyorRotation(mockRotationTwo);

    // Assert that transformMesh rotation is equal to new rotation
    expect(newAbstractMesh.rotation).toEqual(mockRotationTwo);

    // Act
    geometryObject.resetPartOrConveyorRotation();

    // Assert that transformMesh rotation is equal to original rotation
    expect(newAbstractMesh.rotation).toEqual(mockRotationOne);
  });
});
