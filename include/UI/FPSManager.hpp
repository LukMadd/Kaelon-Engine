namespace EngineUI{
    class FPSManager{
        public:
            float updateFPS(float deltaTime);

        private:
            float fps = 0.0f;
            float frameTimeSum = 0.0f;
            int frameCount = 0;
            const int smoothingWindow = 60; 
    };
}