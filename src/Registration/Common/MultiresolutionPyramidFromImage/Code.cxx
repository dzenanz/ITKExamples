/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"

using IntegralImageType = itk::Image<short, 3>;

static void
CreateImage(IntegralImageType::Pointer image);

int
main(int argc, char * argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0];
    std::cout << " [InputFileName] [OutputFileNamePrefix]";
    std::cout << std::endl;
  }

  IntegralImageType::Pointer image = IntegralImageType::New();
  if (argc >= 2) // read given file
  {
    using ReaderType = itk::ImageFileReader<IntegralImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
  }
  else // create synthetic image
  {
    CreateImage(image);
  }

  using FloatImageType = itk::Image<float, 3>;

  unsigned int numberOfLevels = 4;

  using RecursiveMultiResolutionPyramidImageFilterType =
    itk::RecursiveMultiResolutionPyramidImageFilter<IntegralImageType, IntegralImageType>;
  RecursiveMultiResolutionPyramidImageFilterType::Pointer recursiveMultiResolutionPyramidImageFilter =
    RecursiveMultiResolutionPyramidImageFilterType::New();
  recursiveMultiResolutionPyramidImageFilter->SetInput(image);
  recursiveMultiResolutionPyramidImageFilter->SetNumberOfLevels(numberOfLevels);
  const auto * shrinkFactors = recursiveMultiResolutionPyramidImageFilter->GetStartingShrinkFactors();
  recursiveMultiResolutionPyramidImageFilter->SetUseShrinkImageFilter(false);
  recursiveMultiResolutionPyramidImageFilter->Update();

  // This outputs the levels (0 is the lowest resolution)
  for (unsigned int i = 0; i < numberOfLevels; ++i)
  {
    using FileWriterType = itk::ImageFileWriter<IntegralImageType>;
    FileWriterType::Pointer writer = FileWriterType::New();

    std::stringstream ss;
    char *            filenamePrefix = "output_";
    if (argc >= 3)
    {
      filenamePrefix = argv[2];
    }
    ss << filenamePrefix << i << ".mha";
    std::cout << "Writing " << ss.str() << std::endl;

    writer->SetFileName(ss.str());
    writer->SetInput(recursiveMultiResolutionPyramidImageFilter->GetOutput(i));
    writer->SetUseCompression(true);
    writer->Update();
  }

  return EXIT_SUCCESS;
}


void
CreateImage(IntegralImageType::Pointer image)
{
  // Create a black image with a white region

  IntegralImageType::IndexType start;
  start.Fill(0);

  IntegralImageType::SizeType size;
  size.Fill(200);

  IntegralImageType::RegionType region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // Make a cuboid
  for (unsigned int r = 20; r < 80; r++)
  {
    for (unsigned int c = 30; c < 100; c++)
    {
      for (unsigned int l = 40; l < 60; l++)
      {
        IntegralImageType::IndexType pixelIndex;
        pixelIndex[0] = r;
        pixelIndex[1] = c;
        pixelIndex[2] = l;
        image->SetPixel(pixelIndex, 255);
      }
    }
  }
}
