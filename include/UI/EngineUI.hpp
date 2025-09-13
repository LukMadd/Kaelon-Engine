#include "Scene.hpp"
#include "Object.hpp"
#include "CameraManager.hpp"

namespace EngineUI{
    class EngineUI{
        public:
            void drawMainLayout();

            void drawSceneHierarchy(EngineScene::Scene *scene, EngineCamera::CameraManager *cameraManager);

            void drawObjectInspector();

            void drawCamera();
        
        private:
            bool m_showSceneHierarchyWindow = false;
            bool m_showObjectInspector = false;
            bool m_showCameraWindow = false;
            bool m_showResourceWindow = false;
            bool m_showRenderStats = false;

            EngineScene::Object *selectedObject = nullptr;
            EngineCamera::Camera *selectedCamera = nullptr;
    };
}