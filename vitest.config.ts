import { defineConfig, coverageConfigDefaults } from "vitest/config";
import path from "path";

export default defineConfig({
  test: {
    coverage: {
      reporter: ["text", "html", "cobertura"],
      exclude: [
        "mocks/**",
        "src/utilities/**",
        "src/interfaces/**",
        "docs/assets",
        // Ignore TypeScript interface
        "src/meshGeometry/positionPart.ts",
        ...coverageConfigDefaults.exclude,
      ],
    },
    environment: "jsdom",
    alias: {
      "@babylonjs/core/Debug/debugLayer": path.resolve("./mocks/babylon.js"),
      "@babylonjs/inspector": path.resolve("./mocks/babylon.js"),
      "@babylonjs/core": path.resolve("./mocks/babylon.js"),
      "@babylonjs/gui": path.resolve("./mocks/babylon.js"),
    },
  },
});
