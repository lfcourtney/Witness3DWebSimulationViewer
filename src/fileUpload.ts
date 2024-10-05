export class FileUpload {
  private readonly fileInputElement: HTMLInputElement;
  private readonly fileUploadSection: HTMLElement;
  private readonly fileUploadBtn: HTMLButtonElement;
  private readonly formContainer: HTMLElement;

  private w3dFile?: File;

  constructor(
    _fileInputElement: HTMLInputElement,
    _fileUploadSection: HTMLElement,
    _fileUploadBtn: HTMLButtonElement,
    _formContainer: HTMLElement,
  ) {
    this.fileInputElement = _fileInputElement;
    this.fileUploadSection = _fileUploadSection;
    this.fileUploadBtn = _fileUploadBtn;
    this.formContainer = _formContainer;

    this.fileInputElement.addEventListener("change", this.addFile.bind(this));
    this.fileUploadBtn.addEventListener("click", this.submitFile.bind(this));
  }

  private addFile() {
    while (this.fileUploadSection.firstChild) {
      this.fileUploadSection.removeChild(this.fileUploadSection.firstChild);
    }

    while (
      this.fileUploadBtn.previousElementSibling &&
      this.fileUploadBtn.previousElementSibling.nodeName == "P"
    ) {
      this.fileUploadBtn.previousElementSibling.remove();
    }

    const currFiles = this.fileInputElement.files;

    if (!currFiles || currFiles.length === 0) {
      const para = document.createElement("p");
      para.classList.add("error");
      para.textContent = "No w3d file selected for upload";
      this.fileUploadSection.appendChild(para);
      return;
    }

    const w3dFile = currFiles[0];

    if (this.hasW3dExtension(w3dFile.name) === false) {
      const para = document.createElement("p");
      para.classList.add("error");
      para.textContent = "Error: w3d file type required";
      this.fileUploadSection.appendChild(para);
      return;
    }

    const para = document.createElement("p");
    para.textContent = `File name ${w3dFile.name}, file size ${this.returnFileSize(w3dFile.size)}`;
    this.fileUploadSection.appendChild(para);

    this.w3dFile = w3dFile;
  }

  private async submitFile(): Promise<void> {
    if (!this.w3dFile || this.hasW3dExtension(this.w3dFile.name) === false) {
      if (
        !(
          this.fileUploadBtn.previousElementSibling &&
          this.fileUploadBtn.previousElementSibling.nodeName
        )
      ) {
        const para = document.createElement("p");
        para.textContent = "No w3d file to submit";
        this.fileUploadBtn.parentNode?.insertBefore(para, this.fileUploadBtn);
      }
      return;
    }
    // Remove 'formContainer' so that canvas is the only element in the document body
    this.formContainer.remove();

    const { App } = await import("./app");

    // Initialise app, so canvas element will be added to the document body
    new App();
  }

  private hasW3dExtension(filename: string): boolean {
    const w3dRegex = /\.w3d$/;
    return w3dRegex.test(filename);
  }

  private returnFileSize(number) {
    if (number < 1e3) {
      return `${number} bytes`;
    } else if (number >= 1e3 && number < 1e6) {
      return `${(number / 1e3).toFixed(1)} KB`;
    } else {
      return `${(number / 1e6).toFixed(1)} MB`;
    }
  }
}
