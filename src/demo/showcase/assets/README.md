# Target image provenance

`mona_lisa.ppm` is a 396 × 599 RGB P6 PPM derivative of Leonardo da Vinci’s
*Mona Lisa* (artist died 1519), converted to RGB and downsampled with Lanczos.

- Source description: https://commons.wikimedia.org/wiki/File:Mona_Lisa.jpg
- Original raster: https://upload.wikimedia.org/wikipedia/commons/6/6a/Mona_Lisa.jpg
- Retrieved: November 14, 2026. The original SHA-1 was verified before conversion.
- Commons identifies the painting and faithful two-dimensional reproduction as
  public domain (PD-Art / PD-old-100-expired; Public Domain Mark 1.0).
- Source page revision recorded during retrieval: `oldid=1178043264`.
- Original dimensions: 2835 × 4289; SHA-1 listed by Commons:
  `b8f063d7a2e89f6cff856a08b2cfe8900cca138c`.

The image is not covered by a new exclusive software copyright claim. Review
the source page’s jurisdiction notes if redistributing outside the United States.
No build-time network download or image-processing dependency is needed.

The reference is displayed at full detail. Fitness uses a separate area-averaged
raster (128-pixel longest edge by default, configurable with `--resolution`).
The SVG approximation remains vector geometry; the exported PPM is the exact
fitness raster, not an enlarged or sharpened substitute.

The demo implementations are independently written. Their algorithmic inspirations
are credited in `docs/Demos.md`; the original authors’ source code is not included.
