import { defineConfig, coverageConfigDefaults } from "vitest/config";
import path from "path";

export default defineConfig({
  test: {
    coverage: {
      reporter: ["text", "html", "cobertura"],
      exclude: [
        "mocks/**",
        "src/materials/**",
        ...coverageConfigDefaults.exclude,
      ],
    },
    environment: "jsdom",
    alias: {
      "@babylonjs/core/Debug/debugLayer": path.resolve("./mocks/babylon.js"),
      "@babylonjs/inspector": path.resolve("./mocks/babylon.js"),
      "@babylonjs/core": path.resolve("./mocks/babylon.js"),
    },
  },
});
