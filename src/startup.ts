import { FileUpload } from "./fileUpload";

/**
 * Respond to loading of bundled JavaScript in the browser: hide initial loader element
 * and show the loaderContainer, allowing the user to interact with the form to submit w3d file
 * @param formContainer Element of id 'formContainer' in HTML document
 */
function hideSpinner(formContainer: HTMLElement): void {
  const loaderContainer = document.getElementById("loaderContainer");

  if (!loaderContainer) {
    throw new Error(
      "Element of id 'loaderContainer' was unable to be found within index.html",
    );
  }

  loaderContainer.style.display = "none";
  formContainer.style.display = "flex";
}

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
  // and show the form instead
  hideSpinner(formContainer);

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
