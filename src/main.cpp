#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;
using namespace std;
using namespace cv;

// This function checks whether the file is a jpg/jpeg image.
// We use it so only image frames are processed.
bool isJpg(const fs::path& p) {
    string ext = p.extension().string();
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".jpg" || ext == ".jpeg";
}

// This function reads all jpg images from the input folder.
// We return them in sorted order so frames are processed one by one correctly.
vector<fs::path> getImages(const string& dir) {
    vector<fs::path> imgs;

    if (!fs::exists(dir)) {
        cerr << "Input directory does not exist: " << dir << endl;
        return imgs;
    }

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && isJpg(entry.path())) {
            imgs.push_back(entry.path());
        }
    }

    sort(imgs.begin(), imgs.end());
    return imgs;
}

// This function calculates overlap between 2 rectangles.
// We use it to remove duplicate detections of the same face.
double calcIoU(const Rect& a, const Rect& b) {
    int x1 = max(a.x, b.x);
    int y1 = max(a.y, b.y);
    int x2 = min(a.x + a.width, b.x + b.width);
    int y2 = min(a.y + a.height, b.y + b.height);

    int interW = max(0, x2 - x1);
    int interH = max(0, y2 - y1);
    int interArea = interW * interH;

    int unionArea = a.area() + b.area() - interArea;
    if (unionArea == 0) return 0.0;

    return (double)interArea / unionArea;
}

// This function removes highly overlapping face boxes.
// We keep only one box if many boxes are on the same face.
vector<Rect> removeOverlaps(const vector<Rect>& faces) {
    vector<Rect> out;

    for (const auto& r : faces) {
        bool keep = true;

        for (const auto& saved : out) {
            if (calcIoU(r, saved) > 0.30) {
                keep = false;
                break;
            }
        }

        if (keep) out.push_back(r);
    }

    return out;
}

int main(int argc, char* argv[]) {
    // Default paths.
    // These can also be changed from command line if needed.
    string inDir = "frames";
    string outDir = "cropped_faces";
    string xmlPath = "/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml";

    if (argc >= 2) inDir = argv[1];
    if (argc >= 3) outDir = argv[2];
    if (argc >= 4) xmlPath = argv[3];

    // Load the pre-trained Haar Cascade face detector.
    // Without this xml file, face detection cannot run.
    CascadeClassifier faceCascade;
    if (!faceCascade.load(xmlPath)) {
        cerr << "Failed to load cascade file: " << xmlPath << endl;
        return 1;
    }

    // Read all extracted jpg frames from the input folder.
    vector<fs::path> imgs = getImages(inDir);
    if (imgs.empty()) {
        cerr << "No JPEG images found in: " << inDir << endl;
        return 1;
    }

    // Create the main output folder.
    fs::create_directories(outDir);

    int frameNo = 0;

    // Process frames one by one.
    for (const auto& imgPath : imgs) {
        frameNo++;

        // Read current frame image.
        Mat img = imread(imgPath.string());
        if (img.empty()) {
            cerr << "Could not read image: " << imgPath << endl;
            continue;
        }

        // Convert image to grayscale because Haar Cascade works better on grayscale.
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);

        // Improve contrast so facial features are clearer.
        equalizeHist(gray, gray);

        // Light blur reduces tiny noisy patterns and false detections.
        GaussianBlur(gray, gray, Size(3, 3), 0);

        // Raw detections from the model.
        vector<Rect> rawFaces;
        faceCascade.detectMultiScale(
            gray,
            rawFaces,
            1.08,          // balanced scale factor
            5,             // stricter detection to reduce false positives
            0,
            Size(40, 40)   // ignore very small detections
        );

        // Filter bad detections.
        vector<Rect> filteredFaces;
        for (const auto& r : rawFaces) {
            double ratio = (double)r.width / r.height;

            // Remove very small detections.
            if (r.width < 40 || r.height < 40) continue;

            // Keep only roughly face-like shapes.
            if (ratio < 0.75 || ratio > 1.35) continue;

            // Remove detections too close to image border.
            if (r.x < 5 || r.y < 5) continue;
            if (r.x + r.width > img.cols - 5) continue;
            if (r.y + r.height > img.rows - 5) continue;

            filteredFaces.push_back(r);
        }

        // Remove duplicate overlapping boxes.
        vector<Rect> finalFaces = removeOverlaps(filteredFaces);

        // Create output folder for this frame.
        string frameFolder = outDir + "/frame_" + to_string(frameNo);
        fs::create_directories(frameFolder);

        int faceNo = 0;

        // Crop and save each final detected face.
        for (const auto& r : finalFaces) {
            faceNo++;

            // Keep crop safely inside image boundary.
            Rect safeRect = r & Rect(0, 0, img.cols, img.rows);

            // Crop face from original color image.
            Mat face = img(safeRect).clone();

            string outPath = frameFolder + "/face_" + to_string(faceNo) + ".jpg";
            imwrite(outPath, face);
        }

        cout << "Processed: " << imgPath.filename().string()
             << " | raw: " << rawFaces.size()
             << " | saved: " << finalFaces.size() << endl;
    }

    cout << "\nDone.\n";
    cout << "Input frames folder : " << inDir << endl;
    cout << "Output faces folder : " << outDir << endl;

    return 0;
}
