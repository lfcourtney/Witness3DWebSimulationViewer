import { describe, expect, it, beforeEach } from "vitest";
import { JSDOM } from "jsdom";

let document: Document;

describe("test FileUpload class", () => {
  beforeEach(async () => {
    const dom = await JSDOM.fromFile("index.html");

    document = dom.window.document;
  });

  it("test 1", () => {
    const formContainer = document.getElementById("formContainer");

    expect(formContainer).not.toBe(null);
  });
});
