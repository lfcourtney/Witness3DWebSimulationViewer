import { FileUpload } from "./fileUpload";

/**
 * Ascertain the correct HTML elements TypeScript code needs to handle the input form,
 * whereby user upload a w3d file, from the index.html file.
 * @returns Object of the HTML elements TypeScript logic requires to handle input form
 */
export function getFileUploadElements(): {
  w3dFileInput: HTMLInputElement;
  fileUploadSection: HTMLElement;
  fileUploadBtn: HTMLButtonElement;
  formContainer: HTMLElement;
} {
  // 'formContainer' should be the parent element of all the others
  const formContainer = document.getElementById("formContainer");

  if (!formContainer) {
    throw new Error(
      "Element of id 'formContainer' was unable to be found within index.html",
    );
  }

  // We know that 'formContainer' exists, so we can safely hide the spinner
  // by revealing the form on top of the spinner
  formContainer.style.display = "flex";

  const w3dFileInput = document.getElementById("w3dFile");

  if (!w3dFileInput) {
    throw new Error(
      "Element of id 'w3dFileInput' was unable to be found within index.html",
    );
  }

  const fileUploadSection = document.getElementById("fileUploadSection");

  if (!fileUploadSection) {
    throw new Error(
      "Element of id 'fileUploadSection' was unable to be found within index.html",
    );
  }

  const fileUploadBtn = document.getElementById("fileUploadBtn");

  if (!fileUploadBtn) {
    throw new Error(
      "Element of id 'fileUploadBtn' was unable to be found within index.html",
    );
  }

  return {
    w3dFileInput: w3dFileInput as HTMLInputElement,
    fileUploadSection: fileUploadSection as HTMLElement,
    fileUploadBtn: fileUploadBtn as HTMLButtonElement,
    formContainer: formContainer as HTMLElement,
  };
}

try {
  const { w3dFileInput, fileUploadSection, fileUploadBtn, formContainer } =
    getFileUploadElements();
  new FileUpload(w3dFileInput, fileUploadSection, fileUploadBtn, formContainer);
} catch (exception) {
  console.error(exception);
  throw new Error(
    "Witness3DWebSimulationViewer html file does not contain the necessary data to begin the application",
  );
}
