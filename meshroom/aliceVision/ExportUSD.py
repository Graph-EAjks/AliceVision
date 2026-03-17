__version__ = "1.0"

from meshroom.core import desc
from meshroom.core.utils import VERBOSE_LEVEL


class ExportUSD(desc.AVCommandLineNode):
    """
Export a 3D mesh to Universal Scene Description (USD) format.

USD is a versatile scene interchange format developed by Pixar, widely used in visual
effects and real-time rendering pipelines. This node converts an input OBJ mesh file
to either a human-readable USDA (ASCII) or compact USDC (binary) USD file. Associated
textures and materials are referenced in the exported scene.
"""

    commandLine = "aliceVision_exportUSD {allParams}"
    size = desc.DynamicNodeSize("input")

    category = "Utils"
    inputs = [
        desc.File(
            name="input",
            label="Input",
            description="Input mesh file.",
            value="",
        ),
        desc.ChoiceParam(
            name="fileType",
            label="USD File Format",
            description="Output USD file format.",
            value="usda",
            values=["usda", "usdc", "usdz"]
        ),
        desc.ChoiceParam(
            name="verboseLevel",
            label="Verbose Level",
            description="Verbosity level (fatal, error, warning, info, debug, trace).",
            values=VERBOSE_LEVEL,
            value="info",
        ),
    ]

    outputs = [
        desc.File(
            name="output",
            label="Output",
            description="Path to the output file.",
            value="{nodeCacheFolder}/output.{fileTypeValue}",
        ),
    ]
