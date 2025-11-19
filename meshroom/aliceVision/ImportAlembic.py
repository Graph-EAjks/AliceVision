__version__ = "1.1"

from meshroom.core import desc
from meshroom.core.utils import VERBOSE_LEVEL


class ImportAlembic(desc.AVCommandLineNode):
    commandLine = "aliceVision_importAlembic {allParams}"

    category = "Utils"
    documentation = """
Import an external Alembic file that does not follow the SfMData convention, and populates a valid SfMData with its camera poses.
"""

    inputs = [
        desc.File(
            name="input",
            label="Alembic File",
            description="The external Alembic file to import.",
            value="",
        ),
        desc.FloatParam(
            name="framerate",
            label="Framerate",
            description="Alembic frame rate to compute frame id from time",
            value=24.0,
            range=(10.0, 50.0, 1.0),
        ),
        desc.IntParam(
            name="imageWidth",
            label="Image(s) Width",
            description="Alembic doesn't export the camera resolutions. \n"
                        "Setup the images width for all images, the height will depends on the sensor size ratio.",
            value=1920,
            range=(640, 10000, 10),
        ),
        desc.ChoiceParam(
            name="verboseLevel",
            label="Verbose Level",
            description="Verbosity level (fatal, error, warning, info, debug, trace).",
            value="info",
            values=VERBOSE_LEVEL,
        ),
    ]

    outputs = [
        desc.File(
            name="output",
            label="SfMData",
            description="SfMData file populated with the camera poses from the external Alembic file.",
            value="{nodeCacheFolder}/importedAbc.abc",
        ),
    ]
