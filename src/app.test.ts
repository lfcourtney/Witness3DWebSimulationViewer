import { describe, it, expect, afterEach, beforeEach, vi } from "vitest";
import { App } from "./app";

// mock SimulationContentFormat class
vi.mock(import("./simulationContent/simulationContentFormat"), () => {
  const fakeSimulationContentFormatObj = {
    actOnTagLogic: vi.fn(),
    actOnTagLogicAtTimeZero: vi.fn(),
  };

  const SimulationContentFormat = vi.fn(
    () => fakeSimulationContentFormatObj,
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  ) as any;

  return { SimulationContentFormat };
});

// mock needed babylon.js imports
vi.mock(import("@babylonjs/core"), () => {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Vector3 = vi.fn((x, y, z) => ({ x, y, z })) as any;
  Vector3.Zero = vi.fn(() => ({ x: 0, y: 0, z: 0 }));
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Tools = vi.fn(() => ({})) as any;

  const ArcRotateCamera = vi.fn(() => ({
    upperRadiusLimit: null,
    lowerRadiusLimit: null,
    upperBetaLimit: null,
    attachControl: vi.fn(),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  // use 'undefined' to represent meshes
  Tools.ToRadians = vi.fn((inputValue) => inputValue);

  const AdvancedDynamicTexture = {
    CreateFullscreenUI: vi.fn(() => ({ addControl: vi.fn() })),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  } as any;

  const StackPanel = vi.fn(() => ({
    addControl: vi.fn(),
    width: null,
    left: null,
    top: null,
    horizontalAlignment: null,
    verticalAlignment: null,
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  const Control = {
    HORIZONTAL_ALIGNMENT_RIGHT: null,
    VERTICAL_ALIGNMENT_BOTTOM: null,
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  } as any;

  const Button = {
    CreateSimpleButton: vi.fn(() => ({
      width: null,
      height: null,
      color: null,
      background: null,
      onPointerClickObservable: {
        add: vi.fn(),
      },
      onPointerEnterObservable: {
        add: vi.fn(),
      },
      onPointerOutObservable: {
        add: vi.fn(),
      },
    })),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  } as any;

  const Scene = vi.fn(() => ({
    useRightHandedSystem: false,
    render: vi.fn(),
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
  })) as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const HemisphericLight = vi.fn(() => ({ intensity: null })) as any;

  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const Engine = vi.fn(() => ({ runRenderLoop: vi.fn() })) as any;

  return {
    Vector3,
    Tools,
    ArcRotateCamera,
    AdvancedDynamicTexture,
    StackPanel,
    Control,
    Button,
    Scene,
    HemisphericLight,
    Engine,
  };
});

/**
 * Mock instance of 'SimulationTag' super class
 */
function createMockSimulationTag() {
  return {
    time: 0,
    actOnTagLogic: vi.fn(),
  };
}

/**
 * Mock instance of Babylon.js engine (either WebGPU engine or standard WebGL 2.0 engine)
 */
function createMockEngine() {
  return { beginFrame: vi.fn(), endFrame: vi.fn() };
}

/**
 * Mock instance of Babylon.js scene
 */
function createMockScene() {
  return {
    useRightHandedSystem: false,
    render: vi.fn(),
  };
}

describe("Main App class", () => {
  beforeEach(() => {
    vi.useFakeTimers();
  });

  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("should adjust created scene to use right handed cartesian coordinate system", async () => {
    // Arrange

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    const mockScene = createMockScene();

    const createScene_spy = vi
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      .spyOn<any, string>(App.prototype, "createScene")
      .mockImplementation(() => mockScene);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    await app["loadEngine"]();

    // Assert that 'createScene' method has been called
    expect(createScene_spy).toHaveBeenCalledOnce();

    // Assert that the mocked scene is using the right handed cartesian coordinate system
    expect(mockScene.useRightHandedSystem).toBe(true);
  });

  it("should invoke 'formatTag' method of 'SimulationContents' for each tag in tagStore", async () => {
    // Arrange

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    const formatTag_mock = vi.fn();

    // Act
    const app = new App(
      {} as HTMLElement,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { tagStore: [null, null], formatTag: formatTag_mock } as any,
    );

    await app["loadEngine"]();

    // Mock awaiting for 'actOnTagLogic' method call
    const asyncMock = vi.fn(
      async () => "Mock awaiting for 'actOnTagLogic' method call",
    );

    await asyncMock();

    // Assert that 'formatTag' method has been called twice, one each time for the two tag store array items (represented as null)
    expect(formatTag_mock).toHaveBeenCalledTimes(2);
  });

  it("should invoke 'actOnTagLogic' method for each 'SimulationTag' object in tag store at the time they were stored in the tag store at", async () => {
    // Arrange

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // Mock tag store
    const fakeTimeTagStore = new Map();

    // Mock Simulation Tag
    const simulationTag_mock = createMockSimulationTag();

    // Add to fake tag store at time 0
    fakeTimeTagStore.set("0.00", [simulationTag_mock]);

    // Act
    const app = new App(
      {} as HTMLElement,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { tagStore: [] } as any,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (app as any).timeTagStore = fakeTimeTagStore;

    // Need to wait for 'loadEngine' so that the 'mockEngine' initialised above has time to be
    // assigned to 'engine' field of App class
    await app["loadEngine"]();

    // Mock waiting for 'setInterval'
    vi.advanceTimersToNextTimer();

    // Assert that 'actOnTagLogic' method has been called for mocked 'SimulationTag'
    expect(simulationTag_mock.actOnTagLogic).toHaveBeenCalled();
    // Assert that 'beginFrame' has been called so that WebGPU engine can render correctly
    expect(mockEngine.beginFrame).toHaveBeenCalled();
    // Assert that 'endFrame' has been called so that WebGPU engine can render correctly
    expect(mockEngine.endFrame).toHaveBeenCalled();
  });

  it("should throw appropriate exception from 'runApplicationRenderLoop' method if engine is undefined", () => {
    // Arrange

    // Remove implementation of 'runMainAppSetup' so that engine is never initialised
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "runMainAppSetup").mockImplementation(
      () => null,
    );

    const mockScene = createMockScene();

    const undefinedEngineException =
      "Unable to create render loop: engine was undefined";

    // Act
    const app = new App(
      {} as HTMLElement,
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      { tagStore: [] } as any,
    );

    // Assert that correct error has been thrown
    expect(() =>
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      app["runApplicationRenderLoop"](mockScene as any),
    ).toThrowError(undefinedEngineException);
  });

  it("should invoke 'createBackButton' method during successful constructor invocation", async () => {
    // Arrange

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const createBackButton_spy = vi.spyOn<any, string>(
      App.prototype,
      "createBackButton",
    );

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    // Explanation: constructor was Act. This is to simulation awaiting for 'loadEngine' in constructor
    await app["loadEngine"]();

    // Assert that 'createBackButton' has been invoked in constructor
    expect(createBackButton_spy).toHaveBeenCalled();
  });

  it("should invoke 'createScene' method during successful constructor invocation", async () => {
    // Arrange

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const createScene_spy = vi.spyOn<any, string>(App.prototype, "createScene");

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene");

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    // Explanation: constructor was Act. This is to simulation awaiting for 'loadEngine' in constructor
    await app["loadEngine"]();

    // Assert that 'createScene' method has been called
    expect(createScene_spy).toHaveBeenCalled();
  });

  it("should invoke 'createCameraAndPositionToFloor' method during successful constructor invocation", async () => {
    // Arrange

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const createCameraAndPositionToFloor_spy = vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    );

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (mockScene as any).meshes = [
      { name: "Floor 1", position: { x: 0, y: 0, z: 0 } },
      { name: "Floor 2", position: { x: 0, y: 0, z: 0 } },
    ];

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    // Explanation: constructor was Act. This is to simulation awaiting for 'loadEngine' in constructor
    await app["loadEngine"]();

    // Assert that 'createCameraAndPositionToFloor' has been invoked in constructor
    expect(createCameraAndPositionToFloor_spy).toHaveBeenCalled();
  });

  it("should invoke 'loadEngine' method during successful constructor invocation", async () => {
    // Arrange

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const loadEngine_spy = vi.spyOn<any, string>(App.prototype, "loadEngine");

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    // Explanation: constructor was Act. This is to simulation awaiting for 'loadEngine' in constructor
    await app["loadEngine"]();

    // Assert that 'loadEngine' method has been invoked
    expect(loadEngine_spy).toHaveBeenCalled();
  });

  it("should convert frame number to string appropriately when 'formatFrameNumber' method is invoked", () => {
    // Arrange

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // Act
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    const formattedFrameNumber = app["formatFrameNumber"](2);

    expect(formattedFrameNumber).toBe("2.00");
  });

  it("should remove canvas when 'clearCanvas' method is called", async () => {
    // Arrange

    const mockEngine = createMockEngine();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve(mockEngine);
        }),
    );

    const mockCanvas = {
      remove: vi.fn(),
    };

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createCanvas").mockImplementation(
      () => mockCanvas,
    );

    const mockScene = createMockScene();

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createScene").mockImplementation(
      () => mockScene,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(
      App.prototype,
      "createCameraAndPositionToFloor",
    ).mockImplementation(() => null);

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "createBackButton").mockImplementation(
      () => null,
    );

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const app = new App({} as HTMLElement, { tagStore: [] } as any);

    // Need to wait for 'loadEngine' so that the 'mockEngine' initialised above has time to be
    // assigned to 'engine' field of App class
    await app["loadEngine"]();

    // Act
    app["clearCanvas"]();

    // Assert that 'remove' method from canvas has been called
    expect(mockCanvas.remove).toHaveBeenCalled();
  });
});
