{
    "header": {
        "releaseVersion": "2026.1.0+develop",
        "fileVersion": "2.0",
        "nodesVersions": {
            "ApplyCalibration": "1.0",
            "CameraInit": "12.0",
            "CheckerboardDetection": "1.0",
            "ConvertSfMFormat": "2.0",
            "CopyFiles": "1.3",
            "DistortionCalibration": "6.0",
            "ExportAlembic": "1.0",
            "ExportDistortion": "2.0",
            "ExportImages": "1.1",
            "GeometricFilterEstimating": "1.0",
            "ImageDetectionPrompt": "0.2",
            "ImageSegmentationBox": "0.3",
            "IntrinsicsTransforming": "1.1",
            "KeyframeSelection": "5.0",
            "MatchMasking": "1.0",
            "RelativePoseEstimating": "3.1",
            "RomaMatcher": "1.0",
            "RomaReducer": "1.0",
            "RomaSampler": "1.0",
            "ScenePreview": "2.0",
            "SfMBootStrapping": "4.1",
            "SfMExpanding": "2.2",
            "StarListing": "1.0",
            "TracksBuilding": "1.0"
        },
        "template": true
    },
    "graph": {
        "ApplyCalibration_1": {
            "nodeType": "ApplyCalibration",
            "position": [
                212,
                -13
            ],
            "inputs": {
                "input": "{CameraInit_1.output}",
                "calibration": "{DistortionCalibration_1.output}"
            },
            "internalInputs": {
                "color": "#575963"
            }
        },
        "CameraInit_1": {
            "nodeType": "CameraInit",
            "position": [
                2,
                -13
            ],
            "inputs": {},
            "internalInputs": {
                "color": "#575963"
            }
        },
        "CameraInit_2": {
            "nodeType": "CameraInit",
            "position": [
                -415,
                -161
            ],
            "inputs": {},
            "internalInputs": {
                "label": "CameraInitLensGrid",
                "color": "#302e2e"
            }
        },
        "CheckerboardDetection_1": {
            "nodeType": "CheckerboardDetection",
            "position": [
                -205,
                -161
            ],
            "inputs": {
                "input": "{CameraInit_2.output}",
                "useNestedGrids": true,
                "exportDebugImages": true
            },
            "internalInputs": {
                "color": "#302e2e"
            }
        },
        "ConvertSfMFormat_1": {
            "nodeType": "ConvertSfMFormat",
            "position": [
                3782,
                -13
            ],
            "inputs": {
                "fileExt": "json",
                "structure": false,
                "observations": false
            },
            "internalInputs": {
                "color": "#4c594c"
            }
        },
        "CopyFiles_1": {
            "nodeType": "CopyFiles",
            "position": [
                4217,
                -89
            ],
            "inputs": {
                "inputFiles": [
                    "{ScenePreview_1.output}",
                    "{ExportDistortion_1.output}",
                    "{ExportAlembic_1.output}",
                    "{ExportImages_2.output}"
                ]
            }
        },
        "DistortionCalibration_1": {
            "nodeType": "DistortionCalibration",
            "position": [
                3,
                -161
            ],
            "inputs": {
                "input": "{CheckerboardDetection_1.input}",
                "checkerboards": "{CheckerboardDetection_1.output}"
            },
            "internalInputs": {
                "color": "#302e2e"
            }
        },
        "ExportAlembic_1": {
            "nodeType": "ExportAlembic",
            "position": [
                3572,
                -13
            ],
            "inputs": {
                "input": "{ExportImages_2.outputSfMData}"
            },
            "internalInputs": {
                "color": "#80766f"
            }
        },
        "ExportDistortion_1": {
            "nodeType": "ExportDistortion",
            "position": [
                215,
                -161
            ],
            "inputs": {
                "input": "{DistortionCalibration_1.output}"
            },
            "internalInputs": {
                "color": "#302e2e"
            }
        },
        "ExportImages_2": {
            "nodeType": "ExportImages",
            "position": [
                3362,
                -13
            ],
            "inputs": {
                "input": "{IntrinsicsTransforming_2.input}",
                "target": "{IntrinsicsTransforming_2.output}",
                "namingMode": "keep"
            },
            "internalInputs": {
                "color": "#80766f"
            }
        },
        "GeometricFilterEstimating_1": {
            "nodeType": "GeometricFilterEstimating",
            "position": [
                1682,
                -13
            ],
            "inputs": {
                "input": "{RomaReducer_1.inputSfMData}",
                "featuresFolders": [
                    "{RomaReducer_1.featuresFolder}"
                ],
                "matchesFolders": [
                    "{RomaReducer_1.matchesFolder}"
                ],
                "describerTypes": "{RomaReducer_1.describerTypes}",
                "geometricError": 16.0
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "ImageDetectionPrompt_1": {
            "nodeType": "ImageDetectionPrompt",
            "position": [
                217,
                147
            ],
            "inputs": {
                "input": "{CameraInit_1.output}"
            },
            "internalInputs": {
                "color": "#575963"
            }
        },
        "ImageSegmentationBox_1": {
            "nodeType": "ImageSegmentationBox",
            "position": [
                422,
                147
            ],
            "inputs": {
                "input": "{ImageDetectionPrompt_1.input}",
                "bboxFolder": "{ImageDetectionPrompt_1.output}",
                "maskInvert": true,
                "keepFilename": true
            },
            "internalInputs": {
                "color": "#575963"
            }
        },
        "IntrinsicsTransforming_2": {
            "nodeType": "IntrinsicsTransforming",
            "position": [
                3152,
                -13
            ],
            "inputs": {
                "input": "{SfMExpanding_1.output}"
            },
            "internalInputs": {
                "color": "#80766f"
            }
        },
        "KeyframeSelection_1": {
            "nodeType": "KeyframeSelection",
            "position": [
                422,
                -13
            ],
            "inputs": {
                "inputPaths": [
                    "{ApplyCalibration_1.output}"
                ]
            },
            "internalInputs": {
                "color": "#575963"
            }
        },
        "MatchMasking_1": {
            "nodeType": "MatchMasking",
            "position": [
                1052,
                -13
            ],
            "inputs": {
                "inputSfMData": "{RomaMatcher_1.inputSfMData}",
                "imagePairsList": "{RomaMatcher_1.imagePairsList}",
                "warpFolder": "{RomaMatcher_1.outputFolder}",
                "certaintyFolder": "{RomaMatcher_1.outputFolder}",
                "masksFolder": "{ImageSegmentationBox_1.output}"
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "RelativePoseEstimating_1": {
            "nodeType": "RelativePoseEstimating",
            "position": [
                2522,
                -13
            ],
            "inputs": {
                "input": "{TracksBuilding_1.input}",
                "tracksFilename": "{TracksBuilding_1.output}",
                "countIterations": 50000,
                "minInliers": 100,
                "distanceThreshold": 0.0,
                "imagePairsList": "{RomaReducer_2.imagePairsList}"
            },
            "internalInputs": {
                "color": "#575963"
            }
        },
        "RomaMatcher_1": {
            "nodeType": "RomaMatcher",
            "position": [
                842,
                -13
            ],
            "inputs": {
                "inputSfMData": "{StarListing_1.inputSfMData}",
                "imagePairsList": "{StarListing_1.imagePairsList}",
                "checkLoops": true
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "RomaReducer_1": {
            "nodeType": "RomaReducer",
            "position": [
                1472,
                -13
            ],
            "inputs": {
                "inputSfMData": "{RomaSampler_1.inputSfMData}",
                "imagePairsList": "{RomaSampler_1.imagePairsList}",
                "samplesFolder": "{RomaSampler_1.samplesFolder}"
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "RomaReducer_2": {
            "nodeType": "RomaReducer",
            "position": [
                2102,
                -13
            ],
            "inputs": {
                "inputSfMData": "{RomaSampler_2.inputSfMData}",
                "imagePairsList": "{RomaSampler_2.imagePairsList}",
                "samplesFolder": "{RomaSampler_2.samplesFolder}",
                "describerTypes": "{RomaSampler_2.describerTypes}"
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "RomaSampler_1": {
            "nodeType": "RomaSampler",
            "position": [
                1262,
                -13
            ],
            "inputs": {
                "inputSfMData": "{MatchMasking_1.inputSfMData}",
                "imagePairsList": "{MatchMasking_1.imagePairsList}",
                "warpFolder": "{MatchMasking_1.warpFolder}",
                "certaintyFolder": "{MatchMasking_1.outputCertaintyFolder}",
                "maxMatches": 5000
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "RomaSampler_2": {
            "nodeType": "RomaSampler",
            "position": [
                1892,
                -13
            ],
            "inputs": {
                "inputSfMData": "{GeometricFilterEstimating_1.input}",
                "imagePairsList": "{RomaReducer_1.imagePairsList}",
                "warpFolder": "{RomaSampler_1.warpFolder}",
                "certaintyFolder": "{RomaSampler_1.certaintyFolder}",
                "maxMatches": 2500,
                "minCertainty": 0.0,
                "filtersFolder": "{GeometricFilterEstimating_1.output}",
                "describerTypes": "{GeometricFilterEstimating_1.describerTypes}"
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "ScenePreview_1": {
            "nodeType": "ScenePreview",
            "position": [
                3992,
                -13
            ],
            "inputs": {
                "cameras": "{ConvertSfMFormat_1.output}",
                "model": "{ConvertSfMFormat_1.input}",
                "undistortedImages": "{ExportImages_2.output}",
                "masks": "{ImageSegmentationBox_1.output}"
            },
            "internalInputs": {
                "color": "#4c594c"
            }
        },
        "SfMBootStrapping_1": {
            "nodeType": "SfMBootStrapping",
            "position": [
                2732,
                -13
            ],
            "inputs": {
                "input": "{RelativePoseEstimating_1.input}",
                "tracksFilename": "{RelativePoseEstimating_1.tracksFilename}",
                "pairs": "{RelativePoseEstimating_1.output}"
            },
            "internalInputs": {
                "color": "#575963"
            }
        },
        "SfMExpanding_1": {
            "nodeType": "SfMExpanding",
            "position": [
                2942,
                -13
            ],
            "inputs": {
                "input": "{SfMBootStrapping_1.output}",
                "tracksFilename": "{SfMBootStrapping_1.tracksFilename}",
                "meshFilename": "{SfMBootStrapping_1.meshFilename}",
                "minAngleForTriangulation": 1.0,
                "minAngleForLandmark": 0.5
            },
            "internalInputs": {
                "comment": "Estimate cameras parameters for the keyframes.",
                "label": "SfMExpandingKeys",
                "color": "#575963"
            }
        },
        "StarListing_1": {
            "nodeType": "StarListing",
            "position": [
                632,
                -13
            ],
            "inputs": {
                "inputSfMData": "{KeyframeSelection_1.inputPaths[0]}",
                "keySfMData": "{KeyframeSelection_1.outputSfMDataKeyframes}",
                "radiusKeyFrames": 5
            },
            "internalInputs": {
                "color": "#712753"
            }
        },
        "TracksBuilding_1": {
            "nodeType": "TracksBuilding",
            "position": [
                2312,
                -13
            ],
            "inputs": {
                "input": "{RomaReducer_2.inputSfMData}",
                "featuresFolders": [
                    "{RomaReducer_2.featuresFolder}"
                ],
                "matchesFolders": [
                    "{RomaReducer_2.matchesFolder}"
                ],
                "describerTypes": "{RomaReducer_2.describerTypes}",
                "filterTrackForks": true
            },
            "internalInputs": {
                "color": "#575963"
            }
        }
    }
}