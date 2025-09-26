#include "RecourseManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include "SceneManager.hpp"

namespace EngineUI{
    class EngineUI{
        public:
            void drawMainLayout(EngineScene::SceneManager *sceneManager);

            void drawSceneHierarchy(EngineScene::Scene *scene);

            void drawObjectInspector(EngineScene::Scene *scene);

            void drawCameraInspector();

            void drawRecourses(EngineResource::ResourceManager *recourseManager);

            void drawRenderStats(EngineScene::Scene *scene, float fps);

            void drawScenes(EngineScene::SceneManager *sceneManager);
        
        private:
            bool m_showSceneHierarchyWindow = false;
            bool m_showObjectInspector = false;
            bool m_showCameraWindow = false;
            bool m_showResourceWindow = false;
            bool m_showRenderStats = false;
            bool m_showScenesWindow = false;

            EngineObject::Object *selectedObject = nullptr;
            EngineCamera::Camera *selectedCamera = nullptr;
            EngineScene::Scene *selectedScene = nullptr;

            EngineObject::Object *previousObject = nullptr;

        public:
            EngineObject::Object*& getSelectedObject(){return selectedObject;}
            EngineScene::Scene* getSelectedScene(){return selectedScene;}
    };
}