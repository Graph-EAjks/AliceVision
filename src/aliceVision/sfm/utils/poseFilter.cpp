// This file is part of the AliceVision project.
// Copyright (c) 2024 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <ceres/rotation.h>
#include "poseFilter.hpp"
#include <aliceVision/geometry/lie.hpp>
#include <aliceVision/geometry/Pose3.hpp>
#include <aliceVision/sfm/sfmFilters.hpp>

namespace aliceVision {
namespace sfm {

bool poseFilter::process(sfmData::SfMData& sfmData, const bool filterPosition, const bool filterRotation, const int scaleFactor, const int iterationCount)
{
    ALICEVISION_LOG_INFO("poseFilter::process start");

    const int viewCount = sfmData.getViews().size();

    sfmData::Poses& poses = sfmData.getPoses();

    std::vector<IndexT> poseIdsVec(viewCount);

    if (!getOrderedPoseIds(sfmData, poseIdsVec))
    {
        return false;
    }

    tempFilter tFilter;

    tFilter.init();

    Eigen::MatrixXd viewRotations(3, viewCount);
    Eigen::MatrixXd viewCenters(3, viewCount);

    for (int frameIdx = 0; frameIdx < viewCount; frameIdx++)
    {
        const sfmData::CameraPose framePose = poses.at(poseIdsVec[frameIdx]);
        viewCenters.col(frameIdx) = framePose.getTransform().center();
        viewRotations.col(frameIdx) = SO3::logm(framePose.getTransform().rotation());
    }

    if (filterPosition)
    {
        viewCenters = tFilter.applyMultiscale(viewCenters, scaleFactor, iterationCount, false);
    }

    if (filterRotation)
    {
        viewRotations = tFilter.applyMultiscale(viewRotations, scaleFactor, iterationCount, true);
    }

    for (int frameIdx = 0; frameIdx < viewCount; frameIdx++)
    {
        geometry::Pose3 newPose(SO3::expm(viewRotations.col(frameIdx)), viewCenters.col(frameIdx));
        sfmData.setPose(sfmData.getView(poseIdsVec[frameIdx]), sfmData::CameraPose(newPose));
    }

    return true;
}


bool poseFilter::getOrderedPoseIds(sfmData::SfMData& sfmData, std::vector<IndexT>& poseIdsVec)
{
    const int viewCount = sfmData.getViews().size();

    int minFrameId = -1;
    int maxFrameId = -1;
    int minFrameIdWithPose = -1;

    for (const auto& pView : sfmData.getViews())
    {
        const int frameId = int(pView.second->getFrameId());
        if (minFrameId == -1 || frameId < minFrameId)
        {
            minFrameId = frameId;
        }
        if (minFrameIdWithPose == -1 || frameId < minFrameIdWithPose)
        {
            if (sfmData.existsPose(*pView.second))
                minFrameIdWithPose = frameId;
        }
        if (frameId > maxFrameId)
            maxFrameId = frameId;
    }

    const int frameIdRange = maxFrameId - minFrameId + 1;

    ALICEVISION_LOG_INFO(" minFrameId : " << minFrameId);
    ALICEVISION_LOG_INFO(" maxFrameId : " << maxFrameId);

    if (frameIdRange != viewCount)
    {
        return false;
    }

    for (const auto& pView : sfmData.getViews())
    {
        const int frameId = int(pView.second->getFrameId());
        poseIdsVec[frameId-minFrameId] = pView.second->getPoseId();
    }

    ALICEVISION_LOG_INFO(" minFrameIdWithPose : " << minFrameIdWithPose);

    sfmData::Poses& poses = sfmData.getPoses();
    sfmData::CameraPose lastValidPose = poses.at(poseIdsVec[minFrameIdWithPose-minFrameId]);

    // fill in the blanks within the camera poses list

    for (int frameId = minFrameId; frameId <= maxFrameId; frameId++)
    {
        auto framePose = poses.find(poseIdsVec[frameId-minFrameId]);

        if (framePose == poses.end()) // i.e. pose does not exist
            poses.emplace(poseIdsVec[frameId-minFrameId], lastValidPose.getTransform());
        else
            lastValidPose = framePose->second;
    }

    return true;
}

} // namespace sfm
} // namespace aliceVision


bool tempFilter::init()
{
    using namespace Eigen;
    using namespace Eigen::indexing;

    // Savitzky-Golay smoothing filter
    // Reference: https://en.wikipedia.org/wiki/Savitzky-Golay_filter

    // Savitzky-Golay smoothing filter coefficients (window size 9, polynomial order 2)
    std::vector<double> filterCoeffVec = {-21., 14., 39., 54., 59., 54., 39., 14., -21.};
    filterCoeff = Map<VectorXd>(filterCoeffVec.data(), 9) / 231.;

    // Savitzky-Golay first derivative filter coefficients
    filterCoeffVec = {21., 7., -32., -86., 86., 32., -7., -21.};
    diffFilterCoeff = Map<VectorXd>(filterCoeffVec.data(), 8) / 231.;

    // Savitzky-Golay linear term coefficients
    filterCoeffVec = {-4., -3., -2., -1., 0., 1., 2., 3., 4.};
    VectorXd filterCoeff_b = Map<VectorXd>(filterCoeffVec.data(), 9) / 60.;

    // Savitzky-Golay quadratic term coefficients
    filterCoeffVec = {28., 7., -8., -17., -20., -17., -8., 7., 28.};
    VectorXd filterCoeff_c = Map<VectorXd>(filterCoeffVec.data(), 9) / 924.;

    MatrixXd filterCoeff_x = MatrixXd(kernelSize, kernelSize);

    for (int coeffIndex = 0; coeffIndex < kernelSize; coeffIndex++)
    {
        double x = coeffIndex - kernelSize / 2;
        for (int filterIndex = 0; filterIndex < kernelSize; filterIndex++)
        {
            filterCoeff_x(coeffIndex, filterIndex) = filterCoeff(filterIndex) + filterCoeff_b(filterIndex) * x + filterCoeff_c(filterIndex) * (x * x);
        }
    }

    tailFilter = filterCoeff_x(all, seq(0, last/2-1));
    headFilter = filterCoeff_x(all, seq(last/2+1, last));

    MatrixXd diffFilterCoeff_x = MatrixXd(kernelSize-1, kernelSize);

    for (int filterIndex = 0; filterIndex < kernelSize; filterIndex++)
    {
        if (filterIndex > 0)
            diffFilterCoeff_x(0, filterIndex) = -filterCoeff_x(0, filterIndex);

        for (int coeffIndex = 1; coeffIndex < filterIndex; coeffIndex++)
            diffFilterCoeff_x(coeffIndex, filterIndex) = diffFilterCoeff_x(coeffIndex-1, filterIndex) - filterCoeff_x(coeffIndex, filterIndex);

        if (filterIndex < kernelSize-1)
            diffFilterCoeff_x(kernelSize-2, filterIndex) = filterCoeff_x(kernelSize-1, filterIndex);

        for (int coeffIndex = kernelSize-3; coeffIndex >= filterIndex; coeffIndex--)
            diffFilterCoeff_x(coeffIndex, filterIndex) = diffFilterCoeff_x(coeffIndex+1, filterIndex) + filterCoeff_x(coeffIndex+1, filterIndex);
    }

    for (int filterIndex = 0; filterIndex < kernelSize; filterIndex++)
    {
        assert (abs(diffFilterCoeff_x(all, filterIndex).sum()) < 1e-13);

        if (filterIndex == 0)
            assert (abs(filterCoeff_x(filterIndex, filterIndex) + diffFilterCoeff_x(filterIndex, filterIndex) - 1.) < 1e-13);
        else if (filterIndex == kernelSize-1)
            assert (abs(filterCoeff_x(filterIndex, filterIndex) - diffFilterCoeff_x(filterIndex-1, filterIndex) - 1.) < 1e-13);
        else
            assert (abs(filterCoeff_x(filterIndex, filterIndex) - diffFilterCoeff_x(filterIndex-1, filterIndex) + diffFilterCoeff_x(filterIndex, filterIndex) - 1.) < 1e-13);
    }

    tailDiffFilter = diffFilterCoeff_x(all, seq(0, last/2-1));
    headDiffFilter = diffFilterCoeff_x(all, seq(last/2+1, last));

    initialized = true;
    return true;
}


Eigen::MatrixXd tempFilter::apply(Eigen::MatrixXd& inputSignal, bool isAngle)
{
    using namespace aliceVision::SO3;
    using namespace Eigen;
    using namespace indexing;

    assert(initialized);

    if (inputSignal.cols() < kernelSize)
        return inputSignal;

    MatrixXd filteredSignal(inputSignal.rows(), inputSignal.cols());

    const int innersize = inputSignal.cols() - 2 * (kernelSize/2);

    // The filter used for the angles is equivalent to the filter used for the positions
    // but it is designed to work with temporal diff signals to increase accuracy

    if (isAngle)
    {
        MatrixXd diffSignal(inputSignal.rows(), inputSignal.cols()-1);
        // If rotation angles were easy to deal with:
        // diffSignal = inputSignal(all, seqN(1, inputSignal.cols()-1)) - inputSignal(all, seqN(0, inputSignal.cols()-1));

        VectorXd norms(inputSignal.cols()-1);

        for (int col = 0; col < inputSignal.cols() - 1; col++)
        {
            Matrix3d prevMat3 = expm(inputSignal.col(col));
            Matrix3d currMat3 = expm(inputSignal.col(col+1));

            Matrix3d diffMat3 = currMat3 * prevMat3.transpose();
            Vector3d diffSo3 = logm(diffMat3);
            diffSignal.col(col) = diffSo3;
        }

        // The filter is designed to use as input the temporal diff of the input signal,
        // and to output a delta to the input signal
        // i.e. diffSignal = inputSignal_(t+1) - inputSignal_(t)
        //      filteredSignal = inputSignal_(t) + diffFilteredSignal_(t)
        MatrixXd diffFilteredSignal(inputSignal.rows(), inputSignal.cols());

        diffFilteredSignal(all, seqN(kernelSize/2, innersize)) = diffSignal(all, seqN(fix<0>, innersize)) * diffFilterCoeff(0)
                                                               + diffSignal(all, seqN(fix<1>, innersize)) * diffFilterCoeff(1)
                                                               + diffSignal(all, seqN(fix<2>, innersize)) * diffFilterCoeff(2)
                                                               + diffSignal(all, seqN(fix<3>, innersize)) * diffFilterCoeff(3)
                                                               + diffSignal(all, seqN(fix<4>, innersize)) * diffFilterCoeff(4)
                                                               + diffSignal(all, seqN(fix<5>, innersize)) * diffFilterCoeff(5)
                                                               + diffSignal(all, seqN(fix<6>, innersize)) * diffFilterCoeff(6)
                                                               + diffSignal(all, seqN(fix<7>, innersize)) * diffFilterCoeff(7);

        diffFilteredSignal(all, seqN(fix<0>, fix<4>)) = diffSignal(all, seqN(fix<0>, fix<8>)) * tailDiffFilter;
        diffFilteredSignal(all, seqN(last-fix<3>, fix<4>)) = diffSignal(all, seqN(last-fix<7>, fix<8>)) * headDiffFilter;

        // If rotation angles were easy to deal with:
        // filteredSignal = inputSignal + diffFilteredSignal;

        for (int col = 0; col < inputSignal.cols(); col++)
        {
            Matrix3d inputMat3 = expm(inputSignal.col(col));
            Matrix3d diffFilteredMat3 = expm(diffFilteredSignal.col(col));

            Matrix3d resMat3 = diffFilteredMat3 * inputMat3 ;
            Vector3d resSo3 = logm(resMat3);
            filteredSignal.col(col) = resSo3;
        }

        for (int col = 0; col < inputSignal.cols() - 1; col++)
        {
            Matrix3d prevMat3 = expm(filteredSignal.col(col));
            Matrix3d currMat3 = expm(filteredSignal.col(col+1));

            Matrix3d diffMat3 = currMat3 * prevMat3.transpose();
            Vector3d diffSo3 = logm(diffMat3);
            diffSignal.col(col) = diffSo3;
        }
    }
    else
    {
        filteredSignal(all, seqN(kernelSize/2, innersize)) = inputSignal(all, seqN(fix<0>, innersize)) * filterCoeff(0)
                                                           + inputSignal(all, seqN(fix<1>, innersize)) * filterCoeff(1)
                                                           + inputSignal(all, seqN(fix<2>, innersize)) * filterCoeff(2)
                                                           + inputSignal(all, seqN(fix<3>, innersize)) * filterCoeff(3)
                                                           + inputSignal(all, seqN(fix<4>, innersize)) * filterCoeff(4)
                                                           + inputSignal(all, seqN(fix<5>, innersize)) * filterCoeff(5)
                                                           + inputSignal(all, seqN(fix<6>, innersize)) * filterCoeff(6)
                                                           + inputSignal(all, seqN(fix<7>, innersize)) * filterCoeff(7)
                                                           + inputSignal(all, seqN(fix<8>, innersize)) * filterCoeff(8);

        filteredSignal(all, seqN(fix<0>, fix<4>)) = inputSignal(all, seqN(fix<0>, fix<9>)) * tailFilter;
        filteredSignal(all, seqN(last-fix<3>, fix<4>)) = inputSignal(all, seqN(last-fix<8>, fix<9>)) * headFilter;
    }

    return filteredSignal;
}


Eigen::MatrixXd tempFilter::applyMultiscale(Eigen::MatrixXd& inputSignal, const unsigned int scaleFactor, const int iterationCount, bool isAngle)
{
    using namespace Eigen;
    using namespace indexing;

    MatrixXd filteredSignal(inputSignal);

    // The multi-scale filter applies filtering at decreasing scales
    // controlled by the following constant
    const double SCALE_REDUCTION_FACTOR = 1.4;

    for (int scaleF = scaleFactor; scaleF >= 1; scaleF = (scaleF > 1) ? round(double(scaleF) / SCALE_REDUCTION_FACTOR) : 0)
    {
        ALICEVISION_LOG_INFO(" Filter scale factor : " << scaleF);

        if (inputSignal.cols() < scaleF)
            continue;

        for (int phase = 0; phase < scaleF; phase++)
        {
            MatrixXd scaledSignal(filteredSignal(all, seq(phase, last, scaleF)));
            for (int iterFilter = 0; iterFilter < iterationCount; iterFilter++)
            {
                scaledSignal = apply(scaledSignal, isAngle);
            }
            filteredSignal(all, seq(phase, last, scaleF)) = scaledSignal;
        }
    }

    return filteredSignal;
}
