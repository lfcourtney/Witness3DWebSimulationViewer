import { describe, it, expect, afterEach, vi } from "vitest";
import { DeleteTag } from "../interfaces/deleteTag";
import { SimulationDeleteTag } from "./SimulationDeleteTag";

const mockDeleteTag: DeleteTag = {
  time: 0,
  instanceName: "Box - Entity (67)",
};

/**
 * Mock instance of mesh geometry class
 */
function mockMeshGeometry() {
  return {
    setPosition: vi.fn(),
    setScaling: vi.fn(),
    setRotation: vi.fn(),
    changeVisibility: vi.fn(),
  };
}

describe("SimulationDeleteTag class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should set visibility to invisible (0) when 'actOnTagLogic' method is invoked", () => {
    // Arrange

    // Add mocked instance of 'MeshGeometry' class to mocked map
    const fakeGeometriesMap = new Map();

    const mockMeshGeometryToDelete = mockMeshGeometry();

    fakeGeometriesMap.set(mockDeleteTag.instanceName, mockMeshGeometryToDelete);

    const simulationUpdateTag: SimulationDeleteTag = new SimulationDeleteTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { geometriesMap: fakeGeometriesMap } as any,
      mockDeleteTag,
    );

    // Act
    simulationUpdateTag.actOnTagLogic();

    // Assert 'changeVisibility' method has been called
    expect(mockMeshGeometryToDelete.changeVisibility).toHaveBeenCalled();
  });
});
