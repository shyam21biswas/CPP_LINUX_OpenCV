#!/bin/bash

set -e

# Input video file name
INPUT_VIDEO="input.mp4"

# Folder where jpeg frames will be saved
OUTPUT_DIR="frames"

# Create output folder if it does not exist
mkdir -p "$OUTPUT_DIR"

# GStreamer pipeline:
# filesrc      -> read the mp4 file
# qtdemux      -> extract video stream from mp4 container
# decodebin    -> decode compressed video into raw frames
# videoconvert -> convert frame format if needed
# videoscale   -> enable resizing
# caps filter  -> force resolution to 640x640
# jpegenc      -> encode each frame as jpeg
# multifilesink-> save each frame as a separate jpg file

gst-launch-1.0 -e \
filesrc location="$INPUT_VIDEO" ! \
qtdemux ! \
decodebin ! \
videoconvert ! \
videoscale ! \
video/x-raw,width=640,height=640 ! \
jpegenc ! \
multifilesink location="$OUTPUT_DIR/frame_%05d.jpg"
