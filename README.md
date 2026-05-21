# Video Face Cropper using OpenCV and GStreamer

This project extracts frames from an MP4 video and then detects and crops faces from those frames using C++ and OpenCV.

It is designed as a basic OpenCV computer vision project. It demonstrates video frame extraction, image preprocessing, Haar Cascade face detection, bounding-box filtering, duplicate detection removal, and saving cropped face images in an organized folder structure.

## Project Overview

The project has two parts:

1. **Frame extraction with GStreamer**
   - Reads `input.mp4`
   - Resizes frames to `640x640`
   - Saves each frame as a JPEG image inside `frames/`

2. **Face detection and cropping with OpenCV**
   - Reads JPEG frames from `frames/`
   - Uses OpenCV's pre-trained Haar Cascade face detector
   - Detects faces in each frame
   - Filters noisy and duplicate detections
   - Saves cropped faces inside `cropped_faces/`

## Project Structure

```text
CPP_LINUX_OpenCV/
|-- input.mp4              # Input video file
|-- run_part1.sh           # GStreamer script to extract video frames
|-- Makefile               # Build file for the C++ application
|-- face_cropper           # Compiled executable, created after running make
|-- src/
|   `-- main.cpp           # OpenCV face detection and cropping code
|-- frames/                # Extracted video frames
`-- cropped_faces/         # Cropped face outputs
```

## Requirements

This project was developed for Linux/WSL Ubuntu.

Install the required packages:

```bash
sudo apt update
sudo apt install -y g++ make pkg-config
sudo apt install -y libopencv-dev
sudo apt install -y \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-libav
```

The default Haar Cascade file path used by the program is:

```text
/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml
```

This file is usually installed with `libopencv-dev` on Ubuntu.

## How to Run

Run all commands from the project root:

```bash
cd CPP_LINUX_OpenCV
```

### 1. Extract Frames from the Video

Give execute permission to the script:

```bash
chmod +x run_part1.sh
```

Run the GStreamer pipeline:

```bash
./run_part1.sh
```

This creates JPEG frames in:

```text
frames/
```

Example:

```text
frames/frame_00000.jpg
frames/frame_00001.jpg
frames/frame_00002.jpg
```

### 2. Build the C++ Application

```bash
make clean
make
```

This creates the executable:

```text
face_cropper
```

### 3. Detect and Crop Faces

Run with the default folders:

```bash
./face_cropper
```

The program reads from:

```text
frames/
```

and saves cropped faces to:

```text
cropped_faces/
```

Example output structure:

```text
cropped_faces/
|-- frame_1/
|   |-- face_1.jpg
|   `-- face_2.jpg
|-- frame_2/
|   `-- face_1.jpg
`-- frame_3/
    `-- face_1.jpg
```

## Optional Command-Line Arguments

The executable also supports custom paths:

```bash
./face_cropper <input_frames_dir> <output_faces_dir> <cascade_xml_path>
```

Example:

```bash
./face_cropper frames cropped_faces /usr/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml
```

## Full Run Sequence

```bash
chmod +x run_part1.sh
./run_part1.sh
make clean
make
./face_cropper
```

## How the Face Detection Works

The C++ program in `src/main.cpp` follows this pipeline:

1. Collect all `.jpg` and `.jpeg` files from the input frame folder.
2. Sort the frame paths so they are processed in order.
3. Read each frame using OpenCV.
4. Convert the frame to grayscale.
5. Improve contrast using histogram equalization.
6. Apply a small Gaussian blur to reduce noise.
7. Detect faces using Haar Cascade.
8. Remove weak detections using size, shape, and border checks.
9. Remove duplicate detections using IoU overlap filtering.
10. Crop each final face region from the original color frame.
11. Save cropped faces into frame-wise folders.

## Filtering Logic

The project filters bad detections by checking:

- Minimum face size
- Face-like width-to-height ratio
- Whether the detected box is too close to the image border

Duplicate detections are filtered using IoU, which stands for Intersection over Union. If two detected boxes overlap too much, the program keeps only one of them.

## Notes

- Haar Cascade is a classical face detection method. It is simple, fast, and good for learning OpenCV basics.
- Detection accuracy may be lower for side faces, blurred faces, small faces, dark lighting, or heavy occlusion.
- More modern alternatives include OpenCV DNN face detectors and YuNet, but they usually require downloading a separate pre-trained model file.
- Generated folders such as `frames/` and `cropped_faces/` can be deleted and recreated by running the pipeline again.

## Clean Build

To remove the compiled executable:

```bash
make clean
```

## Author

Shyam Sundar Biswas 2026
