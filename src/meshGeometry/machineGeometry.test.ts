import { describe, it, expect, afterEach, vi } from "vitest";

import { MachineGeometry } from "./machineGeometry";

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
      partPositioning: "partOver",
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

  it("should instantiate MachineGeometry with the necessary part if the part is positioned appropriately", () => {
    // Arrange

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const hasQueuePosition_spy = vi.spyOn<any, string>(
      MachineGeometry.prototype,
      "hasQueuePosition",
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const setPositionOfPart_spy = vi.spyOn<any, string>(
      MachineGeometry.prototype,
      "setPositionOfPart",
    );

    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    // Act
    new MachineGeometry(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      transformMesh_mock as any,
      exampleMachineGeometryName,
      queueInfoTag_mock,
    );

    // Assert that 'hasQueuePosition' returned true
    expect(hasQueuePosition_spy).toHaveLastReturnedWith(true);

    // Assert that 'setPositionOfPart' method has been called
    expect(setPositionOfPart_spy).toHaveBeenCalled();
  });

  it("should set part position when 'setPosition' method is invoked to update position", () => {
    // Arrange

    const transformMesh_mock = mockTransformMesh();

    const queueInfoTag_mock = mockQueueInfoTag();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const setPositionOfPart_spy = vi.spyOn<any, string>(
      MachineGeometry.prototype,
      "setPositionOfPart",
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

    // Assert that 'setPositionOfPart' method has been called
    expect(setPositionOfPart_spy).toHaveBeenCalled();
  });
});
