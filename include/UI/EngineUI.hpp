#include "RecourseManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include "SceneManager.hpp"

namespace EngineUI{
    class EngineUI{
        public:
            void drawMainLayout(EngineScene::SceneManager *sceneManager);

            void drawSceneHierarchy(EngineScene::Scene *scene);

            void drawObjectInspector();

            void drawCameraInspector();

            void drawRecourses(EngineResource::ResourceManager *recourseManager);

            void drawRenderStats(EngineScene::Scene *scene, float fps);

            void drawSceneActions();
        
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