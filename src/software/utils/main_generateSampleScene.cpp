// This file is part of the AliceVision project.
// Copyright (c) 2021 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <aliceVision/numeric/numeric.hpp>
#include <aliceVision/system/Logger.hpp>
#include <aliceVision/cmdline/cmdline.hpp>
#include <aliceVision/system/main.hpp>
#include <aliceVision/sfmDataIO/sfmDataIO.hpp>
#include <aliceVision/sfmDataIO/sceneSample.hpp>

#include <filesystem>
#include <boost/program_options.hpp>

// These constants define the current software version.
// They must be updated when the command line is changed.
#define ALICEVISION_SOFTWARE_VERSION_MAJOR 1
#define ALICEVISION_SOFTWARE_VERSION_MINOR 0

using namespace aliceVision;

namespace fs = std::filesystem;
namespace po = boost::program_options;

int aliceVision_main(int argc, char** argv)
{
    // command-line parameters
    std::string sfmOutputDataFilepath;  // output folder for sfm Data
    std::string sceneSample;  // scene type to generate ('cube' or 'sphere')
    float positionNoise = 0.;
    float rotationNoise = 0.;

    // clang-format off
    po::options_description requiredParams("Required parameters");
    requiredParams.add_options()
        ("output,o", po::value<std::string>(&sfmOutputDataFilepath)->required(),
         "Output sfm file to generate.")
        ("scene,s", po::value<std::string>(&sceneSample)->default_value("sphere"),
         "Scene sample to generate. ['cube', 'sphere']");

    po::options_description optionalParams("Optional parameters");
    optionalParams.add_options()
        ("positionNoise", po::value<float>(&positionNoise)->default_value(positionNoise), "Noise level to add to camera positions.")
        ("rotationNoise", po::value<float>(&rotationNoise)->default_value(rotationNoise), "Noise level to add to camera orientations.");
    // clang-format on

    CmdLine cmdline("This program is used to generate a sample scene and save it to a given file path.\n"
                    "AliceVision generateSampleScene");
    cmdline.add(requiredParams);
    cmdline.add(optionalParams);
    if (!cmdline.execute(argc, argv))
    {
        return EXIT_FAILURE;
    }

    sfmData::SfMData sfmData;
    sfmDataIO::generateSampleScene(sfmData, sceneSample, positionNoise, rotationNoise);

    ALICEVISION_LOG_INFO("Export SfM: " << sfmOutputDataFilepath);
    if (!sfmDataIO::save(sfmData, sfmOutputDataFilepath, sfmDataIO::ESfMData(sfmDataIO::ALL)))
    {
        ALICEVISION_LOG_ERROR("The output SfMData file '" << sfmOutputDataFilepath << "' cannot be write.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
