#!/usr/bin/env python3
"""Check enhanced skybox PNGs without game files or image libraries.

Validate PNG checksums and scanlines, exact four-times dimensions from the
asset manifest, preserved panorama placement, and detail inside the
4x4 blocks that a nearest-neighbor enlargement would leave uniformly colored.
"""

import argparse
from collections import namedtuple
import json
from pathlib import Path
import struct
import zlib


ROOT = Path(__file__).resolve().parents[2]
SCENES = ("desert", "tropical", "alpine", "city", "country")
SHAPES = ("scen", "sce2", "sce3", "sce4")
SCALE = 4
SKY_RGB = bytes((93, 255, 255))
RGB_CHANNELS = 3
RGB_COMPONENT_MAX = 255
PALETTE_COLOR_COUNT = 256
PANORAMA_WIDTH = 1024
DETAIL_CHANNEL_RANGE = 8
DETAIL_REQUIRED_DENOMINATOR = 10
PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"
PNG_CHUNK_HEADER_SIZE = struct.calcsize(">I4s")
PNG_CHECKSUM_SIZE = struct.calcsize(">I")
PNG_CHUNK_OVERHEAD = PNG_CHUNK_HEADER_SIZE + PNG_CHECKSUM_SIZE
PNG_IMAGE_HEADER_SIZE = struct.calcsize(">IIBBBBB")
PNG_ANCILLARY_MASK = 0x20
PNG_BIT_DEPTH = 8
PNG_COLOR_RGB = 2
PNG_COLOR_INDEXED = 3
PNG_FILTER_SUB = 1
PNG_FILTER_UP = 2
PNG_FILTER_AVERAGE = 3
PNG_FILTER_PAETH = 4
Image = namedtuple("Image", "width height rgb indices palette")


def require(condition, message):
    if not condition:
        raise ValueError(message)


def paeth(left, up, upper_left):
    prediction = left + up - upper_left
    distances = (abs(prediction - left), abs(prediction - up), abs(prediction - upper_left))
    if distances[0] <= distances[1] and distances[0] <= distances[2]:
        return left
    return up if distances[1] <= distances[2] else upper_left


def read_png(path):
    """Read only the opaque, non-interlaced eight-bit PNG formats in this asset set."""
    data = path.read_bytes()
    require(data[:len(PNG_SIGNATURE)] == PNG_SIGNATURE, f"{path}: invalid PNG signature")
    offset = len(PNG_SIGNATURE)
    header = palette = None
    compressed = bytearray()
    ended = False
    idat_ended = False
    while offset < len(data):
        require(offset + PNG_CHUNK_OVERHEAD <= len(data), f"{path}: truncated PNG chunk")
        size, kind = struct.unpack_from(">I4s", data, offset)
        end = offset + PNG_CHUNK_OVERHEAD + size
        require(end <= len(data), f"{path}: truncated {kind!r} chunk")
        payload = data[offset + PNG_CHUNK_HEADER_SIZE:end - PNG_CHECKSUM_SIZE]
        checksum = struct.unpack_from(">I", data, end - PNG_CHECKSUM_SIZE)[0]
        require(checksum == zlib.crc32(kind + payload), f"{path}: corrupt {kind!r} checksum")
        require(header is not None or kind == b"IHDR", f"{path}: IHDR must be first")
        if compressed and kind != b"IDAT":
            idat_ended = True
        if kind == b"IHDR":
            require(header is None and size == PNG_IMAGE_HEADER_SIZE, f"{path}: invalid IHDR")
            header = struct.unpack(">IIBBBBB", payload)
        elif kind == b"PLTE":
            require(palette is None and not compressed
                    and 0 < size <= PALETTE_COLOR_COUNT * RGB_CHANNELS and size % RGB_CHANNELS == 0,
                    f"{path}: invalid palette")
            palette = payload
        elif kind == b"IDAT":
            require(not idat_ended, f"{path}: noncontiguous image data")
            compressed.extend(payload)
        elif kind == b"IEND":
            require(size == 0 and end == len(data), f"{path}: invalid PNG end")
            ended = True
            break
        else:
            require(kind != b"tRNS" and kind[0] & PNG_ANCILLARY_MASK,
                    f"{path}: unsupported PNG chunk {kind!r}")
        offset = end
    require(ended and header and compressed, f"{path}: incomplete PNG")
    width, height, depth, color, compression, filtering, interlace = header
    require(width > 0 and height > 0 and depth == PNG_BIT_DEPTH
            and color in (PNG_COLOR_RGB, PNG_COLOR_INDEXED)
            and (compression, filtering, interlace) == (0, 0, 0),
            f"{path}: expected opaque, non-interlaced eight-bit RGB or indexed PNG")
    channels = RGB_CHANNELS if color == PNG_COLOR_RGB else 1
    stride = width * channels
    inflater = zlib.decompressobj()
    scanlines = inflater.decompress(compressed) + inflater.flush()
    require(inflater.eof and not inflater.unused_data and len(scanlines) == height * (stride + 1),
            f"{path}: invalid compressed scanline length")
    pixels = bytearray()
    previous = bytearray(stride)
    for y in range(height):
        start = y * (stride + 1)
        filter_type = scanlines[start]
        require(filter_type <= PNG_FILTER_PAETH, f"{path}: invalid PNG scanline filter")
        row = bytearray(scanlines[start + 1:start + 1 + stride])
        for x in range(stride):
            left = row[x - channels] if x >= channels else 0
            up = previous[x]
            upper_left = previous[x - channels] if x >= channels else 0
            if filter_type == PNG_FILTER_SUB:
                prediction = left
            elif filter_type == PNG_FILTER_UP:
                prediction = up
            elif filter_type == PNG_FILTER_AVERAGE:
                prediction = (left + up) // 2
            elif filter_type == PNG_FILTER_PAETH:
                prediction = paeth(left, up, upper_left)
            else:
                prediction = 0
            row[x] = (row[x] + prediction) & RGB_COMPONENT_MAX
        pixels.extend(row)
        previous = row
    indices = bytes(pixels) if color == PNG_COLOR_INDEXED else None
    if indices is not None:
        require(palette and max(indices) < len(palette) // RGB_CHANNELS,
                f"{path}: invalid palette index")
        pixels = b"".join(palette[index * RGB_CHANNELS:(index + 1) * RGB_CHANNELS]
                          for index in indices)
    return Image(width, height, bytes(pixels), indices, palette)


def enhanced_image(directory, name, width, height):
    path = directory / name
    image = read_png(path)
    require((image.width, image.height) == (width * SCALE, height * SCALE),
            f"{path}: expected exactly {width * SCALE}x{height * SCALE}")
    require(image.indices is None, f"{path}: enhanced image must retain RGB color detail")
    return image


def detail_blocks(image):
    detailed = total = 0
    sky_block = SKY_RGB * (SCALE * SCALE)
    for y in range(image.height // SCALE):
        for x in range(image.width // SCALE):
            block = bytearray()
            for row in range(SCALE):
                start = ((y * SCALE + row) * image.width + x * SCALE) * RGB_CHANNELS
                block.extend(image.rgb[start:start + SCALE * RGB_CHANNELS])
            if block == sky_block:
                continue
            total += 1
            if any(max(block[channel::RGB_CHANNELS]) - min(block[channel::RGB_CHANNELS])
                   >= DETAIL_CHANNEL_RANGE for channel in range(RGB_CHANNELS)):
                detailed += 1
    return detailed, total


def verify(directory):
    manifest = json.loads((directory / "manifest.json").read_text(encoding="utf-8"))
    require([scene["name"] for scene in manifest["scenes"]] == list(SCENES),
            "Expected all five original skybox scenes in selector order")
    minimum_detail = 1.0
    expected_files = set()
    for scene in manifest["scenes"]:
        require([item["resource"] for item in scene["images"]] == list(SHAPES),
                f"{scene['name']}: expected all four horizon strips in rendering order")
        panorama = enhanced_image(directory, scene["panorama"], scene["width"], scene["height"])
        expected_files.add(scene["panorama"])
        x = 0
        for item in scene["images"]:
            name = item["file"]
            expected_files.add(name)
            image = enhanced_image(directory, name, item["width"], item["height"])
            require(item["panorama_x"] == x
                    and item["panorama_y"] == scene["height"] - item["height"],
                    f"{name}: panorama must join strips consecutively at the bottom edge")
            x += item["width"]
            start_x = item["panorama_x"] * SCALE
            start_y = item["panorama_y"] * SCALE
            for row in range(image.height):
                source = row * image.width * RGB_CHANNELS
                target = ((start_y + row) * panorama.width + start_x) * RGB_CHANNELS
                require(image.rgb[source:source + image.width * RGB_CHANNELS]
                        == panorama.rgb[target:target + image.width * RGB_CHANNELS],
                        f"{name}: enhanced strip differs from its panorama placement")
            detailed, total = detail_blocks(image)
            # Retain the original scenery-area baseline so replacing most of a
            # strip with sky cannot inflate the fraction of remaining detail.
            baseline = item["scenery_pixels"]
            require(isinstance(baseline, int) and 0 < baseline <= item["width"] * item["height"],
                    f"{name}: invalid original scenery coverage")
            total = max(total, baseline)
            # At least 10% of scenery blocks must vary by 8/255 or more within
            # an original pixel. This rejects repeated pixels and trivial noise.
            require(detailed * DETAIL_REQUIRED_DENOMINATOR >= total,
                    f"{name}: insufficient detail beyond nearest-neighbor enlargement "
                    f"({detailed}/{total} scenery blocks)")
            minimum_detail = min(minimum_detail, detailed / total)
        require(x == scene["width"] == PANORAMA_WIDTH,
                f"{scene['name']}: incorrect panorama width")
    require({path.name for path in directory.glob("*.png")} == expected_files,
            "Expected exactly 20 enhanced strips and five enhanced panoramas")
    print("Verified 25 enhanced PNGs: valid checksums/scanlines, "
          "exact 4x dimensions, matching panorama placement, and added pixel detail "
          f"(minimum {minimum_detail:.0%} of scenery blocks).")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--directory", type=Path, default=ROOT / "assets/skyboxes")
    args = parser.parse_args()
    try:
        verify(args.directory)
    except (OSError, ValueError, KeyError, TypeError, struct.error, zlib.error) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
