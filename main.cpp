#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string_view>
#include <algorithm>
#include <cmath>

// WINDOWS
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void downgradeOsuFile(std::filesystem::path filePath, bool keepOD);

std::string removeCarriageReturn(std::string str)
{
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    return str;
}
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}
std::string replaceString(std::string subject, const std::string& search, const std::string& replace)
{
    size_t pos = subject.find(search);
    while (pos != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos = subject.find(search, pos + replace.length());
    }
    return subject;
}
bool string_contains(std::string input, std::string key)
{
    if (input.find(key) != std::string::npos)
    {
        return true;
    }
    return false;
}
std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
std::vector<std::string> split_first(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    if ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        res.push_back(token);
        pos_start = pos_end + delim_len;
    }

    res.push_back(s.substr(pos_start));
    return res;
}
struct Vector2 {
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x_, float y_) : x(x_), y(y_) {}

    float LengthSquared() const {
        return x * x + y * y;
    }

    float Length() const {
        return std::sqrt(LengthSquared());
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    static float Dot(const Vector2& a, const Vector2& b) {
        return a.x * b.x + a.y * b.y;
    }
};
std::vector<Vector2> CircularArcAutoPointCount(const Vector2& A, const Vector2& B, const Vector2& C) {
    float d = 2 * (A.x * (B - C).y + B.x * (C - A).y + C.x * (A - B).y);
    float aSq = A.LengthSquared();
    float bSq = B.LengthSquared();
    float cSq = C.LengthSquared();

    Vector2 centre = Vector2(
        aSq * (B - C).y + bSq * (C - A).y + cSq * (A - B).y,
        aSq * (C - B).x + bSq * (A - C).x + cSq * (B - A).x
    ) / d;

    Vector2 dA = A - centre;
    Vector2 dC = C - centre;
    float radius = dA.Length();

    double thetaStart = std::atan2(dA.y, dA.x);
    double thetaEnd = std::atan2(dC.y, dC.x);

    while (thetaEnd < thetaStart)
        thetaEnd += 2 * M_PI;

    int direction = 1;
    double thetaRange = thetaEnd - thetaStart;

    // Side check
    Vector2 orthoAtoC = Vector2((C - A).y, -(C - A).x);
    if (Vector2::Dot(orthoAtoC, B - A) < 0) {
        direction = -direction;
        thetaRange = 2 * M_PI - thetaRange;
    }

    // Determine number of points
    int numPoints;
    if (2 * radius <= 0.1f) {
        numPoints = 2;
    }
    else {
        double denom = 2 * std::acos(1 - 0.1f / radius);
        double raw = thetaRange / denom;
        int autoPts = static_cast<int>(std::ceil(raw));
        numPoints = std::max(2, autoPts);
    }

    std::vector<Vector2> output;
    output.reserve(numPoints);

    for (int i = 0; i < numPoints; ++i) {
        double fract = static_cast<double>(i) / (numPoints - 1);
        double theta = thetaStart + direction * fract * thetaRange;
        float cosT = static_cast<float>(std::cos(theta));
        float sinT = static_cast<float>(std::sin(theta));

        Vector2 offset = Vector2(cosT, sinT) * radius;
        output.push_back(centre + offset);
    }

    return output;
}

int main()
{
    int option_range = 0;
    std::cout << "##################\nosuTo2007 v1.7\nosu! : _Railgun_\nDiscord : @railgun_osu\n##################\n\n";
    std::vector<std::filesystem::path> map_list;
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
    {
        if (entry.path().extension() == ".osu")
        {
            map_list.push_back(entry.path());
        }
    }
    if (map_list.size() == 0)
    {
        std::cout << "No .osu files found\nExiting...";
        return 0;
    }
    else
    {
        std::cout << "Choose which .osu file you want to convert to v3:\n\n";
        if (map_list.size() > 1)
        {
            std::cout << "0 = All listed below\n";
        }
        for (size_t i = 1; i - 1 < map_list.size(); i++)
        {
            std::cout << i << " = " << map_list[i - 1].filename() << "\n";
            option_range++;
        }
    }
opt:
    std::cout << "Choose an option:\n";
    std::string input;
    std::cin >> input;
    if (!is_number(input))
    {
        std::cout << "Not a number\n";
        goto opt;
    }
    if (!(std::stoi(input) < option_range + 1))
    {
        std::cout << "Invalid Option\n";
        goto opt;
    }
    bool keep_OD;
opt2:
    std::cout << "Because in v3 AR(ApproachRate) is tied to OD(OverallDifficulty):\nOD = AR\nDo you prefer to have:\n1 = same OverallDifficulty (map might be hard to read)\n2 = same ApproachRate (map might be hard to acc)\n";
    std::string input2;
    std::cin >> input2;
    if (!is_number(input2))
    {
        std::cout << "Not a number\n";
        goto opt2;
    }
    if ((std::stoi(input2) < 1) || (std::stoi(input2) > 2))
    {
        std::cout << "Invalid Option\n";
        goto opt2;
    }
    if (input2 == "1")
    {
        keep_OD = true;
    }
    else
    {
        keep_OD = false;
    }
    if (std::stoi(input) == 0)
    {
        for (int i = 0; i < map_list.size(); i++)
        {
            downgradeOsuFile(map_list[i], keep_OD);
        }
    }
    else
    {
        downgradeOsuFile(map_list[std::stoi(input) - 1], keep_OD);
    }
    return 0;
}

void downgradeOsuFile(std::filesystem::path filePath, bool keepOD)
{
    // predefined keys to search for
    static std::vector<std::string> general_Var = { "AudioFilename", "PreviewTime", "SampleSet" };
    static std::vector<std::string> metaData_Var = { "Title", "Artis", "Creator", "Version" };
    static std::vector<std::string> difficulty_Var = { "HPDrainRate", "CircleSize", "OverallDifficulty", "ApproachRate", "SliderMultiplier", "SliderTickRate" };

    // current filename
    std::string fileName = "(converted) " + filePath.filename().string();

    // osu file format version
    std::string fileFormat = "";
    std::string OD_line = "";
    std::string AR_line = "";
    std::vector<std::pair<std::string, std::string>> general, metadata, difficulty;
    std::vector<std::string> events, timingPoints, hitObjects;
    bool skip = false;
    int section = 0;
    std::cout << "Downgrading " << filePath.filename() << " to v3 file format...\n";
    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line))
        {

            if (line.starts_with("osu file format v"))
            {
                fileFormat = replaceString(line, "osu file format v", "");
                std::cout << "Target file is version: " << fileFormat << "\n";
                if (fileFormat == "3")
                {
                    std::cout << "Skipping conversion..." << "\n";
                    skip = true;
                    file.close();
                    goto abort;
                }
            }
            if (line.starts_with("["))
            {
                // this is fine as it only gets executed once on a new section, code looks shit but it works
                if (line.starts_with("[General]"))
                {
                    section = 1;
                }
                if (line.starts_with("[Editor]"))
                {
                    // skip -> not present in 2007 client
                    section = 0;
                }
                if (line.starts_with("[Metadata]"))
                {
                    section = 2;
                }
                if (line.starts_with("[Difficulty]"))
                {
                    section = 3;
                }
                if (line.starts_with("[Events]"))
                {
                    section = 4;
                }
                if (line.starts_with("[TimingPoints]"))
                {
                    section = 5;
                }
                if (line.starts_with("[Colours]"))
                {
                    // skip -> not present in 2007 client
                    section = 0;
                }
                if (line.starts_with("[HitObjects]"))
                {
                    section = 6;
                }
            }

            switch (section)
            {
            case 0:
                // skip
                break;
            case 1:
                // General
                if (line != "" && !(line.starts_with(":")) && !(line.starts_with("[")))
                {
                    for (int i = 0; i < general_Var.size(); i++)
                    {
                        if (line.starts_with(general_Var[i]))
                        {
                            general.push_back(std::make_pair(std::string(general_Var[i]), std::string((replaceString(line, general_Var[i] + ": ", "")))));
                        }
                    }
                }
                break;
            case 2:
                // Metadata
                if (line != "" && !(line.starts_with(":")) && !(line.starts_with("[")))
                {
                    for (int i = 0; i < metaData_Var.size(); i++)
                    {
                        if (line.starts_with(metaData_Var[i] + ":"))
                        {
                            metadata.push_back(std::make_pair(std::string(metaData_Var[i]), std::string((replaceString(line, metaData_Var[i] + ":", "")))));
                        }
                    }
                }
                break;
            case 3:
                // Difficulty
                if (line != "" && !(line.starts_with(":")) && !(line.starts_with("[")))
                {
                    for (int i = 0; i < difficulty_Var.size(); i++)
                    {
                        if (line.starts_with(difficulty_Var[i]))
                        {
                            //   std::cout << line << "\n";
                            switch (i)
                            {
                            case 2:
                                // skip
                                OD_line = line;
                                break;
                            case 3:
                                if (keepOD)
                                {
                                    // keep OD
                                    difficulty.push_back(std::make_pair(std::string(difficulty_Var[2]), std::string((replaceString(OD_line, difficulty_Var[2] + ":", "")))));
                                }
                                else
                                {
                                    // overwrite OD with AR
                                    difficulty.push_back(std::make_pair(std::string(difficulty_Var[2]), std::string((replaceString(line, difficulty_Var[3] + ":", "")))));
                                }
                                break;
                            default:
                                difficulty.push_back(std::make_pair(std::string(difficulty_Var[i]), std::string((replaceString(line, difficulty_Var[i] + ":", "")))));
                            }
                        }
                    }
                }
                break;
            case 4:
                // Events
                if (line != "" && !(line.starts_with("[")) && !(line.starts_with("//")) && !line.empty())
                {

                    // handle spaces from the beginning
                    if (line[0] == ' ')
                    {

                        for (int sp = 0; sp < line.length(); sp++)
                        {
                            if (line[sp] == ' ')
                            {
                            }
                            else
                            {
                                // first non space char found
                                // number check bc b99 doesnt support storyboard fully
                                std::string eChar = { line[sp] };
                                if (is_number(eChar))
                                {
                                    // all good
                                    events.push_back(line);
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        // number check bc b99 doesnt support storyboard fully
                        std::string eChar = { line[0] };
                        if (!line.starts_with("//") && is_number(eChar))
                        {
                            events.push_back(line);
                        }
                    }
                }
                break;
            case 5:
                // Timing Points
                if (line != "" && !(line.starts_with("[")))
                {
                    if (line.starts_with("[HitObjects]"))
                    {
                        section = 6;
                        break;
                    }
                    else
                    {
                        if (!line.empty())
                        {
                            timingPoints.push_back(line);
                        }
                    }
                }
                break;
            case 6:
                // HitObjects
                if (line != "" && !line.starts_with("["))
                {
                    hitObjects.push_back(line);
                }

                break;
            }
        }
        file.close();
    }
abort:

    if (!skip)
    {
        // process file
        std::string output = "osu file format v3\n\n[General]\n";

        for (int i = 0; i < general.size(); i++)
        {
            output += general[i].first + ": " + general[i].second + "\n";
        }

        output += "\n[Metadata]\n";

        for (int i = 0; i < metadata.size(); i++)
        {
            if (metadata[i].first == "Version")
            {
                metadata[i].second = "(Converted) " + metadata[i].second;
            }
            output += metadata[i].first + ":" + metadata[i].second + "\n";
        }

        output += "\n[Difficulty]\n";

        for (int i = 0; i < difficulty.size(); i++)
        {
            if (string_contains(difficulty[i].second, ".") && i < 3)
            {
                // 2007 client doesnt support decimal input on HP/CS/OD -> resulting in crash
                difficulty[i].second = difficulty[i].second[0];
            }

            output += difficulty[i].first + ":" + difficulty[i].second + "\n";
        }

        // Events
        output += "\n[Events]\n";
        for (int i = 0; i < events.size(); i++)
        {
            output += events[i] + "\n";
        }

        // Timing Points
        // Because v3 doesn't support sv points,
        // we need to convert those to bpm points.
        output += "\n[TimingPoints]\n";
        double currentBPM = 120.0f; // temp storing bpm for sv conversion
        double currentSV = 1.0f;    // temp storing sv
        double multiplier = 1.0f;
        for (int i = 0; i < timingPoints.size(); i++)
        {
            // std::cout << "Current TimingPoint Line: " << timingPoints[i] << "\n";
            if (split(timingPoints[i], ",").size() < 2)
            {
                //  std::cout << "This would lead to a Segmentation Fault\n";
            }
            else
            {
                bool duplicate = false;
                std::vector<std::string> v = split(removeCarriageReturn(timingPoints[i]), ",");

                if (i + 1 < timingPoints.size())
                {
                    // next timing point exists.
                    std::vector<std::string> v1 = split(removeCarriageReturn(timingPoints[i + 1]), ",");
                    if (v[0] == v1[0])
                    {
                        // check if next timing point is the same time, if so, skip
                        if (v1[1].starts_with("-"))
                        {
                            currentBPM = (((1 / std::stod(v[1])) * 1000) * 60);
                            continue;
                        }
                        else
                        {
                            // 2x timing points at same ms wtf????
                            continue;
                        }
                    }
                }

                output += v[0] + ","; // ms
                if (v[1].starts_with("-"))
                {
                    // slider velocity point
                    currentSV = std::stod(replaceString(v[1], "-", ""));
                    multiplier = 100 / currentSV;
                    //   std::cout << "New SV Point: " << multiplier << "x | Converted BPM: ";
                    if (i == 0)
                    {
                        //  **I assume there is no SV Point before a timing point, proof me wrong**
                        //
                        //  if slider velocity before timing point
                        //  get the first timing point present
                        //  std::string indexT = 0;
                        //  for (int iii = 0; iii < timingPoints.size(); iii++)
                        //  {
                        //    std::vector<std::string> vT = split(timingPoints[iii], ",");
                        //   if (!vT[1].starts_with("-"))
                        //   {
                        //      indexT = vT[1];
                        //      break; // found timing point
                        //    }
                        //  }
                        //  1/beatLength*1000*60
                        //  currentBPM = (((1 / std::stof(indexT)) * 1000) * 60);
                        //  output += std::to_string((currentBPM * multiplier)*1/1000/60) + "\n";
                    }
                    else
                    {
                        //  std::cout << currentBPM << "BPM -> " << (currentBPM * multiplier) << "BPM\n";
                        output += std::to_string(60000 / (currentBPM * multiplier)) + "\n";
                    }
                }
                else
                {
                    // timing point
                    output += v[1] + "\n";
                    currentBPM = (((1 / std::stod(v[1])) * 1000) * 60);
                    //  std::cout << "New Timing Point: " << currentBPM << "BPM\n";
                }
            }
        }
    
        // HitObjects
        output += "\n[HitObjects]\n";
        for (int i = 0; i < hitObjects.size(); i++)
        {
            if (string_contains(hitObjects[i], "|"))
            {
                // -------------------------------------------
                // Experimental: Pre Patch: Convert "P" - Perfect Circle Slider Types to Bezier
                // -------------------------------------------
                // Thanks Digitalfear117 :3
                // -------------------------------------------
                if (string_contains(hitObjects[i], ",P|"))
                {
                    hitObjects[i] = replaceString(hitObjects[i], ",P|", ",B|");
                    std::string new_slider_anchor="";
                    std::vector<std::string> v3 = split(hitObjects[i], "|");
                    std::vector<std::string> v4 = split(v3[0], ",");
                    std::vector<std::string> v5 = split(v3[1], ":");
                    std::vector<std::string> v6 = split(split(v3[2], ",")[0], ":");
                    std::vector<Vector2> newAnchor = CircularArcAutoPointCount(Vector2(std::stoi(v4[0]), std::stoi(v4[1])), Vector2(std::stoi(v5[0]), std::stoi(v5[1])), Vector2(std::stoi(v6[0]), std::stoi(v6[1])));
                    for (size_t vk = 0; vk < newAnchor.size() - 1; vk++) {
                        new_slider_anchor += std::to_string(newAnchor[vk].x) + ":" + std::to_string(newAnchor[vk].y);
                        if (vk < newAnchor.size() - 2)
                            new_slider_anchor += "|";
                        
                    }
                    // Patch
                    v3[1] = new_slider_anchor;

                    // Resonstruct Slider String
                    std::string sliderString = "";
                    for (size_t sT = 0; sT < v3.size(); sT++) {
                        sliderString += v3[sT];
                        if (sT < v3.size() - 1)
                            sliderString += "|";
                        
                    }
                    // Overwrite
                    hitObjects[i] = sliderString;

                }

                // Slider
                std::vector<std::string> v = split(hitObjects[i], ",");
                std::string Sx = v[0];
                std::string Sy = v[1];
                std::string slider = "";

                // -------------------------------------------
                // Fist Slider Patch: anchor fix
                // -------------------------------------------
                // In v3 the first anchor point must represent the starting circle of the slider.

                v[5] = v[5].insert(1, "|" + Sx + ":" + Sy);
                for (int b = 0; b < v.size(); b++)
                {
                    if (b == v.size() - 1)
                    {
                        slider += v[b];
                    }
                    else
                    {
                        slider += v[b] + ",";
                    }
                }

                // -------------------------------------------
                // Second Slider Patch: multi-type slider fix
                // -------------------------------------------
                // Because v3 doesn't support multiplie slider types in one slider...
                // Example: Bezir -> Linear -> Bezir
                // We need to duplicate the anchor point a few times at the same spot & time,
                // to "create" a linear slider inside of a bezir slider.

                std::string slider2 = "";
                std::vector<std::string> v1 = split(slider, "|");
                for (int c = 0; c < v1.size(); c++)
                {
                    if (c != v1.size() - 1)
                    {
                        if (v1[c] == v1[c + 1])
                        {
                            // Red anchor points are detected by a pair of the same anchor point
                            // Example ...B|142:107|208:64|208:64|186:140|...
                            //                      ^^^^^^ ^^^^^^
                            std::string red_anchor = "";
                            for (int anc = 0; anc < 10; anc++) // should be enough?
                            {
                                red_anchor += v1[c] + "|";
                            }
                            slider2 += red_anchor;
                        }
                        else
                        {
                            slider2 += v1[c] + "|";
                        }
                    }
                    else
                    {
                        slider2 += v1[c];
                    }
                }

                output += slider2 + "\n";
            }
            else
            {
                // Circle, write to output as it is, as no issues with modern circle lines are found YET
                output += hitObjects[i] + "\n";
            }
        }
        std::cout << fileName << "\n";
        std::ofstream out(fileName);
        out << output;
        out.close();
        std::cout << "----------------\nDONE\n----------------\n";
    }
}