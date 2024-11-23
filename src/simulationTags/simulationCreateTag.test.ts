import { describe, it, expect, afterEach, vi } from "vitest";
import { CreateTag } from "../interfaces/createTag";
import { SimulationCreateTag } from "./simulationCreateTag";

const mockCreateTag: CreateTag = {
  time: 0,
  geometry:
    "C:\\Users\\Public\\Documents\\Royal HaskoningDHV\\Witness 27\\W3D\\Assets\\Shapes\\dg-vh-Agv1",
  instanceName: "AGV(1) - Entity (88)",
};

// Example create tag that is designed for the rendering of floors
const mockFloorCreateTag: CreateTag = {
  time: 0,
  instanceName: "Floor 1",
  surface: {
    x1: 14.135,
    y1: 0,
    z1: 1.76,
    x2: 48.365,
    y2: 0,
    z2: 30.74,
    normalX: 0,
    normalY: 1,
    normalZ: 0,
    color1: "#e8e8e8ff",
    color2: "#f3f3f3ff",
    singleSided: true,
  },
};

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), async () => {
  const SceneLoader = vi.fn(() => ({}));

  const mockAbstractMesh = { name: null };

  // use 'undefined' to represent meshes
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (SceneLoader as any).ImportMeshAsync = vi.fn(() => ({
    meshes: [mockAbstractMesh, mockAbstractMesh],
  }));

  const MeshBuilder = vi.fn(() => ({}));

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (MeshBuilder as any).CreateGround = vi.fn(() => ({
    position: {
      x: undefined,
      y: undefined,
      z: undefined,
    },
  }));

  const babylonJSImport = await import("@babylonjs/core");

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (babylonJSImport as any).SceneLoader = SceneLoader;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  (babylonJSImport as any).MeshBuilder = MeshBuilder;

  return { ...babylonJSImport };
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

  it("should call 'importMeshSuccess' when imvoking 'actOnTagLogic' and it is necessary to handle creation of geometry", async () => {
    // Arrange
    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      {
        scene: undefined,
        geometriesMap: {
          set: vi.fn(),
        },
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
      } as any,
      mockCreateTag,
    );

    const importMeshSuccess_spy = vi
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      .spyOn<any, string>(simulationCreateTag, "importMeshSuccess");

    // Act
    await simulationCreateTag.actOnTagLogic();

    // Assert 'importMeshSuccess' has been invoked
    expect(importMeshSuccess_spy).toHaveBeenCalledOnce();
  });

  it("should call 'renderGround' when imvoking 'actOnTagLogic' and it is necessary to render ground", async () => {
    // Arrange
    const simulationCreateTag: SimulationCreateTag = new SimulationCreateTag(
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { scene: undefined } as any,
      mockFloorCreateTag,
    );

    const renderGround_spy = vi
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      .spyOn<any, string>(simulationCreateTag, "renderGround");

    // Act
    await simulationCreateTag.actOnTagLogic();

    // Assert 'renderGround' has been invoked
    expect(renderGround_spy).toHaveBeenCalledOnce();
  });
});
