/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTriangleCellTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMesh.h"
#include "itkTriangleCell.h"

#include <iostream>
#include <string>


int itkTriangleCellTest(int, char* [] )
{

   
  /**
   * Define a mesh type that stores a PixelType of "int".  Use the defaults for
   * the other template parameters.
   */
  typedef itk::Mesh<int>  MeshType;
  typedef MeshType::CellTraits  CellTraits;

  /**
   * Define a few cell types which uses a PixelType of "int".  Again,
   * use the defaults for the other parameters.  Note that a cell's template
   * parameters must match those of the mesh into which it is inserted.
   */
  typedef itk::CellInterface< int, CellTraits >           CellInterfaceType;
  typedef itk::TriangleCell<CellInterfaceType>            TriangleCellType;

  class TriangleHelper : public TriangleCellType
    {
    typedef TriangleCellType  Superclass;
    typedef Superclass::CoordRepType CoordRepType;
    typedef Superclass::PointsContainer PointsContainer;
    typedef Superclass::InterpolationWeightType InterpolationWeightType;

    public:
     bool EvaluatePosition(CoordRepType* inputPoint,
                                PointsContainer* points,
                                CoordRepType* closestPoint,
                                CoordRepType pcoord [],
                                double * distance,
                                InterpolationWeightType* weights)
      {
      return this->Superclass::EvaluatePosition( inputPoint,
        points, closestPoint, pcoord, distance, weights );
      }
      
    };


  /**
   * Typedef the generic cell type for the mesh.  It is an abstract class,
   * so we can only use information from it, like get its pointer type.
   */
  typedef MeshType::CellType              CellType;
  typedef CellType::CellAutoPointer       CellAutoPointer;

  /**
   * The type of point stored in the mesh. Because mesh was instantiated
   * with defaults (itkDefaultStaticMeshTraits), the point dimension is 3 and
   * the coordinate representation is float.
   */
  typedef MeshType::PointType  PointType;

  
  /**
   * Create the mesh through its object factory.
   */
  MeshType::Pointer mesh = MeshType::New();  
  mesh->DebugOn();

  const unsigned int numberOfPoints = 4;
  /**
   * Define the 3d geometric positions for 4 points in a square.
   */
  MeshType::CoordRepType testPointCoords[numberOfPoints][3]
    = { {0,0,0}, {10,0,0}, {10,10,0}, {0,10,0} };
 
  /**
   * Add our test points to the mesh.
   * mesh->SetPoint(pointId, point)
   * Note that the constructor for Point is public, and takes an array
   * of coordinates for the point.
   */
  for(unsigned int i=0; i < numberOfPoints ; ++i)
    {
    mesh->SetPoint(i, PointType( testPointCoords[i] ) );
    }

  /** 
   * Specify the method used for allocating cells
   */
   mesh->SetCellsAllocationMethod( MeshType::CellsAllocatedDynamicallyCellByCell );

  /**
   * Create the test cell. Note that testCell is a generic auto
   * pointer to a cell; in this example it ends up pointing to
   * different types of cells.
   */
  CellAutoPointer testCell; 
  TriangleHelper * newcell = new TriangleHelper;
  testCell.TakeOwnership( newcell ); // polymorphism

  /**
   * List the points that the polygon will use from the mesh.
   */
  unsigned long polygon1Points1[3] = {2,0,1};
 
  /**
   * Assign the points to the tetrahedron through their identifiers.
   */
  testCell->SetPointIds(polygon1Points1);

  /**
   * Add the test cell to the mesh.
   * mesh->SetCell(cellId, cell)
   */
  mesh->SetCell(0, testCell ); // Transfer ownership to the mesh
  std::cout << "TriangleCell pointer = " << (void const *)testCell.GetPointer() << std::endl;
  std::cout << "TriangleCell Owner   = " << testCell.IsOwner() << std::endl;
  
  {
  std::cout << "Test MakeCopy" << std::endl;

  CellAutoPointer anotherCell;

  testCell->MakeCopy( anotherCell );

  if( anotherCell->GetNumberOfPoints() != testCell->GetNumberOfPoints() )
    {
    std::cerr << "Make Copy failed !" << std::endl;
    return EXIT_FAILURE;
    }
  }
 
  

  //
  // Exercise the EvaluatePosition() method of the TriangleCell
  //
  TriangleCellType::CoordRepType inputPoint[3];
  TriangleCellType::PointsContainer * points = mesh->GetPoints();
  TriangleCellType::CoordRepType closestPoint[3];
  TriangleCellType::CoordRepType pcoords[3];
  double distance;
  TriangleCellType::InterpolationWeightType weights[3];

  bool isInside;

  // Test 1:
  inputPoint[0] = 5.0;
  inputPoint[1] = 3.0;
  inputPoint[2] = 0.0;

  std::cout << "Calling EvaluatePosition for " << inputPoint << std::endl;

  isInside = testCell->EvaluatePosition(inputPoint, 
    points, closestPoint, pcoords , &distance, weights);
 
  if( !isInside )
    {
    std::cerr << "Error: point should be reported as being inside" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;  
}

