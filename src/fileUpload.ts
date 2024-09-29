export class FileUpload {
  private readonly fileInputElement: HTMLInputElement;
  private readonly previewFormSection: HTMLElement;

  constructor(
    _fileInputElement: HTMLInputElement,
    _previewFormSection: HTMLElement,
  ) {
    this.fileInputElement = _fileInputElement;
    this.previewFormSection = _previewFormSection;

    this.fileInputElement.addEventListener("change", this.addFile.bind(this));
  }

  private addFile() {
    while (this.previewFormSection.firstChild) {
      this.previewFormSection.removeChild(this.previewFormSection.firstChild);
    }

    const currFiles = this.fileInputElement.files;

    if (currFiles.length === 0) {
      const para = document.createElement("p");
      para.classList.add("error");
      para.textContent = "No w3d file selected for upload";
      this.previewFormSection.appendChild(para);
      return;
    }

    const w3dFile = currFiles[0];

    if (this.hasW3dExtension(w3dFile.name) === false) {
      const para = document.createElement("p");
      para.classList.add("error");
      para.textContent = "Error: w3d file type required";
      this.previewFormSection.appendChild(para);
      return;
    }

    const para = document.createElement("p");
    para.textContent = `File name ${w3dFile.name}, file size ${this.returnFileSize(w3dFile.size)}`;
    this.previewFormSection.appendChild(para);
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
