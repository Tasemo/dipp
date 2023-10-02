# Dipp

Dipp is a **d**istributed **i**mage **p**rocessing **p**ipeline for image analysis in big data contexts. Its main use case is astronomy, where terabyte-sized images are regularly produced. To cope with these data valumes, the framework relies heavily on the MapReduce paradigm, which is implemented by [thrill](https://github.com/thrill/thrill). The limits of the framework are tested using images from the [SDSS](https://www.sdss.org/) DR18 data set.
