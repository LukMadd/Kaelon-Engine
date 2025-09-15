#include "RecourseManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"

namespace EngineUI{
    class EngineUI{
        public:
            void drawMainLayout();

            void drawSceneHierarchy(EngineScene::Scene *scene);

            void drawObjectInspector();

            void drawCamera();

            void drawRecourses(EngineResource::ResourceManager *recourseManager);

            void drawRenderStats(EngineScene::Scene *scene, float fps);
        
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