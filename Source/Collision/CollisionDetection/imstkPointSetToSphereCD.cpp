/*=========================================================================

   Library: iMSTK

   Copyright (c) Kitware, Inc. & Center for Modeling, Simulation,
   & Imaging in Medicine, Rensselaer Polytechnic Institute.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

=========================================================================*/

#include "imstkPointSetToSphereCD.h"
#include "imstkNarrowPhaseCD.h"
#include "imstkCollisionData.h"
#include "imstkParallelUtils.h"
#include "imstkPointSet.h"

namespace imstk
{
PointSetToSphereCD::PointSetToSphereCD(std::shared_ptr<PointSet>      pointSet,
                                       std::shared_ptr<Sphere>        sphere,
                                       std::shared_ptr<CollisionData> colData) :
    CollisionDetection(CollisionDetection::Type::PointSetToSphere, colData),
    m_pointSet(pointSet),
    m_sphere(sphere)
{
}

void
PointSetToSphereCD::computeCollisionData()
{
    m_colData->clearAll();
    ParallelUtils::parallelFor(static_cast<unsigned int>(m_pointSet->getVertexPositions().size()),
        [&](const unsigned int idx)
        {
            const auto& point = m_pointSet->getVertexPosition(idx);
            NarrowPhaseCD::pointToSphere(point, idx, m_sphere.get(), m_colData);
       });
}
} // imstk
