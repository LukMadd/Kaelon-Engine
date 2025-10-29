#include "Camera/Camera.hpp"
#include "Core/ResourceManager.hpp"
#include "Scene/Scene.hpp"
#include "Object/Object.hpp"
#include "Scene/SceneManager.hpp"

namespace EngineUI{
    class EngineUI{
        public:
            void drawMainLayout(EngineScene::SceneManager *sceneManager, 
                                EngineCamera::CameraManager *cameraManager);

            void drawSceneHierarchy(EngineScene::Scene *scene);

            void drawObjectInspector(EngineScene::Scene *scene);

            void drawCameraInspector();

            void drawSceneInspector(SceneManager *sceneManager);

            void drawRecourses(EngineResource::ResourceManager *recourseManager);

            void drawRenderStats(EngineScene::Scene *scene, float fps);

            void drawScenes(EngineScene::SceneManager *sceneManager);
        
        private:
            bool m_showSceneHierarchyWindow = false;
            bool m_showObjectInspector = false;
            bool m_showCameraInspector = false;
            bool m_showSceneInspector = false;
            bool m_showResourceWindow = false;
            bool m_showRenderStats = false;
            bool m_showScenesWindow = false;

            bool m_drawBoundingBoxes = false;

            EngineObject::Object *selectedObject = nullptr;
            EngineCamera::Camera *selectedCamera = nullptr;
            EngineScene::Scene *selectedScene = nullptr;

            EngineObject::Object *previousObject = nullptr;

        public:
            EngineObject::Object*& getSelectedObject(){return selectedObject;}
            EngineCamera::Camera*& getSelectedCamera(){return selectedCamera;}
            EngineScene::Scene* getSelectedScene(){return selectedScene;}

            bool getValueofDrawBoundingBoxes(){return m_drawBoundingBoxes;}

            void setSelectedCamera(EngineCamera::Camera *camera){selectedCamera = camera;}
    };
}