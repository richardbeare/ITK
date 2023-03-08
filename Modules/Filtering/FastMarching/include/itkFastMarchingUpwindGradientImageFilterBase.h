/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkFastMarchingUpwindGradientImageFilterBase_h
#define itkFastMarchingUpwindGradientImageFilterBase_h

#include "itkFastMarchingImageFilterBase.h"
#include "itkImage.h"

namespace itk
{
/**
 * \class FastMarchingUpwindGradientImageFilterBase
 *
 * \brief Generates the upwind gradient field of fast marching arrival times.
 *
 * This filter adds some extra functionality to its base class. While the
 * solution T(x) of the Eikonal equation is being generated by the base class
 * with the fast marching method, the filter generates the upwind gradient
 * vectors of T(x), storing them in an image.
 *
 * Since the Eikonal equation generates the arrival times of a wave traveling
 * at a given speed, the generated gradient vectors can be interpreted as the
 * slowness (1/velocity) vectors of the front (the quantity inside the modulus
 * operator in the Eikonal equation).
 *
 * Gradient vectors are computed using upwind finite differences, that is,
 * information only propagates from points where the wavefront has already
 * passed. This is consistent with how the fast marching method works.
 *
 * For an alternative implementation, see itk::FastMarchingUpwindGradientImageFilter.
 *
 * \author Luca Antiga Ph.D.  Biomedical Technologies Laboratory,
 *                            Bioengineering Department, Mario Negri Institute, Italy.
 *
 * \sa FastMarchingUpwindGradientImageFilter
 * \ingroup ITKFastMarching
 */
template <typename TInput, typename TOutput>
class ITK_TEMPLATE_EXPORT FastMarchingUpwindGradientImageFilterBase
  : public FastMarchingImageFilterBase<TInput, TOutput>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(FastMarchingUpwindGradientImageFilterBase);

  /** Standard class typedefs. */
  using Self = FastMarchingUpwindGradientImageFilterBase;
  using Superclass = FastMarchingImageFilterBase<TInput, TOutput>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;
  using typename Superclass::Traits;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FastMarchingUpwindGradientImageFilterBase, FastMarchingImageFilterBase);

  /** The dimension of the level set. */
  static constexpr unsigned int ImageDimension = Superclass::ImageDimension;

  using typename Superclass::NodeType;
  using typename Superclass::OutputImageType;
  using typename Superclass::OutputPixelType;
  using typename Superclass::OutputSpacingType;

  /** GradientPixel type alias support */
  using GradientPixelType = CovariantVector<OutputPixelType, ImageDimension>;

  /** GradientImage type alias support */
  using GradientImageType = Image<GradientPixelType, ImageDimension>;

  /** GradientImagePointer type alias support */
  using GradientImagePointer = typename GradientImageType::Pointer;

  /** Get the gradient image. */
  GradientImageType *
  GetGradientImage();

protected:
  FastMarchingUpwindGradientImageFilterBase();
  ~FastMarchingUpwindGradientImageFilterBase() override = default;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  void
  InitializeOutput(OutputImageType * output) override;

  void
  UpdateNeighbors(OutputImageType * oImage, const NodeType & iNode) override;

  virtual void
  ComputeGradient(OutputImageType * oImage, const NodeType & iNode);
};

/* this class was made in the case where isotropic and anisotropic fast
    marching would be implemented in this effort. As of now, we focus only
    in the isotropic case. Let's keep it for reference (just as a reminder).
*/
// template< unsigned int VDimension,
//         typename TInputPixel,
//         typename TOutputPixel >
// class IsotropicFastMarchingUpwindGradientImageFilterBase:
//    public FastMarchingUpwindGradientImageFilterBase< VDimension, TInputPixel,
//    TOutputPixel,
//    FastMarchingImageFilterBase< VDimension, TInputPixel, TOutputPixel >
//    >
//  {
// public:
//  using GrandParentClassType = FastMarchingImageFilterBase< VDimension, TInputPixel,
//    TOutputPixel >;

//  /** Standard class typedefs. */
//  using Self = IsotropicFastMarchingUpwindGradientImageFilterBase;
//  using Superclass = FastMarchingUpwindGradientImageFilterBase< VDimension, TInputPixel,
//    TOutputPixel, GrandParentClassType >;
//  using Pointer = SmartPointer< Self >;
//  using ConstPointer = SmartPointer< const Self >;

//  /** Method for creation through the object factory. */
//  itkNewMacro(Self);

//  /** Run-time type information (and related methods). */
//  itkTypeMacro(IsotropicFastMarchingUpwindGradientImageFilterBase,
//               FastMarchingUpwindGradientImageFilterBase );

//  /** The dimension of the level set. */
//  static constexpr unsigned int ImageDimension = //                      Superclass::ImageDimension;

//  using typename Superclass::NodeType;
//  using typename Superclass::OutputImageType;
//  using typename Superclass::OutputPixelType;
//  using typename Superclass::OutputSpacingType;

//  /** GradientPixel type alias support */
//  using GradientPixelType = CovariantVector< OutputPixelType,
//                           ImageDimension >;

//  /** GradientImage type alias support */
//  using GradientImageType = Image< GradientPixelType,
//                 ImageDimension >;

//  /** GradientImagePointer type alias support */
//  using GradientImagePointer = typename GradientImageType::Pointer;

// protected:
//  IsotropicFastMarchingUpwindGradientImageFilterBase() : Superclass() {}
//  ~IsotropicFastMarchingUpwindGradientImageFilterBase() {}

// private:
//  void operator = ( const Self& );
//  IsotropicFastMarchingUpwindGradientImageFilterBase( const Self& );
//  };
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkFastMarchingUpwindGradientImageFilterBase.hxx"
#endif

#endif
