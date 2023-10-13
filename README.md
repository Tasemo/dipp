# Dipp

Dipp is a **d**istributed **i**mage **p**rocessing **p**ipeline for image analysis in big data contexts. Its main use case is astronomy, where terabyte-sized images are regularly produced. To cope with these data valumes, the framework relies heavily on the MapReduce paradigm, which is implemented by [thrill](https://github.com/thrill/thrill). The limits of the framework are tested using images from the [SDSS](https://www.sdss.org/) DR18 data set.

## How to get started

Instructions on building the framework can be found [here](BUILDING.md). If you just want to test the application or use it in a cluster environment, follow [these](CONTAINER.MD) instructions instead.
