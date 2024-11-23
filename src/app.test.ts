import { describe, it, expect, afterEach, vi } from "vitest";
import { App } from "./app";

describe("Main App class", () => {
  afterEach(() => {
    vi.restoreAllMocks(); // Restores all mocks to original implementations
  });

  it("created scene should be set to use right handed cartesian coordinate system", async () => {
    // Arrange
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    vi.spyOn<any, string>(App.prototype, "loadEngine").mockImplementation(
      () =>
        new Promise((resolve) => {
          resolve({
            runRenderLoop: vi.fn(),
          });
        }),
    );

    const mockScene = {
      useRightHandedSystem: false,
    };

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
});
