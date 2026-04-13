# C++ Internship Assessment Submission

## Overview
This submission contains the solution for the C++ Internship Assessment.

The assignment has two parts:

- **Part 1:** Use a GStreamer pipeline with `gst-launch-1.0` to read an MP4 video, resize each frame to `640x640`, and save the frames as JPEG images.
- **Part 2:** Write a C++ application using OpenCV’s pre-trained XML classifier to read the extracted JPEG frames one by one, detect faces, crop them, and save the cropped faces in a frame-wise folder structure.

---

## Project Structure

```text
cpp_internship_assignment/
├── input.mp4
├── run_part1.sh
├── Makefile
├── haarcascade_frontalface_alt2.xml
├── src/
│   └── main.cpp
├── frames/
└── cropped_faces/
```

---

## Part 1: GStreamer Pipeline

The shell script `run_part1.sh` uses a GStreamer pipeline to:

1. Read the input MP4 file
2. Decode the video stream
3. Convert and scale frames to `640x640`
4. Encode each frame as JPEG
5. Save frames as separate image files in the `frames/` folder

### Run Part 1

```bash
chmod +x run_part1.sh
./run_part1.sh
```

### Output of Part 1

JPEG frames are generated in:

```text
frames/
```

Example output:

```text
frames/frame_00000.jpg
frames/frame_00001.jpg
frames/frame_00002.jpg
```

---

## Part 2: C++ Face Detection and Cropping

The C++ application in `src/main.cpp` performs the following steps:

1. Reads all JPEG frames from the `frames/` directory
2. Loads OpenCV’s pre-trained Haar Cascade XML classifier
3. Converts each frame to grayscale
4. Applies histogram equalization and slight smoothing for better detection
5. Detects faces in each frame
6. Filters noisy or duplicate detections
7. Crops the detected face regions
8. Saves cropped faces in frame-wise folders under `cropped_faces/`

### Build Part 2

```bash
make
```

### Run Part 2

```bash
./face_cropper
```

If you want to provide explicit paths:

```bash
./face_cropper frames cropped_faces haarcascade_frontalface_alt2.xml
```

### Output of Part 2

Cropped faces are saved in:

```text
cropped_faces/
```

Example output:

```text
cropped_faces/
  frame_1/
    face_1.jpg
    face_2.jpg
  frame_2/
    face_1.jpg
```

---

## Dependencies

This project was developed and tested in **WSL Ubuntu**.

### Install required packages

```bash
sudo apt update
sudo apt install -y g++ make pkg-config unzip
sudo apt install -y libopencv-dev
sudo apt install -y \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-libav
```

---

## Full Execution Steps

Run the following commands in order:

```bash
chmod +x run_part1.sh
./run_part1.sh
make clean
make
./face_cropper
```

---

## Notes on Face Detection

- The implementation uses OpenCV’s pre-trained Haar Cascade XML classifier.
- A slightly tuned detection pipeline was used to improve output quality and reduce false positives.
- Since Haar Cascade is a classical detector, detection quality may reduce for side faces, blurred faces, very small faces, or faces with heavy occlusion such as dark sunglasses.

---

## Submission Contents

This submission includes:

- `run_part1.sh` for Part 1
- Output JPEG frames from Part 1
- `src/main.cpp` for Part 2
- `Makefile` for compilation
- Output cropped face folders from Part 2

---


## Author

**Shyam Sundar Biswas**
