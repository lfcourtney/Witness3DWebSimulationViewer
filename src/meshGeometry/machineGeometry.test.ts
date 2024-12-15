import { describe, it, expect, afterEach, vi } from "vitest";

import { MachineGeometry } from "./machineGeometry";
import { PartPositioning } from "../interfaces/queueInfoTag";

const exampleMachineGeometryName = "exampleMachineGeometry";

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

function mockQueueInfoTag() {
  return {
    queueParent: "dg-vh-Agv1",
    behaviour: {
      partPositioning: "partOver" as PartPositioning,
      partRoll: 0,
      partPitch: 0,
      partYaw: 0,
    },
    position: {
      x: 0,
      y: 0.3,
      z: -0.5,
    },
    direction: {
      dx: 0,
      dy: 0,
      dz: 1,
    },
  };
}

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const MeshBuilder = vi.fn(() => ({})) as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (MeshBuilder as any).CreateBox = vi.fn(() => ({
    position: {
      x: undefined,
      y: undefined,
      z: undefined,
    },
    material: null,
  }));

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));

  return { MeshBuilder, Vector3 };
});

// mock necessary materials
vi.mock("../materials/colors", () => ({
  blueMat: vi.fn(),
}));

describe("MachineGeometry class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should set queue position when 'setPosition' method is invoked to update position", () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const setPositionOfQueue_spy = vi.spyOn<any, string>(
      MachineGeometry.prototype,
      "setPositionOfQueue",
    );

    const machineGeometry = new MachineGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleMachineGeometryName,
      queueInfoTag_mock,
    );

    const mockPosition = { x: 10, y: 10, z: 10 };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    machineGeometry.setPosition(mockPosition as any);

    // Assert that 'setPositionOfQueue' method has been called
    expect(setPositionOfQueue_spy).toHaveBeenCalled();
  });

  it("should set queue position when 'setScaling' method is invoked to update position", () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const setPositionOfQueue_spy = vi.spyOn<any, string>(
      MachineGeometry.prototype,
      "setPositionOfQueue",
    );

    const machineGeometry = new MachineGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleMachineGeometryName,
      queueInfoTag_mock,
    );

    const mockScaling = { x: 10, y: 10, z: 10 };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    machineGeometry.setScaling(mockScaling as any);

    // Assert that 'setPositionOfQueue' method has been called
    expect(setPositionOfQueue_spy).toHaveBeenCalled();
  });

  it(`should return the correct y-axis part position from 'processPartPositioningAttribute' when 'partPositioning' attribute
    is set to 'partUnder'`, () => {
    // Arrange
    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    queueInfoTag_mock.behaviour.partPositioning = "partUnder";

    const machineGeometry = new MachineGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleMachineGeometryName,
      queueInfoTag_mock,
    );

    const mockQueueYPosition = 10;

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (machineGeometry as any).queuePosition = {
      x: 0,
      y: mockQueueYPosition,
      z: 0,
    };

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      machineGeometry,
      "scaledQueueInfo",
      "get",
    ).mockImplementation(() => ({
      ...queueInfoTag_mock.position,
    }));

    // Act
    const processPartPositioningAttributeReturnValue =
      machineGeometry["processPartPositioningAttribute"]();

    const partSizeHalved = machineGeometry["PART_SIZE"] / 2;

    // Assert that 'processPartPositioningAttribute' has returned the correct value for a value of 'partUnder'
    expect(processPartPositioningAttributeReturnValue).toBe(
      mockQueueYPosition - partSizeHalved,
    );
  });

  it(`should return the correct y-axis part position from 'processPartPositioningAttribute' when 'partPositioning' attribute
    is set to 'partCentre'`, () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    queueInfoTag_mock.behaviour.partPositioning = "partCentre";

    const machineGeometry = new MachineGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleMachineGeometryName,
      queueInfoTag_mock,
    );

    const mockQueueYPosition = 10;

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (machineGeometry as any).queuePosition = {
      x: 0,
      y: mockQueueYPosition,
      z: 0,
    };

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      machineGeometry,
      "scaledQueueInfo",
      "get",
    ).mockImplementation(() => ({
      ...queueInfoTag_mock.position,
    }));

    // Act
    const processPartPositioningAttributeReturnValue =
      machineGeometry["processPartPositioningAttribute"]();

    // Assert that 'processPartPositioningAttribute' has returned the correct value for a value of 'partCentre'
    expect(processPartPositioningAttributeReturnValue).toBe(mockQueueYPosition);
  });

  it(`should position part at the correct position in the queue when 'positionPart' method is invoked`, () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    const machineGeometry = new MachineGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleMachineGeometryName,
      queueInfoTag_mock,
    );

    // Note: Manually set queue position because getter method of 'scaledQueueInfo' is not
    // interpreted correctly in testing environment
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (machineGeometry as any).queuePosition = { ...queueInfoTag_mock.position };

    // Represents 60% of the way in the queue
    const mockPartPosition = 0.6;

    const partSetPosition_mock = vi.fn();

    const mockPart = {
      setPosition: partSetPosition_mock,
    };

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    machineGeometry.positionPart(mockPart as any, mockPartPosition);

    // Perform calculation of 'calculateQueueSizeLocation' method
    const endingPositionZ =
      queueInfoTag_mock.position.z + queueInfoTag_mock.direction.dz;

    const percentageDifferenceForZ =
      (endingPositionZ - queueInfoTag_mock.position.z) * mockPartPosition;

    // Assert that 'setPosition' method of part has been invoked with the correct argument
    expect(partSetPosition_mock).toHaveBeenCalledWith({
      x: queueInfoTag_mock.position.x,
      y: machineGeometry["processPartPositioningAttribute"](),
      z: queueInfoTag_mock.position.z + percentageDifferenceForZ,
    });
  });
});
