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

// imstk
#include "imstkSceneManager.h"
#include "imstkCameraController.h"
#include "imstkSceneObjectController.h"
#include "imstkDynamicObject.h"
#include "imstkPbdObject.h"
#include "imstkDeformableObject.h"
#include "imstkGeometryMap.h"
#include "imstkTimer.h"
#include "imstkPbdSolver.h"

#include "g3log/g3log.hpp"

namespace imstk
{
void
SceneManager::initModule()
{
	m_scene->launchModules();

	if (m_scene->getCPDScene() != nullptr)
		m_scene->getCPDScene()->preSimulation();

}

void
SceneManager::cleanUpModule()
{
    // End Camera Controller
    if (auto camController = m_scene->getCamera()->getController())
    {
        camController->end();
        m_threadMap.at(camController->getName()).join();
    }

	if (m_scene->getCPDScene() != nullptr)
		m_scene->getCPDScene()->postSimulation();

}

void
SceneManager::startModuleInNewThread(std::shared_ptr<Module> module)
{
    m_threadMap[module->getName()] = std::thread([module] { module->start(); });
}
} // imstk
