Convert an ITK Gray Scale Image to CV Mat
=========================================

.. index::
   single: OpenCVImageBridge

Synopsis
--------

Convert an itk::Image (gray scale) to OpenCV cv::Mat.


Results
-------

.. figure:: Yinyang.png
  :scale: 100%
  :alt: Input image

  Input image

.. figure:: OutputBaseline.png
  :scale: 100%
  :alt: Output image

  Output image


Code
----

C++
...

.. literalinclude:: Code.cxx
   :lines: 18-


Classes demonstrated
--------------------

.. breathelink:: itk::OpenCVImageBridge
