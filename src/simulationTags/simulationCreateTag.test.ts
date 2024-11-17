import { describe, it, expect, afterEach, vi } from "vitest";
import { CreateTag } from "../interfaces/createTag";
import { SimulationCreateTag } from "./simulationCreateTag";

const mockCreateTag: CreateTag = {
  time: 0,
  geometry:
    "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dg-vh-Agv1",
  instanceName: "AGV(1) - Entity (88)",
};

// mock SimulationTagData class
vi.mock(import("../simulationTags/simulationTag"), () => {
  const SimulationTag = vi.fn();
  const SimulationTagData = vi.fn();
  return { SimulationTag, SimulationTagData };
});

describe("SimulationCreateTag class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should successfully 'extractGeometry' if supplied with a correctly formatted 'geometry' attribute of a <create> tag", () => {
    // Arrange
    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      {} as any,
      mockCreateTag,
    );

    // Act
    const returnedGeometry = simulationCreateTag["extractGeometry"](
      mockCreateTag.geometry!,
    );

    // Assert geometry was successfully returned
    expect(returnedGeometry).toBe("dg-vh-Agv1");
  });
});
